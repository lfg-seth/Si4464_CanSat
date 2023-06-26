TubeSat 2.0 Communication System Testing
----------------------------------------

### Contributing Team
-   Iris Okoro (<iao8@txstate.edu>)
-   Seth Smith (<setsmith@tesla.com>)

### Introduction
This page documents our testing process, setup, and findings related to the SI4464 communication chip used in TubeSat 2.0. Our objective was to establish reliable communication between two SI4464 breakout boards, controlled via Arduino Uno microcontrollers over SPI.

### Test Setup
#### Hardware
-   Microcontrollers: Two Arduino Uno units.
-   Communication Modules: Two SI4464 breakout boards.
-   Antennas: Small coil antennas soldered directly onto the breakout boards.

#### Arduino Pinouts
The following pinout mapping was used to connect the Arduino Unos with the SI4464 breakout boards:

[Include the specific pinout details here]

#### Software
-   Codebase: The Ping_Server and Ping_Client example codes from the GitHub library.
-   Configuration: The radio_config.h file from the same GitHub library (config_normal).
-   WDS: We did not use WDS for this testing other than to open the config_normal.xml in the library to see what setting were set. 

#### Antenna Configuration
In the course of this experiment, we utilized rudimentary coiled wire antennas, which were directly soldered onto the SI4464 breakout boards. These antennas, although elementary in design and construction, proved sufficient for the short-range test scenario that spanned approximately 10 feet.
It's important to note that the antennas employed during these tests are not indicative of optimal performance or final design considerations. Their use in this instance was purely for initial testing purposes and should be replaced with a more robust solution for longer range or mission-critical operations.

### RF Configuration
We used the included config_normal.h header file included in the library. The associated .xml can be used to view the configuration in detail in the WDS software. 

| Parameter           | Value        |
| ------------------- | ------------ |
| Modulation          | 2GFSK        |
| Data Rate           | 100 Kbps     |
| Deviation           | 50 KHz       |
| RX Bandwidth        | 150 KHz      |
| Base Frequency      | 433 MHz      |
| Channel Spacing     | 250 KHz      |
| CRC                 | CRC-16 (IBM) |
| Data Whitening      | Enabled      |
| Manchester Encoding | Disabled     |
| Crystal Frequency   | 30 MHz       |

### Testing Process
The testing process involved loading the Ping_Server code onto one Arduino and the Ping_Client code onto the other. The goal was to send and receive a 'ping' message between the two modules. Both Arduinos were on a single desk no further than 10 feet apart. 

### Usage of PlatformIO for Programming and Debugging
In this project, we opted to use PlatformIO, an open-source ecosystem for IoT development, over the native Arduino software for programming and debugging the Arduino Unos. Here's a rundown on how we leveraged PlatformIO:

#### Setting Up PlatformIO
PlatformIO was installed as an extension on Visual Studio Code, providing a unified, cross-platform, and consistent environment for our team to work on. This allowed us to work in a more efficient and streamlined IDE compared to the Arduino IDE.

#### Programming the Arduino Unos
We used PlatformIO for writing, compiling, and uploading the code to our Arduino Unos. We selected the Arduino Uno board and wrote our code in C++. After writing the code, we used PlatformIO's building and uploading features to compile and upload the code to the Arduino Unos.

### Test Findings
We were able to successfully communicate between both breakout boards. 

### Further Exploration
In the GitHub library, several other configuration options are available, including long-range and high-speed versions. We intend to conduct further tests using these configurations.

### Conclusion
This testing allowed us to establish a reliable communication protocol between two SI4464 breakout boards, controlled by Arduino Unos. However, this setup is not without its limitations, as we discovered during our unsuccessful tests with ESP32 microcontrollers.

### ESP32 Interoperability Issues
In our initial testing, we attempted to use ESP32 microcontrollers. However, we encountered numerous issues due to Arduino-specific interrupts in the code that weren't compatible with ESP32. These issues were persistent and varied, leading us to switch to Arduino Unos for our testing. The library could potentially be modified to support ESP32 devices but that was outside of the scope of this test.