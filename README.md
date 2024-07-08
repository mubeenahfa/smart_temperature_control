# smart_temperature_control

## Project Description

This project implements a smart temperature control system using the TM4C123G microcontroller. The system continuously senses the temperature inside a cardboard box using a BMP280 pressure and temperature sensor and maintains the temperature within a predefined range using a resistive heating pad and a peltier module. The current configuration and measurements are displayed on a Nokia 5110 LCD screen. This was a two-day coding run for my university course project.

## Table of Contents

- [Project Description](#project-description)
- [Objectives](#objectives)
- [System Functions](#system-functions)
  - [Temperature Sensing](#temperature-sensing)
  - [Controlling Temperature](#controlling-temperature)
  - [User Interface](#user-interface)
- [Hardware Components](#hardware-components)
- [Requirements and Restrictions](#requirements-and-restrictions)
- [Development Environment](#development-environment)
- [Installation and Usage](#installation-and-usage)
- [Contributions](#contributions)
- [License](#license)

## Objectives

- Interpret complex tasks and encapsulate them into sub-tasks.
- Achieve cooperation of utility modules.
- Understand the compatibility of components within a complex hardware setup.
- Write multi-task software for a complex system.
- Utilize serial communication on the TM4C123G, specifically SPI and I2C protocols.

## System Functions

### Temperature Sensing
- Continuously senses temperature using BMP280 sensor.
- Stores samples in an array of 128 elements and calculates the average temperature.

### Controlling Temperature
- Maintains temperature using a resistive heating pad for heating and a peltier module for cooling.
- Activates the heating pad or peltier based on the current temperature compared to predefined thresholds.

### User Interface
- Displays current temperature and thresholds on a Nokia 5110 LCD.
- Uses on-board LEDs to indicate temperature status:
  - Red LED: Temperature below the range.
  - Green LED: Temperature within the range.
  - Blue LED: Temperature above the range.
- Adjusts green LED brightness proportionally to the current temperature (bonus feature).
- Allows user to set temperature range using a potentiometer.

## Hardware Components

- BMP280 Pressure and Temperature Sensor
- Nokia 5110 LCD Screen
- Potentiometer
- On-board RGB LED on the TM4C123G Board
- Resistive Heating Pad
- Peltier Module

## Requirements and Restrictions

- BMP280 sensor must be read using I2C protocol.
- The system collects and averages 128 temperature samples.
- Heating pad and peltier must be driven using transistors and supplied from an external battery.
- ARM assembly language is used for reading temperature data from BMP280.
- C programming language is used for the remaining parts of the project.

## Development Environment

The project was developed using the Keil µVision IDE. The entire project folder has been uploaded, which includes all source files, header files, and project configuration.


