#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define RST_PIN 9
#define SS_PIN A4
#define LED_PIN 4
#define BUZZER_PIN 5

MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo servo;

byte validUID[] = {0x05, 0x89, 0xD3, 0xD7, 0x42, 0x32, 0x00}; 

void setup() {
  
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  servo.attach(3); 
  servo.write(0); 
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  Serial.print("UID Kartu: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  if (checkUID(mfrc522.uid.uidByte, mfrc522.uid.size)) {
    Serial.println("Kartu valid!");
  
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(1000); 
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);

    servo.write(90); 
    delay(2000); 
    servo.write(0); 
  } else {
    Serial.println("Kartu tidak valid!");
    
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(3000); 
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  } 
  mfrc522.PICC_HaltA();
}
bool checkUID(byte* uid, byte size) {
  if (size != sizeof(validUID)) return false;
  for (byte i = 0; i < size; i++) {
    if (uid[i] != validUID[i]) return false;
  }
  return true;
}
