#include <WiFi.h>
#include <HTTPClient.h>

#define SENSOR_PIN 34 // ADC pin for HW-038

const char* ssid = ""; // WiFi SSID
const char* password = ""; // WiFi paroli

const String telegramBotToken = "";
const String chatID = "";

void setup() {
  Serial.begin(115200);
  pinMode(SENSOR_PIN, INPUT);

  // WiFi ga ulanish
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");
}

void sendTelegramMessage(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "https://api.telegram.org/bot" + telegramBotToken + "/sendMessage?chat_id=" + chatID + "&text=" + message;
    http.begin(url);
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending GET: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}

void loop() {
  int sensorValue = analogRead(SENSOR_PIN);
  float voltage = sensorValue * (3.3 / 4095.0); // ESP32 ADC qiymatini voltajga aylantirish

  // Sensor qiymati suyuqlik darajasiga yetganini aniqlash
  if (voltage > 1.0) { // Bu yerda 1.0V misol tariqasida olingan. O'z sensoringizni kalibratsiya qiling
    sendTelegramMessage("Water level detected!");
  }

  Serial.print("Sensor Value: ");
  Serial.print(sensorValue);
  Serial.print("\tVoltage: ");
  Serial.print(voltage);
  Serial.println(" V");

  delay(1000); // 1 soniya kutish
}
