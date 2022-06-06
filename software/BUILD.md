# <img align="left" src="/images/icon.png" alt="usbflashprog" title="usbflashprog">USB Flash/EPROM Programmer

A memory device programmer (Flash/EPROM/E2PROM) board and software, connected to PC by USB port.

## Build Instructions

Here are instructions on how to build the project.

## Contents

* [GNU/Linux&copy;](#gnulinux)
	* [Requirements](#requirements)
	* [Install Packages](#install-packages)
	* [Install Visual Studio Code \[Optional\]](#install-visual-studio-code-optional)
	* [Build](#build)
	* [Generate Doxygen Documentation \[Optional\]](#generate-doxygen-documentation-optional)
* [Microsoft Windows&copy;](#microsoft-windows)
	* [Requirements](#requirements)
	* [Install Git](#install-git)
	* [Install CMake](#install-cmake)
	* [Install Qt and MinGW](#install-qt-and-mingw)
	* [Install Doxygen \[Optional\]](#install-doxygen-optional)
	* [Install GraphViz \[Optional\]](#install-graphviz-optional)
	* [Install Visual Studio Code \[Optional\]](#install-visual-studio-code-optional)
	* [Update Environment Variables](#update-environment-variables)
	* [Build](#build)
	* [Generate Installation Package \[Optional\](#generate-installation-package-optional)
	* [Generate Doxygen Documentation \[Optional\]](#generate-doxygen-documentation-optional)

## GNU/Linux&copy;

### Requirements

- GNU/Linux (tested with [Ubuntu Linux 22.04 LTS](https://releases.ubuntu.com/jammy/));
- Git ([Git 1:2.34.1-1ubuntu1.2](https://packages.ubuntu.com/jammy/git));
- CMake ([CMake 3.22.1-1ubuntu1](https://packages.ubuntu.com/jammy/cmake));
- Qt ([Qt 5.15.3-1](https://packages.ubuntu.com/jammy/qttools5-dev-tools));
- GNU C/C++ Compiler ([GCC 4:11.2.0-1ubuntu1](https://packages.ubuntu.com/jammy/gcc));
- Doxygen \[Optional\] ([Doxygen 1.9.1-2ubuntu2](https://packages.ubuntu.com/jammy/doxygen));
- GraphViz \[Optional\] ([GraphViz 2.42.2-6](https://packages.ubuntu.com/jammy/graphviz));
- Visual Studio Code \[Optional\] ([code_1.67.2-1652812855_amd64.deb](https://code.visualstudio.com/sha/download?build=stable&os=linux-deb-x64), ~78.9MB)

### Install Packages

1. Run the following commands:

```shell
sudo apt update
sudo apt install git cmake doxygen graphviz build-essential libglu1-mesa-dev libpulse-dev libglib2.0-dev
sudo apt --no-install-recommends install libqt*5-dev qt*5-dev libqt5waylandcompositor5-dev
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
- Qt tools

### Build

1. Clone the project from the repository:

```shell
git clone https://github.com/robsonsmartins/usbflashprog.git
```

2. Change to `software/usbflashprog` directory:

```shell
cd usbflashprog/software/usbflashprog
```

3. Run the following commands:

```shell
mkdir build
cd build
cmake ..
make -j$(nproc)
```

**Note**: If there is more than one version of the gcc/g++ compiler installed on the system, you can explicitly specify to CMake which compiler to use with the following command: 

```shell
cmake -DCMAKE_CXX_COMPILER=<full_path_of_compiler>/g++ ..
# e.g.: cmake -DCMAKE_CXX_COMPILER=/opt/gcc/8.1.0/bin/g++ ..
```

4. To run the program:

```shell
./ufprog
```

### Generate Doxygen Documentation \[Optional\]

1. Clone the project from the repository:

```shell
git clone https://github.com/robsonsmartins/usbflashprog.git
```

2. Change to `software/usbflashprog` directory:

```shell
cd usbflashprog/software/usbflashprog
```

3. Run the following commands:

```shell
doxygen
```

The documentation is generated under `usbflashprog/docs/software/html` directory.

## Microsoft Windows&copy;

### Requirements

- Microsoft Windows&copy; 10 or above;
- Git ([Git-2.31.1-64-bit.exe](https://github.com/git-for-windows/git/releases/download/v2.31.1.windows.1/Git-2.31.1-64-bit.exe), ~47.5MB);
- CMake ([cmake-3.23.2-windows-x86_64.msi](https://github.com/Kitware/CMake/releases/download/v3.23.2/cmake-3.23.2-windows-x86_64.msi), ~27.9MB);
- Qt and MinGW ([qt-opensource-windows-x86-5.12.12.exe](https://download.qt.io/official_releases/qt/5.12/5.12.12/qt-opensource-windows-x86-5.12.12.exe), ~3.7GB);
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

### Install Qt and MinGW

MinGW (short for Minimalist GNU for Windows) is a collection of open source compilers and linkers, which allow you to build native applications (written in languages such as C and C++) for Microsoft Windows.

Qt is a C++ framework based on libraries and tools that enables the development of powerful, interactive, cross-platform applications and devices.

The installation of both tools is performed at once, through the following procedure:

1. Download the latest version of Qt5 from the official website: [Qt Downloads Page](https://www.qt.io/offline-installers).
The version used in this tutorial was [qt-opensource-windows-x86-5.12.12.exe](https://download.qt.io/official_releases/qt/5.12/5.12.12/qt-opensource-windows-x86-5.12.12.exe).

**Note**: Qt 6 is currently not supported by this project.

2. Run the installer.

3. Log in into your Qt account or create a new free account.

4. Accept the user license.

5. In "Destination Folder", write a short and easy path, for example `C:\Qt\5.12.12`.

6. In "Select Components", check the following:

- "Qt <version>" / "MinGW <version> 64-bit" (to build 64-bit binaries)
- "Qt <version>" / "MinGW <version> 32-bit" (to build 32-bit binaries)
- "Developer and Designer Tools" / "Qt Creator <version>"
- "Developer and Designer Tools" / "MinGW <version> 64-bit" (to build both 64-bit binaries)
- "Developer and Designer Tools" / "MinGW <version> 32-bit" (to build only 32-bit binaries)

7. Accept the user license, and other default options from installer.

8. To use the tools, it will be necessary to follow procedure [Update Environment Variables](#update-environment-variables).

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
- `C:\Qt\5.12.12\5.12.12\mingw73_64\bin`
- `C:\Qt\5.12.12\Tools\mingw730_64\bin`
- `C:\cmake\bin`
- `C:\Program Files\Git\cmd`
- `C:\doxygen\bin` (if Doxygen was installed)
- `C:\graphviz\bin` (if GraphViz was installed)
- `C:\Program Files\Microsoft VS Code\bin` (if Visual Studio Code was installed)

**`Qt5_DIR`**
- `C:\Qt\5.12.12\5.12.12\mingw73_64\lib\cmake\Qt5`

### Build

1. Clone the project from the repository (using the Git Bash Shell):

```shell
git clone https://github.com/robsonsmartins/usbflashprog.git
```

2. Change to `software/usbflashprog` directory:

```shell
cd usbflashprog/software/usbflashprog
```

3. Run the following commands:

```shell
mkdir build
cd build
cmake -G "MinGW Makefiles" .. -DCMAKE_BUILD_TYPE=Release
# or, for a debug version:
# cmake -G "MinGW Makefiles" .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)
```

**Note**: If there is more than one version of the MinGW compiler installed on the system, you can explicitly specify to CMake which compiler to use with the following command: 

```shell
cmake -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=<full_path_of_compiler>/g++.exe ..
# e.g.: cmake -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=/c/Qt/5.12.12/Tools/mingw730_64/bin/g++.exe ..
```

**Note**: If there is more than one version of the Qt installed on the system, you can explicitly specify to CMake which version to use with the following command: 

```shell
cmake -G "MinGW Makefiles" -DQt5_DIR=<path_of_qt_lib_cmake_qt5> ..
# e.g.: cmake -G "MinGW Makefiles" -DQt5_DIR=/c/Qt/5.12.12/5.12.12/mingw73_64/lib/cmake/Qt5 ..
```

4. To run the program:

```shell
./ufprog.exe
```

### Generate Installation Package \[Optional\]

1. Clone the project from the repository (using the Git Bash Shell):

```shell
git clone https://github.com/robsonsmartins/usbflashprog.git
```

2. Change to `software/usbflashprog/scripts` directory:

```shell
cd usbflashprog/software/usbflashprog/scripts
```

3. Run the following commands:

```shell
iscc.exe ufprog.iss
```

**Note**: To create a package for 32-bit OS, run the following command instead:

```shell
iscc.exe //Darch=win32 ufprog.iss
```

The install package is generated under `software/usbflashprog/build` subdirectory.

**Note**: The Qt executables and the compiler libraries are found with use of the Qt5_DIR variable.
To change this, run the following command instead:

```shell
iscc.exe //DQT_PATH=<path_of_qt> //DMINGW_PATH=<path_of_mingw> ufprog.iss
# e.g.: iscc.exe //DQT_PATH="d:\\Qt\\5.12.12\\5.12.12\\mingw73_32" //DMINGW_PATH="d:\\mingw73_32" ufprog.iss
```

MINGW_PATH

### Generate Doxygen Documentation \[Optional\]

1. Clone the project from the repository (using the Git Bash Shell):

```shell
git clone https://github.com/robsonsmartins/usbflashprog.git
```

2. Change to `software/usbflashprog` directory:

```shell
cd usbflashprog/software/usbflashprog
```

3. Run the following commands:

```shell
doxygen
```

The documentation is generated under `usbflashprog/docs/software/html` directory.
