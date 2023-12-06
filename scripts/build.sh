#!/bin/bash
workspaceRoot=$(cd -- "$(dirname -- ${BASH_SOURCE[0]})/.." &> /dev/null && pwd)
echo "* Workspace root: ${workspaceRoot}"

if [[ "$OSTYPE" =~ ^msys ]]; then
    CMAKE="cmake -G \"MinGW Makefiles\" -DC_32BIT_COMPILER=\"C:/MinGW/Qt/5.12.12/Tools/mingw730_32/bin/gcc.exe\""
else
    CMAKE="cmake -DC_32BIT_COMPILER=\"/usr/bin/i686-w64-mingw32-gcc\""
fi

echo "* Generating build files..."
rm -Rf ${workspaceRoot}/build
eval "${CMAKE} -B ${workspaceRoot}/build -S ${workspaceRoot} -DCMAKE_BUILD_TYPE=Release"

echo "* Building project..."
cmake --build ${workspaceRoot}/build --config Release
