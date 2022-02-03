#include <LiquidCrystal.h>
#include<math.h>
#include <stdlib.h>
#define DISTANCE_UNIT 7

//init the LCD
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

//for the ultrasonic sensor
const int trigger = A5;
const int echo = A4;

int currentLedMode = 0;
volatile int currentUnitMode = 0;

//chosen values of leds based on which mode we are currently in
int chosen_high = HIGH;
int chosen_low = LOW;

int chosenUnitMode = 0;


//other variables for computing the distance in cm
int duration = 0;
int previous_distance = 0;
int distance = 0;
float distanceInMeters = 0.0f;
float previousDistanceInMeters = 0.0f;

char buff[33];

//led status of the LED module
unsigned char LED_status = 0;

void setup()
{
  //configure the data pins for the ultrasonic sensor
  pinMode(trigger , OUTPUT);
  pinMode(echo , INPUT);


  //configure digital pins as inputs (the buttons responsable for switching between lighting modes)
  pinMode(20 , INPUT);
  pinMode(21 , INPUT);

  //initialize the buttons as being "unpressed"
  digitalWrite(20, LOW);
  digitalWrite(21, LOW);

  //attach the interrupts to the buttons
  attachInterrupt(digitalPinToInterrupt(20), modeOne, RISING);
  attachInterrupt(digitalPinToInterrupt(21), modeTwo, RISING);


  // Sets the no. of rows and columns of the LCD
  lcd.begin(16, 2);


  // activate PORTA, as output (for LED module)
  DDRA = 0b11111111;

  Serial.begin(9600); //for PC
  Serial1.begin(9600); // Serial 1 interface for Bluetooth module
}

void loop()
{
  //read data and compute distance
  digitalWrite(trigger , HIGH);
  delay(200);
  digitalWrite(trigger , LOW);

  previous_distance = distance;
  duration = pulseIn(echo , HIGH);
  distance = (duration / 2) / 28.5 ;

  previousDistanceInMeters = distanceInMeters;
  distanceInMeters = (float) distance / 100.0f;
  distanceInMeters  = floor(10000 * distanceInMeters) / 10000;

  //write all the data into Serial port of the Bluetooth Module
  //send to Bluetooth and write the info there
  Serial1.write("Distance: ");
  Serial1.write(itoa(distance, buff, 10));
  Serial1.write(" cm. ");
  Serial1.write("Duration: ");
  Serial1.write(itoa(duration, buff, 10));
  Serial1.write(". ");
  Serial1.write(itoa(currentUnitMode, buff, 10));
  Serial1.write(" ");
  Serial1.write(itoa(currentLedMode, buff, 10));
  Serial1.write("\n");

  //omit some unnecessary lcd clear function calls
  if (previous_distance != distance || previousDistanceInMeters != distanceInMeters)
    lcd.clear();
  lcd.setCursor(5, 0);
  if (chosenUnitMode == 0) {
    //print in CM
    lcd.print(distance);
    lcd.print(" cm");
  }
  else {
    //print in M
    lcd.print(distanceInMeters);
    lcd.print(" m");
  }

  //refresh the led status
  LED_status = 0;

  //biuld up current led status and transfer it to PORTA
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
  LED_status = PORTA;
}


void modeOne () {
  if (currentLedMode == 0) {
    //the lights turn on sequentially as the object gets closer relative to the ultrasonic sensor
    chosen_high = HIGH;
    chosen_low = LOW;
    currentLedMode = 1;
  }
  else {
    //the lights shut down sequentially as the object gets closer relative to the ultrasonic sensor
    chosen_high = LOW;
    chosen_low = HIGH;
    currentLedMode = 0;
  }
}

void modeTwo () {
  if (currentUnitMode == 0) {
    chosenUnitMode = 1;
    currentUnitMode = 1;
  }
  else if (currentUnitMode == 1) {
    chosenUnitMode = 0;
    currentUnitMode = 0;
  }
}
