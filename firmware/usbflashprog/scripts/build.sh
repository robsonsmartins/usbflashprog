#!/bin/bash
workspaceRoot=$(cd -- "$(dirname -- ${BASH_SOURCE[0]})/.." &> /dev/null && pwd)
echo "* Workspace root: ${workspaceRoot}"
echo "* Generating build files..."
rm -Rf ${workspaceRoot}/build
cmake -B ${workspaceRoot}/build -S ${workspaceRoot} -DNORMAL_BUILD=ON
echo "* Building project..."
pushd ${workspaceRoot}/build
make
popd
