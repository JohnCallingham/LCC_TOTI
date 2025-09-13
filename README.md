# LCC_TOTI

This component is part of a suite of components which can be used as part of a program which implements an OpenLCB/LCC node. It has been developed using PlatformIO and has been tested on an Arduino Nano ESP32.

The file TOTI.h contains a single class which represents one TOTI (Train On Track Indicator).

This component has two dependencies;-
- LCC_NODE_COMPONENT_BASE: https://github.com/JohnCallingham/LCC_NODE_COMPONENT_BASE.git
- DEBOUNCE: https://github.com/JohnCallingham/DEBOUNCE.git

This will be automatically downloaded when using PlatformIO.


## PlatformIO installation

It can be installed in PlatformIO by adding the following line to platform.ini;-

lib_deps = https://github.com/JohnCallingham/LCC_TOTI.git

## Arduino IDE installation

It can be installed into the Arduino IDE by downloading the .zip file and adding to the Arduino library using Sketch -> Include Library -> Add .Zip Library
