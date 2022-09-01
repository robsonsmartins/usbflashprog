#!/bin/bash
workspaceRoot=$(cd -- "$(dirname -- ${BASH_SOURCE[0]})/.." &> /dev/null && pwd)
echo "* Workspace root: ${workspaceRoot}"
echo "* Generating installer..."
pushd "${workspaceRoot}/scripts"
iscc.exe emuprog.iss
popd
