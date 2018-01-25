#include <Servo.h>
#include <Math.h>

Servo motor_one;
Servo motor_two;

void setup() {
  Serial.begin(9600);
  Serial.print("Starting..\n");

  // Power the high pins
  pinMode(5,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(11,OUTPUT);

  digitalWrite(5,HIGH);
  digitalWrite(7,HIGH);
  digitalWrite(11,HIGH);

  // Ground the low pins
  pinMode(3,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(12,OUTPUT);

  digitalWrite(3,LOW);
  digitalWrite(6,LOW);
  digitalWrite(8,LOW);
  digitalWrite(12,LOW);

  // Activate the switch inputs
  pinMode(4,INPUT_PULLUP);
  pinMode(2,INPUT_PULLUP);

  // Attach the servos
  motor_one.attach(9);
  motor_two.attach(10);
}

int analog_value = 0;
int switch_one = 0;
int switch_two = 0;
int target_output = 0;
int output_us = 0;

float linear;

void loop() {
  // Get the latest potentiometer data
  analog_value = analogRead(1);

  // The following code is specific to the POT and may need to be tuned... 
  // this is used on a logarithmic pot so its a bit irritating
  linear = log((float)analog_value) / log((float)1000);
  linear = max(linear,0.4);
  linear = min(linear, 0.85);

  linear = (linear - 0.4) / 0.45;

  // Get the switch data
  switch_one = digitalRead(4);
  switch_two = digitalRead(2);

  // FRC jags operate on a 670-2330 range, with 1500 neutral microsecond duty cycle

  // calculate the target output offset
  target_output = linear * 830;

  output_us = 1500;

  if (switch_one == 0)
  {
    output_us = output_us - target_output;
  }
  else if (switch_two == 0)
  {
    output_us = output_us + target_output;
  }
  
  // Set the pulse width outputs
  motor_one.writeMicroseconds(output_us);
  motor_two.writeMicroseconds(output_us);

  // Print out diagnostic data - this is harmless if not connected to a PC
  Serial.print("Switch 1: ");
  Serial.print(switch_one);
  Serial.print(" Switch 2: ");
  Serial.print(switch_two);
  Serial.print(" Analog: ");
  Serial.print(analog_value);
  Serial.print(" Linear: ");
  Serial.print(linear);
  Serial.print(" Output us: ");
  Serial.print(output_us);
  Serial.print("\n");
}
