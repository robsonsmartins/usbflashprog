#!/bin/bash
workspaceRoot=$(cd -- "$(dirname -- ${BASH_SOURCE[0]})/.." &> /dev/null && pwd)
echo "* Workspace root: ${workspaceRoot}"
echo "* Copying files..."
cp -Rf "${workspaceRoot}/scripts/deb" "${workspaceRoot}/build/"
debRoot="${workspaceRoot}/build/deb/ufprog"
mkdir -p "${debRoot}/opt/ufprog"
cp -f "${workspaceRoot}/build/ufprog" "${debRoot}/opt/ufprog/"
cp -Rf "${workspaceRoot}/scripts/linux-resources/*" "${debRoot}/"
chmod 775 "${debRoot}/DEBIAN/*"
chmod +x "${debRoot}/opt/ufprog/ufprog"
echo "* Generating DEB package..."
pushd "${workspaceRoot}/build/deb"
dpkg -b ufprog
mv ufprog.deb "${workspaceRoot}/build/"
popd
