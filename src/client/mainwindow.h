#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QListWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QGroupBox>
#include <QFormLayout>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTimer>

#include "public.hpp"
#include "user.hpp"
#include "group.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void connectToServer();
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError socketError);
    void onReadyRead();

    // 登录相关
    void login();
    void logout();
    void regist();

    // 主界面操作
    void showHelp();
    void sendPrivateMessage();
    void addFriend();
    void createGroup();
    void joinGroup();
    void sendGroupMessage();

    // UI切换
    void switchToLoginUI();
    void switchToMainUI();

private:
    void setupLoginUI();
    void setupMainUI();
    void processResponse(const QJsonDocument &doc);
    void showUserInfo();

    QTcpSocket *m_socket;
    QString m_ip;
    quint16 m_port;

    // UI组件
    QWidget *m_centralWidget;
    QStackedWidget *m_stackedWidget;

    // 登录界面组件
    QLineEdit *m_ipEdit;
    QLineEdit *m_portEdit;
    QPushButton *m_connectButton;

    QLineEdit *m_userIdEdit;
    QLineEdit *m_passwordEdit;
    QPushButton *m_loginButton;
    QPushButton *m_registerButton;

    // 主界面组件
    QLabel *m_userInfoLabel;
    QListWidget *m_friendList;
    QListWidget *m_groupList;
    QTextEdit *m_chatDisplay;

    QLineEdit *m_messageEdit;
    QLineEdit *m_targetIdEdit;
    QPushButton *m_sendButton;

    QLineEdit *m_addFriendEdit;
    QPushButton *m_addFriendButton;

    QLineEdit *m_createGroupNameEdit;
    QLineEdit *m_createGroupDescEdit;
    QPushButton *m_createGroupButton;

    QLineEdit *m_joinGroupIdEdit;
    QPushButton *m_joinGroupButton;

    QLineEdit *m_groupMsgTargetEdit;
    QLineEdit *m_groupMessageEdit;
    QPushButton *m_sendGroupMsgButton;

    QPushButton *m_logoutButton;
    QPushButton *m_helpButton;

    // 用户状态
    User m_currentUser;
    std::vector<User> m_friendListData;
    std::vector<Group> m_groupListData;

    bool m_isLoginSuccess;
};

#endif // MAINWINDOW_H