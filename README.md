# Earth Field NMR Spectrometer

This repository contains the complete design, schematics, PCB layout, microcontroller firmware, and computer software for an Earth Field Nuclear Magnetic Resonance (EFNMR) spectrometer. This device is designed to operate using the Earth's magnetic field, providing a cost-effective and accessible NMR solution for educational and research purposes.

## Overview

Nuclear Magnetic Resonance (NMR) is a powerful analytical technique used to study the physical, chemical, and biological properties of matter. Conventional NMR spectrometers require high magnetic fields, making them expensive and limited to specialized laboratories. The Earth Field NMR (EFNMR) spectrometer, however, utilizes the Earth's magnetic field, significantly reducing the cost and complexity of the device.

This project aims to develop an EFNMR spectrometer that can be used for educational purposes, allowing students and researchers to gain hands-on experience with NMR technology without the need for high-field NMR equipment.

## Components

The EFNMR spectrometer consists of the following main components:

1. **Polarizing Coil**
   - Generates a temporary magnetic field to polarize the sample.
   - Specifications:
     - Enamelled copper wire: 1.7 mm or 14 AWG
     - Inner diameter: 80 mm
     - Outer diameter: 90 mm
     - Height: 145 mm
     - Approximate inductance: 3.4 mH
     - Approximate resistance: 0.8 Ω

2. **Receiver Coils (Antenna)**
   - Detects the signal from the polarized sample.
   - Specifications:
     - Enamelled copper wire: 0.812 mm or 21 AWG
     - Inner diameter: 25 mm
     - Outer diameter: 40 mm
     - Height: 100 mm
     - Approximate inductance per coil: 2.56 mH
     - Approximate resistance per coil: 2.5 Ω

3. **Power Circuit**
   - Controls the power supply to the polarizing coil.
   - Utilizes a MOSFET for fast switching and high current handling.

4. **Receiver Circuit**
   - Amplifies and filters the weak NMR signal from the receiver coils.
   - Consists of a low-noise instrumentation amplifier, additional amplification stages, and a band-pass filter.

5. **Microcontroller (STM32F373)**
   - Handles control tasks, signal digitization, and communication with a computer.
   - Features a 16-bit ADC and USB communication.

6. **Computer Software**
   - Provides a user interface for configuring experiments, visualizing results, and performing signal processing.
   - Developed in C# for ease of use and rapid implementation of new features.

## Usage

To use the EFNMR spectrometer, follow these steps:

1. **Assemble the Hardware**
   - Ensure all components are correctly assembled and connected as per the provided schematics and PCB layout.

2. **Upload the Firmware**
   - Program the STM32F373 microcontroller with the provided firmware.

3. **Install the Software**
   - Install the computer software on a compatible system.

4. **Perform an Experiment**
   - Use the software to configure the experimental parameters (e.g., number of samples, number of scans, pulse time).
   - Align the coils with the Earth's magnetic field using the built-in magnetometer.
   - Start the experiment and visualize the results in the software.

## Repository Contents

- `PCB/`: PCB layout files for manufacturing the PCBs.
- `Firmware/`: Source code for the microcontroller firmware.
- `EFMNR/`: Source code and binaries for the computer software.
- `Assembly/`: CAD designs for 3D printed parts used in the spectrometer assembly.

## Acknowledgements

This project was developed at the University of Almeria, Spain. Special thanks to the contributors who made this project possible.

## Contact

For questions or further information, please contact [your email address].
