#!/bin/bash
workspaceRoot=$(cd -- "$(dirname -- ${BASH_SOURCE[0]})/.." &> /dev/null && pwd)
echo "* Workspace root: ${workspaceRoot}"
echo "* Generating build files..."
rm -Rf ${workspaceRoot}/build
cmake -G "MinGW Makefiles" -B  -S ${workspaceRoot} -DCMAKE_BUILD_TYPE=Release
echo "* Building project..."
cmake --build ${workspaceRoot}/build --config Release
