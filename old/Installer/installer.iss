; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Lost Heaven Multiplayer"
#define MyAppVersion "1.0.9"
#define MyAppPublisher "Lost Heaven Multiplayer"
#define MyAppURL "http://vortrex.info/lhmp/"
#define MyAppExeName "launcher.exe"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{F2916169-B3CD-4710-BCD5-7ED3C4C778B3}}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyAppName}
DefaultGroupName={#MyAppName}
OutputDir=F:\abram\dev\game\lhmp\old\source\lhmp-reboot\Installer\release
OutputBaseFilename=lhmp_setup
SetupIconFile=F:\abram\dev\game\lhmp\old\source\lhmp-reboot\Installer\icon.ico
Compression=lzma
SolidCompression=yes
WizardImageFile=left.bmp
WizardSmallImageFile=right.bmp

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
                                                
[Files]
Source: "F:\abram\dev\game\lhmp\old\source\lhmp-reboot\Installer\bin\launcher.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "F:\abram\dev\game\lhmp\old\source\lhmp-reboot\Installer\bin\icudt53.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "F:\abram\dev\game\lhmp\old\source\lhmp-reboot\Installer\bin\icuin53.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "F:\abram\dev\game\lhmp\old\source\lhmp-reboot\Installer\bin\icuuc53.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "F:\abram\dev\game\lhmp\old\source\lhmp-reboot\Installer\bin\Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "F:\abram\dev\game\lhmp\old\source\lhmp-reboot\Installer\bin\Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "F:\abram\dev\game\lhmp\old\source\lhmp-reboot\Installer\bin\Qt5Network.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "F:\abram\dev\game\lhmp\old\source\lhmp-reboot\Installer\bin\Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "F:\abram\dev\game\lhmp\old\source\lhmp-reboot\Installer\bin\platforms\*"; DestDir: "{app}\platforms"; Flags: ignoreversion recursesubdirs createallsubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent shellexec