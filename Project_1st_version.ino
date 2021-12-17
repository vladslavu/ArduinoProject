#include <LiquidCrystal.h>

#define DISTANCE_UNIT 7

//init the LCD
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

//for the ultrasonic sensor
const int trigger = A5;
const int echo = A4;

//chosen values of leds based on which mode we are currently in
int chosen_high = HIGH;
int chosen_low = LOW;


//other variables for computing the distance in cm
int duration = 0;
int previous_distance = 0;
int distance = 0;

//led status of the LED module
unsigned char LED_status = 0;

void setup() 
{
  //configure the data pins for the ultrasonic sensor
  pinMode(trigger , OUTPUT);
  pinMode(echo , INPUT);

  
  //configure digital pins as inputs (the buttons responsable for switching between lighting modes)
  pinMode(20 ,INPUT);
  pinMode(21 ,INPUT);
  
  //initialize the buttons as being "unpressed"
  digitalWrite(20, LOW);
  digitalWrite(21, LOW);
  
  //attach the interrupts to the buttons
  attachInterrupt(digitalPinToInterrupt(20), modeOne, RISING);
  attachInterrupt(digitalPinToInterrupt(21), modeTwo, CHANGE);

  
  // Sets the no. of rows and columns of the LCD
  lcd.begin(16, 2);

  
  // activate PORTA, as output (for LED module)
  DDRA = 0b11111111;
  
  Serial.begin(9600);
}

void loop()
{
  digitalWrite(trigger , HIGH);
  delay(50);
  digitalWrite(trigger , LOW);

  previous_distance = distance;
  duration = pulseIn(echo , HIGH);
  distance = (duration/2) / 28.5 ;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm. ");
  Serial.println(duration);
  if (previous_distance != distance)
    lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print(distance);
  lcd.print(" cm"); 
  //refresh the led status
  LED_status = 0;
  
  //biuld up current led status
  if ( distance <= 1 * DISTANCE_UNIT )
  {
    LED_status |= (chosen_high << 5);
  }
  else
  {
    LED_status |= (chosen_low << 5);
  }
  
  if ( distance <= 2 * DISTANCE_UNIT )
  {
    LED_status |= (chosen_high << 4);
  }
  else
  {
    LED_status |= (chosen_low << 4);
  }
  
  if ( distance <= 3 * DISTANCE_UNIT )
  {
    LED_status |= (chosen_high << 3);
  }
  else
  {
    LED_status |= (chosen_low << 3);
  }
  
  if ( distance <= 4 * DISTANCE_UNIT )
  {
    LED_status |= (chosen_high << 2);
  }
  else
  {
    LED_status |= (chosen_low << 2);
  }
  
  if ( distance <= 5 * DISTANCE_UNIT )
  {
    LED_status |= (chosen_high << 1);
  }
  else
  {
    LED_status |= (chosen_low << 1);
  }
  
  if ( distance <= 6 * DISTANCE_UNIT )
  {
    LED_status |= chosen_high;
  }
  else
  {
    LED_status |= chosen_low;
  }

  PORTA = LED_status;
}


void modeOne () {
  //the lights turn on sequentially as the object gets closer relative to the ultrasonic sensor
  chosen_high = HIGH;
  chosen_low = LOW;
}

void modeTwo () {
  //the lights shut down sequentially as the object gets closer relative to the ultrasonic sensor
  chosen_high = LOW;
  chosen_low = HIGH;
}
