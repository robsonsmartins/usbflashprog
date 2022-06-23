#!/bin/bash
workspaceRoot=$(cd -- "$(dirname -- ${BASH_SOURCE[0]})/.." &> /dev/null && pwd)
echo "* Workspace root: ${workspaceRoot}"
echo "* Generating build files..."
rm -Rf ${workspaceRoot}/build
if [[ "$OSTYPE" =~ ^msys ]]; then
    cmake -G "MinGW Makefiles" -B ${workspaceRoot}/build -S ${workspaceRoot} -DTEST_BUILD=ON
else
    cmake -B ${workspaceRoot}/build -S ${workspaceRoot} -DTEST_BUILD=ON
fi
echo "* Building test project..."
cmake --build ${workspaceRoot}/build
echo "* Running tests..."
pushd ${workspaceRoot}/build
ctest
popd
