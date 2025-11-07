#!/bin/bash
set -x  # 开启调试模式，执行时会输出每一步命令（方便排查问题）

# 关键修正1：用 $() 获取当前目录，且去掉单引号（单引号会让变量/命令失效）
CURRENT_DIR=$(pwd)  # 把当前目录路径存到变量，后续复用更清晰
BUILD_DIR="${CURRENT_DIR}/build"  # 定义build目录路径

# 关键修正2：先判断build目录是否存在，不存在则创建（避免首次执行cd失败）
if [ ! -d "${BUILD_DIR}" ]; then
    mkdir -p "${BUILD_DIR}"  # -p 确保递归创建，即使上级目录不存在也不会报错
fi

# 清理build目录下的所有文件（避免旧编译产物干扰）
rm -rf "${BUILD_DIR}"/*

# 进入build目录，执行cmake和make（&& 表示前一步成功才执行下一步）
cd "${BUILD_DIR}" && cmake .. && make