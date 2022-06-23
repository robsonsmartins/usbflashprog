#!/bin/bash
workspaceRoot=$(cd -- "$(dirname -- ${BASH_SOURCE[0]})/.." &> /dev/null && pwd)
echo "* Workspace root: ${workspaceRoot}"
echo "* Generating build files..."
rm -Rf ${workspaceRoot}/build
if [[ "$OSTYPE" =~ ^msys ]]; then
    cmake -G "MinGW Makefiles" -B ${workspaceRoot}/build -S ${workspaceRoot} -DNORMAL_BUILD=ON
else
    cmake -B ${workspaceRoot}/build -S ${workspaceRoot} -DNORMAL_BUILD=ON
fi
echo "* Building project..."
cmake --build ${workspaceRoot}/build
