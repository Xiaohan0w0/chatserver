# 只添加纯数据模型类（不涉及数据库操作）
set(CLIENT_MODEL_SRCS
    ../server/model/user.cpp
    ../server/model/group.cpp
    ../server/model/groupuser.cpp
)

add_executable(ChatClient
    main.cpp
    ${CLIENT_MODEL_SRCS}
)

# 指定可执行文件链接时需要的库文件
target_link_libraries(ChatClient pthread)