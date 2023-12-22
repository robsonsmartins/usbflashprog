; ------------------------------------------------------------------------------
; USB EPROM/Flash Programmer
;
; Copyright (2022) Robson Martins
;
; This work is licensed under a Creative Commons Attribution-NonCommercial-
; ShareAlike 4.0 International License.
; ------------------------------------------------------------------------------

#ifndef QT_PATH
# define QT_PATH GetEnv('Qt5_DIR') + "\..\..\.."
#endif

#ifndef MINGW_PATH
# define MINGW_PATH QT_PATH
#endif

#ifndef QT_BIN_PATH
# define QT_BIN_PATH     StringChange(QT_PATH, "/", "\") + "\bin"
#endif

#ifndef QT_PLUGINS_PATH
# define QT_PLUGINS_PATH StringChange(QT_PATH, "/", "\") + "\plugins"
#endif

#ifndef MINGW_BIN_PATH
# define MINGW_BIN_PATH  StringChange(MINGW_PATH, "/", "\") + "\bin"
#endif

#ifndef BUILD_PATH
# define BUILD_PATH     SourcePath + "..\build"
#endif

#ifndef RESOURCES_PATH
# define RESOURCES_PATH SourcePath + "..\resources"
#endif

#ifndef THIRD_PATH
# define THIRD_PATH SourcePath + "..\third"
#endif

#ifndef ARCH
# define ARCH "WIN64"
#endif

#ifndef VERSION
# define VERSION "0.2"
#endif

; ------------------------------------------------------------------------------

#define WIN32_ARCH     "WIN32"

#define APP_NAME      "USB EPROM Flash Programmer"
#define APP_VER       VERSION
#define APP_VERNAME   APP_NAME + " " + APP_VER
#define APP_PUBLISHER "Robson Martins"
#define APP_URL       "https://usbflashprog.robsonmartins.com/"
#define APP_MAIN_EXE  "ufprog.exe"
#define APP_VER_REV   APP_VER + "-0"

#if WIN32_ARCH == UpperCase(ARCH)
# define OUTPUT_FILE_NAME "ufprog-" + APP_VER_REV + "-win32-setup"
#else
# define OUTPUT_FILE_NAME "ufprog-" + APP_VER_REV + "-win64-setup"
#endif

; ------------------------------------------------------------------------------

[Setup]
AppId={{34F2D442-D23F-482A-81C5-1FA7CE42C377}
AppName={#APP_NAME}
AppVersion={#APP_VER}
AppVerName={#APP_VERNAME}
AppPublisher={#APP_PUBLISHER}
AppPublisherURL={#APP_URL}
AppSupportURL={#APP_URL}
AppUpdatesURL={#APP_URL}
DefaultDirName={commonpf}\{#APP_NAME}
DefaultGroupName={#APP_VERNAME}
Compression=lzma
SolidCompression=yes
PrivilegesRequired=admin
DirExistsWarning=no
AllowUNCPath=false
UninstallRestartComputer=false
VersionInfoVersion={#APP_VER}
VersionInfoCompany={#APP_PUBLISHER}
VersionInfoDescription={#APP_VERNAME} Setup for Windows
VersionInfoCopyright={#APP_PUBLISHER}(R)
VersionInfoProductName={#APP_NAME}
VersionInfoProductVersion={#APP_VER}
MinVersion=6.1sp1
#if WIN32_ARCH != UpperCase(ARCH)
  ArchitecturesInstallIn64BitMode=x64
  ArchitecturesAllowed=x64
#endif
OutputBaseFilename={#OUTPUT_FILE_NAME}
DisableWelcomePage=False
WizardImageFile=ufprog.bmp
WizardSmallImageFile=ufprog-small.bmp
UninstallDisplayIcon={app}\ufprog.ico
OutputDir={#BUILD_PATH}

; ------------------------------------------------------------------------------

[Languages]
Name: en; MessagesFile: "compiler:Default.isl"
Name: hy; MessagesFile: "compiler:Languages\Armenian.isl"
Name: pt_br; MessagesFile: "compiler:Languages\BrazilianPortuguese.isl"
Name: ca; MessagesFile: "compiler:Languages\Catalan.isl"
Name: co; MessagesFile: "compiler:Languages\Corsican.isl"
Name: cs; MessagesFile: "compiler:Languages\Czech.isl"
Name: da; MessagesFile: "compiler:Languages\Danish.isl"
Name: nl; MessagesFile: "compiler:Languages\Dutch.isl"
Name: de; MessagesFile: "compiler:Languages\German.isl"
Name: fi; MessagesFile: "compiler:Languages\Finnish.isl"
Name: fr; MessagesFile: "compiler:Languages\French.isl"
Name: he; MessagesFile: "compiler:Languages\Hebrew.isl"
Name: is; MessagesFile: "compiler:Languages\Icelandic.isl"
Name: it; MessagesFile: "compiler:Languages\Italian.isl"
Name: ja; MessagesFile: "compiler:Languages\Japanese.isl"
Name: no; MessagesFile: "compiler:Languages\Norwegian.isl"
Name: pl; MessagesFile: "compiler:Languages\Polish.isl"
Name: pt_pt; MessagesFile: "compiler:Languages\Portuguese.isl"
Name: ru; MessagesFile: "compiler:Languages\Russian.isl"
Name: sk; MessagesFile: "compiler:Languages\Slovak.isl"
Name: sl; MessagesFile: "compiler:Languages\Slovenian.isl"
Name: es; MessagesFile: "compiler:Languages\Spanish.isl"
Name: tr; MessagesFile: "compiler:Languages\Turkish.isl"
Name: uk; MessagesFile: "compiler:Languages\Ukrainian.isl"

; ------------------------------------------------------------------------------

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"

; ------------------------------------------------------------------------------

[Files]
Source: "{#BUILD_PATH}\ufprog.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#RESOURCES_PATH}\ufprog.ico"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#RESOURCES_PATH}\win\ufprog\ufprog.inf"; DestDir: "{app}\driver"; Flags: ignoreversion; OnlyBelowVersion: 6.2
Source: "{#RESOURCES_PATH}\win\ufprog\ufprog.cat"; DestDir: "{app}\driver"; Flags: ignoreversion; OnlyBelowVersion: 6.2
Source: "{#RESOURCES_PATH}\win\ufprog.cer"; DestDir: "{tmp}"; Flags: ignoreversion; OnlyBelowVersion: 6.2
Source: "{#THIRD_PATH}\win\certmgr.exe"; DestDir: "{tmp}"; Flags: ignoreversion; OnlyBelowVersion: 6.2

#if WIN32_ARCH == UpperCase(ARCH)
  Source: "{#MINGW_BIN_PATH}\libgcc_s_dw2-1.dll"; DestDir: "{app}"; Flags: ignoreversion
#else
  Source: "{#MINGW_BIN_PATH}\libgcc_s_seh-1.dll"; DestDir: "{app}"; Flags: ignoreversion
#endif
Source: "{#MINGW_BIN_PATH}\libstdc++-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MINGW_BIN_PATH}\libwinpthread-1.dll"; DestDir: "{app}"; Flags: ignoreversion

Source: "{#QT_BIN_PATH}\Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT_BIN_PATH}\Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT_BIN_PATH}\Qt5Svg.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT_BIN_PATH}\Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT_BIN_PATH}\Qt5SerialPort.dll"; DestDir: "{app}"; Flags: ignoreversion

Source: "{#QT_PLUGINS_PATH}\platforms\qwindows.dll"; DestDir: "{app}\platforms"; Flags: ignoreversion
Source: "{#QT_PLUGINS_PATH}\imageformats\qico.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "{#QT_PLUGINS_PATH}\imageformats\qgif.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "{#QT_PLUGINS_PATH}\imageformats\qsvg.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "{#QT_PLUGINS_PATH}\imageformats\qtiff.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "{#QT_PLUGINS_PATH}\imageformats\qwebp.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "{#QT_PLUGINS_PATH}\imageformats\qicns.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "{#QT_PLUGINS_PATH}\imageformats\qjpeg.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "{#QT_PLUGINS_PATH}\imageformats\qtga.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion
Source: "{#QT_PLUGINS_PATH}\imageformats\qwbmp.dll"; DestDir: "{app}\imageformats"; Flags: ignoreversion

; ------------------------------------------------------------------------------

[Icons]
Name: "{group}\{#APP_VERNAME}"; Filename: "{app}\{#APP_MAIN_EXE}"
Name: "{group}\Project Home Page"; Filename: "{#APP_URL}"
Name: "{group}\{cm:UninstallProgram,{#APP_NAME}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#APP_VERNAME}"; Filename: "{app}\{#APP_MAIN_EXE}"; Tasks: desktopicon

; ------------------------------------------------------------------------------

[Run]
Filename: "{tmp}\certmgr.exe"; Parameters: "-add ""{tmp}\ufprog.cer"" -s -r localMachine ROOT"; Flags: runhidden waituntilterminated; OnlyBelowVersion: 6.2
Filename: "{tmp}\certmgr.exe"; Parameters: "-add ""{tmp}\ufprog.cer"" -s -r localMachine TRUSTEDPUBLISHER"; Flags: runhidden waituntilterminated; OnlyBelowVersion: 6.2
Filename: "pnputil.exe"; Parameters: "/i /a ""{app}\driver\ufprog.inf"""; Flags: runhidden waituntilterminated; OnlyBelowVersion: 6.2

; ------------------------------------------------------------------------------

[UninstallDelete]
Type: filesandordirs; Name: {app}
