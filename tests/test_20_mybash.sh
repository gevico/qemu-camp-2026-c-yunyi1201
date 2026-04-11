#!/bin/bash
# Test script for exercise 20_mybash
# Runs mybash with the command script and outputs results
MYBASH=/workspace/exercises/20_mybash/bin/mybash
CMD_SCRIPT=/workspace/tests/mybash_cmd.sh

exec "$MYBASH" "$CMD_SCRIPT"
