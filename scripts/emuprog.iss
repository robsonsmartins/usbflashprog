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

#ifndef VERSION
# define VERSION "1.0"
#endif

; ------------------------------------------------------------------------------

#define APP_NAME      "EmuProg Programmer"
#define APP_VER       VERSION
#define APP_VERNAME   APP_NAME + " " + APP_VER
#define APP_PUBLISHER "Robson Martins"
#define APP_URL       "https://usbflashprog.robsonmartins.com/"
#define APP_MAIN_EXE  "emuprog.exe"
#define APP_VER_REV   APP_VER + "-0"

#define OUTPUT_FILE_NAME "emuprog-" + APP_VER_REV + "-win32-setup"

; ------------------------------------------------------------------------------

[Setup]
AppId={{FF240CDE-6C62-4D2C-9023-4787555E50E7}
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
OutputBaseFilename={#OUTPUT_FILE_NAME}
DisableWelcomePage=False
WizardImageFile=emuprog.bmp
WizardSmallImageFile=emuprog-small.bmp
UninstallDisplayIcon={app}\emuprog.ico
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
Source: "{#BUILD_PATH}\emuprog.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BUILD_PATH}\io.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BUILD_PATH}\willem.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BUILD_PATH}\pcb45.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BUILD_PATH}\sram.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BUILD_PATH}\eprom.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BUILD_PATH}\eeprom28c.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#RESOURCES_PATH}\emuprog.ico"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#RESOURCES_PATH}\third\epr097ja.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#RESOURCES_PATH}\third\epr098d5.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#RESOURCES_PATH}\third\epr098d10.exe"; DestDir: "{app}"; Flags: ignoreversion

Source: "{#MINGW_BIN_PATH}\libgcc_s_seh-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MINGW_BIN_PATH}\libstdc++-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MINGW_BIN_PATH}\libwinpthread-1.dll"; DestDir: "{app}"; Flags: ignoreversion

Source: "{#QT_BIN_PATH}\Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT_BIN_PATH}\Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT_BIN_PATH}\Qt5Svg.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT_BIN_PATH}\Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion

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
Name: "{group}\{cm:UninstallProgram,{#APP_NAME}}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#APP_VERNAME}"; Filename: "{app}\{#APP_MAIN_EXE}"; Tasks: desktopicon

; ------------------------------------------------------------------------------

[UninstallDelete]
Type: filesandordirs; Name: {app}
