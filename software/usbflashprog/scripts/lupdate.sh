#!/bin/bash
workspaceRoot=$(cd -- "$(dirname -- ${BASH_SOURCE[0]})/.." &> /dev/null && pwd)
echo "* Workspace root: ${workspaceRoot}"

echo "* Building project..."
${workspaceRoot}/scripts/build.sh

echo "* Updating translation files..."
pushd ${workspaceRoot}
lupdate -recursive -no-obsolete . -ts i18n/*.ts
popd
