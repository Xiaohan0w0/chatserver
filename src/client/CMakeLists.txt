# 手动触发MOC处理
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# 查找Qt5包
find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui Network)

# 启用Qt的自动moc、uic和rcc处理
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 添加包含目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../server/model
    ${Qt5Core_INCLUDE_DIRS}
    ${Qt5Network_INCLUDE_DIRS}
    ${Qt5Widgets_INCLUDE_DIRS}
    ${Qt5Gui_INCLUDE_DIRS}
)

# 只添加纯数据模型类（不涉及数据库操作）
set(CLIENT_MODEL_SRCS
    ../server/model/user.cpp
    ../server/model/group.cpp
    ../server/model/groupuser.cpp
)

aux_source_directory(. SRC_LIST)

add_executable(ChatClient ${CLIENT_MODEL_SRCS} ${SRC_LIST})

# 指定可执行文件链接时需要的库文件
target_link_libraries(ChatClient 
    pthread
    Qt5::Core 
    Qt5::Widgets 
    Qt5::Gui
    Qt5::Network
)