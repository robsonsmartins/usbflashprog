#!/bin/bash
workspaceRoot=$(cd -- "$(dirname -- ${BASH_SOURCE[0]})/.." &> /dev/null && pwd)
echo "* Workspace root: ${workspaceRoot}"
echo "* Generating build files..."
rm -Rf ${workspaceRoot}/build
cmake -B ${workspaceRoot}/build -S ${workspaceRoot} -DTEST_BUILD=ON
echo "* Building test project..."
cmake --build ${workspaceRoot}/build
echo "* Running tests..."
pushd ${workspaceRoot}/build
ctest
popd
