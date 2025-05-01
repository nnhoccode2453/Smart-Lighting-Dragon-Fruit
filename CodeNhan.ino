#include <ESP8266WiFi.h> 
#include <SimpleTBDevice.h> 
#define RELAY1 D1
#define MOTOR D0 
#define RELAY2 D3 
#define BUZZER D2 
#define cambienquang A0

// Thông tin Wi-Fi và ThingsBoard
#define SSID "VLDT-E303_2.4G"  // Tên Wi-Fi
#define PASSWORD "dpee2024@vldt" // Mật khẩu Wi-Fi 
#define SERVER_ADDR "demo.thingsboard.io"
#define ACCESS_TOKEN "dkPiZeE2zrwuI2PLbU0t"

WiFiClient client;
TBDevice device(ACCESS_TOKEN);


unsigned long TimeMark = 0; void setup() { 

Serial.begin(115200);

// Cấu hình chân output 
pinMode(RELAY1, OUTPUT); 
pinMode(MOTOR, OUTPUT); 
pinMode(BUZZER, OUTPUT); 
pinMode(RELAY2, OUTPUT);

// Kết nối Wi-Fi 
WiFi.begin(SSID, PASSWORD);
Serial.println("Đang kết nối Wi-Fi...");
while (WiFi.status() != WL_CONNECTED) {
 delay(500); 
 Serial.print(".");
}
Serial.println("\nĐã kết nối Wi-Fi!");


// Kết nối ThingsBoard 
device.begin(SERVER_ADDR, client);
}
void loop() {
int cbq = analogRead(cambienquang); // Đọc cảm biến quang 
int thr1 = device.read("nguongquang1").asInt();
int cbd = device.read("cambiendoamdat").asInt(); 
int thr2 = device.read("nguongquang2").asInt();
bool mode_chedo = device.read("mode").asBoolean(); 
bool but1 = device.read("buttonden").asBoolean(); 
bool but2 = device.read("buttonmotor").asBoolean(); 
bool but3 = device.read("buttonbuzzer").asBoolean();

if (mode_chedo) { // Chế độ tự động 
 if (cbq > thr1 && cbq > thr2) { 
  digitalWrite(RELAY1, HIGH); 
  digitalWrite(RELAY2, LOW);
  } else if (cbq < thr1 && cbq < thr2) { 
   digitalWrite(RELAY1, LOW); 
   digitalWrite(RELAY2, LOW);
  } else if (cbq < thr1 && cbq > thr2) { 
   digitalWrite(RELAY1, LOW); 
   digitalWrite(RELAY2, HIGH);
  }
} else { // Chế độ điều khiển nút bấm 
 digitalWrite(RELAY1, but1 ? HIGH : LOW); 
 digitalWrite(MOTOR, but2 ? HIGH : LOW);
 digitalWrite(BUZZER, but3 ? LOW : HIGH);
}


if (millis() - TimeMark >= 1000) { 
 TimeMark = millis();

// Gửi dữ liệu cảm biến quang lên ThingsBoard 
device.write("cambienquang", cbq); 
Serial.printf("Cảm biến quang: %d\n", cbq);

 }
}
