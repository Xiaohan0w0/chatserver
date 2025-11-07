# 配置头文件搜索路径
# include_directories()
# 配置库文件搜索路径
# link_directories()

# 设置需要编译的源文件列表
set(SRC_LIST ./muduo_server.cpp)

# 把.指定路径下的所有源文件名字放入变量名SRC_LIST中
# aux_source_directory(. SRC_LIST)

# 表示生成可执行文件server，由SERVER_LIST变量指定的源文件编译而成
add_executable(testmuduo ${SRC_LIST})

# 表示server这个目标程序，需要链接的库文件有muduo_net muduo_base pthread
target_link_libraries(testmuduo muduo_net muduo_base pthread)