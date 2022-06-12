#!/bin/bash
workspaceRoot=$(cd -- "$(dirname -- ${BASH_SOURCE[0]})/.." &> /dev/null && pwd)
echo "* Workspace root: ${workspaceRoot}"
echo "* Generating installer..."
pushd "${workspaceRoot}/scripts"
iscc.exe //Darch=win64 //Dversion=`cat "${workspaceRoot}/VERSION"` ufprog.iss
popd
