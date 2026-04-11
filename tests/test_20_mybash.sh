#!/bin/bash
# 运行 mybash 脚本，输出各命令结果供测试框架校验

MYBASH=../exercises/20_mybash/bin/mybash
SCRIPT=../tests/mybash_cmd.sh

# 先编译（确保二进制是最新的）
(cd ../exercises/20_mybash && make 2>/dev/null)

# 执行脚本
"$MYBASH" "$SCRIPT"
