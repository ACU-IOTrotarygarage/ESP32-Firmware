# FirmwareSerial

Implementation of software serial with hardware serial fallback library for the ESP8266
Implementation of dual UART hardware serial for the ESP32
Implementation of single UART hardware serial for the ESP32-S2

Allows for several instances to be active at the same time.

Please note that due to the fact that the ESP always have other activities ongoing, there will be some inexactness in interrupt timings. This may lead to bit errors when having heavy data traffic.
