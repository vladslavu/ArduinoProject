# ArduinoProject (DMP - Design with Microprocessors)
Current state of the project (Finished):
  - I have created a **_distance sensor_** using an ultrasonic sensor and a LED module (having 6 leds).
  - In order to include interrups, I have chosen the external ones.
  - We also have a LCD screen attached to the Arduino board via shield.
  - Two buttons are present and meaningful for the project(K1, K2): used for swithing between lighting modes (K1) and the mesurement unit displayed on the LCD screen (cm or m).
  - First Mode(K1): the LEDs turn on sequentially as the object gets closer relative to the ultrasonic sensor, and shut down as it moves away.
  - Second Mode (opposite) (K1): the LEDs shut down sequentially as the object gets closer relative to the ultrasonic sensor, and turn on as it moves away.
  - First Mode(K2): the distance is displayed on the screen in centimeters.
  - Second Mode (K2): the distance will be displayed in meters.
  
![distance_sensor_project](https://user-images.githubusercontent.com/64836463/152309050-063a684a-fed8-48c2-b45e-7dbd61b48de1.png)
