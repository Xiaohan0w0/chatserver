#include "mainwindow.h"
#include <QJsonArray>
#include <QInputDialog>
#include <QApplication>
#include <QHostAddress>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_socket(new QTcpSocket(this)), m_isLoginSuccess(false)
{
    setWindowTitle("聊天客户端");
    resize(800, 600);

    connect(m_socket, &QTcpSocket::connected, this, &MainWindow::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &MainWindow::onDisconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
    connect(m_socket, static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error),
            this, &MainWindow::onError);

    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);

    m_stackedWidget = new QStackedWidget(m_centralWidget);

    setupLoginUI();
    setupMainUI();

    QVBoxLayout *layout = new QVBoxLayout(m_centralWidget);
    layout->addWidget(m_stackedWidget);

    switchToLoginUI();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupLoginUI()
{
    QWidget *loginWidget = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(loginWidget);

    QGroupBox *connectionGroup = new QGroupBox("服务器连接");
    QFormLayout *connectionLayout = new QFormLayout(connectionGroup);

    m_ipEdit = new QLineEdit("127.0.0.1");
    m_portEdit = new QLineEdit("6000");
    m_connectButton = new QPushButton("连接");

    connectionLayout->addRow("IP地址:", m_ipEdit);
    connectionLayout->addRow("端口:", m_portEdit);
    connectionLayout->addRow(m_connectButton);

    QGroupBox *authGroup = new QGroupBox("用户认证");
    QFormLayout *authLayout = new QFormLayout(authGroup);

    m_userIdEdit = new QLineEdit;
    m_passwordEdit = new QLineEdit;
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_loginButton = new QPushButton("登录");
    m_registerButton = new QPushButton("注册");

    authLayout->addRow("用户ID:", m_userIdEdit);
    authLayout->addRow("密码:", m_passwordEdit);
    authLayout->addRow(m_loginButton, m_registerButton);

    layout->addWidget(connectionGroup);
    layout->addWidget(authGroup);
    layout->addStretch();

    connect(m_connectButton, &QPushButton::clicked, this, &MainWindow::connectToServer);
    connect(m_loginButton, &QPushButton::clicked, this, &MainWindow::login);
    connect(m_registerButton, &QPushButton::clicked, this, &MainWindow::regist);

    m_stackedWidget->addWidget(loginWidget);
}

void MainWindow::setupMainUI()
{
    QWidget *mainWidget = new QWidget;
    QHBoxLayout *mainLayout = new QHBoxLayout(mainWidget);

    // 左侧面板
    QWidget *leftPanel = new QWidget;
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftPanel->setMaximumWidth(250);

    m_userInfoLabel = new QLabel;
    m_friendList = new QListWidget;
    m_groupList = new QListWidget;

    leftLayout->addWidget(new QLabel("用户信息:"));
    leftLayout->addWidget(m_userInfoLabel);
    leftLayout->addWidget(new QLabel("好友列表:"));
    leftLayout->addWidget(m_friendList);
    leftLayout->addWidget(new QLabel("群组列表:"));
    leftLayout->addWidget(m_groupList);

    // 中间面板
    QWidget *centerPanel = new QWidget;
    QVBoxLayout *centerLayout = new QVBoxLayout(centerPanel);

    m_chatDisplay = new QTextEdit;
    m_chatDisplay->setReadOnly(true);

    QWidget *messagePanel = new QWidget;
    QHBoxLayout *messageLayout = new QHBoxLayout(messagePanel);

    m_messageEdit = new QLineEdit;
    m_sendButton = new QPushButton("发送");

    messageLayout->addWidget(m_messageEdit);
    messageLayout->addWidget(m_sendButton);

    centerLayout->addWidget(new QLabel("聊天记录:"));
    centerLayout->addWidget(m_chatDisplay);
    centerLayout->addWidget(messagePanel);

    // 右侧面板
    QWidget *rightPanel = new QWidget;
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);

    // 私聊部分
    QGroupBox *privateGroup = new QGroupBox("私聊");
    QFormLayout *privateLayout = new QFormLayout(privateGroup);

    m_targetIdEdit = new QLineEdit;
    QPushButton *sendPrivateButton = new QPushButton("发送私信");

    privateLayout->addRow("目标ID:", m_targetIdEdit);
    privateLayout->addRow(sendPrivateButton);

    // 添加好友
    QGroupBox *friendGroup = new QGroupBox("好友管理");
    QHBoxLayout *friendLayout = new QHBoxLayout(friendGroup);

    m_addFriendEdit = new QLineEdit;
    m_addFriendButton = new QPushButton("添加好友");

    friendLayout->addWidget(m_addFriendEdit);
    friendLayout->addWidget(m_addFriendButton);

    // 群组操作
    QGroupBox *groupCreateGroup = new QGroupBox("创建群组");
    QFormLayout *groupCreateLayout = new QFormLayout(groupCreateGroup);

    m_createGroupNameEdit = new QLineEdit;
    m_createGroupDescEdit = new QLineEdit;
    m_createGroupButton = new QPushButton("创建群组");

    groupCreateLayout->addRow("群名称:", m_createGroupNameEdit);
    groupCreateLayout->addRow("群描述:", m_createGroupDescEdit);
    groupCreateLayout->addRow(m_createGroupButton);

    QGroupBox *groupJoinGroup = new QGroupBox("加入群组");
    QHBoxLayout *groupJoinLayout = new QHBoxLayout(groupJoinGroup);

    m_joinGroupIdEdit = new QLineEdit;
    m_joinGroupButton = new QPushButton("加入群组");

    groupJoinLayout->addWidget(m_joinGroupIdEdit);
    groupJoinLayout->addWidget(m_joinGroupButton);

    QGroupBox *groupSendGroup = new QGroupBox("群发消息");
    QFormLayout *groupSendLayout = new QFormLayout(groupSendGroup);

    m_groupMsgTargetEdit = new QLineEdit;
    m_groupMessageEdit = new QLineEdit;
    m_sendGroupMsgButton = new QPushButton("发送群消息");

    groupSendLayout->addRow("群ID:", m_groupMsgTargetEdit);
    groupSendLayout->addRow("消息:", m_groupMessageEdit);
    groupSendLayout->addRow(m_sendGroupMsgButton);

    // 其他按钮
    m_helpButton = new QPushButton("帮助");
    m_logoutButton = new QPushButton("注销");

    rightLayout->addWidget(privateGroup);
    rightLayout->addWidget(friendGroup);
    rightLayout->addWidget(groupCreateGroup);
    rightLayout->addWidget(groupJoinGroup);
    rightLayout->addWidget(groupSendGroup);
    rightLayout->addWidget(m_helpButton);
    rightLayout->addWidget(m_logoutButton);
    rightLayout->addStretch();

    mainLayout->addWidget(leftPanel);
    mainLayout->addWidget(centerPanel);
    mainLayout->addWidget(rightPanel);

    // 连接信号和槽
    connect(m_sendButton, &QPushButton::clicked, this, &MainWindow::sendPrivateMessage);
    connect(sendPrivateButton, &QPushButton::clicked, this, &MainWindow::sendPrivateMessage);
    connect(m_addFriendButton, &QPushButton::clicked, this, &MainWindow::addFriend);
    connect(m_createGroupButton, &QPushButton::clicked, this, &MainWindow::createGroup);
    connect(m_joinGroupButton, &QPushButton::clicked, this, &MainWindow::joinGroup);
    connect(m_sendGroupMsgButton, &QPushButton::clicked, this, &MainWindow::sendGroupMessage);
    connect(m_helpButton, &QPushButton::clicked, this, &MainWindow::showHelp);
    connect(m_logoutButton, &QPushButton::clicked, this, &MainWindow::logout);
    connect(m_messageEdit, &QLineEdit::returnPressed, this, &MainWindow::sendPrivateMessage);

    m_stackedWidget->addWidget(mainWidget);
}

void MainWindow::connectToServer()
{
    m_ip = m_ipEdit->text();
    m_port = m_portEdit->text().toUShort();

    m_socket->connectToHost(m_ip, m_port);
}

void MainWindow::onConnected()
{
    QMessageBox::information(this, "连接成功", "已成功连接到服务器");
    m_loginButton->setEnabled(true);
    m_registerButton->setEnabled(true);
}

void MainWindow::onDisconnected()
{
    QMessageBox::warning(this, "连接断开", "与服务器的连接已断开");
    switchToLoginUI();
}

void MainWindow::onError(QAbstractSocket::SocketError socketError)
{
    QMessageBox::critical(this, "连接错误",
                          QString("连接出错: %1").arg(m_socket->errorString()));
}

void MainWindow::onReadyRead()
{
    QByteArray data = m_socket->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);

    if (!doc.isNull() && doc.isObject())
    {
        processResponse(doc);
    }
}

void MainWindow::login()
{
    if (m_socket->state() != QAbstractSocket::ConnectedState)
    {
        QMessageBox::warning(this, "错误", "未连接到服务器");
        return;
    }

    QJsonObject obj;
    obj["msgid"] = LOGIN_MSG;
    obj["id"] = m_userIdEdit->text().toInt();
    obj["password"] = m_passwordEdit->text();

    QJsonDocument doc(obj);
    m_socket->write(doc.toJson(QJsonDocument::Compact));
    m_socket->flush();
}

void MainWindow::regist()
{
    bool ok;
    QString name = QInputDialog::getText(this, "注册", "用户名:", QLineEdit::Normal, "", &ok);
    if (!ok || name.isEmpty())
        return;

    QString password = QInputDialog::getText(this, "注册", "密码:", QLineEdit::Password, "", &ok);
    if (!ok || password.isEmpty())
        return;

    QJsonObject obj;
    obj["msgid"] = REGISTER_MSG;
    obj["name"] = name;
    obj["password"] = password;

    QJsonDocument doc(obj);
    m_socket->write(doc.toJson(QJsonDocument::Compact));
    m_socket->flush();
}

void MainWindow::logout()
{
    QJsonObject obj;
    obj["msgid"] = LOGINOUT_MSG;
    obj["id"] = m_currentUser.getId();

    QJsonDocument doc(obj);
    m_socket->write(doc.toJson(QJsonDocument::Compact));
    m_socket->flush();

    m_socket->disconnectFromHost();
    switchToLoginUI();
}

void MainWindow::sendPrivateMessage()
{
    QString message = m_messageEdit->text();
    if (message.isEmpty())
        return;

    int toId = m_targetIdEdit->text().toInt();
    if (toId <= 0)
    {
        toId = m_userIdEdit->text().toInt();
    }

    QJsonObject obj;
    obj["msgid"] = ONE_CHAT_MSG;
    obj["id"] = m_currentUser.getId();
    obj["name"] = m_currentUser.getName().c_str();
    obj["toid"] = toId;
    obj["msg"] = message;
    obj["time"] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    QJsonDocument doc(obj);
    m_socket->write(doc.toJson(QJsonDocument::Compact));
    m_socket->flush();

    m_chatDisplay->append(QString("[%1] 我 -> %2: %3")
                              .arg(obj["time"].toString())
                              .arg(toId)
                              .arg(message));
    m_messageEdit->clear();
}

void MainWindow::addFriend()
{
    int friendId = m_addFriendEdit->text().toInt();
    if (friendId <= 0)
    {
        QMessageBox::warning(this, "错误", "请输入有效的好友ID");
        return;
    }

    QJsonObject obj;
    obj["msgid"] = ADD_FRIEND_MSG;
    obj["id"] = m_currentUser.getId();
    obj["friendid"] = friendId;

    QJsonDocument doc(obj);
    m_socket->write(doc.toJson(QJsonDocument::Compact));
    m_socket->flush();

    m_addFriendEdit->clear();
}

void MainWindow::createGroup()
{
    QString groupName = m_createGroupNameEdit->text();
    QString groupDesc = m_createGroupDescEdit->text();

    if (groupName.isEmpty())
    {
        QMessageBox::warning(this, "错误", "请输入群名称");
        return;
    }

    QJsonObject obj;
    obj["msgid"] = CREATE_GROUP_MSG;
    obj["id"] = m_currentUser.getId();
    obj["groupname"] = groupName;
    obj["groupdesc"] = groupDesc;

    QJsonDocument doc(obj);
    m_socket->write(doc.toJson(QJsonDocument::Compact));
    m_socket->flush();

    m_createGroupNameEdit->clear();
    m_createGroupDescEdit->clear();
}

void MainWindow::joinGroup()
{
    int groupId = m_joinGroupIdEdit->text().toInt();
    if (groupId <= 0)
    {
        QMessageBox::warning(this, "错误", "请输入有效的群ID");
        return;
    }

    QJsonObject obj;
    obj["msgid"] = ADD_GROUP_MSG;
    obj["id"] = m_currentUser.getId();
    obj["groupid"] = groupId;

    QJsonDocument doc(obj);
    m_socket->write(doc.toJson(QJsonDocument::Compact));
    m_socket->flush();

    m_joinGroupIdEdit->clear();
}

void MainWindow::sendGroupMessage()
{
    int groupId = m_groupMsgTargetEdit->text().toInt();
    QString message = m_groupMessageEdit->text();

    if (groupId <= 0)
    {
        QMessageBox::warning(this, "错误", "请输入有效的群ID");
        return;
    }

    if (message.isEmpty())
    {
        QMessageBox::warning(this, "错误", "请输入消息内容");
        return;
    }

    QJsonObject obj;
    obj["msgid"] = GROUP_CHAT_MSG;
    obj["id"] = m_currentUser.getId();
    obj["name"] = m_currentUser.getName().c_str();
    obj["groupid"] = groupId;
    obj["msg"] = message;
    obj["time"] = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    QJsonDocument doc(obj);
    m_socket->write(doc.toJson(QJsonDocument::Compact));
    m_socket->flush();

    m_chatDisplay->append(QString("[%1] 我在群%2中说: %3")
                              .arg(obj["time"].toString())
                              .arg(groupId)
                              .arg(message));
    m_groupMessageEdit->clear();
}

void MainWindow::showHelp()
{
    QString helpText =
        "命令说明:\n"
        "help - 显示所有支持的命令\n"
        "chat - 一对一聊天\n"
        "addfriend - 添加好友\n"
        "creategroup - 创建群组\n"
        "addgroup - 加入群组\n"
        "groupchat - 群聊\n"
        "loginout - 注销\n";

    QMessageBox::information(this, "帮助", helpText);
}

void MainWindow::switchToLoginUI()
{
    m_stackedWidget->setCurrentIndex(0);
    m_loginButton->setEnabled(false);
    m_registerButton->setEnabled(false);
}

void MainWindow::switchToMainUI()
{
    m_stackedWidget->setCurrentIndex(1);
    showUserInfo();
}

void MainWindow::processResponse(const QJsonDocument &doc)
{
    QJsonObject obj = doc.object();
    int msgId = obj["msgid"].toInt();

    switch (msgId)
    {
    case LOGIN_MSG_ACK:
    {
        if (obj["errno"].toInt() == 0)
        {
            // 登录成功
            m_isLoginSuccess = true;

            QJsonObject data = obj["data"].toObject();
            m_currentUser.setId(data["id"].toInt());
            m_currentUser.setName(data["name"].toString().toStdString());
            m_currentUser.setState(data["state"].toString().toStdString());

            // 处理好友列表
            m_friendListData.clear();
            QJsonArray friends = data["friends"].toArray();
            for (auto fri : friends)
            {
                QJsonObject f = fri.toObject();
                User user;
                user.setId(f["id"].toInt());
                user.setName(f["name"].toString().toStdString());
                user.setState(f["state"].toString().toStdString());
                m_friendListData.push_back(user);
            }

            // 处理群组列表
            m_groupListData.clear();
            QJsonArray groups = data["groups"].toArray();
            for (auto grp : groups)
            {
                QJsonObject g = grp.toObject();
                Group group;
                group.setId(g["id"].toInt());
                group.setName(g["groupname"].toString().toStdString());
                group.setDesc(g["groupdesc"].toString().toStdString());

                QJsonArray groupUsers = g["users"].toArray();
                for (auto gu : groupUsers)
                {
                    QJsonObject u = gu.toObject();
                    GroupUser user;
                    user.setId(u["id"].toInt());
                    user.setName(u["name"].toString().toStdString());
                    user.setState(u["state"].toString().toStdString());
                    user.setRole(u["role"].toString().toStdString());
                    group.getUsers().push_back(user);
                }

                m_groupListData.push_back(group);
            }

            switchToMainUI();
        }
        else
        {
            QMessageBox::warning(this, "登录失败", obj["errmsg"].toString());
        }
        break;
    }
    case REG_MSG_ACK:
    {
        if (obj["errno"].toInt() == 0)
        {
            QMessageBox::information(this, "注册成功",
                                     QString("注册成功，您的用户ID是: %1").arg(obj["id"].toInt()));
        }
        else
        {
            QMessageBox::warning(this, "注册失败", obj["errmsg"].toString());
        }
        break;
    }
    case ONE_CHAT_MSG:
    {
        QString time = obj["time"].toString();
        QString fromName = obj["name"].toString();
        int fromId = obj["id"].toInt();
        QString message = obj["msg"].toString();

        m_chatDisplay->append(QString("[%1] %2(%3): %4")
                                  .arg(time)
                                  .arg(fromName)
                                  .arg(fromId)
                                  .arg(message));
        break;
    }
    case GROUP_CHAT_MSG:
    {
        QString time = obj["time"].toString();
        QString fromName = obj["name"].toString();
        int fromId = obj["id"].toInt();
        int groupId = obj["groupid"].toInt();
        QString message = obj["msg"].toString();

        m_chatDisplay->append(QString("[%1] %2(%3)在群%4中说: %5")
                                  .arg(time)
                                  .arg(fromName)
                                  .arg(fromId)
                                  .arg(groupId)
                                  .arg(message));
        break;
    }
    default:
        m_chatDisplay->append("收到未知消息类型: " + QString::number(msgId));
        break;
    }
}

void MainWindow::showUserInfo()
{
    m_userInfoLabel->setText(QString("ID: %1\n姓名: %2\n状态: %3")
                                 .arg(m_currentUser.getId())
                                 .arg(QString::fromStdString(m_currentUser.getName()))
                                 .arg(QString::fromStdString(m_currentUser.getState())));

    m_friendList->clear();
    for (const auto &user : m_friendListData)
    {
        QString itemText = QString("%1 - %2 (%3)")
                               .arg(user.getId())
                               .arg(QString::fromStdString(user.getName()))
                               .arg(QString::fromStdString(user.getState()));
        m_friendList->addItem(itemText);
    }

    m_groupList->clear();
    for (const auto &group : m_groupListData)
    {
        QString itemText = QString("%1 - %2")
                               .arg(group.getId())
                               .arg(QString::fromStdString(group.getName()));
        m_groupList->addItem(itemText);
    }
}