# Using WiCAN Firmware with ESP32-DevKitM-1
## Aim
The WiCAN Firmware is supported on its native hardware but can quite easily be ported to the readily available ESP32-DevKitM-1 with minor changes outlined below:
- [changed fw to USB from ODB](https://github.com/mittechteam/wican-fw/commit/462d24fc51631157283d357ebf49e48354de8ea5)
- [changed uart baud rate to 3M from 4M](https://github.com/mittechteam/wican-fw/commit/02a89c874bfc5ae2ab74d545405d14f804ed4b70)

For this,you will need to build the codebase with the new changes and flash your ESP32-DevKitM-1.

References given [here](#USB-to-CAN) can be used for this purpose.



<img src="https://github.com/slimelec/ollie-hw/blob/master/images/mpi_logo.png" width=300>

[www.meatpi.com](https://www.meatpi.com)
---
## MeatPi [Discord server](https://discord.gg/WXy8KQCE7V)
## Please update to the [latest firmware version](https://github.com/meatpiHQ/wican-fw/releases/)

## Order on [**Mouser**](https://www.mouser.com/c/?m=MeatPi) or [**Crowd Supply!**](https://www.crowdsupply.com/meatpi-electronics/wican)

<br/><br/>

---
![65465](https://github.com/meatpiHQ/wican-fw/assets/94690098/537b5062-cb8a-485f-9354-6c351d08aa49)

## WiCAN-USB Pinout

<p align="center">
<img src="https://user-images.githubusercontent.com/94690098/218081136-fc3da520-1851-497e-90dc-ccc5d6543f1f.png" >
</p>

### *** To activate the 120R termination resistor, simply connect the TR pin the CANH. The other end of the termination resistor is connected to CANL internally.


## WiCAN-OBD2 Pinout

<p align="center">
<img src="https://user-images.githubusercontent.com/94690098/182854687-911bae04-9bdd-4947-8363-e088e278b3b8.png" >
</p>

---
## [webCAN](http://webcan.meatpi.com/) Comming soon!

- [Features Wishlist](#features-wishlist)
- [Important Notes](#important-notes)
- [API](#api)
- [Build](#build)
- [Description](#description)
- [Features](#features)
  - [Sleep mode](#1-sleep-mode)
  - [Battery Alert](#2-battery-alert)
- [Configuration](#configuration)
  - [WiFi/CAN Configuration](#1-wifican-configuration)
  - [BUSMaster](#2-busmaster)
  - [Realdash](#3-realdash)
  - [SavvyCAN](#4-savvycan)
  - [webCAN](http://webcan.meatpi.com)
- [SocketCAN](#socketcan)
  - [WiFi](#1-wifi)
  - [USB](#2-usb)
- [ELM327 OBD2 Protocol](#elm327-obd2-protocol)
- [MQTT](#mqtt)
  - [Status](#1-status)
  - [MQTT TX RX Frame](#2-mqtt-tx-rx-frame)
  - [Receive Frames](#3-receive-frames)
  - [Transmit Frames](#4-transmit-frames)
  - [OBDII PID Request Over MQTT](#5-obdii-pid-request-over-mqtt)
  - [Request Battery SoC MQTT Example](#6-request-battery-soc-mqtt-example)
  - [CAN to JSON interpreter - Filtring](#7-can-to-json-interpreter---filtring)
- [Home Assistant](#home-assistant)

- [Firmware Update](#firmware-update)
  - [OTA](#1-ota)
  - [USB Flash](#2-usb-flash)

# **Features Wishlist**:

- ~~CAN Auto baudrate~~ **DONE**
- ~~Support ELM327 OBD2 protocol~~ **DONE** (Basic support) 
- ~~Home Assistant Integrations~~ **DONE**
- Extended ELM327 support 
- Support ELM327 protocol and Realdash protocol simultaneously on WiFi and BLE 
- Add option to send an Email on battery alert 
- Expand alerts to include other CAN parameters parsed from DBC files.
- Support for extra wifi settings
# **Important Notes**:

- The OBD2 adapter is not designed to powered of the USB connecter. The USB connector can power the adapter to flash custom firmware or hardreset the deivce and can also be used for debuging.
- It is highly recommanded to turn OFF the BLE if not used. Otherwise it might affect the preformance.
- When the BLE is connected, the device configuration access point will be disabled i.e you won't be able to configure the device unless you disconnect the BLE, by turning off the BLE on your phone or device.
- If AP+Station mode is enabled, only use station IP to communicate with the device and AP must be used for configuration only.
  
# **API**
[API Documentation](https://drive.google.com/drive/folders/1qJelUAHGrn_YbNIP0Jk_KmNENG-hKbtl?usp=sharing)

[Programing examples](https://github.com/meatpiHQ/programming_examples/tree/master/CAN)

# **Build**:

1. Install ESP-IDF v4.4.
2. Clone porject.
3. Open project and build.

# **Description**:

WiCAN is a powerful ESP32-C3-based CAN adapter for car hacking and general CAN-bus development. It is available in two form factors, OBD-II and standard USB-CAN. The original firmware can use Wi-Fi or BLE to interface directly with RealDash, which allows you to create your own custom dashboards with stunning graphics. It is available for Android, iOS, and Windows 10. WiCAN connects to your existing Wi-Fi network and any device on that network, where it allows you to configure Wi-Fi and CAN settings through a built-in web interface. Both versions have a power-saving mode that detects when the voltage drops under 13 V or some other preset value. When this power-saving mode is engaged, WiCAN is capable of entering sleep mode, which drops current consumption below 1 mA.

WiCAN is a simple, ready-to-use solution for CAN-bus development and hacking. It accelerates development by providing vehicle-diagnostic APIs and libraries in various languages and for various operating systems. WiCAN works with a large array of pre-existing vehicle-diagnostic libraries, including RealDash, SavvyCAN, BUSmaster, python-can/SocketCA, and more. APIs are also available for LabView, C#, VB.Net, Delphi, and Python in case you’re writing your own software.

# WiCAN-OBD
![image](https://user-images.githubusercontent.com/94690098/231444160-08842087-55ad-4165-8291-b379da63aeeb.png)

WiCAN-OBD will be of great interest to car enthusiasts and tinkers who want to modernize or customize the head-unit displays in their cars using RealDash. 
Another great feature of WiCAN-OBD is its MQTT battery alerts. It can monitor your battery voltage and send an alert if that voltage drops under a set threshold. 

# WiCAN-USB
![image](https://user-images.githubusercontent.com/94690098/231443956-fbf2de46-ef19-4ba5-83b1-6058ab123f56.png)

WiCAN-USB can be powered through USB or through a screw-terminal connector. The hardware and firmware is almost identical to WiCAN-OBD, but with an extended voltage range up to 36 V. Many of the projects we work on involve 12 V or 24 V trucks, and WiCAN-USB comes in handy when we need to monitor those trucks remotely, from the comfort of our desks, by configuring them to connect to Wi-Fi. And, thanks to its low cost, we don’t need to worry about breaking or losing it the WiCAN-USB device itself.

WiCAN-USB can also be used as a USB-to-CAN adapter when Wi-Fi connectivity is not available or when a hardwired connection is needed.

# USB-to-CAN

One potential enhancement for the WiCAN device could be its use as a cost-effective alternative to traditional USB-to-CAN connectors, particularly for low-frequency CAN bus applications. 
While it may not suit all frequency ranges, it has shown promising results in scenarios with less demanding data rates and offers excellent value for its cost.
This can be achieved by implementing-

- Minor Code Changes:

-- [Changed baud rate from 4M to 3M](https://github.com/mittechteam/wican-fw/commit/02a89c874bfc5ae2ab74d545405d14f804ed4b70)

This needs to be done as the USB Bridge on the Development Kit has a maximum limit of 3MBPS so making these changes makes it compatible with the ESP32-DevKitM-1 module.


-- [Changed fw to USB from ODB](https://github.com/mittechteam/wican-fw/commit/462d24fc51631157283d357ebf49e48354de8ea5)


- Hardware Requirements: 

1. ESP32 module
2. Prebuild transceiver
3. Jumpers for connections.

- Building and Flashing the Code

  Refer these instructions to build and flash the code:
  -- [Build](#build) 
  -- [Flash](https://github.com/espressif/vscode-esp-idf-extension/blob/master/docs/tutorial/basic_use.md)

Run the following commands in your terminal to configure the CAN interface and receive data:

```bash
     sudo slcand -o -s8 -t sw -S 3000000 /dev/ttyUSB0 can0
     sudo ifconfig can0 txqueuelen 1000
     sudo ifconfig can0 up
     candump -c -ta -x can0
```

## [**Programming Examples**](https://github.com/meatpiHQ/programming_examples/tree/master/CAN)

### **Features and Specifications**:

- Supports CAN2.0A/B up to 1Mbits.
- Works with Realdash, based on "realdash 66"
- Supports SocketCAN and works with BUSMaster
- Supports TCP and UDP
- WiFi can be used in AP and station mode
- WiFi and CAN configured using web interface.
- Diode protection for the USB port

![Modes](https://user-images.githubusercontent.com/94690098/222961571-bd137341-808a-4f0a-9528-789fe24d640e.png "Connection Mode")

Details related to each feature can be checked out at [meatpiHQ/wican-fw](https://github.com/meatpiHQ/wican-fw/blob/main/README.md) directly.


© 2023 meatPi Electronics | www.meatpi.com | PO Box 5005 Clayton, VIC 3168, Australia
