# # 查找Qt5包
# find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui)

# # 启用Qt的自动moc、uic和rcc处理
# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTOUIC ON)
# set(CMAKE_AUTORCC ON)

# # 设置需要编译的源文件列表
# set(SRC_LIST ./qt.cpp)

# # 把.指定路径下的所有源文件名字放入变量名SRC_LIST中
# aux_source_directory(. SRC_LIST)

# # 表示生成可执行文件server，由SERVER_LIST变量指定的源文件编译而成
# add_executable(testqt ${SRC_LIST})

# # 表示server这个目标程序，需要链接的库文件有muduo_net muduo_base pthread
# # target_link_libraries(testmuduo muduo_net muduo_base pthread)

# # 链接Qt库
# target_link_libraries(testqt 
#     Qt5::Core 
#     Qt5::Widgets 
#     Qt5::Gui
# )