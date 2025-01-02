# I2C-Communications-Project

This project is designed to test the basic functionality of several peripherals connected to a TM4C123 launchpad ARM Cortex M4 microcontroller using I2C communication. It specifically tests the **TCS34727 RGB Sensor**, the **MPU6050 6-DOF IMU**, a **Servo Motor**, and a **16x2 LCD Display**.

## Features
- **I2C Communication**: Read and write to the TCS34727 RGB color sensor and the MPU6050 6-DOF IMU.
- **Servo Motor Control**: Control a servo motor based on input.
- **LCD Display**: Output data to a 16x2 LCD.
- **Button and LED**: Initialize and use a button and an LED for basic interaction.
- **Modular Testing**: The code supports individual testing of each peripheral or a full system test.

## Initialization
The code initializes various peripherals based on the defined macros. Peripheral initialization happens only for the components that are enabled through the macros.

### Enabled Components
- **UART0**: Serial communication is initialized.
- **LED** and **Button**: Set up for testing.
- **WTIMER0**: Timer used for various time-dependent operations.
- **I2C0**: I2C interface used for communication with sensors.
- **TCS34727**: RGB color sensor initialization.
- **MPU6050**: 6-DOF IMU initialization.
- **Servo Motor**: Servo initialization for motor control.
- **LCD**: 16x2 LCD initialization for display output.

### Modular Testing
The code uses a modular approach where individual peripherals can be tested by defining specific macros. Each module is tested in the main loop depending on the enabled macros.

### Predefined Macros
To control which peripheral is tested, the following macros can be enabled or disabled:

- `DELAY`: Test delay functionality.
- `UART`: Test UART communication.
- `I2C`: Test I2C communication.
- `TCS34727`: Test the TCS34727 RGB sensor.
- `MPU6050`: Test the MPU6050 IMU.
- `SERVO`: Test the servo motor control.
- `LCD`: Test the LCD display.
- `FULL_SYSTEM`: Test all connected peripherals together.

### Usage Instructions
1. Modify the macros to select which peripherals you wish to test:
   - Uncomment the desired peripherals (e.g., `#define TCS34727`) to enable specific tests.
2. Build and upload the code to the TM4C123 microcontroller.
3. The system will continuously test the selected peripherals in a loop.

## Code Structure
The `main()` function initializes the peripherals and enters an infinite loop where each module's test function is called based on the defined macros. 

### Peripheral Initialization
- **UART0_Init()**: Initializes UART0 for communication.
- **LED_Init()**: Initializes LEDs.
- **BTN_Init()**: Initializes buttons.
- **WTIMER0_Init()**: Initializes a timer for timing purposes.
- **I2C0_Init()**: Initializes the I2C bus for communication with sensors.
- **TCS34727_Init()**: Initializes the TCS34727 RGB sensor.
- **MPU6050_Init()**: Initializes the MPU6050 IMU.
- **Servo_Init()**: Initializes the servo motor.
- **LCD_Init()**: Initializes the 16x2 LCD display.

### Testing
- Each peripheral has a corresponding test defined in the `Module_Test()` function. If the macro is enabled, the respective test is executed within the infinite loop.
  - `Module_Test(DELAY_TEST)`: Tests delay functionality.
  - `Module_Test(UART_TEST)`: Tests UART communication.
  - `Module_Test(I2C_TEST)`: Tests I2C communication.
  - `Module_Test(TCS34727_TEST)`: Tests the TCS34727 RGB sensor.
  - `Module_Test(MPU6050_TEST)`: Tests the MPU6050 IMU.
  - `Module_Test(SERVO_TEST)`: Tests servo motor control.
  - `Module_Test(LCD_TEST)`: Tests the LCD display.
  - `Module_Test(FULL_SYSTEM_TEST)`: Tests all peripherals together.

## Requirements
- **Hardware**: TM4C123 microcontroller, TCS34727 RGB sensor, MPU6050 IMU, Servo motor, 16x2 LCD, Button, and LED.
- **Software**: The code is written for the TM4C123 microcontroller and uses peripheral initialization functions from the following libraries:
  - `I2C.h`
  - `UART0.h`
  - `TCS34727.h`
  - `MPU6050.h`
  - `ButtonLED.h`
  - `util.h`
  - `Servo.h`
  - `LCD.h`
  - `ModuleTest.h`
