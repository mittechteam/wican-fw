Note: This offers a suggestion for how the WiCAN device could be adapted to serve as a USB-to-CAN adapter, based on our use case. This approach might be a useful addition for broader use, should you find it valuable.




<img src="https://github.com/slimelec/ollie-hw/blob/master/images/mpi_logo.png" width=300>

[www.meatpi.com](https://www.meatpi.com)
---
## MeatPi [Discord server](https://discord.gg/WXy8KQCE7V)
## Please update to the [latest firmware version](https://github.com/meatpiHQ/wican-fw/releases/)

## Order on [**Mouser**](https://www.mouser.com/c/?m=MeatPi) or [**Crowd Supply!**](https://www.crowdsupply.com/meatpi-electronics/wican)

<br/><br/>

---
![65465](https://github.com/meatpiHQ/wican-fw/assets/94690098/537b5062-cb8a-485f-9354-6c351d08aa49)

### WiCAN-USB Pinout

<p align="center">
<img src="https://user-images.githubusercontent.com/94690098/218081136-fc3da520-1851-497e-90dc-ccc5d6543f1f.png" >
</p>

### *** To activate the 120R termination resistor, simply connect the TR pin the CANH. The other end of the termination resistor is connected to CANL internally.


### WiCAN-OBD2 Pinout

<p align="center">
<img src="https://user-images.githubusercontent.com/94690098/182854687-911bae04-9bdd-4947-8363-e088e278b3b8.png" >
</p>

# **API**

- [API Documentation](https://drive.google.com/drive/folders/1qJelUAHGrn_YbNIP0Jk_KmNENG-hKbtl?usp=sharing)
- [Programming Examples](https://github.com/meatpiHQ/programming_examples/tree/master/CAN)

### Build Instructions

1. Install ESP-IDF v4.4.
2. Clone the project.
3. Open and build the project.

## Description

WiCAN is a powerful ESP32-C3-based CAN adapter for car hacking and general CAN-bus development.

WiCAN is a simple, ready-to-use solution for CAN-bus development and hacking. It accelerates development by providing vehicle-diagnostic APIs and libraries in various languages and for various operating systems.

# WiCAN-USB
![image](https://user-images.githubusercontent.com/94690098/231443956-fbf2de46-ef19-4ba5-83b1-6058ab123f56.png)

WiCAN-USB can be powered through USB or through a screw-terminal connector. The hardware and firmware is almost identical to WiCAN-OBD, but with an extended voltage range up to 36 V. Many of the projects we work on involve 12 V or 24 V trucks, and WiCAN-USB comes in handy when we need to monitor those trucks remotely, from the comfort of our desks, by configuring them to connect to Wi-Fi. And, thanks to its low cost, we don’t need to worry about breaking or losing it the WiCAN-USB device itself.

WiCAN-USB can also be used as a USB-to-CAN adapter when Wi-Fi connectivity is not available or when a hardwired connection is needed.

# WiCAN-USB as a USB-to-CAN adapter

One potential enhancement for the WiCAN device could be its use as a cost-effective alternative to traditional USB-to-CAN connectors, particularly for low-frequency CAN bus applications. 
While it may not suit all frequency ranges, it has shown promising results in scenarios with less demanding data rates and offers excellent value for its cost.

To test this approach, consider implementing WiCAN in a setup with the following changes:

### Code 

Changing the baud rate from 4M to 3M makes it compatible for use with dev-kit.

[main/wc_uart.c](https://github.com/mittechteam/wican-fw/blob/WiCAN_opensource/main/wc_uart.c)

## Setup Guide

1. **Connect the Module to Your Laptop:**
   - Connect the setup module to your laptop.
   - Connect your wican module to receive can data with the relevant device.

2. **Configure the CAN Interface:**
   - Run the following commands in your terminal to configure the CAN interface and receive data:

     ```bash
     sudo slcand -o -s8 -t sw -S 3000000 /dev/ttyUSB0 can0
     ```

     > **Tip:** Press `Tab` after typing `/dev/ttyUSB` to automatically select the correct port.

     ```bash
     sudo ifconfig can0 txqueuelen 1000
     ```

     Configures the transmit queue length for the `can0` interface.

     ```bash
     sudo ifconfig can0 up
     ```

     Activates the `can0` network interface.

     ```bash
     candump -c -ta -x can0
     ```

     Use this command to receive CAN messages.

     **Result:**

     ![CAN Data Capture](https://github.com/user-attachments/assets/c02e70c2-da89-46b1-ac8e-135100d76aac)

5. **Troubleshooting Common Issues:**
   If you encounter any problems while running these commands, follow these steps to resolve them:

   - Install `can-utils`:

     ```bash
     sudo apt install can-utils
     ```

     ![Install can-utils](https://github.com/user-attachments/assets/611a4577-ca55-4452-87d8-9ae6668efff9)

   - Install `net-tools`:

     ```bash
     sudo apt install net-tools
     ```

     ![Install net-tools](https://github.com/user-attachments/assets/ce695524-6c41-456f-976b-1228c4e95861)



