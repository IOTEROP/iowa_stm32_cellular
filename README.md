(WORK IN PROGRESS)

Changelog:
20211216:
 - Add IAR IDE support (IAR Embeded Workbench for ARM rel.: 9.10)
 - Reorg source code tree (Iowa sample is now under Misc/Samples/Iowa_Client



![IOWA Logo](https://github.com/IOTEROP/IOWA/raw/master/.images/IoTerop_logo.jpg)


# IOWA sample for STM32

- Board: B-L462E-CELL1 Discovery (https://www.st.com/en/evaluation-tools/b-l462e-cell1.html)
- IDE STM32CubeIDE 1.7 ( https://www.st.com/en/development-tools/stm32cubeide.html)
- X-Cube-CELLULAR 6.0.1 ( https://www.st.com/en/embedded-software/x-cube-cellular.html)
- IOWA (github version / Paid version)

# Setup
- **Iowa** code (from https://github.com/IOTEROP/IOWA) should be copied under *Middlewares\Third_Party\Iowa*

- Open *.project* under STM32CubeIDE 
- Run

# IOWA Connecticut server
You can interact with your device using the [Ioterop Connecticut server](https://iowa-server.ioterop.com) (https://iowa-server.ioterop.com). This server can get/send commands/datas with your device and validate the way your code runs.
(* Connecticut* is the LwM2M Ioterop test server where your device will be connected. This is one solution among others (e.g.: [Alaska platform](https://ioterop.com/alaska/), ...).

