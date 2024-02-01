# Gesture-Recognition-Arduino

## Demo Video
https://www.youtube.com/watch?v=Hk8Xb-klm-c

## Download Link
https://github.com/Saim-Akhtar/Gesture-Recognition-Arduino

## Overview

This project focuses on developing a gesture recognition system to identify ideal and vertical motion through hand gestures. The primary hardware components used in this project are ESP32 and MPU (Motion Processing Unit).

## Hardware Components

- **ESP32**: This powerful microcontroller provides the necessary processing power and wireless connectivity for our gesture recognition system.

- **MPU (Motion Processing Unit)**: The MPU is employed to capture and process motion-related data, aiding in the identification of specific hand gestures.

## Edge Impulse

The heart of our gesture recognition lies in the Age Impulse module. We leverage this module to train data, allowing it to generate a library that can be seamlessly integrated into our Arduino environment. This library becomes the backbone for recognizing and interpreting hand gestures.

## Usage

To use the generated library in your Arduino project, follow these steps:

1. **Train Data with Edge Impulse:**
   - Use the Edge Impulse module to train the system with various hand gestures.
   - This training process will result in the creation of a library specific to the gestures learned.

2. **Integrate Library into Arduino:**
   - Incorporate the generated library into your Arduino project.

3. **Console Output:**
   - Display the output of the gesture recognition system on the Arduino console.
   - This allows real-time monitoring of recognized gestures.

## Getting Started

To get started with this project, follow the steps outlined in the documentation:

1. Clone this repository to your local machine.
   ```bash
   git clone https://github.com/Saim-Akhtar/Gesture-Recognition-Arduino.git
   ```

2. Set up the hardware components (ESP32 and MPU) as per the provided schematics.

3. Train the system using the Edge Impulse module.

4. Integrate the generated library into your Arduino project.

5. Upload the Arduino code to your ESP32.

6. Monitor the output on the Arduino console to observe the recognized hand gestures.

