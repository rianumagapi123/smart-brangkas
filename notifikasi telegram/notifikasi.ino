#include <CTBot.h>

#define pinreed  5

CTBot mybot;
String ssid = "ABCD";
String pass = "00000000";

String token = "7215795170:AAG9lTDiIyCo9VQ965HL0bJenKTiEH3hW3g";
const int id = 1035445916;

void setup() {
  Serial.begin(9600);
  pinMode(pinreed, INPUT_PULLUP); 

  mybot.wifiConnect(ssid, pass);
  mybot.setTelegramToken(token);

  if (mybot.testConnection()) {
    Serial.println("Terhubung ke Telegram.");
  } else {
    Serial.println("Gagal terhubung ke Telegram.");
  }
}

void loop() {
  int state = digitalRead(pinreed);
  Serial.print("Status Reed: ");
  Serial.println(state);

  if (state == HIGH) {
    mybot.sendMessage(id, "PINTU DIBUKA");
    delay(1000); 
  }

  delay(900);
}
