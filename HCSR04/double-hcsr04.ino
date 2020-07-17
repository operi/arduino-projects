/************************************************************************************************************************************************************************                                              
 * - Author           : BEE MB                       *                                               
 * - Profession       : (Developer) MEGA DAS owner   *                                                                                              
 * - Main purpose     : Industrial Application       *                                                                                  
 * - Copyright holder : No copyrights                *                                  
 * - Date             : 23/03/2017                   *
 * ***********************************************************************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------------------|
 This code allows you to know the disctance between an Ultrasonic sensor and an object                                           |
 I used two Ultrasonic sensors so you can place them in two different sides to know if there is any object near to your Robot    |
 You can add more Ultrasonic sensors and follow the same steps two be sure that you are getting the desired information          |
---------------------------------------------------------------------------------------------------------------------------------|*/
//Start :
//define the pins that we will use for the first ultrasonic sensor
//----------------------------------------------------------------------------------------------------------------------
#define trigPin1 14                                   //pin number 9 in arduino MEGA2560
#define echoPin1 5                                   // we'll use this pin to read the signal from the first sensor
#define LED_first_ping 22                            // I/O digital or analogique ( we will use pin 22 to command an LED (on/off))
//----------------------------------------------------------------------------------------------------------------------

//define the pins that we will use for the second ultrasonic sensor
//----------------------------------------------------------------------------------------------------------------------
#define trigPin2 12
#define echoPin2 4
#define LED_second_ping 24
//----------------------------------------------------------------------------------------------------------------------

//used variables
//----------------------------------------------------------------------------------------------------------------------
long duration, distance, UltraSensor1, UltraSensor2; //we'll use these variable to store and generate data

char data;
String SerialData="";
//----------------------------------------------------------------------------------------------------------------------

//Make the setup of your pins
//----------------------------------------------------------------------------------------------------------------------
void setup()
{// START SETUP FUNCTION
Serial.begin (9600);                              // we will use the serial data transmission to display the distance value on the serial monitor 

// setup pins first sensor
pinMode(trigPin1, OUTPUT);                        // from where we will transmit the ultrasonic wave
pinMode(echoPin1, INPUT);                         //from where we will read the reflected wave 
pinMode(LED_first_ping, OUTPUT);                  // from where we will control the LED

//setup pins second sensor
pinMode(trigPin2, OUTPUT);
pinMode(echoPin2, INPUT);
pinMode(LED_second_ping, OUTPUT);


//inisialize LED status 
digitalWrite(LED_first_ping,LOW);
digitalWrite(LED_second_ping,LOW);
}// END SETUP FUNCTION

//write the code in the loop function
void loop() 
{
// START THE LOOP FUNCTION
SonarSensor(trigPin1, echoPin1);              // look bellow to find the difinition of the SonarSensor function
UltraSensor1 = distance;                      // store the distance in the first variable
SonarSensor(trigPin2,echoPin2);               // call the SonarSensor function again with the second sensor pins
UltraSensor2 = distance;                      // store the new distance in the second variable


// display the distances on the serial monitor for the first sensor
//----------------------------------------------------------------------------------------------------------------------
Serial.print("distance measured by the first sensor: ");
Serial.print(UltraSensor1);
Serial.println(" cm");
//----------------------------------------------------------------------------------------------------------------------

//display the distance on the serial monitor for the second sensor
//----------------------------------------------------------------------------------------------------------------------
Serial.print("distance measured by the second sensor: ");
Serial.print(UltraSensor2);
Serial.println(" cm");
Serial.println("---------------------------------------------------------------------------------------------------------");
//----------------------------------------------------------------------------------------------------------------------

}//END LOOP FUNTION

// SonarSensor function used to generate and read the ultrasonic wave
void SonarSensor(int trigPinSensor,int echoPinSensor)//it takes the trigPIN and the echoPIN 
{
  //START SonarSensor FUNCTION
  //generate the ultrasonic wave
//---------------------------------------------------------------------------------------------------------------------- 
digitalWrite(trigPinSensor, LOW);// put trigpin LOW 
delayMicroseconds(2);// wait 2 microseconds
digitalWrite(trigPinSensor, HIGH);// switch trigpin HIGH
delayMicroseconds(10); // wait 10 microseconds
digitalWrite(trigPinSensor, LOW);// turn it LOW again
//----------------------------------------------------------------------------------------------------------------------

//read the distance
//----------------------------------------------------------------------------------------------------------------------
duration = pulseIn(echoPinSensor, HIGH);//pulseIn funtion will return the time on how much the configured pin remain the level HIGH or LOW; in this case it will return how much time echoPinSensor stay HIGH
distance= (duration/2) / 29.1; // first we have to divide the duration by two  
}// END SonarSensor FUNCTION

/****************************----------------------- END PROGRAM -----------------------****************************/
