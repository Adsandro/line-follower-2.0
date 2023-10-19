
## RFID Car Control System

This Arduino sketch provides a simple implementation of a car control system using an RFID module (MFRC522), infrared sensors, an L298N motor driver module, and an LCD display. The system reads RFID cards and controls the movement of a car based on the input from the infrared sensors. Below are the key components and functionalities of the code:

### Components Used:

-   **MFRC522 RFID Module:** Used for reading RFID cards.
-   **Infrared Sensors:** Three infrared sensors (left, middle, and right) are used to detect the presence or absence of a line on the ground.
-   **L298N Motor Driver Module:** Controls the movement of the car motors.
-   **LCD Display:** A 16x2 LCD display is used to provide visual feedback.

### Functionalities:

-   The system reads RFID cards and authenticates them to perform operations.
-   The car can move forward, backward, turn left, or turn right based on the input from the infrared sensors.
-   The movement of the car is determined by the presence or absence of lines detected by the infrared sensors.

### Pin Configuration:

-   **RFID Module:** SDA - 10, SCK - 13, MOSI - 11, MISO - 12, RST - 9
-   **Motor Driver Module:** ENA - 6, IN1 - 8, IN2 - 7, IN3 - 4, IN4 - 2, ENB - 5
-   **Infrared Sensors:** SENSOR_ESQ - 15, SENSOR_MID - 16, SENSOR_DIR - 17
-   **LCD Display:** I2C address - 0x27

### How It Works:

1.  **RFID Card Authentication:** When a new RFID card is detected, the system authenticates the card using the provided authentication key.
2.  **Infrared Sensor Inputs:** The infrared sensors detect the presence or absence of lines on the ground. Based on these inputs, the system determines the movement of the car.
3.  **Motor Control:** The L298N motor driver module controls the motors of the car. The car can move forward, backward, turn left, or turn right.

### Usage:

-   Connect the RFID module, infrared sensors, motor driver module, and LCD display to the specified pins on the Arduino board.
-   Upload the provided Arduino sketch to the board.
-   Ensure that the RFID cards to be used are compatible with the MFRC522 module and have appropriate authentication keys.
-   Power on the system and place RFID cards in the vicinity of the RFID module to control the car's movement.

### Note:

-   Ensure that the RFID cards are properly initialized and have the correct authentication keys set up for the system to work correctly.

For any issues, questions, or improvements, feel free to contact the developer.

**Developers:** [Guilherme  Cruz](https://www.linkedin.com/in/guilherme-cruz-20809324a/),  [Adsandro Carvalho](https://www.linkedin.com/in/adsandrocarvalho/), [Thiago Natalio](https://www.linkedin.com/in/tiagonatalio/).