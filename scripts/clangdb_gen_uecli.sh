#!/usr/bin/env bash

ue4 gen
project="${PWD##*/}"
cat ".vscode/compileCommands_${project}.json" | python -c 'import json,sys
j = json.load(sys.stdin)
for o in j:
    file = o["file"]
    arg = o["arguments"][1]
    o["arguments"] = ["clang++ -std=c++20 -ferror-limit=0 -Wall -Wextra -Wpedantic -Wshadow-all -Wno-unused-parameter " + file + " --query-driver " + arg]
print(json.dumps(j, indent=2))' > compile_commands.json
