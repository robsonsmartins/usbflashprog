# <img align="left" src="/images/icon.png" alt="usbflashprog" title="usbflashprog">USB Flash/EPROM Programmer

A memory device programmer (Flash/EPROM/E2PROM) board and software, connected to PC by USB port.

## How to sign a Microsoft Windows&copy; Driver with a Self-Signed Certificate

Here are instructions on how to sign a Microsoft Windows&copy; driver with a self-signed certificate.

### Introduction

By default, all 64-bit versions of Microsoft Windows&copy;, starting with Windows 7, prohibit the installation of device drivers that aren't signed with a valid digital signature. Unsigned drivers are blocked by the operating system. Digital signature ensures that the driver was released by a particular developer or vendor, and its code has not been modified after it was signed.

![Error installing driver](https://i.stack.imgur.com/bxu7E.png)

On Windows 7 64-bit (x64) or newer, there are a few ways to disable driver signature verification for unsigned drivers: using a group policy or a test boot mode.

However, these approaches require changing the operating system, leaving it vulnerable.

Here it will be demonstrated how to sign any unsigned driver for the 64-bit version of Windows 7 or newer (including 8, 8.1, 10 or 11).

### Requirements

- [Microsoft Windows Software Development Kit (Windows SDK)](https://developer.microsoft.com/windows/downloads/windows-SDK/)
- [Microsoft Windows Driver Development Kit (Windows WDK)](https://docs.microsoft.com/windows-hardware/drivers/download-the-wdk)
- Windows PowerShell support
- Driver INF file

All commands must be run from a command prompt (`cmd`) with administrator privileges.

### Create a new Self-Signed Certificate

1. Create a directory to store the certificate files:

```Batchfile
md c:\mycert 
```

2. Create the certificate. Run the command:

```Batchfile
powershell -ExecutionPolicy Bypass ^
    $cert = New-SelfSignedCertificate -Subject "robsonmartins.com" -Type CodeSigningCert -CertStoreLocation cert:\LocalMachine\My; ^
    $certpassword = ConvertTo-SecureString -String "p@ssw0rd" -Force -AsPlainText; ^
    Export-Certificate -Cert $cert -FilePath "c:\mycert\mycert.cer"; ^
    Export-PfxCertificate -Cert $cert -FilePath "c:\mycert\mycert.pfx" -Password $certpassword
```

**Note**: `p@ssw0rd` can be changed with a valid password string.

The newly created self-signed certificate will be in the `c:\mycert\mycert.pfx` file (public and private keys); and the `c:\mycert\mycert.cer` file (public key only).

### Create the CAT file from the INF file

1. Create a directory to store the driver files (INF and CAT):

```Batchfile
md c:\mydriver
```

2. Copy the INF file to this directory:

```Batchfile
copy <INF_FILE> c:\mydriver\
```

3. Create the CAT file. Run the command:

```Batchfile
<WINDDK_BIN_PATH>\selfsign\inf2cat.exe /driver:"c:\mydriver" /os:7_X64,7_X86 /verbose
```

**Note**: `/os:7_X64,7_X86` defines that the driver is intended for both 64-bit and 32-bit Windows 7 versions. This parameter can be changed according to the driver's target.

The newly created CAT file will be in the `c:\mydriver\<INF_NAME>.cat` file.

### Sign the Driver

1. Sign the CAT file. Run the command:

```Batchfile
<WINSDK_BIN_PATH>\signtool.exe sign /fd sha1 /f "c:\mycert\mycert.pfx" /p p@ssw0rd /t "http://timestamp.digicert.com" /v "c:\mydriver\<CAT_FILE_NAME>.cat"
```
**Note**: `p@ssw0rd` must be the same password set when creating the self-signed certificate.

The CAT file contains digital signatures (fingerprints) of all files that are in the driver directory (files listed in the INF file in the `CopyFiles` section). If any of these files have been changed, the checksum of the files will not match the data in the CAT file and as a result the installation of this driver will fail.

### Install the Driver into Target Machine

1. Install the certificate. Run the commands:
   
```Batchfile
<WINDDK_BIN_PATH>\<ARCH>\certmgr.exe -add <CER_FILE>.cer -s -r localMachine ROOT
<WINDDK_BIN_PATH>\<ARCH>\certmgr.exe -add <CER_FILE>.cer -s -r localMachine TRUSTEDPUBLISHER
```
2. Install the driver. Run the command:
   
```Batchfile
pnputil.exe /i /a <INF_FILE>.inf
```
