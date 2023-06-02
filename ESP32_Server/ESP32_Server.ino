// ref: Examples > BluetoothSerial > SerialToSerialBT


#include "BluetoothSerial.h"
#include "esp_bt_device.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

//TFT_eSPI tft = TFT_eSPI();
#define Vx 25 //D26
#define Vy 26 //D25
String xAxis, yAxis;
const String deviceName = "ESP32_SPP";

String getMAC(){
  const uint8_t* point = esp_bt_dev_get_address();

  String s = "";

  for (int i = 0; i < 6; i++) {
    char str[3];
    sprintf(str, "%02X", (int)point[i]);
    s = s + str;
    if (i < 5){
      s = s+ ":";
    }
  }
  return s;
}

//hàm này xử lý các sự kiện của giao thức Bluetooth SPP (Serial Port Profile) 
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
        //tft.println("ESP_SPP_CLOSE_EVT");

        //startUpScr();
        break;

    case ESP_SPP_CONG_EVT://connection congestion status changed
        Serial.println("ESP_SPP_CONG_EVT");
        //tft.println("ESP_SPP_CONG_EVT");
        break;

    case ESP_SPP_WRITE_EVT://write operation completed
        Serial.println("ESP_SPP_WRITE_EVT");
        //tft.println("ESP_SPP_WRITE_EVT");
        break;

    case ESP_SPP_DATA_IND_EVT://connection received data
        Serial.println("ESP_SPP_DATA_IND_EVT");
        //tft.println("ESP_SPP_DATA_IND_EVT");
        break;

    case ESP_SPP_DISCOVERY_COMP_EVT://discovery complete
        Serial.println("ESP_SPP_DISCOVERY_COMP_EVT");
        //tft.println("ESP_SPP_DISCOVERY_COMP_EVT");
        break;

    case ESP_SPP_OPEN_EVT://Client connection open
        Serial.println("ESP_SPP_OPEN_EVT");
        //tft.println("ESP_SPP_OPEN_EVT");
        break;

    case ESP_SPP_START_EVT://server started
        Serial.println("ESP_SPP_START_EVT");
        //tft.println("ESP_SPP_START_EVT");
        break;

    case ESP_SPP_CL_INIT_EVT://client initiated a connection
        Serial.println("ESP_SPP_CL_INIT_EVT");
        //tft.println("ESP_SPP_CL_INIT_EVT");
        break;

    default:
        Serial.println("unknown event!");
        //tft.println("unknown event!");
        break;
    }
}
/*
ESP_SPP_INIT_EVT: được gọi khi Bluetooth SPP được khởi tạo.
ESP_SPP_SRV_OPEN_EVT: được gọi khi server mở kết nối Bluetooth.
ESP_SPP_CLOSE_EVT: được gọi khi client đóng kết nối Bluetooth.
ESP_SPP_CONG_EVT: được gọi khi tình trạng kết nối Bluetooth thay đổi.
ESP_SPP_WRITE_EVT: được gọi khi hoạt động ghi hoàn thành.
ESP_SPP_DATA_IND_EVT: được gọi khi kết nối nhận dữ liệu.
ESP_SPP_DISCOVERY_COMP_EVT: được gọi khi tìm kiếm thiết bị Bluetooth hoàn tất.
ESP_SPP_OPEN_EVT: được gọi khi client mở kết nối Bluetooth.
ESP_SPP_START_EVT: được gọi khi server bắt đầu kết nối Bluetooth.
ESP_SPP_CL_INIT_EVT: được gọi khi client khởi tạo kết nối Bluetooth.
*/
void setup() {
 
  Serial.begin(115200);
  Serial.println("\n---Start---");
  SerialBT.begin(deviceName); //Bluetooth device name
  
  Serial.println("The device started, now you can pair it with bluetooth!");
  Serial.println("Device Name: " + deviceName);
  Serial.print("BT MAC: ");
  Serial.print(getMAC());
  Serial.println();
 SerialBT.register_callback(btCallback);

}

void loop() {
 
   if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  } 

   xAxis = analogRead(Vx); // Read Joysticks X-axis
  yAxis = analogRead(Vy); // Read Joysticks Y-axis
 
    uint8_t xdata[xAxis.length()];
   
  
memcpy(xdata,xAxis.c_str(),xAxis.length());// sao chép các byte vào mảng xdata

SerialBT.write(xdata,xAxis.length());

SerialBT.write('\n');
  Serial.println("xAxis: ");

 Serial.write(xdata,xAxis.length());
Serial.write('\n'); 

  
 uint8_t ydata[yAxis.length()];
 memcpy(ydata,yAxis.c_str(),yAxis.length());

 SerialBT.write(ydata,yAxis.length());
// SerialBT.write(';');
 SerialBT.write('\n');
 Serial.println("yAxis: "); 
  Serial.write(ydata,yAxis.length());  
Serial.write('\n'); 


  delay(20);
}
// Y-axis used for forward and backward control
 
