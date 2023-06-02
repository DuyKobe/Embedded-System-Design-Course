/*
 * SPP Client on ESP32
 */


#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

String ServerMACadd = "24:D7:EB:18:4C:7A";
uint8_t ServerMAC[6]  = {0x24, 0xD7, 0xEB, 0x18, 0x4C, 0x7A};
String ServerName = "ESP32_SPP";
char *pin = "1234"; //<- standard pin would be provided by default
bool connected;
bool isSppOpened = false;

#define enA 12 //D18
#define enB 33 //D19
#define in1 14 //D25
#define in2 27 //D26
#define in3 26 //D27
#define in4 25 //D14

//RF24 radio(8, 9); // CE, CSN
//const byte address[6] = "00001";
char receivedData[32] = "";
//int buffer[1024] ;
int  xAxis, yAxis;
int motorSpeedA = 0;
int motorSpeedB = 0;
 //int Mspeed =0;
const int ledChannel0 = 0;
const int ledChannel1 = 1;
const int freq = 30000;
const int resolution = 8;
void btCallback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param){
  
  switch (event)
    {
    case ESP_SPP_INIT_EVT:
        Serial.println("ESP_SPP_INIT_EVT");
        break;

    case ESP_SPP_SRV_OPEN_EVT://Server connection open
        Serial.println("ESP_SPP_SRV_OPEN_EVT");
        break;

    case ESP_SPP_CLOSE_EVT://Client connection closed
        Serial.println("ESP_SPP_CLOSE_EVT");
        isSppOpened = false;
        break;

    case ESP_SPP_CONG_EVT://connection congestion status changed
        Serial.println("ESP_SPP_CONG_EVT");
        break;

    case ESP_SPP_WRITE_EVT://write operation completed
        Serial.println("ESP_SPP_WRITE_EVT");
        break;

    case ESP_SPP_DATA_IND_EVT://connection received data
        //Serial.println("ESP_SPP_DATA_IND_EVT");
        break;

    case ESP_SPP_DISCOVERY_COMP_EVT://discovery complete
        Serial.println("ESP_SPP_DISCOVERY_COMP_EVT");
        break;

    case ESP_SPP_OPEN_EVT://Client connection open
        Serial.println("ESP_SPP_OPEN_EVT");
        isSppOpened = true;
        break;

    case ESP_SPP_START_EVT://server started
        Serial.println("ESP_SPP_START_EVT");
        break;

    case ESP_SPP_CL_INIT_EVT://client initiated a connection
        Serial.println("ESP_SPP_CL_INIT_EVT");
        break;

    default:
        Serial.println("unknown event!");
        break;
    }
}
void setup()
{ 
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
   // Khởi tạo kênh PWM
  ledcSetup(ledChannel0, freq, resolution);
  ledcSetup(ledChannel1, freq, resolution);
  // Liên kết kênh PWM với chân GPIO
  ledcAttachPin(enA , ledChannel0);
  ledcAttachPin(enB , ledChannel1);
  Serial.begin(115200);   
    Serial.println("\n------ begin ----------------\n");
    delay(5);

    Serial.println("- to connect -");
    
    SerialBT.begin("ESP32_Client", true);
    SerialBT.register_callback(btCallback);
    connected = SerialBT.connect(ServerName);
    connected = SerialBT.connect(ServerMAC);
    /*
     * In my trial, 
     * SerialBT.connect() always return  true, even no server exist.
     * To solve it, I implemented bluetooth event callback function,
     * double varify if ESP_SPP_OPEN_EVT raised.
     */
     
    if(connected) {
      Serial.println("SerialBT.connect() == true");
    } else {
      Serial.println("Failed to connect! Reset to re-try");
      Serial.println("SerialBT.connect() == false");
      //ssd1306_printFixed(0, 32, 
          //"Failed to connect! Reset to re-try", STYLE_NORMAL);
      while(true){
      }
    }
    
    //may be there are some delay to call callback function,
    //delay before check
    delay(5);
    if(isSppOpened == false){
      Serial.println("isSppOpened == false");
      Serial.println("Reset to re-try");
      while(true){
      }
    }
    
    Serial.println("isSppOpened == true");
    Serial.println("CONNECTED");
}


void loop()
{
    if(!isSppOpened){
      Serial.println("isSppOpened == false : DISCONNECTED");
      Serial.println("Reset to re-connect");
      while(true){
      }
    }
    
 
    ledcWrite(ledChannel0,  motorSpeedA);
    ledcWrite(ledChannel1, motorSpeedB);
    if (SerialBT.available()) {
      //char c = SerialBT.read();
      String c= SerialBT.readStringUntil('\n');
      //String c= SerialBT.readString();
       //  Serial.print(c);
        // Serial.write('\n');
      xAxis = c.toInt(); // Convert the data from the character array (received X value) into integer
      Serial.print("xAxis: ");
  //Serial.println(c);

     Serial.println(xAxis);
     delay(10);
      String d = SerialBT.readStringUntil('\n');
    // Serial.print(c);
     //    Serial.write('\n');
      //xAxis = atoi(&c); // Convert the data from the character array (received X value) into integer
      //Serial.print("xAxis: ");
     //Serial.println(xAxis);
   //   delay(10);
      // c= SerialBT.readStringUntil('\n');
      //   Serial.println(c);
     //    delay(10);
      // c = SerialBT.read();
     //Serial.print(c);
     yAxis = d.toInt();
     Serial.print("yAxis: ");
    // Serial.println(d);
    
     Serial.println(yAxis);
     delay(10);
     
     //String c= SerialBT.readStringUntil(';'); // Đọc kích thước mảng từ module Bluetooth
   //Serial.println(c);
    // delay(20);
    }
    
 // Y-axis used for forward and backward control
  if (yAxis < 2500) { 
      // Set Motor A backward 
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    // Set Motor B backward
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);

 
 // Convert the declining Y-axis readings for going backward from 2900 to 0 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(yAxis, 2500, 0, 0, 255);
    motorSpeedB = map(yAxis, 2500, 0, 0, 255);
    
  }
  
else if (yAxis > 3400) {
 //Set Motor B forward
digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    // Set Motor B forward
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW); 
    

  motorSpeedA = map(yAxis, 3400, 4095, 0, 255);
    motorSpeedB = map(yAxis, 3400, 4095, 0, 255);
 
  }
  // If joystick stays in middle the motors are not moving
  else {
    motorSpeedA = 0;
    motorSpeedB = 0;
  }
  // X-axis used for left and right control
  if (xAxis > 3400) {
    // Convert the declining X-axis readings from 3100 to 4095 into increasing 0 to 255 value
    int xMapped = map(xAxis, 3400, 4095, 0, 255);
    // Move to left - decrease left motor speed, increase right motor speed
    motorSpeedA = motorSpeedA + xMapped;
    motorSpeedB = motorSpeedB - xMapped;
    
    // Confine the ran
    //ge from 0 to 255
    if (motorSpeedB < 0 ) {
      motorSpeedB = 0;
    }
    if (motorSpeedA >255) {
      motorSpeedA = 255;
    }
  }
  if (xAxis < 2500) {
    // Convert the increasing X-axis readings from 2900 to 0 into 0 to 255 value
    int xMapped = map(xAxis, 2500, 0, 0, 255);
    // Move right - decrease right motor speed, increase left motor speed
    motorSpeedA = motorSpeedA - xMapped;
    motorSpeedB = motorSpeedB + xMapped;
   
    // Confine the range from 0 to 255
    if (motorSpeedB >255  ) {
      motorSpeedB = 255;  
    }
    if (motorSpeedA <0) {
      motorSpeedA = 0;
    }
  }
  // Prevent buzzing at low speeds (Adjust according to your motors. My motors couldn't start moving if PWM value was below value of 70)
  if (motorSpeedA < 70) {
    motorSpeedA = 0;
  }
  if (motorSpeedB < 70) {
    motorSpeedB = 0;
  }
  Serial.print("motorSpeedA: ");
    // Serial.println(d);
    
     Serial.println(motorSpeedA);
      Serial.print("motorSpeedB: ");
    // Serial.println(d);
    
     Serial.println(motorSpeedB);
//}
}

   

