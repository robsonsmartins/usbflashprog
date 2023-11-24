#!/bin/bash
workspaceRoot=$(cd -- "$(dirname -- ${BASH_SOURCE[0]})/.." &> /dev/null && pwd)
echo "* Workspace root: ${workspaceRoot}"

if [[ "$OSTYPE" =~ ^msys ]]; then
    CMAKE="cmake -G \"MinGW Makefiles\""
else
    CMAKE="cmake"
fi

echo "* Generating build files..."
rm -Rf ${workspaceRoot}/build
eval "${CMAKE} -B ${workspaceRoot}/build -S ${workspaceRoot} -DCMAKE_BUILD_TYPE=Release"

echo "* Building project..."
cmake --build ${workspaceRoot}/build --config Release
