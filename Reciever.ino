#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

Servo servoX;
Servo servoY1;
Servo servoY2;
Servo ESC;

RF24 radio(7, 8); // CSN, CE
const byte address[6] = "00001";

int servoX_pin = 3;
int servoY1_pin = 4;
int servoY2_pin = 5;
int servoZ_pin = 6;
int x_pos;
int y_pos;
int z_pos;

String radioData;

void setup() {
  Serial.begin(9600);
  radio.begin();
  servoX.attach (servoX_pin ) ; 
  servoY1.attach (servoY1_pin ) ;
  servoY2.attach (servoY2_pin ) ;
  ESC.attach (servoZ_pin,1000,2000);
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
}

void loop() {

  //Reading signal 
  if (radio.available()) {
   char radioData[32] = "";
   radio.read(&radioData, sizeof(radioData));
   Serial.println(radioData);

    //X positioning
    x_pos = parseDataX(radioData);
    //Serial.println(x_pos);
    x_pos = map(x_pos, 0, 1023, 0, 135);
    if (x_pos>400 && x_pos<600)
    {
      
    }
    else{
    servoX.write (180-x_pos) ;
    }
  
    //Y positioning 
    y_pos = parseDataY(radioData);
    //Serial.println(y_pos);
    y_pos = map(y_pos, 0, 1023, 0, 135);
    if (y_pos>400 && y_pos<600)
    {
      
    }
    else{
    servoY1.write (y_pos) ;
    servoY2.write (180 - y_pos) ;
  }
  
  //Z - Brushless Motor 
    z_pos = parseDataZ(radioData);
    //Serial.println(z_pos);
    z_pos= map(z_pos, 0, 1023,1000,2000); //mapping val to minimum and maximum(Change if needed) 
    ESC.writeMicroseconds(z_pos); //using val as the signal to esc
    }
    delay(10);
}


  //Declaring ParseX function
  int parseDataX(String data){
  data.remove(data.indexOf("Y"));
  data.remove(data.indexOf("X"),1);

  return data.toInt();
  }

  //Declaring ParseY function
  int parseDataY(String data){
  data.remove(0,data.indexOf("Y") + 1);
  data.remove(data.indexOf("Z"));
  
  return data.toInt();
}

  //Declaring ParseZ function
  int parseDataZ(String data){
  data.remove(0,data.indexOf("Z") + 1);

  return data.toInt();
  }
