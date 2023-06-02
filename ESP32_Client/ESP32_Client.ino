/*
 * SPP Client on ESP32
 */


#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

String ServerMACadd = "58:BF:25:36:A9:DA";
uint8_t ServerMAC[6]  = {0x58, 0xBF, 0x25, 0x36, 0xA9, 0xDA};
String ServerName = "ESP32_SPP";
char *pin = "1234"; //<- standard pin would be provided by default
bool connected;
bool isSppOpened = false;

#define enA 32 //D18
#define enB 33 //D19
#define in1 25 //D25
#define in2 26 //D26
#define in3 27 //D27
#define in4 14 //D14

//RF24 radio(8, 9); // CE, CSN
//const byte address[6] = "00001";
char receivedData[32] = "";
char data[4];
//int buffer[1024] ;
int  xAxis=0;
int  yAxis=0;
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
    //delay(5);

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
    //delay(5);
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
  
    //int c=   SerialBT.read();
    //  int c= SerialBT.readBytes(data, sizeof(data)); // Đọc dữ liệu vào mảng data
     
//Serial.println(c);
//for(int i = 0; i < c; i++){
  //  xAxis = xAxis * 10 + data[i];
//}
    // Xử lý và hiển thị dữ liệu được đọc
   // for (int i = 0; i<len; i++) {
    //  Serial.print(data[i]);
   //   Serial.print(" ");
   //Serial.println(c);
  String c= SerialBT.readStringUntil(';'); // Đọc kích thước mảng từ module Bluetooth
   Serial.println(c);
    
     delay(100);
  char d= SerialBT.read(); // Đọc kích thước mảng từ module Bluetooth
   Serial.println(d);
    
     delay(100);
 // c=SerialBT.read() ; 
   // c=  SerialBT.readBytes(data,sizeof(data));
    // Serial.println(c);
     //for(int i = 0; i < c; i++){
   // yAxis = yAxis * 10 + data[i];
//}
//Serial.println(yAxis);
     //delay(10);
   // }
   // Serial.println();
    // xAxis = atoi(&c); // Convert the data from the character array (received X value) into integer
      //Serial.print("xAxis: ");
     //Serial.println(xAxis);
     //delay(100);
      //c = SerialBT.read();
    // Serial.println(c);
 // yAxis = atoi(&c);
  //Serial.print("yAxis: ");
    // Serial.println(yAxis);
     
    }
    
 // Y-axis used for forward and backward control
  if (yAxis < 470) { 
      // Set Motor A backward 
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    // Set Motor B backward
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);

 
 // Convert the declining Y-axis readings for going backward from 470 to 0 into 0 to 255 value for the PWM signal for increasing the motor speed
    motorSpeedA = map(yAxis, 470, 0, 0, 255);
    motorSpeedB = map(yAxis, 470, 0, 0, 255);
    
  }
  
else if (yAxis > 550) {
 
digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    // Set Motor B forward
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH); 
    

  motorSpeedA = map(yAxis, 550, 1023, 0, 255);
    motorSpeedB = map(yAxis, 550, 1023, 0, 255);
 
  }
  // If joystick stays in middle the motors are not moving
  else {
    motorSpeedA = 0;
    motorSpeedB = 0;
  }
  // X-axis used for left and right control
  if (xAxis < 470) {
    // Convert the declining X-axis readings from 470 to 0 into increasing 0 to 255 value
    int xMapped = map(xAxis, 470, 0, 0, 255);
    // Move to left - decrease left motor speed, increase right motor speed
    motorSpeedA = motorSpeedA + xMapped;
    motorSpeedB = motorSpeedB - xMapped;
    
    // Confine the range from 0 to 255
    if (motorSpeedA < 0) {
      motorSpeedA = 0;
    }
    if (motorSpeedB > 255) {
      motorSpeedB = 255;
    }
  }
  if (xAxis > 550) {
    // Convert the increasing X-axis readings from 550 to 1023 into 0 to 255 value
    int xMapped = map(xAxis, 550, 1023, 0, 255);
    // Move right - decrease right motor speed, increase left motor speed
    motorSpeedA = motorSpeedA - xMapped;
    motorSpeedB = motorSpeedB + xMapped;
   
    // Confine the range from 0 to 255
    if (motorSpeedA > 255) {
      motorSpeedA = 255;
    }
    if (motorSpeedB < 0) {
      motorSpeedB = 0;
    }
  }
  // Prevent buzzing at low speeds (Adjust according to your motors. My motors couldn't start moving if PWM value was below value of 70)
  if (motorSpeedA < 70) {
    motorSpeedA = 0;
  }
  if (motorSpeedB < 70) {
    motorSpeedB = 0;
  }
  
//}
}

   

