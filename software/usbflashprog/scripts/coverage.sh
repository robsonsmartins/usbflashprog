#!/bin/bash
workspaceRoot=$(cd -- "$(dirname -- ${BASH_SOURCE[0]})/.." &> /dev/null && pwd)
echo "* Workspace root: ${workspaceRoot}"

if [[ "$OSTYPE" =~ ^msys ]]; then
    CMAKE="cmake -G \"MinGW Makefiles\""
    LCOV="perl c:/ProgramData/chocolatey/lib/lcov/tools/bin/lcov"
    GENHTML="perl c:/ProgramData/chocolatey/lib/lcov/tools/bin/genhtml"
else
    CMAKE="cmake"
    LCOV="lcov"
    GENHTML="genhtml"
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

echo "* Capturing coverage info..."
eval "${LCOV} --capture --directory ${workspaceRoot}/build/ --output-file ${workspaceRoot}/build/coverage.info --rc lcov_branch_coverage=1 \
      --exclude \/c\/MinGW\/\* --exclude \/usr\/include\/\* --exclude \/usr\/local\/include\/\* --exclude ${workspaceRoot}\/build\/\* --exclude ${workspaceRoot}\/test\/\*"

echo "* Generating coverage report..."
rm -Rf ${workspaceRoot}/../../docs/software/lcov/
eval "${GENHTML} --prefix ${workspaceRoot} --ignore-errors source ${workspaceRoot}/build/coverage.info --legend --title \"`git rev-parse --short HEAD`\" \
      --output-directory ${workspaceRoot}/../../docs/software/lcov/"
