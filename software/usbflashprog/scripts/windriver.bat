@echo off
set CERT_PASSWORD=P@ss0wrd
set WIN_RESOURCES_DIR="%~dp0..\resources\win"
set THIRD_WIN_DIR="%~dp0..\third\win"

set /P c=Create a new certificate [y/n]? (default: y) 
if /I "%c%" EQU "y" (goto :create_cert) else (goto :sign_driver)

:create_cert
echo Creating a new certificate...
powershell -ExecutionPolicy Bypass ^
    $cert = New-SelfSignedCertificate -Subject "robsonmartins.com" -Type CodeSigningCert -CertStoreLocation cert:\LocalMachine\My; ^
    $certpassword = ConvertTo-SecureString -String "%CERT_PASSWORD%" -Force -AsPlainText; ^
    Export-Certificate -Cert $cert -FilePath "%WIN_RESOURCES_DIR%\ufprog.cer"; ^
    Export-PfxCertificate -Cert $cert -FilePath "%WIN_RESOURCES_DIR%\ufprog.pfx" -Password $certpassword

:sign_driver

set /P c=Sign the driver [y/n]? (default: y) 
if /I "%c%" EQU "n" exit

echo Signing the driver...
%THIRD_WIN_DIR%\selfsign\inf2cat.exe /driver:"%WIN_RESOURCES_DIR%\ufprog" /os:7_X64,7_X86 /verbose
%THIRD_WIN_DIR%\selfsign\signtool.exe sign /fd sha1 /f "%WIN_RESOURCES_DIR%\ufprog.pfx" /p %CERT_PASSWORD% /t "http://timestamp.digicert.com" /v "%WIN_RESOURCES_DIR%\ufprog\ufprog.cat"
