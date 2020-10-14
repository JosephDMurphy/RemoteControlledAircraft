#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CSN, CE
const byte address[6] = "00001";

int x_key = A0;                                               
int y_key = A1;    
int z_key = A2;                                      
int x_pos;
int y_pos;
int z_pos;

char X_Store[5];
char Y_Store[5];
char Z_Store[5];
String strX;
String strY;
String strZ;
String radioData;

void setup() {
  //Setting up powerpin
  pinMode(4,OUTPUT);
  digitalWrite(4,HIGH);
  
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();

  pinMode (x_key, INPUT) ;                     
  pinMode (y_key, INPUT) ;    
  pinMode (z_key, INPUT) ;
}

void loop() {
  //Reading analog values and mapping them to a string array
  x_pos = analogRead (x_key) ;  
  y_pos = analogRead (y_key) ;   
  z_pos = analogRead (z_key) ; 
  strX = String(x_pos);
  strY = String(y_pos);
  strZ = String(z_pos);
  strX.toCharArray(X_Store,5);
  strY.toCharArray(Y_Store,5);
  strZ.toCharArray(Z_Store,5);
  
  char radioData[32];
  radioData[0] = '\0';
  strcat(radioData, "X");
  strcat(radioData, X_Store);
  strcat(radioData, "Y");
  strcat(radioData, Y_Store);
  strcat(radioData, "Z");
  strcat(radioData, Z_Store);
  
  
  //Serial.println(radioData);

  //Sending string array of analog values to flight controller
  radio.write(&radioData, sizeof(radioData));
  delay(10);
}
