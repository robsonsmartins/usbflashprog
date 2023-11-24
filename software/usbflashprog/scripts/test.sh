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
eval "${CMAKE} -B ${workspaceRoot}/build -S ${workspaceRoot} -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_FLAGS_DEBUG=\"-g -O0\" -DCMAKE_CXX_FLAGS_DEBUG=\"-g -O0\" -DTEST_BUILD=ON"

echo "* Building test project..."
cmake --build ${workspaceRoot}/build

echo "* Running tests..."
pushd ${workspaceRoot}/build
ctest
popd
