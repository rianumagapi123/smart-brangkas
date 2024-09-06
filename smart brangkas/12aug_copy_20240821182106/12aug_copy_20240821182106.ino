#include <EEPROM.h>  
#include <Servo.h>   
#include <Keypad.h>  
#include <MFRC522.h>   
#include <SPI.h>   
#include <Wire.h>   
#include <LiquidCrystal_I2C.h>   
LiquidCrystal_I2C lcd(0x27, 20, 4);   
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo ServoMotor;
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = { A0, A1, A2, A3 };
byte colPins[COLS] = { 5, 4, 3, 2 };
Keypad cutemkey = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
String read_rfid;
String ok_rfid_1 = "405ad0cf";   
String ok_rfid_2 = "e3a67630";   
String ok_rfid_3 = "58f5f7574910";
const String password_1 = "1234";   
const String password_2 = "ABCD";   
const String password_3 = "1111"; 
String input_password;
boolean RFIDMode = true;
char key = 0;
char dataAplikasi;
int sudutservo;
String kondisi_Pintu;
unsigned long waktuSekarang = millis();
unsigned long waktu_lcd = 0;
unsigned long waktu_matilcd = 20000;  

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  ServoMotor.attach(6);
  input_password.reserve(10);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  sudutservo = EEPROM.read(0);
  ServoMotor.write(sudutservo);
}

void loop() {
  if (millis() >= waktu_lcd + waktu_matilcd) {
    lcd.noBacklight();
  }
  while (Serial.available() > 0) {
    dataAplikasi = Serial.read();
    delay(1000);
    if (dataAplikasi == 'A') {
      Buka_pintu ();
    }
    if (dataAplikasi == 'B') {
      Tutup_pintu ();
    }
  }
  key = cutemkey.getKey();
  if (key) {
    lcd.backlight();
    waktuSekarang = millis();
    waktu_lcd = waktuSekarang;
    Serial.println(key);
    if (key == '0') {
      Tutup_pintu ();
    }
  }
  if (sudutservo == 90) {
    kondisi_Pintu = ("PINTU TIDAK DI KUNCI");
  }
  if (sudutservo == 0) {
    kondisi_Pintu = ("   PINTU TERKUNCI");
  }


  if (RFIDMode == true) {
    lcd.setCursor (3, 0);
    lcd.print("PROJEK AKHIR");
    lcd.setCursor (2, 1);
    lcd.print("BRANGKAS PINTAR");
    lcd.setCursor (1, 2);
    lcd.print("<< STATUS PINTU >>");
    lcd.setCursor (0, 3);
    lcd.print(kondisi_Pintu);

    if (!mfrc522.PICC_IsNewCardPresent()) {
      return;
    }
    if (!mfrc522.PICC_ReadCardSerial()) {
      return;
    }

    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println(read_rfid);

    if (read_rfid == ok_rfid_1 || read_rfid == ok_rfid_2 ) {
      lcd.clear();
      lcd.setCursor (2, 0);
      lcd.print("AKSES DITERIMA");
      lcd.setCursor (1, 1);
      lcd.print("SILAHKAN MASUKKAN");
      lcd.setCursor (5, 2);
      lcd.print("PASSWORD");
      RFIDMode = false;
    }
    else {
      lcd.clear();
      lcd.setCursor (3, 0);
      lcd.print("AKSES DITOLAK");
      lcd.setCursor (2, 1);
      lcd.print("KARTU AKSES ANDA");
      lcd.setCursor (0, 2);
      lcd.print("BELUM TERDAFTAR!!!!!");
      delay(5000);
      lcd.clear();
      RFIDMode = true;
    }
  }
  if (RFIDMode == false) {
    if (key) {
      Serial.println(key);
      if (key == '*') {
        input_password = ""; 
        lcd.clear();
        lcd.setCursor (2, 0);
        lcd.print("AKSES DITERIMA");
        lcd.setCursor (1, 1);
        lcd.print("SILAHKAN MASUKKAN");
        lcd.setCursor (5, 2);
        lcd.print("PASSWORD");
      }
      else if (key == '#') {
        lcd.clear();
        if (input_password == password_1 || input_password == password_2 || input_password == password_3) {
          Buka_pintu ();
          lcd.clear();
          RFIDMode = true;
        }
        else {
          lcd.setCursor(2, 0);
          lcd.print("PASSWORD SALAH");
          lcd.setCursor(4, 1);
          lcd.print("COBA LAGI");
          delay(2000);
          lcd.clear();
          RFIDMode = true;
        }

        input_password = ""; 
      else {
        if (input_password.length() == 0) {
        }
        input_password += key; 
        lcd.setCursor(input_password.length(), 3); 
        lcd.print('*');                 
      }
    }
    if (!mfrc522.PICC_IsNewCardPresent()) {
      return;
    }
    if (!mfrc522.PICC_ReadCardSerial()) {
      return;
    }
    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println(read_rfid);

    if (read_rfid == ok_rfid_1 || read_rfid == ok_rfid_2 ) {
      lcd.clear();
      lcd.setCursor (2, 0);
      lcd.print("AKSES DITERIMA");
      lcd.setCursor (1, 1);
      lcd.print("SILAHKAN MASUKKAN");
      lcd.setCursor (5, 2);
      lcd.print("PASSWORD");
      RFIDMode = false;
    }
    else {
      lcd.clear();
      lcd.setCursor (3, 0);
      lcd.print("AKSES DITOLAK");
      lcd.setCursor (2, 1);
      lcd.print("KARTU AKSES ANDA");
      lcd.setCursor (0, 2);
      lcd.print("BELUM TERDAFTAR!!!!!");
      delay(5000);
      lcd.clear();
      RFIDMode = true;
    }
  }
}
void dump_byte_array(byte * buffer, byte bufferSize) {
  lcd.backlight();
  waktuSekarang = millis();
  waktu_lcd = waktuSekarang;
  read_rfid = "";
  for (byte i = 0; i < bufferSize; i++) {
    read_rfid = read_rfid + String(buffer[i], HEX);
  }
}
void Buka_pintu () {
  lcd.backlight();
  waktuSekarang = millis();
  waktu_lcd = waktuSekarang;
  sudutservo = 0;          
  ServoMotor.write(sudutservo);
  delay(500);
  EEPROM.write(0, sudutservo);
  delay(200);
  lcd.clear();
  lcd.setCursor (1, 2);
  lcd.print("KUNCI PINTU DIBUKA");
  delay(5000);
  lcd.clear();
}
void Tutup_pintu () {
  lcd.backlight();
  waktuSekarang = millis();
  waktu_lcd = waktuSekarang;
  sudutservo = 90;   
  ServoMotor.write(sudutservo);
  delay(500);
  EEPROM.write(0, sudutservo);
  delay(200);
  lcd.clear();
  lcd.setCursor (3, 2);
  lcd.print("PINTU DIKUNCI");
 
  delay(5000);
  lcd.clear();
}
