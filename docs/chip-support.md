# <img align="left" src="https://raw.githubusercontent.com/robsonsmartins/usbflashprog/main/images/icon.png" alt="usbflashprog" title="usbflashprog">USB Flash/EPROM Programmer

A memory device programmer (Flash/EPROM/E2PROM) board and software, connected to PC by USB port.

## Chip Support List

This document contains the most up-to-date list of devices (chips) supported and tested by the USB Flash/EPROM Programmer in the current version/revision.

## Contents

* [Parallel Devices](#parallel-devices)
    * [SRAM](#sram)
    * [EPROM 27](#eprom-27)
    * [EPROM 27C](#eprom-27c)
    * [EPROM 27C 16-bit](#eprom-27c-16-bit)
    * [Electrical Erasable EPROM](#electrical-erasable-eprom)
    * [EEPROM 28C](#eeprom-28c)
    * [EEPROM AT28C](#eeprom-at28c)
    * [Flash 28F](#flash-28f)

### Parallel Devices

#### SRAM
[Top](#usb-flasheprom-programmer)

|   Chip                                        | Manufacturer  |    ID    |  Capacity  | Bits | Pinout  | Adapter  | Software Supported | Algotithm | Adapter Tested | Software Tested |
| --------------------------------------------- |:-------------:|:--------:| ----------:|:----:|:-------:|:--------:|:------------------:|:---------:|:--------------:|:---------------:|
| [6116](/reference/datasheets/sram/6116.pdf)   | Any           | N/A      |        2KB | 8    | DIP24   | ?        | Yes                | SRAM      | No             | Yes (0.3)       |
| 6264                                          | Any           | N/A      |        8KB | 8    | DIP28   | ?        | Yes                | SRAM      | No             | Yes (0.3)       |
| 62128                                         | Any           | N/A      |       16KB | 8    | DIP28   | ?        | Yes                | SRAM      | No             | No              |
| [62256](/reference/datasheets/sram/61256.pdf) | Any           | N/A      |       32KB | 8    | DIP28   | ?        | Yes                | SRAM      | No             | Yes (0.3)       |

#### EPROM 27
[Top](#usb-flasheprom-programmer)

|   Chip                                                 | Manufacturer  |    ID    |  Capacity  | Bits | Pinout  | Adapter  | Software Supported | Algotithm | Adapter Tested | Software Tested |
| ------------------------------------------------------ |:-------------:|:--------:| ----------:|:----:|:-------:|:--------:|:------------------:|:---------:|:--------------:|:---------------:|
| [2716](/reference/datasheets/eprom/eprom27/2716.pdf)   | Any           | N/A      |        2KB | 8    | DIP24   | ?        | Yes                | EPROM     | No             | Yes (0.3)       |
| 2516                                                   | Any           | N/A      |        2KB | 8    | DIP24   | ?        | Yes                | EPROM     | No             | No              |
| K573RF2                                                | Russia        | N/A      |        2KB | 8    | DIP24   | ?        | Yes                | EPROM     | No             | No              |
| K573RF5                                                | Russia        | N/A      |        2KB | 8    | DIP24   | ?        | Yes                | EPROM     | No             | No              |
| KC573RF2                                               | Russia        | N/A      |        2KB | 8    | DIP24   | ?        | Yes                | EPROM     | No             | No              |
| [2732](/reference/datasheets/eprom/eprom27/2732.pdf)   | Any           | N/A      |        4KB | 8    | DIP24   | ?        | Yes                | EPROM     | No             | No              |
| 2532                                                   | Any           | N/A      |        4KB | 8    | DIP24   | DIP24INV | Yes                | EPROM     | No             | No              |
| 2764                                                   | Any           | N/A      |        8KB | 8    | DIP28   | ?        | Yes                | EPROM     | No             | No              |
| 2764                                                   | Any           | N/A      |        8KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| 27128                                                  | Any           | N/A      |       16KB | 8    | DIP28   | ?        | Yes                | EPROM     | No             | No              |
| 27128                                                  | Any           | N/A      |       16KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| [27256](/reference/datasheets/eprom/eprom27/27256.pdf) | Any           | N/A      |       32KB | 8    | DIP28   | ?        | Yes                | EPROM     | No             | No              |
| [27256](/reference/datasheets/eprom/eprom27/27256.pdf) | Any           | N/A      |       32KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| 27512                                                  | Any           | N/A      |       64KB | 8    | DIP28   | ?        | Yes                | EPROM     | No             | No              |
| 27512                                                  | Any           | N/A      |       64KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| 27010                                                  | Any           | N/A      |      128KB | 8    | DIP32   | ?        | Yes                | EPROM     | No             | No              |
| 27010                                                  | Any           | N/A      |      128KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| 27020                                                  | Any           | N/A      |      256KB | 8    | DIP32   | ?        | Yes                | EPROM     | No             | No              |
| 27020                                                  | Any           | N/A      |      256KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| 27040                                                  | Any           | N/A      |      512KB | 8    | DIP32   | ?        | Yes                | EPROM     | No             | No              |
| 27040                                                  | Any           | N/A      |      512KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |

#### EPROM 27C
[Top](#usb-flasheprom-programmer)

|   Chip                                                    | Manufacturer  |    ID    |  Capacity  | Bits | Pinout  | Adapter  | Software Supported | Algotithm | Adapter Tested | Software Tested |
| --------------------------------------------------------- |:-------------:|:--------:| ----------:|:----:|:-------:|:--------:|:------------------:|:---------:|:--------------:|:---------------:|
| [27C16](/reference/datasheets/eprom/eprom27c/27C16.pdf)   | Any           | N/A      |        2KB | 8    | DIP24   | ?        | Yes                | EPROM     | No             | No              |
| [27C32](/reference/datasheets/eprom/eprom27c/27Cxxx.pdf)  | Any           | N/A      |        4KB | 8    | DIP24   | ?        | Yes                | EPROM     | No             | No              |
| 25C32                                                     | Any           | N/A      |        4KB | 8    | DIP24   | ?        | Yes                | EPROM     | No             | No              |
| [27C64](/reference/datasheets/eprom/eprom27c/27Cxxx.pdf)  | Any           | N/A      |        8KB | 8    | DIP28   | ?        | Yes                | EPROM     | No             | No              |
| [27C64](/reference/datasheets/eprom/eprom27c/27Cxxx.pdf)  | Any           | N/A      |        8KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| [27C128](/reference/datasheets/eprom/eprom27c/27C128.pdf) | Any           | N/A      |       16KB | 8    | DIP28   | ?        | Yes                | EPROM     | No             | No              |
| [27C128](/reference/datasheets/eprom/eprom27c/27C128.pdf) | Any           | N/A      |       16KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| [27C256](/reference/datasheets/eprom/eprom27c/27C256.pdf) | Any           | N/A      |       32KB | 8    | DIP28   | ?        | Yes                | EPROM     | No             | Yes (0.3)       |
| [27C256](/reference/datasheets/eprom/eprom27c/27C256.pdf) | Any           | N/A      |       32KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| 87C257                                                    | Any           | N/A      |       32KB | 8    | DIP28   | ?        | Yes                | EPROM     | No             | No              |
| 87C257                                                    | Any           | N/A      |       32KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| [27C512](/reference/datasheets/eprom/eprom27c/27C512.pdf) | Any           | N/A      |       64KB | 8    | DIP28   | ?        | Yes                | EPROM     | No             | No              |
| [27C512](/reference/datasheets/eprom/eprom27c/27C512.pdf) | Any           | N/A      |       64KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| [27C010](/reference/datasheets/eprom/eprom27c/27C010.pdf) | Any           | N/A      |      128KB | 8    | DIP32   | ?        | Yes                | EPROM     | No             | No              |
| [27C010](/reference/datasheets/eprom/eprom27c/27C010.pdf) | Any           | N/A      |      128KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| 27C1000                                                   | Any           | N/A      |      128KB | 8    | DIP32   | ?        | Yes                | EPROM     | No             | No              |
| 27C1000                                                   | Any           | N/A      |      128KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| 57C1000                                                   | Any           | N/A      |      128KB | 8    | DIP32   | ?        | Yes                | EPROM     | No             | No              |
| 57C1000                                                   | Any           | N/A      |      128KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| 27C1001                                                   | Any           | N/A      |      128KB | 8    | DIP32   | ?        | Yes                | EPROM     | No             | No              |
| 27C1001                                                   | Any           | N/A      |      128KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| 27C020                                                    | Any           | N/A      |      256KB | 8    | DIP32   | ?        | Yes                | EPROM     | No             | No              |
| 27C020                                                    | Any           | N/A      |      256KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| 27C2000                                                   | Any           | N/A      |      256KB | 8    | DIP32   | ?        | Yes                | EPROM     | No             | No              |
| 27C2000                                                   | Any           | N/A      |      256KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| 27C2001                                                   | Any           | N/A      |      256KB | 8    | DIP32   | ?        | Yes                | EPROM     | No             | No              |
| 27C2001                                                   | Any           | N/A      |      256KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| 27C201                                                    | Any           | N/A      |      256KB | 8    | DIP32   | ?        | Yes                | EPROM     | No             | No              |
| 27C201                                                    | Any           | N/A      |      256KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| 27C040                                                    | Any           | N/A      |      512KB | 8    | DIP32   | ?        | Yes                | EPROM     | No             | No              |
| 27C040                                                    | Any           | N/A      |      512KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| 27C401                                                    | Any           | N/A      |      512KB | 8    | DIP32   | ?        | Yes                | EPROM     | No             | No              |
| 27C401                                                    | Any           | N/A      |      512KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| 27C4001                                                   | Any           | N/A      |      512KB | 8    | DIP32   | ?        | Yes                | EPROM     | No             | No              |
| 27C4001                                                   | Any           | N/A      |      512KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| 57C4000                                                   | Any           | N/A      |      512KB | 8    | DIP32   | ?        | Yes                | EPROM     | No             | No              |
| 57C4000                                                   | Any           | N/A      |      512KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| 27C4000                                                   | Any           | N/A      |      512KB | 8    | DIP40   | DIP42    | Yes                | EPROM     | No             | No              |
| [27C080](/reference/datasheets/eprom/eprom27c/27C080.pdf) | Any           | N/A      |        1MB | 8    | DIP32   | ?        | Yes                | EPROM     | No             | No              |
| [27C080](/reference/datasheets/eprom/eprom27c/27C080.pdf) | Any           | N/A      |        1MB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| 27C801                                                    | Any           | N/A      |        1MB | 8    | DIP32   | ?        | Yes                | EPROM     | No             | No              |
| 27C801                                                    | Any           | N/A      |        1MB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |

#### EPROM 27C 16-bit
[Top](#usb-flasheprom-programmer)

|   Chip                                                      | Manufacturer  |    ID    |  Capacity  | Bits | Pinout  | Adapter  | Software Supported | Algotithm | Adapter Tested | Software Tested |
| ----------------------------------------------------------- |:-------------:|:--------:| ----------:|:----:|:-------:|:--------:|:------------------:|:---------:|:--------------:|:---------------:|
| [27C1024](/reference/datasheets/eprom/eprom27c/27C1024.pdf) | Any           | N/A      |      128KB | 16   | DIP40   | ?        | Yes                | EPROM     | No             | No              |
| 27C210                                                      | Any           | N/A      |      128KB | 16   | DIP40   | ?        | Yes                | EPROM     | No             | No              |
| 27C220                                                      | Any           | N/A      |      256KB | 16   | DIP40   | ?        | Yes                | EPROM     | No             | No              |
| [27C202](/reference/datasheets/eprom/eprom27c/27C202.pdf)   | Any           | N/A      |      256KB | 16   | DIP40   | ?        | Yes                | EPROM     | No             | No              |
| 27C2048                                                     | Any           | N/A      |      256KB | 16   | DIP40   | ?        | Yes                | EPROM     | No             | No              |
| [27C400](/reference/datasheets/eprom/eprom27c/27C400.pdf)   | Any           | N/A      |      512KB | 16   | DIP40   | DIP42    | Yes                | EPROM     | No             | No              |
| 27C4002                                                     | Any           | N/A      |      512KB | 16   | DIP40   | ?        | Yes                | EPROM     | No             | No              |
| 27C4096                                                     | Any           | N/A      |      512KB | 16   | DIP40   | ?        | Yes                | EPROM     | No             | No              |
| 27C402                                                      | Any           | N/A      |      512KB | 16   | DIP40   | ?        | Yes                | EPROM     | No             | No              |
| 27C240                                                      | Any           | N/A      |      512KB | 16   | DIP40   | ?        | Yes                | EPROM     | No             | No              |
| [27C452](/reference/datasheets/eprom/eprom27c/27C452.pdf)   | OKI           | N/A      |      512KB | 16   | DIP40   | ?        | Yes                | EPROM     | No             | No              |
| [27C800](/reference/datasheets/eprom/eprom27c/27C800.pdf)   | Any           | N/A      |        1MB | 16   | DIP42   | ?        | Yes                | EPROM     | No             | No              |
| [27C160](/reference/datasheets/eprom/eprom27c/27C160.pdf)   | Any           | N/A      |        2MB | 16   | DIP42   | ?        | Yes                | EPROM     | No             | No              |
| [27C322](/reference/datasheets/eprom/eprom27c/27C322.pdf)   | Any           | N/A      |        4MB | 16   | DIP42   | ?        | Yes                | EPROM     | No             | No              |

#### Electrical Erasable EPROM
[Top](#usb-flasheprom-programmer)

|   Chip                                                         | Manufacturer  |    ID    |  Capacity  | Bits | Pinout  | Adapter  | Software Supported | Algotithm | Adapter Tested | Software Tested |
| -------------------------------------------------------------- |:-------------:|:--------:| ----------:|:----:|:-------:|:--------:|:------------------:|:---------:|:--------------:|:---------------:|
| W27x256                                                        | Winbond       | N/A      |       32KB | 8    | DIP28   | ?        | Yes                | EPROM     | No             | No              |
| W27x256                                                        | Winbond       | N/A      |       32KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| [W27E257](/reference/datasheets/eprom/erasable/W27E257.pdf)    | Winbond       | N/A      |       32KB | 8    | DIP28   | ?        | Yes                | EPROM     | No             | No              |
| [W27E257](/reference/datasheets/eprom/erasable/W27E257.pdf)    | Winbond       | N/A      |       32KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | Yes (0.3)       |
| [W27C512](/reference/datasheets/eprom/erasable/W27C512.pdf)    | Winbond       | N/A      |       64KB | 8    | DIP28   | ?        | Yes                | EPROM     | No             | No              |
| [W27C512](/reference/datasheets/eprom/erasable/W27C512.pdf)    | Winbond       | N/A      |       64KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | Yes (0.3)       |
| W27x010                                                        | Winbond       | N/A      |      128KB | 8    | DIP32   | ?        | Yes                | EPROM     | No             | No              |
| W27x010                                                        | Winbond       | N/A      |      128KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| W27C01                                                         | Winbond       | N/A      |      128KB | 8    | DIP32   | ?        | Yes                | EPROM     | No             | No              |
| W27C01                                                         | Winbond       | N/A      |      128KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| [W27C020](/reference/datasheets/eprom/erasable/W27C020.pdf)    | Winbond       | N/A      |      256KB | 8    | DIP32   | ?        | Yes                | EPROM     | No             | No              |
| [W27C020](/reference/datasheets/eprom/erasable/W27C020.pdf)    | Winbond       | N/A      |      256KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| [W27C02](/reference/datasheets/eprom/erasable/W27C02.pdf)      | Winbond       | N/A      |      256KB | 8    | DIP32   | ?        | Yes                | EPROM     | No             | No              |
| [W27C02](/reference/datasheets/eprom/erasable/W27C02.pdf)      | Winbond       | N/A      |      256KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| W27x040                                                        | Winbond       | N/A      |      512KB | 8    | DIP32   | ?        | Yes                | EPROM     | No             | No              |
| W27x040                                                        | Winbond       | N/A      |      512KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| MX26C512                                                       | MXIC          | N/A      |       64KB | 8    | DIP28   | ?        | Yes                | EPROM     | No             | No              |
| MX26C512                                                       | MXIC          | N/A      |       64KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| [MX26C1000](/reference/datasheets/eprom/erasable/26C1000.pdf)  | MXIC          | N/A      |      128KB | 8    | DIP32   | ?        | Yes                | EPROM     | No             | No              |
| [MX26C1000](/reference/datasheets/eprom/erasable/26C1000.pdf)  | MXIC          | N/A      |      128KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| MX26C2000                                                      | MXIC          | N/A      |      256KB | 8    | DIP32   | ?        | Yes                | EPROM     | No             | No              |
| MX26C2000                                                      | MXIC          | N/A      |      256KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| [MX26C4000](/reference/datasheets/eprom/erasable/26C4000.pdf)  | MXIC          | N/A      |      512KB | 8    | DIP32   | ?        | Yes                | EPROM     | No             | No              |
| [MX26C4000](/reference/datasheets/eprom/erasable/26C4000.pdf)  | MXIC          | N/A      |      512KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| [SST27SF256](/reference/datasheets/eprom/erasable/27SF256.pdf) | SST           | N/A      |       32KB | 8    | DIP28   | ?        | Yes                | EPROM     | No             | No              |
| [SST27SF256](/reference/datasheets/eprom/erasable/27SF256.pdf) | SST           | N/A      |       32KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| [SST27SF512](/reference/datasheets/eprom/erasable/27SF256.pdf) | SST           | N/A      |       64KB | 8    | DIP28   | ?        | Yes                | EPROM     | No             | No              |
| [SST27SF512](/reference/datasheets/eprom/erasable/27SF256.pdf) | SST           | N/A      |       64KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| [SST27SF010](/reference/datasheets/eprom/erasable/27SF256.pdf) | SST           | N/A      |      128KB | 8    | DIP32   | ?        | Yes                | EPROM     | No             | No              |
| [SST27SF010](/reference/datasheets/eprom/erasable/27SF256.pdf) | SST           | N/A      |      128KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| [SST27SF020](/reference/datasheets/eprom/erasable/27SF256.pdf) | SST           | N/A      |      256KB | 8    | DIP32   | ?        | Yes                | EPROM     | No             | No              |
| [SST27SF020](/reference/datasheets/eprom/erasable/27SF256.pdf) | SST           | N/A      |      256KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| SST27SF040                                                     | SST           | N/A      |      512KB | 8    | DIP32   | ?        | Yes                | EPROM     | No             | No              |
| SST27SF040                                                     | SST           | N/A      |      512KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| [SST37VF512](/reference/datasheets/eprom/erasable/37VF512.pdf) | SST           | N/A      |       64KB | 8    | DIP28   | ?        | Yes                | EPROM     | No             | No              |
| [SST37VF512](/reference/datasheets/eprom/erasable/37VF512.pdf) | SST           | N/A      |       64KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| [SST37VF010](/reference/datasheets/eprom/erasable/37VF512.pdf) | SST           | N/A      |      128KB | 8    | DIP32   | ?        | Yes                | EPROM     | No             | No              |
| [SST37VF010](/reference/datasheets/eprom/erasable/37VF512.pdf) | SST           | N/A      |      128KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| [SST37VF020](/reference/datasheets/eprom/erasable/37VF512.pdf) | SST           | N/A      |      256KB | 8    | DIP32   | ?        | Yes                | EPROM     | No             | No              |
| [SST37VF020](/reference/datasheets/eprom/erasable/37VF512.pdf) | SST           | N/A      |      256KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |
| [SST37VF040](/reference/datasheets/eprom/erasable/37VF512.pdf) | SST           | N/A      |      512KB | 8    | DIP32   | ?        | Yes                | EPROM     | No             | No              |
| [SST37VF040](/reference/datasheets/eprom/erasable/37VF512.pdf) | SST           | N/A      |      512KB | 8    | PLCC32  | ?        | Yes                | EPROM     | No             | No              |

#### EEPROM 28C
[Top](#usb-flasheprom-programmer)

|   Chip                                                            | Manufacturer  |    ID    |  Capacity  | Bits | Pinout  | Adapter  | Software Supported |  Algotithm   | Adapter Tested | Software Tested |
| ----------------------------------------------------------------- |:-------------:|:--------:| ----------:|:----:|:-------:|:--------:|:------------------:|:------------:|:--------------:|:---------------:|
| X2804                                                             | Xicor         | N/A      |        512 | 8    | DIP24   | ?        | Yes                | EEPROM28C64  | No             | No              |
| X2808                                                             | Xicor         | N/A      |        1KB | 8    | DIP24   | ?        | Yes                | EEPROM28C64  | No             | No              |
| [X2816C](/reference/datasheets/eeprom/eeprom28c/X2816C.pdf)       | Xicor         | N/A      |        2KB | 8    | DIP24   | ?        | Yes                | EEPROM28C64  | No             | Yes (0.3)       |
| [X2816C](/reference/datasheets/eeprom/eeprom28c/X2816C.pdf)       | Xicor         | N/A      |        2KB | 8    | PLCC32  | ?        | Yes                | EEPROM28C64  | No             | No              |
| [X28C64](/reference/datasheets/eeprom/eeprom28c/X28C64.pdf)       | Xicor         | N/A      |        8KB | 8    | DIP28   | ?        | Yes                | EEPROM28C64  | No             | No              |
| [X28C64](/reference/datasheets/eeprom/eeprom28c/X28C64.pdf)       | Xicor         | N/A      |        8KB | 8    | PLCC32  | ?        | Yes                | EEPROM28C64  | No             | No              |
| X28C256                                                           | Xicor         | N/A      |       32KB | 8    | DIP28   | ?        | Yes                | EEPROM28C256 | No             | No              |
| X28C256                                                           | Xicor         | N/A      |       32KB | 8    | PLCC32  | ?        | Yes                | EEPROM28C256 | No             | No              |
| X28C010                                                           | Xicor         | N/A      |      128KB | 8    | DIP32   | ?        | Yes                | EEPROM28C256 | No             | No              |
| X28C010                                                           | Xicor         | N/A      |      128KB | 8    | PLCC32  | ?        | Yes                | EEPROM28C256 | No             | No              |
| [28C16A](/reference/datasheets/eeprom/eeprom28c/28C16A.pdf)       | Microchip     | N/A      |        2KB | 8    | DIP24   | ?        | Yes                | EEPROM28C64  | No             | No              |
| [28C16A](/reference/datasheets/eeprom/eeprom28c/28C16A.pdf)       | Microchip     | N/A      |        2KB | 8    | PLCC32  | ?        | Yes                | EEPROM28C64  | No             | No              |
| [28C64A](/reference/datasheets/eeprom/eeprom28c/28C64A.pdf)       | Microchip     | N/A      |        8KB | 8    | DIP28   | ?        | Yes                | EEPROM28C64  | No             | No              |
| [28C64A](/reference/datasheets/eeprom/eeprom28c/28C64A.pdf)       | Microchip     | N/A      |        8KB | 8    | PLCC32  | ?        | Yes                | EEPROM28C64  | No             | No              |
| [M28C16A](/reference/datasheets/eeprom/eeprom28c/M28C16A.pdf)     | ST            | N/A      |        2KB | 8    | DIP24   | ?        | Yes                | EEPROM28C64  | No             | No              |
| [M28C16A](/reference/datasheets/eeprom/eeprom28c/M28C16A.pdf)     | ST            | N/A      |        2KB | 8    | PLCC32  | ?        | Yes                | EEPROM28C64  | No             | No              |
| [M28C17A](/reference/datasheets/eeprom/eeprom28c/M28C16A.pdf)     | ST            | N/A      |        2KB | 8    | DIP24   | ?        | Yes                | EEPROM28C64  | No             | No              |
| [M28C17A](/reference/datasheets/eeprom/eeprom28c/M28C16A.pdf)     | ST            | N/A      |        2KB | 8    | PLCC32  | ?        | Yes                | EEPROM28C64  | No             | No              |
| [M28C64](/reference/datasheets/eeprom/eeprom28c/M28C64.pdf)       | ST            | N/A      |        8KB | 8    | DIP28   | ?        | Yes                | EEPROM28C64  | No             | No              |
| [M28C64](/reference/datasheets/eeprom/eeprom28c/M28C64.pdf)       | ST            | N/A      |        8KB | 8    | PLCC32  | ?        | Yes                | EEPROM28C64  | No             | No              |
| [CAT28C64A](/reference/datasheets/eeprom/eeprom28c/CAT28C64A.pdf) | Catalyst      | N/A      |        8KB | 8    | DIP28   | ?        | Yes                | EEPROM28C64  | No             | No              |
| [CAT28C64A](/reference/datasheets/eeprom/eeprom28c/CAT28C64A.pdf) | Catalyst      | N/A      |        8KB | 8    | PLCC32  | ?        | Yes                | EEPROM28C64  | No             | No              |
| CAT28C65                                                          | Catalyst      | N/A      |        8KB | 8    | DIP28   | ?        | Yes                | EEPROM28C64  | No             | No              |
| CAT28C65                                                          | Catalyst      | N/A      |        8KB | 8    | PLCC32  | ?        | Yes                | EEPROM28C64  | No             | No              |
| [28C16](/reference/datasheets/eeprom/eeprom28c/M28C16A.pdf)       | Any           | N/A      |        2KB | 8    | DIP24   | ?        | Yes                | EEPROM28C64  | No             | No              |
| [28C16](/reference/datasheets/eeprom/eeprom28c/M28C16A.pdf)       | Any           | N/A      |        2KB | 8    | PLCC32  | ?        | Yes                | EEPROM28C64  | No             | No              |
| [28C64](/reference/datasheets/eeprom/eeprom28c/M28C64.pdf)        | Any           | N/A      |        8KB | 8    | DIP28   | ?        | Yes                | EEPROM28C64  | No             | No              |
| [28C64](/reference/datasheets/eeprom/eeprom28c/M28C64.pdf)        | Any           | N/A      |        8KB | 8    | PLCC32  | ?        | Yes                | EEPROM28C64  | No             | No              |
| 28C65                                                             | Any           | N/A      |        8KB | 8    | DIP28   | ?        | Yes                | EEPROM28C64  | No             | No              |
| 28C65                                                             | Any           | N/A      |        8KB | 8    | PLCC32  | ?        | Yes                | EEPROM28C64  | No             | No              |
| 58C65                                                             | Hitachi       | N/A      |        8KB | 8    | DIP28   | ?        | Yes                | EEPROM28C64  | No             | No              |
| 28C128                                                            | Any           | N/A      |       16KB | 8    | DIP28   | ?        | Yes                | EEPROM28C64  | No             | No              |
| 28C128                                                            | Any           | N/A      |       16KB | 8    | PLCC32  | ?        | Yes                | EEPROM28C64  | No             | No              |
| 28C256                                                            | Any           | N/A      |       32KB | 8    | DIP28   | ?        | Yes                | EEPROM28C256 | No             | No              |
| 28C256                                                            | Any           | N/A      |       32KB | 8    | PLCC32  | ?        | Yes                | EEPROM28C256 | No             | No              |
| 28C512                                                            | Any           | N/A      |       64KB | 8    | DIP28   | ?        | Yes                | EEPROM28C256 | No             | No              |
| 28C512                                                            | Any           | N/A      |       64KB | 8    | PLCC32  | ?        | Yes                | EEPROM28C256 | No             | No              |
| 28C010                                                            | Any           | N/A      |      128KB | 8    | DIP32   | ?        | Yes                | EEPROM28C256 | No             | No              |
| 28C010                                                            | Any           | N/A      |      128KB | 8    | PLCC32  | ?        | Yes                | EEPROM28C256 | No             | No              |
| 28C020                                                            | Any           | N/A      |      256KB | 8    | DIP32   | ?        | Yes                | EEPROM28C256 | No             | No              |
| 28C020                                                            | Any           | N/A      |      256KB | 8    | PLCC32  | ?        | Yes                | EEPROM28C256 | No             | No              |
| 28C040                                                            | Any           | N/A      |      512KB | 8    | DIP32   | ?        | Yes                | EEPROM28C256 | No             | No              |
| 28C040                                                            | Any           | N/A      |      512KB | 8    | PLCC32  | ?        | Yes                | EEPROM28C256 | No             | No              |

#### EEPROM AT28C
[Top](#usb-flasheprom-programmer)

|   Chip                                                              | Manufacturer  |    ID    |  Capacity  | Bits | Pinout  | Adapter  | Software Supported |  Algotithm   | Adapter Tested | Software Tested |
| ------------------------------------------------------------------- |:-------------:|:--------:| ----------:|:----:|:-------:|:--------:|:------------------:|:------------:|:--------------:|:---------------:|
| AT28C16                                                             | Atmel         | N/A      |        2KB | 8    | DIP24   | ?        | Yes                | EEPROM28C64  | No             | No              |
| AT28C16                                                             | Atmel         | N/A      |        2KB | 8    | PLCC32  | ?        | Yes                | EEPROM28C64  | No             | No              |
| AT28C17                                                             | Atmel         | N/A      |        2KB | 8    | DIP24   | ?        | Yes                | EEPROM28C64  | No             | No              |
| AT28C17                                                             | Atmel         | N/A      |        2KB | 8    | PLCC32  | ?        | Yes                | EEPROM28C64  | No             | No              |
| [AT28C64B](/reference/datasheets/eeprom/eepromAT28c/AT28C64B.pdf)   | Atmel         | N/A      |        8KB | 8    | DIP28   | ?        | Yes                | EEPROM28C64  | No             | No              |
| [AT28C64B](/reference/datasheets/eeprom/eepromAT28c/AT28C64B.pdf)   | Atmel         | N/A      |        8KB | 8    | PLCC32  | ?        | Yes                | EEPROM28C64  | No             | No              |
| [AT28C256](/reference/datasheets/eeprom/eepromAT28c/AT28C256.pdf)   | Atmel         | N/A      |       32KB | 8    | DIP28   | ?        | Yes                | EEPROM28C256 | No             | No              |
| [AT28C256](/reference/datasheets/eeprom/eepromAT28c/AT28C256.pdf)   | Atmel         | N/A      |       32KB | 8    | PLCC32  | ?        | Yes                | EEPROM28C256 | No             | No              |
| AT28C512                                                            | Atmel         | N/A      |       64KB | 8    | DIP28   | ?        | Yes                | EEPROM28C256 | No             | No              |
| AT28C512                                                            | Atmel         | N/A      |       64KB | 8    | PLCC32  | ?        | Yes                | EEPROM28C256 | No             | No              |
| AT28C513                                                            | Atmel         | N/A      |       64KB | 8    | DIP28   | ?        | Yes                | EEPROM28C256 | No             | No              |
| AT28C513                                                            | Atmel         | N/A      |       64KB | 8    | PLCC32  | ?        | Yes                | EEPROM28C256 | No             | No              |
| [AT28C010](/reference/datasheets/eeprom/eepromAT28c/AT28C010.pdf)   | Atmel         | N/A      |      128KB | 8    | DIP32   | ?        | Yes                | EEPROM28C256 | No             | No              |
| [AT28C010](/reference/datasheets/eeprom/eepromAT28c/AT28C010.pdf)   | Atmel         | N/A      |      128KB | 8    | PLCC32  | ?        | Yes                | EEPROM28C256 | No             | No              |
| AT28C020                                                            | Atmel         | N/A      |      256KB | 8    | DIP32   | ?        | Yes                | EEPROM28C256 | No             | No              |
| AT28C020                                                            | Atmel         | N/A      |      256KB | 8    | PLCC32  | ?        | Yes                | EEPROM28C256 | No             | No              |
| [AT28C040](/reference/datasheets/eeprom/eepromAT28c/AT28C040.pdf)   | Atmel         | N/A      |      512KB | 8    | DIP32   | ?        | Yes                | EEPROM28C256 | No             | No              |
| [AT28C040](/reference/datasheets/eeprom/eepromAT28c/AT28C040.pdf)   | Atmel         | N/A      |      512KB | 8    | PLCC32  | ?        | Yes                | EEPROM28C256 | No             | No              |
| [CAT28C256](/reference/datasheets/eeprom/eepromAT28c/CAT28C256.pdf) | Catalyst      | N/A      |       32KB | 8    | DIP28   | ?        | Yes                | EEPROM28C256 | No             | No              |
| [CAT28C256](/reference/datasheets/eeprom/eepromAT28c/CAT28C256.pdf) | Catalyst      | N/A      |       32KB | 8    | PLCC32  | ?        | Yes                | EEPROM28C256 | No             | No              |
| [CAT28C512](/reference/datasheets/eeprom/eepromAT28c/CAT28C512.pdf) | Catalyst      | N/A      |       64KB | 8    | DIP28   | ?        | Yes                | EEPROM28C256 | No             | No              |
| [CAT28C512](/reference/datasheets/eeprom/eepromAT28c/CAT28C512.pdf) | Catalyst      | N/A      |       64KB | 8    | PLCC32  | ?        | Yes                | EEPROM28C256 | No             | No              |

#### Flash 28F
[Top](#usb-flasheprom-programmer)

|   Chip      | Manufacturer  |    ID    |  Capacity  | Bits | Pinout  | Adapter  | Software Supported | Algotithm | Adapter Tested | Software Tested |
| ----------- |:-------------:|:--------:| ----------:|:----:|:-------:|:--------:|:------------------:|:---------:|:--------------:|:---------------:|
|             |               |          |            |      |         |          |                    |           |                |                 |
