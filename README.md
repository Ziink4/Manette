# Manette
Radio transmitter PPM decoder built on STM32F103.

## Requirements
- [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) or [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html).
- A STM32F103-equipped device (This project was built for [Maple Mini](https://github.com/leaflabs/maplemini), but porting it to other similar hardware should be feasible).

## Features
Decodes up to 8-channel PPM signals in-hardware and outputs the control data as a USB joystick using a custom HID device.

## Compatibility
Tested working on Windows 10, with a Futaba 6-channel remote.

## Credits
- [thomaspfeifer/PPM2USB](https://github.com/thomaspfeifer/PPM2USB) and his [blog](http://thomaspfeifer.net/ppm2usb_adapter_en.htm).
- [Smeat/ppm2usb](https://github.com/Smeat/ppm2usb)
