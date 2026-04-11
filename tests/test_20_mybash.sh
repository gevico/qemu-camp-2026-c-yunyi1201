#!/bin/bash
# 运行 mybash 脚本，输出各命令结果供测试框架校验

MYBASH=../exercises/20_mybash/bin/mybash
SCRIPT=../tests/mybash_cmd.sh

# 执行脚本
"$MYBASH" "$SCRIPT"
