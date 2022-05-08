#!/bin/bash
workspaceRoot=$(cd -- "$(dirname -- ${BASH_SOURCE[0]})/.." &> /dev/null && pwd)
echo "Workspace root: ${workspaceRoot}"
echo "Generating build files..."
rm -Rf ${workspaceRoot}/build
cmake -B ${workspaceRoot}/build -S ${workspaceRoot} -DTEST_BUILD=ON
echo "Building test project..."
cmake --build ${workspaceRoot}/build
echo "Running tests..."
pushd ${workspaceRoot}/build
ctest
popd
echo "Capturing coverage info..."
lcov --directory ${workspaceRoot}/build/ --capture --output-file ${workspaceRoot}/build/coverage.info -rc lcov_branch_coverage=1 --exclude \/usr\/include\/\* --exclude \/usr\/local\/include\/\* --exclude ${workspaceRoot}\/build\/\* --exclude ${workspaceRoot}\/test\/\*
echo "Generating coverage report..."
genhtml ${workspaceRoot}/build/coverage.info --output-directory ${workspaceRoot}/../../docs/firmware/lcov/
echo "Success!"
