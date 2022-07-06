#!/bin/bash
workspaceRoot=$(cd -- "$(dirname -- ${BASH_SOURCE[0]})/.." &> /dev/null && pwd)
echo "* Workspace root: ${workspaceRoot}"
if [[ "$OSTYPE" =~ ^msys ]]; then
    echo "* Generating installer..."
    pushd "${workspaceRoot}/scripts"
    iscc.exe //Darch=win64 //Dversion=`cat "${workspaceRoot}/VERSION"` ufprog.iss
    popd
elif [[ "$OSTYPE" =~ ^FreeBSD ]]; then
    buildRoot="${workspaceRoot}/build"
    appName="ufprog"
    echo "* Copying files..."
    cp -Rf "${workspaceRoot}/scripts/freebsd" "${buildRoot}/"
    bsdRoot="${buildRoot}/freebsd"
    mkdir -p "${bsdRoot}/${appName}/opt/ufprog"
    cp -f "${buildRoot}/ufprog" "${bsdRoot}/${appName}/opt/ufprog/"
    mkdir -p "${bsdRoot}/${appName}/usr/local"
    cp -Rf "${workspaceRoot}/resources/linux/usr/share" "${bsdRoot}/${appName}/usr/local/"
    chmod +x ${bsdRoot}/${appName}/opt/ufprog/ufprog
    echo "* Generating PKG package..."
    pushd "${bsdRoot}/${appName}"
    sed -i '' -e "/^[[:space:]]*version:/ s/:.*/: \"`cat "${workspaceRoot}/VERSION"`\"/" "+MANIFEST"
    cat "+MANIFEST"
    pkg create -v -m ${bsdRoot}/${appName}/ -r ${bsdRoot}/${appName}/ -p ${bsdRoot}/${appName}/plist -o .
    mv ufprog-`cat "${workspaceRoot}/VERSION"`.pkg "${buildRoot}/ufprog-`cat "${workspaceRoot}/VERSION"`-0-x86_64.pkg"
    popd
else
    buildRoot="${workspaceRoot}/build"
    appName="ufprog"
    echo "* Copying files..."
    cp -Rf "${workspaceRoot}/scripts/deb" "${buildRoot}/"
    debRoot="${buildRoot}/deb"
    mkdir -p "${debRoot}/${appName}/opt/ufprog"
    cp -f "${buildRoot}/ufprog" "${debRoot}/${appName}/opt/ufprog/"
    cp -Rf "${workspaceRoot}/resources/linux/usr" "${debRoot}/${appName}/"
    chmod 775 ${debRoot}/${appName}/DEBIAN/*
    chmod +x ${debRoot}/${appName}/opt/ufprog/ufprog

    cp -Rf "${workspaceRoot}/scripts/rpm" "${buildRoot}/"
    rpmRoot="${buildRoot}/rpm"
    mkdir -p ${rpmRoot}/${appName}/{BUILD,BUILDROOT,RPMS,SRPMS,SOURCES}
    rpmSrcRoot="${rpmRoot}/${appName}/SOURCES"
    mkdir -p "${rpmSrcRoot}/opt/ufprog"
    cp -f "${buildRoot}/ufprog" "${rpmSrcRoot}/opt/ufprog/"
    cp -Rf "${workspaceRoot}/resources/linux/usr" "${rpmSrcRoot}/"
    chmod 775 ${rpmSrcRoot}/*
    chmod +x ${rpmSrcRoot}/opt/ufprog/ufprog

    echo "* Generating DEB package..."
    pushd "${debRoot}"
    dpkg -b ${appName}
    mv ufprog.deb "${buildRoot}/ufprog-`cat "${workspaceRoot}/VERSION"`-0-x86_64.deb"
    popd

    echo "* Generating RPM package..."
    pushd "${rpmRoot}/${appName}"
    sed -i "/^[[:space:]]*Version:/ s/:.*/: `cat "${workspaceRoot}/VERSION"`/" SPECS/ufprog.spec
    rpmbuild -bb SPECS/ufprog.spec
	mv ${rpmRoot}/${appName}/RPMS/x86_64/ufprog*.rpm "${buildRoot}/"
    popd
fi
