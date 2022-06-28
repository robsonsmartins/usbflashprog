#!/bin/bash
workspaceRoot=$(cd -- "$(dirname -- ${BASH_SOURCE[0]})/.." &> /dev/null && pwd)
echo "* Workspace root: ${workspaceRoot}"
if [[ "$OSTYPE" =~ ^msys ]]; then
    echo "* Generating installer..."
    pushd "${workspaceRoot}/scripts"
    iscc.exe //Darch=win64 //Dversion=`cat "${workspaceRoot}/VERSION"` ufprog.iss
    popd
else
    echo "* Copying files..."
    cp -Rf "${workspaceRoot}/scripts/deb" "${workspaceRoot}/build/"
    debRoot="${workspaceRoot}/build/deb/ufprog"
    mkdir -p "${debRoot}/opt/ufprog"
    cp -f "${workspaceRoot}/build/ufprog" "${debRoot}/opt/ufprog/"
    cp -Rf "${workspaceRoot}/resources/linux/usr" "${debRoot}/"
    chmod 775 ${debRoot}/DEBIAN/*
    chmod +x ${debRoot}/opt/ufprog/ufprog
    echo "* Generating DEB package..."
    pushd "${workspaceRoot}/build/deb"
    dpkg -b ufprog
    mv ufprog.deb "${workspaceRoot}/build/ufprog-`cat "${workspaceRoot}/VERSION"`-0-x86_64.deb"
    popd
fi
