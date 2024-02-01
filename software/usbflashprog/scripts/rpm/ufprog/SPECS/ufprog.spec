%define        __spec_install_post %{nil}
%define          debug_package %{nil}
%define        __os_install_post %{_dbpath}/brp-compress
%global         _topdir %{getenv:PWD}

Summary: USB EPROM/Flash Programmer
Name: ufprog
Version: 0.3
Release: 0
License: CC-BY-NC-SA 4.0
BuildArch: x86_64
Group: Applications/Engineering
URL: https://usbflashprog.robsonmartins.com/
Requires: zlib1g,libc6,libstdc++6,libqt5core5a,libqt5gui5,libqt5widgets5,libqt5serialport5

%description
%{summary}

%build
cp -aR %{_sourcedir}/* . 

%install
rm -rf %{buildroot}
mkdir -p  %{buildroot}
cp -aR * %{buildroot}

%clean
rm -rf %{buildroot}

%pre
killall -q -9 ufprog || true

%post
chown -R root:root /opt/ufprog 2>/dev/null || true
chown root:root /usr/share/applications/ufprog.desktop 2>/dev/null || true
if [ "$1" = "configure" ] && [ -x "`which update-menus 2>/dev/null`" ]; then update-menus ; fi
echo "Installing users' desktop icons ..."
__HOMELIST="root "`ls /home`
for __USER_NAME in $__HOMELIST; do
	if [ $__USER_NAME = "root" ]; then __HOMEDIR="/root" ; else __HOMEDIR="/home/"$__USER_NAME ; fi  
	if [ -d $__HOMEDIR ]; then
		__DESKTOP=`grep XDG_DESKTOP_DIR $__HOMEDIR"/.config/user-dirs.dirs" 2>/dev/null | awk -F= '{ print $2}' | xargs | awk -F/ '{ print $2}'`
		__DESKTOP=$__HOMEDIR"/"$__DESKTOP
		if [ x"$__DESKTOP" = x ]; then __DESKTOP=$__HOMEDIR"/Desktop" ; fi
		if [ -d "$__DESKTOP" ]; then
			cp -f /usr/share/applications/ufprog.desktop "$__DESKTOP" 2>/dev/null
			chown --reference=$__HOMEDIR "$__DESKTOP/ufprog.desktop" 2>/dev/null
			chgrp --reference=$__HOMEDIR "$__DESKTOP/ufprog.desktop" 2>/dev/null
			chmod +x "$__DESKTOP/ufprog.desktop" 2>/dev/null
		fi
	fi
done

%preun
killall -q -9 ufprog || true

%postun
if [ -x "`which update-menus 2>/dev/null`" ]; then update-menus ; fi
echo "Uninstalling users' desktop icons and settings ..."
__HOMELIST="root "`ls /home`
for __USER_NAME in $__HOMELIST; do
	if [ $__USER_NAME = "root" ]; then __HOMEDIR="/root" ; else __HOMEDIR="/home/"$__USER_NAME ; fi  
	if [ -d $__HOMEDIR ]; then
		__DESKTOP=`grep XDG_DESKTOP_DIR $__HOMEDIR"/.config/user-dirs.dirs" 2>/dev/null | awk -F= '{ print $2}' | xargs | awk -F/ '{ print $2}'`
		__DESKTOP=$__HOMEDIR"/"$__DESKTOP
		if [ x"$__DESKTOP" = x ]; then __DESKTOP=$__HOMEDIR"/Desktop" ; fi
		if [ -d "$__DESKTOP" ]; then
			rm -f "$__DESKTOP/ufprog.desktop" 2>/dev/null
		fi
	fi
done
echo "Removing residual files ..."
rm -f /opt/ufprog/*~ 2>/dev/null || true

%files
/*

%changelog
* Wed Jan 31 2024  Robson Martins <robson@robsonmartins.com> 0.3
- Release 0.3
* Fri Dec 22 2023  Robson Martins <robson@robsonmartins.com> 0.2
- Release 0.2
* Tue Jun 28 2022  Robson Martins <robson@robsonmartins.com> 0.1
- First Build
