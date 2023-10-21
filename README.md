# <img align="left" src="/images/icon.png" alt="usbflashprog" title="usbflashprog">USB Flash/EPROM Programmer
A memory device programmer (Flash/EPROM/E2PROM) board and software, connected to PC by USB port.

[![Latest Tag][tag-shield]][tag-url] [![Latest Commit][commit-shield]][commit-url] [![Forks][forks-shield]][forks-url] [![Stargazers][stars-shield]][stars-url] [![Open Issues][issues-shield]][issues-url] [![License][license-shield]][license-url] [![Project Site][site-shield]][site-url]

[![Build Firmware][build-firmware-shield]][build-firmware-url]
[![Build Software][build-software-shield]][build-software-url]
[![Test Software][test-software-shield]][test-software-url]
[![Test Firmware][test-firmware-shield]][test-firmware-url]
[![CodeQL][codeql-shield]][codeql-url]

## Contents

* [About the Project](#about-the-project)
* [Current Status](#current-status)
* [Releases](#releases)
* [Operating Systems](#operating-systems)
* [Documentation](#documentation)
* [Hardware Design](#hardware-design)
* [Firmware Project](#firmware-project)
* [Software Project](#software-project)
* [Contributing](#contributing)
* [License](#license)
* [Contact](#contact)
* [Reference](#reference)

## About The Project

The purpose of this board is to allow the programming, reading and verification of writable/rewritable memory devices, such as EPROM, EEPROM, Flash, SRAM, NVRAM – those with parallel bus as well as serial ones (I2C, SPI, Microwire, LPC).

In a future release, programming of some microcontroller families (eg. Microchip PIC, or 8051) may also be supported, via firmware and software upgrade.

## Current Status

The current status of this project is:

![Current Revision](https://img.shields.io/badge/Current%20Version-v0.1.0--rev.G-blue?style=plastic)
![Specifications](https://img.shields.io/badge/Specifications-almost%20stable-green?style=plastic)
![Hardware Design SCH](https://img.shields.io/badge/Hardware%20Design%20(Schematics)-almost%20stable-yellowgreen?style=plastic)
![Hardware Design PCB](https://img.shields.io/badge/Hardware%20Design%20(PCB)-not%20started%20yet-red?style=plastic)
![Firmware](https://img.shields.io/badge/Firmware-unstable-yellow?style=plastic)
![PC Software](https://img.shields.io/badge/PC%20Software-unstable-yellow?style=plastic)
![User Documentation](https://img.shields.io/badge/User%20Documentation-not%20started%20yet-red?style=plastic)
![Developer/Engineer Documentation](https://img.shields.io/badge/Developer%20and%20Engineer%20Documentation-almost%20stable-green?style=plastic)

## Releases

The releases of this project are here: [Releases of the USB Flash/EPROM Programmer](https://github.com/robsonsmartins/usbflashprog/releases).

## Operating Systems

The USB Flash/EPROM Programmer has compiled installation packages for the following Operating Systems:

### Microsoft Windows©

* Windows 7, or above (32 or 64 bits)

### GNU/Linux

* Ubuntu Linux 20.04, or above (64 bits)
* RedHat/CentOS 8, or above (64 bits)

### Apple MacOSX©

* MacOSX 10.13, or above (Intel)

### FreeBSD

* FreeBSD 13.1, or above (64 bits)

## Documentation

The most up-to-date project documentation can be accessed here: [Documentation of the USB Flash/EPROM Programmer](https://robsonsmartins.github.io/usbflashprog/).

## Hardware Design

The most up-to-date hardware design can be accessed here: [Hardware design of the USB Flash/EPROM Programmer](https://github.com/robsonsmartins/usbflashprog/blob/main/hardware/).

## Firmware Project

The most up-to-date firmware project can be accessed here: [Firmware Project of the USB Flash/EPROM Programmer](https://github.com/robsonsmartins/usbflashprog/blob/main/firmware/).

Instructions on how to build the firmware are described in following document: [Firmware Build Instructions](https://github.com/robsonsmartins/usbflashprog/blob/main/firmware/BUILD.md).

## Software Project

The most up-to-date software project can be accessed here: [Software Project of the USB Flash/EPROM Programmer](https://github.com/robsonsmartins/usbflashprog/blob/main/software/).

Instructions on how to build the software are described in following document: [Software Build Instructions](https://github.com/robsonsmartins/usbflashprog/blob/main/software/BUILD.md).

## Contributing

Please read [CONTRIBUTING](https://github.com/robsonsmartins/usbflashprog/blob/main/CONTRIBUTING.md) for more information.

## License

Distributed under the [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License][cc-by-nc-sa]. 

See [LICENSE](https://github.com/robsonsmartins/usbflashprog/blob/main/LICENSE) file for more information.

The same "Terms and Conditions" and "Privacy Policy" of the [Robson Martins Home Page](https://www.robsonmartins.com) apply here:

- [Terms and Conditions](https://www.robsonmartins.com/content/policy/terms.php)
- [Privacy Policy](https://www.robsonmartins.com/content/policy/privacy.php)

## Contact

[![Home Page][contact-site-shield]][contact-site-url] [![LinkedIn][contact-linkedin-shield]][contact-linkedin-url]

## Reference
* [EzoFlash+](http://www.ezoflash.com/) - Parallel Port EPROM/Flash Programmer.
* [MPSP](https://mpsp.robsonmartins.com) - Microchip&copy; PIC Serial Port Programmer.
* [PK2C](https://pk2c.robsonmartins.com) - Microchip&copy; PIC Kit 2 Clone Programmer.

---

This work is licensed under a [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License][cc-by-nc-sa].

[![CC BY-NC-SA 4.0][cc-by-nc-sa-image]][cc-by-nc-sa]

[cc-by-nc-sa]: http://creativecommons.org/licenses/by-nc-sa/4.0/
[cc-by-nc-sa-image]: https://licensebuttons.net/l/by-nc-sa/4.0/88x31.png

[tag-shield]: https://img.shields.io/github/v/tag/robsonsmartins/usbflashprog?style=plastic
[tag-url]: https://github.com/robsonsmartins/usbflashprog/tags
[commit-shield]: https://img.shields.io/github/last-commit/robsonsmartins/usbflashprog?style=plastic
[commit-url]: https://github.com/robsonsmartins/usbflashprog/graphs/commit-activity
[forks-shield]: https://img.shields.io/github/forks/robsonsmartins/usbflashprog?style=plastic
[forks-url]: https://github.com/robsonsmartins/usbflashprog/network/members
[stars-shield]: https://img.shields.io/github/stars/robsonsmartins/usbflashprog?style=plastic
[stars-url]: https://github.com/robsonsmartins/usbflashprog/stargazers
[issues-shield]: https://img.shields.io/github/issues/robsonsmartins/usbflashprog?style=plastic
[issues-url]: https://github.com/robsonsmartins/usbflashprog/issues
[license-shield]: https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg?style=plastic
[license-url]: https://github.com/robsonsmartins/usbflashprog/blob/master/LICENSE
[site-shield]: https://img.shields.io/badge/project%20site-usbflashprog-orange?style=plastic
[site-url]: https://usbflashprog.robsonmartins.com

[build-firmware-shield]: https://github.com/robsonsmartins/usbflashprog/actions/workflows/firmware.yml/badge.svg?style=plastic
[build-firmware-url]: https://github.com/robsonsmartins/usbflashprog/actions/workflows/firmware.yml
[build-software-shield]: https://github.com/robsonsmartins/usbflashprog/actions/workflows/software.yml/badge.svg?style=plastic
[build-software-url]: https://github.com/robsonsmartins/usbflashprog/actions/workflows/software.yml
[test-firmware-shield]: https://github.com/robsonsmartins/usbflashprog/actions/workflows/test-firmware.yml/badge.svg?style=plastic
[test-firmware-url]: https://github.com/robsonsmartins/usbflashprog/actions/workflows/test-firmware.yml
[test-software-shield]: https://github.com/robsonsmartins/usbflashprog/actions/workflows/test-software.yml/badge.svg?style=plastic
[test-software-url]: https://github.com/robsonsmartins/usbflashprog/actions/workflows/test-software.yml
[codeql-shield]: https://github.com/robsonsmartins/usbflashprog/actions/workflows/codeql-analysis.yml/badge.svg?style=plastic
[codeql-url]:https://github.com/robsonsmartins/usbflashprog/actions/workflows/codeql-analysis.yml

[contact-site-shield]: https://img.shields.io/badge/Home%20Page-robsonmartins.com-green?style=plastic
[contact-site-url]: https://www.robsonmartins.com
[contact-linkedin-shield]: https://img.shields.io/badge/LinkedIn-robsonmartins-blue?style=plastic
[contact-linkedin-url]: https://www.linkedin.com/in/robsonmartins/
