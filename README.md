# 4-Floor Elevator Controller

A comprehensive elevator control system featuring real-time monitoring, precise motor control, and modular architecture for reliable 4-floor vertical transportation.

## Features

- **4-floor control** (Ground Floor, Floor 1, Floor 2, Floor 3)
- **Real-time clock (RTC)** for time and date display
- **Temperature monitoring** using LM35 sensor
- **Motor control** with two-speed and dual-direction operation
- **7-segment display** for floor indicators
- **Modular firmware** architecture with task scheduling

## Components

### Hardware
- Microcontroller: Central controller for all components
- 7-segment displays: For floor indicator display
- Push buttons: For floor requests
- LM35 temperature sensor: For monitoring cabin temperature
- RTC (Real-Time Clock): For time and date display
- Motor driver: Controls motor speed and direction
- Limit switches: Ensures precise floor positioning

## Installation

1. Clone the repository:
    ```bash
    git clone https://github.com/Carlossawan/FirmwareProject.git
    ```

## Setup:

Open the project file in PIC-C compiler and rebuild the project. 
Open proteus and start the simulation.

## Usage

Press the floor buttons (GF, F1, F2, F3) to request the elevator to move to the desired floor. The elevator motor will adjust its speed based on proximity to the requested floor. The 7-segment display will show the current floor, and the cabin display will show time, date, and temperature.

## Troubleshooting

### Common Issues

- **Speed Inconsistency**: If the elevator moves too fast or too slow, adjust the motor control logic to ensure consistent speed.

## Future Enhancements

- Implement additional floors with modified control logic
- Add wireless monitoring and control for remote oversight
- Integrate advanced safety features like emergency brakes or door control mechanisms
