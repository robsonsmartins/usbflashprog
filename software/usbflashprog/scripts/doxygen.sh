#!/bin/bash
workspaceRoot=$(cd -- "$(dirname -- ${BASH_SOURCE[0]})/.." &> /dev/null && pwd)
echo "* Workspace root: ${workspaceRoot}"
echo "* Generating documentation..."
rm -Rf ${workspaceRoot}/../../docs/software/html/
pushd ${workspaceRoot}
doxygen
cat doxy.log
popd
