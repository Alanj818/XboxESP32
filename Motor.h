#include <Arduino.h>

//Magic Will Happen here
/*
  This is where we will define pins for switches
*/

#define Left_S8550_PIN 18
#define Right_S8550_PIN 19
#define Left_S8050_PIN 21
#define Right_S8050_PIN 22


//UP and DOWN will be the PWM control 
/*
  If I add a Servo Motor, Left and Right will have function, 0 to UP will be foward, 0 to DOWN will be reverse. 
  
  For Now, only UP-0-Down logic will be used, despite how far left or right the joystick points. Essentially only Y coordinates will matter.

  Having the Idea of using Triggers as well, Right for foward, Left for reverse (Based on how far pressed down)
*/
int32_t getAnalogWriteValue(int32_t num, int32_t overall){
  //I could return a map() here but I think ima keep it like this for now, to familiarize myself with the process before i abstract this 
  return ((num * 255) / overall);
}
void forwardMotion(ControllerPtr ctl){
  //TRIGGERS
  int32_t throttle = ctl->throttle();

  //PWM
  /*
    There is a scaling method, heres the math: 
        scaled_value = (current_value * scaling_highest_value) / overall_value
        
        EXAMPLE HERE: condensing throttle (0-1023) into analogWrite() scales (0-255)

        analog_write_value = (1023 * 255) / 1023 *this would give me 255, or 100% duty cycle (full throttle)*
  */
  int32_t throttleAnalog = getAnalogWriteValue(throttle, 1023);

  //Now we just set the analog values for Joy and Throttle into analogWrite(); main file will handle checking if joy is negative or positive, if throttle or brake is pressed
  digitalWrite(Left_S8550_PIN, LOW);
  digitalWrite(Right_S8550_PIN, HIGH); 
  digitalWrite(Left_S8050_PIN, LOW);
  analogWrite(Right_S8050_PIN, throttleAnalog); //just throttle for now, this is essentially controlling the path to ground, whether the path is fully connected or not at PWM rate. 
}

void reverseMotion(ControllerPtr ctl){
  //TRIGGERS
  int32_t brake = ctl->brake();

  //PWM
  int32_t brakeAnalog = getAnalogWriteValue(brake, 1023);

  //Now we just set the analog values for Joy and Throttle into analogWrite(); main file will handle checking if joy is negative or positive, if throttle or brake is pressed
  digitalWrite(Left_S8550_PIN, HIGH);
  digitalWrite(Right_S8550_PIN, LOW); 
  digitalWrite(Right_S8050_PIN, LOW);
  analogWrite(Left_S8050_PIN, brakeAnalog);
}

void standStill(){
  digitalWrite(Left_S8550_PIN, HIGH);
  digitalWrite(Right_S8550_PIN, HIGH);
  analogWrite(Right_S8050_PIN, 0); 
  analogWrite(Left_S8050_PIN, 0);
}
