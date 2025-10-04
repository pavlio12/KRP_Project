# KRP Semestral Project

This project aims to provide hands-on experience with USB 2.0 device-side implementation while combining it with a human–machine interface (HMI) on the STM32H747I-DISCO board.
The STM board will act as a virtual sensor device, sending live data to a PC over the implemented USB and receiving commands back from the PC.

## Workflow
| Phase | Task | Time |
| ---   | ---  | --- |
| 1 | Setup project environment, enable USB device + LCD | 3 h
| 2 | Implement descriptors} & EP0 control transfer logic | 5 h
| 3 | Add the bulk IN/OUT endpoints | 4 h
| 4 | Implement virtual sensor generator + packet format | 3 h
| 5 | HMI screen for USB states + data | 5 h
| 6 | PC Python tool for communication | 4 h
| 7 | Integration testing | 2 h
