#!/bin/bash
workspaceRoot=$(cd -- "$(dirname -- ${BASH_SOURCE[0]})/.." &> /dev/null && pwd)
echo "* Workspace root: ${workspaceRoot}"
echo "* Generating build files..."
rm -Rf ${workspaceRoot}/build
if [[ "$OSTYPE" =~ ^msys ]]; then
    cmake -G "MinGW Makefiles" -B ${workspaceRoot}/build -S ${workspaceRoot} -DCMAKE_BUILD_TYPE=Debug -DTEST_BUILD=ON
else
    cmake -B ${workspaceRoot}/build -S ${workspaceRoot} -DCMAKE_BUILD_TYPE=Debug -DTEST_BUILD=ON
fi
echo "* Building test project..."
cmake --build ${workspaceRoot}/build
echo "* Running tests..."
pushd ${workspaceRoot}/build
ctest
popd
echo "* Capturing coverage info..."
lcov --directory ${workspaceRoot}/build/ --capture --output-file ${workspaceRoot}/build/coverage.info -rc lcov_branch_coverage=1 --exclude \/usr\/include\/\* --exclude \/usr\/local\/include\/\* --exclude ${workspaceRoot}\/build\/\* --exclude ${workspaceRoot}\/test\/\*
echo "* Generating coverage report..."
genhtml ${workspaceRoot}/build/coverage.info --output-directory ${workspaceRoot}/../../docs/software/lcov/
