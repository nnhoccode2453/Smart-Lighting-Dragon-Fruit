#include <ESP8266WiFi.h>
//#include <espnow.h> #include <SimpleTBDevice.h>
// Thông tin Wi-Fi và ThingsBoard
#define SSID "VLDT-E303_2.4G"  // Tên Wi-Fi
#define PASSWORD "dpee2024@vldt" // Mật khẩu Wi-Fi #define SERVER_ADDR "demo.thingsboard.io"
#define ACCESS_TOKEN "dkPiZeE2zrwuI2PLbU0t" #define cambiendat A0
#define MOTOR D0 #define BUZZER D2

unsigned long TimeMark = 0; WiFiClient client;
TBDevice device(ACCESS_TOKEN);

void setup() { Serial.begin(115200);
// Kết nối Wi-Fi WiFi.begin(SSID, PASSWORD);
Serial.println("Đang kết nối Wi-Fi...");
while (WiFi.status() != WL_CONNECTED) { delay(500);
Serial.print(".");
}
Serial.println("\nĐã kết nối Wi-Fi!"); 


// Kết nối ThingsBoard device.begin(SERVER_ADDR, client);
pinMode(MOTOR, OUTPUT); pinMode(BUZZER, OUTPUT);
pinMode(cambiendat, INPUT);
}


void loop() {
int cbd = analogRead(cambiendat); // Đọc cảm biến độ ẩm đất
int mappedValue = map(cbd, 0, 1024, 0, 100); // Ánh xạ giá trị từ 0-1023
thành 0-100
int phantram = 100 - mappedValue; // Chuyển đổi thành giá trị phần trăm int thr3 = device.read("nguongdat").asInt();

bool mode_chedo = device.read("mode").asBoolean(); if (mode_chedo) {
if (phantram < thr3) { digitalWrite(MOTOR, HIGH); digitalWrite(BUZZER, LOW);
} else if (phantram > thr3) { digitalWrite(MOTOR, LOW); digitalWrite(BUZZER, HIGH);
}
}else{
digitalWrite(MOTOR, LOW); digitalWrite(BUZZER, HIGH);
}


if (millis() - TimeMark >= 1000) { TimeMark = millis(); device.write("cambiendoamdat", phantram);
Serial.printf("Cảm biến độ ẩm đất: %d (%%: %d)\n", cbd, phantram);
}
}
