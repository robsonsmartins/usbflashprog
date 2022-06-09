# <img align="left" src="/images/icon.png" alt="usbflashprog" title="usbflashprog">USB Flash/EPROM Programmer

A memory device programmer (Flash/EPROM/E2PROM) board and software, connected to PC by USB port.

## Build Instructions

Here are instructions on how to build the project.

## Contents

* [GNU/Linux&copy;](#gnulinux)
	* [Requirements](#requirements)
	* [Install Packages](#install-packages)
	* [Install Pico C SDK](#install-pico-c-sdk)
	* [Install Visual Studio Code \[Optional\]](#install-visual-studio-code-optional)
	* [Build](#build)
	* [Generate Doxygen Documentation \[Optional\]](#generate-doxygen-documentation-optional)
* [Microsoft Windows&copy;](#microsoft-windows)
	* [Requirements](#requirements)
	* [Install Git](#install-git)
	* [Install CMake](#install-cmake)
	* [Install GNU ARM Embedded Toolchain](#install-gnu-arm-embedded-toolchain)
	* [Install MSys2 and MinGW](#install-msys2-and-mingw)
	* [Install Python](#install-python)
	* [Install Pico C SDK](#install-pico-c-sdk)
	* [Install Doxygen \[Optional\]](#install-doxygen-optional)
	* [Install GraphViz \[Optional\]](#install-graphviz-optional)
	* [Install Visual Studio Code \[Optional\]](#install-visual-studio-code-optional)
	* [Update Environment Variables](#update-environment-variables)
	* [Build](#build)
	* [Debug \[Optional\]](#debug-optional)
	* [Generate Doxygen Documentation \[Optional\]](#generate-doxygen-documentation-optional)

## GNU/Linux&copy;

### Requirements

- GNU/Linux (tested with [Ubuntu Linux 22.04 LTS](https://releases.ubuntu.com/jammy/));
- Git ([Git 1:2.34.1-1ubuntu1.2](https://packages.ubuntu.com/jammy/git));
- CMake ([CMake 3.22.1-1ubuntu1](https://packages.ubuntu.com/jammy/cmake));
- GCC-ARM ([GCC-ARM 15:10.3-2021.07-4](https://packages.ubuntu.com/jammy/gcc-arm-none-eabi));
- GNU C/C++ Compiler ([GCC 4:11.2.0-1ubuntu1](https://packages.ubuntu.com/jammy/gcc));
- Pico C SDK Repository ([pico-sdk](https://github.com/raspberrypi/pico-sdk), ~80,5MB);
- Doxygen \[Optional\] ([Doxygen 1.9.1-2ubuntu2](https://packages.ubuntu.com/jammy/doxygen));
- GraphViz \[Optional\] ([GraphViz 2.42.2-6](https://packages.ubuntu.com/jammy/graphviz));
- Visual Studio Code \[Optional\] ([code_1.67.2-1652812855_amd64.deb](https://code.visualstudio.com/sha/download?build=stable&os=linux-deb-x64), ~78.9MB)

### Install Packages

1. Run the following commands:

```shell
sudo apt update
sudo apt install git cmake doxygen graphviz build-essential gcc-arm-none-eabi libstdc++-arm-none-eabi-newlib automake autoconf texinfo libtool libftdi-dev libusb-1.0-0-dev
```

### Install Pico C SDK

The Pico SDK contains a collection of tools and libraries used to facilitate development on the Raspberry Pi Pico (and other RP2040-enabled boards).
There is also a set of C examples in the official repositories that are useful demonstrations of how to use the SDK.

1. To install the required Pico C SDK, run the following commands:

```shell
cd ~
git clone -b master https://github.com/raspberrypi/pico-sdk.git
cd pico-sdk
git submodule update --init
cd ..
echo "export PICO_SDK_PATH=$HOME/pico-sdk" >> .bashrc
source .bashrc
```

### Install Visual Studio Code \[Optional\]

The purpose of this tutorial is to configure the popular cross-platform IDE Visual Studio Code, free and provided by Microsoft, to edit and compile the project.
However, you can use another IDE instead (like Eclipse, for example), or even not use any IDE.

To install Visual Studio Code:

1. Download the latest version from the [Visual Studio Code Site](https://code.visualstudio.com/).
The version installed at the time of writing this tutorial was [code_1.67.2-1652812855_amd64.deb](https://code.visualstudio.com/sha/download?build=stable&os=linux-deb-x64).

2. Run the following command:

```shell
sudo dpkg -i code_1.67.2-1652812855_amd64.deb
```

3. After installation, open Visual Studio Code and in the "Extensions tab", install the following extensions:
- CMake
- CMake Tools
- C/C++
- Cortex-Debug

### Build

1. Clone the project from the repository:

```shell
git clone https://github.com/robsonsmartins/usbflashprog.git
```

2. Change to `firmware/usbflashprog` directory:

```shell
cd usbflashprog/firmware/usbflashprog
```

3. Run the following commands:

```shell
mkdir build
cd build
cmake ..
make -j$(nproc)
```

4. The generated firmware binary will be in `build/` directory, with the filename `usbflashprog.uf2`.

### Generate Doxygen Documentation \[Optional\]

1. Clone the project from the repository:

```shell
git clone https://github.com/robsonsmartins/usbflashprog.git
```

2. Change to `firmware/usbflashprog` directory:

```shell
cd usbflashprog/firmware/usbflashprog
```

3. Run the following commands:

```shell
doxygen
```

The documentation is generated under `usbflashprog/docs/firmware/html` directory.

## Microsoft Windows&copy;

### Requirements

- Microsoft Windows&copy; 10 or above;
- Git ([Git-2.31.1-64-bit.exe](https://github.com/git-for-windows/git/releases/download/v2.31.1.windows.1/Git-2.31.1-64-bit.exe), ~47.5MB);
- CMake ([cmake-3.23.2-windows-x86_64.msi](https://github.com/Kitware/CMake/releases/download/v3.23.2/cmake-3.23.2-windows-x86_64.msi), ~27.9MB);
- GNU ARM Embedded Toolchain ([gcc-arm-none-eabi-10-2020-q4-major-win32.exe](https://developer.arm.com/-/media/Files/downloads/gnu-rm/10-2020q4/gcc-arm-none-eabi-10-2020-q4-major-win32.exe?revision=9a4bce5a-7577-4b4f-910d-4585f55d35e8&la=en&hash=068C813EEFFB68060B5FB40E6541BDE7159AFAA0), ~122MB);
- Python ([python-3.9.4-amd64.exe](https://www.python.org/ftp/python/3.9.4/python-3.9.4-amd64.exe), ~27MB);
- Pico C SDK Repository ([pico-sdk](https://github.com/raspberrypi/pico-sdk), ~80,5MB);
- MSys2/MinGW ([msys2-x86_64-20210419.exe](https://repo.msys2.org/distrib/x86_64/msys2-x86_64-20210419.exe), ~95.8MB);
- Doxygen \[Optional\] ([doxygen-1.9.4-setup.exe](https://www.doxygen.nl/files/doxygen-1.9.4-setup.exe), ~44.4MB);
- GraphViz \[Optional\] ([stable_windows_10_cmake_Release_x64_graphviz-install-2.47.1-win64.exe](https://gitlab.com/graphviz/graphviz/-/package_files/9574245/download), ~4.5MB);
- Visual Studio Code \[Optional\] ([VSCodeSetup-x64-1.55.1.exe](https://code.visualstudio.com/docs/?dv=win64), ~68.9MB)

### Install Git

Git makes downloading the code a lot easier, and the Windows version comes with Git Bash, which is a very useful shell that makes the Microsoft Windows command line a little more effective, like GNU/Linux.
It is even possible to change the default terminal from VS Code to Git Bash, making it more powerful and similar to GNU/Linux.

1. Download the latest Git SCM installer for Microsoft Windows from the [Git for Windows Downloads Page](https://git-scm.com/download/win).
In this tutorial the [Git-2.31.1-64-bit.exe](https://github.com/git-for-windows/git/releases/download/v2.31.1.windows.1/Git-2.31.1-64-bit.exe) version was used.

2. Run the installer. Use all default options.

3. When you get to the screen asking you to "choose a default editor", feel free to choose whatever you want.
A good option is [Notepad++](https://notepad-plus-plus.org/), and if you don't have it installed, you can select Notepad (Windows default), if you don't want to use an editor in console mode.

### Install CMake

CMake is a build utility, which helps to automate the process of building programs.
Unlike Make, it does not call the compiler/linker tools, but generates (platform dependent) file and directory structures needed to run Make.

1. Download the latest version of CMake from the official website: [CMake Downloads Page](https://cmake.org/download/).
The version used in this tutorial was [cmake-3.23.2-windows-x86_64.msi](https://github.com/Kitware/CMake/releases/download/v3.23.2/cmake-3.23.2-windows-x86_64.msi).

2. Run the installer and accept the user license.

3. Under "Install Options", select "Add CMake to the system PATH for all users".

4. In "Destination Folder", write a short and easy path, for example `c:\cmake`.

### Install GNU ARM Embedded Toolchain

The GNU ARM Embedded Toolchain package contains the set of ARM compilers needed to compile the C and C++ source code for the RP2040 microcontroller (present on the Raspberry Pi Pico).

1. Open the [GNU ARM Embedded Toolchain downloads page](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads), and download the latest installer for Windows.
At the time of writing this tutorial, the version used was [gcc-arm-none-eabi-10-2020-q4-major-win32.exe](https://developer.arm.com/-/media/Files/downloads/gnu-rm/10-2020q4/gcc-arm-none-eabi-10-2020-q4-major-win32.exe?revision=9a4bce5a-7577-4b4f-910d-4585f55d35e8&la=en&hash=068C813EEFFB68060B5FB40E6541BDE7159AFAA0).

2. Run the installer. When prompted, change the install location to a short and easy path, for example `c:\gcc-arm`.
The installer will automatically populate the destination folder with the name of the current version of the toolset.

3. Continue with the installation process. On the last screen, leave all options selected.

### Install MSys2 and MinGW

MSys2 is a collection of tools and libraries that provide an easy-to-use environment for building, installing and running native software for Microsoft Windows.
It offers a terminal (command line) similar to the GNU/Linux console, several standard POSIX and GNU commands and utilities, and a package installer similar to the Arch Linux distribution, Pacman.

1. To install MSys2, download the latest version of the installer package from [msys2.org](https://www.msys2.org/).
In this tutorial, the [msys2-x86_64-20210419.exe](https://repo.msys2.org/distrib/x86_64/msys2-x86_64-20210419.exe) version was used.

2. Run the installer. Enter the desired installation folder (short path with no accents, no spaces, no symlinks, no subdirectories or network drives).

3. When finished, check Run MSYS2 64bit now.

To install MinGW on MSys2, follow the steps:

1. First, update the package database and base packages. In the open console, run the command:

```shell
pacman -Syu
```

2. After closing the MSys2 console, reopen it through the shortcut in the Start Menu, and execute the commands:

```shell
pacman -Sy
pacman -Su
```

3. Install the packages for compilation by running the command:

```shell
pacman -S mingw-w64-x86_64-toolchain git make libtool pkg-config autoconf automake texinfo git
```

4. When asked which members to `mingw-w64-x86_64-toolchain` install, press __Enter__ to install them all (default).

5. When asked about a conflict between `pkg-config` and `pkgconf`, choose option Y (Remove pkgconf).

6. Downloading and installing the packages will take a long time.

7. To configure Path in MSys2:

- In the MSys2 terminal, open the file `~/.bashrc`, like this: `nano ~/.bashrc`.
- At the end of the file, add the line `PATH=${PATH}:/c/cmake/bin` (path of the cmake).
- Save the file <Ctrl + O> and close <Ctrl + X> the editor, and run `source ~/.bashrc`.

Now it will be possible to execute and get the return of `cmake --version`.

### Install Python

Pico's SDK needs Python to script and automate some of the build functions.

The Python version recommended in the [Raspberry Pi Pico Datasheet for the C/C++ SDK](https://datasheets.raspberrypi.org/pico/getting-started-with-pico.pdf) is 3.9 or higher.
Note that this version does not support Windows 7, and Python versions lower than this may not work with Pico.

1. Download the latest Python version installer from the [Python Downloads Page](https://www.python.org/downloads/).
The version used in this tutorial was [python-3.9.4-amd64.exe](https://www.python.org/ftp/python/3.9.4/python-3.9.4-amd64.exe).

2. Run the installer.
On the first screen, keep the "Install launcher for all users (recommended)" and "Add Python 3.9 to PATH" options checked.
Choose the "Customize installation" option.

3. Under "Optional Features", leave all options checked.
Under "Advanced Options", select "Install for all users", thus leaving the first 5 options checked.
In "Customize install location", type a short path as `C:\python`.

4. Click "Install Now" and wait for Python to install.

At the end of the installation process, select the "Disable path length limit" option.
If this option is not available, or if you leave it for later, it is important to manually disable the `MAX_PATH` length limit for the Pico SDK to work.
Pico's SDK (and other SDKs from other cards) often have long, nested directory trees, resulting in pathnames that exceed the original Windows limit (260 characters).

5. \[Optional\] If you haven't selected the "Disable path length limit" option in the Python installer, or want to do it manually:

- Run `regedit` (Windows Registry Editor);
- Open the subkey `Computer\HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\FileSystem`;
- Add a value entry (if it doesn't already exist), with name `LongPathsEnabled`, of type `DWORD`;
- Change the value data to `1`;
- Before using the build tools for Pico, the computer must be restarted.

### Install Pico C SDK

The Pico SDK contains a collection of tools and libraries used to facilitate development on the Raspberry Pi Pico (and other RP2040-enabled boards).
There is also a set of C examples in the official repositories that are useful demonstrations of how to use the SDK.

1. To install the required Pico C SDK, run the following commands (inside MSys2):

```shell
cd $HOME
git clone -b master https://github.com/raspberrypi/pico-sdk.git
cd pico-sdk
git submodule update --init
cd ..
```

### Install Doxygen \[Optional\]

Doxygen is a tool designated for creating documentation (HTML, LATEX, Unix Man Pages, RTF or PostScript) from comments annotated in source code.
Supports multiple languages including C and C++.

Doxygen is not required to compile the binaries, but it can be useful if you want to generate the project's API documentation.

To install Doxygen:

1. Download the latest version for Windows from the [Doxygen Downloads Page](https://www.doxygen.nl/download.html).
The version used in this tutorial was [doxygen-1.9.4-setup.exe](https://www.doxygen.nl/files/doxygen-1.9.4-setup.exe).

2. Run the installer. When prompted, change the install location to a simple and easy path, such as `c:\doxygen`.

3. Install all components.

### Install GraphViz \[Optional\]

GraphViz is a software package capable of generating graphs from a programming API in languages such as C/C++, Python, C#, Ruby.
It's used by Doxygen to generate class diagrams, dependency between packages, etc.

GraphViz is not needed to compile binaries, and it only makes sense to install it if Doxygen is also installed.

To install GraphViz:

1. Download the latest version for Windows from the [GraphViz Downloads Page](https://graphviz.org/download/#windows).
The version used in this tutorial was [stable_windows_10_cmake_Release_x64_graphviz-install-2.47.1-win64.exe](https://gitlab.com/graphviz/graphviz/-/package_files/9574245/download).

2. When running the installer, under "Install Options", select the "Add Graphviz to the system PATH for all users" option.

3. When prompted, enter the installation location ("Destination Folder") for a short, easy path such as `c:\graphviz`.

### Install Visual Studio Code \[Optional\]

The purpose of this tutorial is to configure the popular cross-platform IDE Visual Studio Code, free and provided by Microsoft, to edit and compile the project.
However, you can use another IDE instead (like Eclipse, for example), or even not use any IDE.

To install Visual Studio Code:

1. Download the latest version from the [Visual Studio Code Site](https://code.visualstudio.com/).
The version installed at the time of writing this tutorial was [VSCodeSetup-x64-1.55.1.exe](https://code.visualstudio.com/docs/?dv=win64).

2. Visual Studio Code can be installed with all default options.

3. After installation, open Visual Studio Code and in the "Extensions tab", install the following extensions:
- CMake
- CMake Tools
- C/C++
- Qt tools

The "CMake Tools extension" must be configured to generate files for MinGW.

To do this:

1. Go to the CMake Tools extension settings, "Extension settings".

2. Find "CMake: Generator" field, and enter the value "MinGW Makefiles".

### Update Environment Variables

Some of the installed tools automatically update the Windows environment variables (specifically, the PATH).
However, some do not. Either way, this section helps you identify all the necessary variables.

To check and change environment variables, search the "Windows Search Bar" for the word "env".
Choose the option "Edit system environment variables", and click on "Environment variables...".

The following variables and inputs are required:

**`PATH`**
- `C:\msys\mingw64\bin`
- `C:\cmake\bin`
- `C:\gcc-arm\10 2020-q4-major\bin`
- `C:\Program Files\Git\cmd`
- `C:\python\Scripts\`
- `C:\python\`
- `C:\doxygen\bin` (if Doxygen was installed)
- `C:\graphviz\bin` (if GraphViz was installed)
- `C:\Program Files\Microsoft VS Code\bin` (if Visual Studio Code was installed)

**`PICO_SDK_PATH`**
- `%userprofile%\pico-sdk`

### Build

1. Clone the project from the repository (using the MSys2):

```shell
git clone https://github.com/robsonsmartins/usbflashprog.git
```

2. Change to `firmware/usbflashprog` directory:

```shell
cd usbflashprog/firmware/usbflashprog
```

3. Run the following commands:

```shell
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
make -j$(nproc)
```

4. The generated firmware binary will be in `build/` directory, with the filename `usbflashprog.uf2`.

### Debug \[Optional\]

To debug the firmware follow the tutorial [Debugging the Raspberry Pi Pico in C/C++ with VS Code and MinGW for Windows (NO Build Tools for Visual Studio!)](https://www.robsonmartins.com/content/eletr/raspi/pico/csdkwind.php).

### Generate Doxygen Documentation \[Optional\]

1. Clone the project from the repository (using the Git Bash Shell):

```shell
git clone https://github.com/robsonsmartins/usbflashprog.git
```

2. Change to `firmware/usbflashprog` directory:

```shell
cd usbflashprog/firmware/usbflashprog
```

3. Run the following commands:

```shell
doxygen
```

The documentation is generated under `usbflashprog/docs/firmware/html` directory.
