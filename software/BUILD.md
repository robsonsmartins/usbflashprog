# <img align="left" src="/images/icon.png" alt="usbflashprog" title="usbflashprog">USB Flash/EPROM Programmer

A memory device programmer (Flash/EPROM/E2PROM) board and software, connected to PC by USB port.

## Build Instructions

Here are instructions on how to build the project.

## Contents

* [GNU/Linux&copy;](#gnu/linux&copy;)
* [Microsoft Windows&copy;](#microsoft-windows&copy;)
	* [Requirements](#requirements)
	* [Install Git](#install-git)
	* [Install CMake](#install-cmake)
	* [Install Qt and MinGW](#install-qt-and-mingw)
	* [Install Doxygen \[Optional\]](#install-doxygen-\[optional\])
	* [Install GraphViz \[Optional\]](#install-graphviz-\[optional\])
	* [Install Visual Studio Code \[Optional\]](#install-visual-studio-code-\[optional\])
	* [Update Environment Variables](#update-environment-variables)
	* [Build Under Command Line](#build-under-command-line)

### GNU/Linux&copy;

TODO.

### Microsoft Windows&copy;

#### Requirements

- Microsoft Windows&copy; 10 or above;
- Git ([Git-2.31.1-64-bit.exe](https://github.com/git-for-windows/git/releases/download/v2.31.1.windows.1/Git-2.31.1-64-bit.exe), ~47.5MB);
- CMake ([cmake-3.20.1-windows-x86_64.msi](https://github.com/Kitware/CMake/releases/download/v3.20.1/cmake-3.20.1-windows-x86_64.msi), ~26.4MB);
- Qt and MinGW ([qt-opensource-windows-x86-5.12.12.exe](https://download.qt.io/official_releases/qt/5.12/5.12.12/qt-opensource-windows-x86-5.12.12.exe), ~3.7GB);
- Doxygen \[Optional\] ([doxygen-1.9.1-setup.exe](https://doxygen.nl/files/doxygen-1.9.1-setup.exe), ~47.1MB);
- GraphViz \[Optional\] ([stable_windows_10_cmake_Release_x64_graphviz-install-2.47.1-win64.exe](https://gitlab.com/graphviz/graphviz/-/package_files/9574245/download), ~4.5MB);
- Visual Studio Code \[Optional\] ([VSCodeSetup-x64-1.55.1.exe](https://code.visualstudio.com/docs/?dv=win64), ~68.9MB)

#### Install Git

Git makes downloading the code a lot easier, and the Windows version comes with Git Bash, which is a very useful shell that makes the Microsoft Windows command line a little more effective, like GNU/Linux.
It is even possible to change the default terminal from VS Code to Git Bash, making it more powerful and similar to GNU/Linux.

Download the latest Git SCM installer for Microsoft Windows from the [Git for Windows Downloads Page](https://git-scm.com/download/win).
In this tutorial the [Git-2.31.1-64-bit.exe version](https://github.com/git-for-windows/git/releases/download/v2.31.1.windows.1/Git-2.31.1-64-bit.exe) was used.

Run the installer. Use all default options. When you get to the screen asking you to choose a default editor, feel free to choose whatever you want.
A good option is [Notepad++](https://notepad-plus-plus.org/), and if you don't have it installed, you can select Notepad (Windows default), if you don't want to use an editor in console mode.

#### Install CMake

CMake is a build utility, which helps to automate the process of building programs.
Unlike Make, it does not call the compiler/linker tools, but generates (platform dependent) file and directory structures needed to run Make.

Download the latest version of CMake from the official website: [CMake Downloads Page](https://cmake.org/download/).
The version used in this tutorial was [cmake-3.20.1-windows-x86_64.msi](https://github.com/Kitware/CMake/releases/download/v3.20.1/cmake-3.20.1-windows-x86_64.msi).

Run the installer and accept the user license. Under "Install Options", select "Add CMake to the system PATH for all users".

In Destination Folder, write a short and easy path, for example `c:\cmake`.

#### Install Qt and MinGW

TODO

#### Install Doxygen \[Optional\]

Doxygen is a tool designated for creating documentation (HTML, LATEX, Unix Man Pages, RTF or PostScript) from comments annotated in source code.
Supports multiple languages including C and C++.

Doxygen is not required to compile the binaries, but it can be useful if you want to generate the project's API documentation.

To install Doxygen, download the latest version for Windows from the [Doxygen Downloads Page](https://www.doxygen.nl/download.html).
The version used in this tutorial was [doxygen-1.9.1-setup.exe](https://doxygen.nl/files/doxygen-1.9.1-setup.exe).

Run the installer. When prompted, change the install location to a simple and easy path, such as `c:\doxygen`. Install all components.

#### Install GraphViz \[Optional\]

GraphViz is a software package capable of generating graphs from a programming API in languages such as C/C++, Python, C#, Ruby.
It's used by Doxygen to generate class diagrams, dependency between packages, etc.

GraphViz is not needed to compile binaries, and it only makes sense to install it if Doxygen is also installed.

To install GraphViz, download the latest version for Windows from the [GraphViz Downloads Page](https://graphviz.org/download/#windows).
The version used in this tutorial was [stable_windows_10_cmake_Release_x64_graphviz-install-2.47.1-win64.exe](https://gitlab.com/graphviz/graphviz/-/package_files/9574245/download).

When running the installer, under "Install Options", select the "Add Graphviz to the system PATH for all users" option.

When prompted, enter the installation location ("Destination Folder") for a short, easy path such as `c:\graphviz`.

#### Install Visual Studio Code \[Optional\]

The purpose of this tutorial is to configure the popular cross-platform IDE Visual Studio Code, free and provided by Microsoft, to edit and compile the project.
However, you can use another IDE instead (like Eclipse, for example), or even not use any IDE.

To install Visual Studio Code, you must download the latest version from the [Visual Studio Code Site](https://code.visualstudio.com/).
The version installed at the time of writing this tutorial was [VSCodeSetup-x64-1.55.1.exe](https://code.visualstudio.com/docs/?dv=win64).

Visual Studio Code can be installed with all default options.

After installation, open Visual Studio Code and in the "Extensions tab", install the following extensions:
- CMake
- CMake Tools
- C/C++
- Qt tools

The "CMake Tools extension" must be configured to generate files for MinGW.
To do this, go to the CMake Tools extension settings, "Extension settings", "CMake: Generator", and enter the value "MinGW Makefiles".

#### Update Environment Variables

Some of the installed tools automatically update the Windows environment variables (specifically, the PATH).
However, some do not. Either way, this section helps you identify all the necessary variables.

To check and change environment variables, search the "Windows Search Bar" for the word "env".
Choose the option "Edit system environment variables", and click on "Environment variables...".

The following variables and inputs are required:

**`PATH`**
- `C:\Qt\5.12.12\Tools\mingw730_64\bin`
- `C:\cmake\bin`
- `C:\Program Files\Git\cmd`
- `C:\doxygen\bin` (if Doxygen was installed)
- `C:\graphviz\bin` (if GraphViz was installed)
- `C:\Program Files\Microsoft VS Code\bin` (if Visual Studio Code was installed)

**`Qt5_DIR`**
- `C:\Qt\5.12.12\5.12.12\mingw73_64\lib\cmake\Qt5`

#### Build Under Command Line

TODO
