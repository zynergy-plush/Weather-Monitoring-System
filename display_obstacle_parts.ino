#include <U8g2lib.h>
#include <SPI.h>

const int trigPin = 4;
const int echoPin = 3;
const int buzzerPin = 2;

U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 8, /* reset=*/ 9);

void setup() {
    Serial.begin(9600);
    
    u8g2.begin();

    pinMode(trigPin, OUTPUT);
    // 
    pinMode(echoPin, INPUT);
    pinMode(buzzerPin, OUTPUT);

    // === "CAM CAR" (Letter by Letter) ===
    char camCarText[] = "CAM CAR";
    for (int i = 0; i < 7; i++) { 
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_logisoso16_tr);
        u8g2.setCursor(30, 40);
        for (int j = 0; j <= i; j++) {
            u8g2.print(camCarText[j]);
        }
        u8g2.sendBuffer();
        delay(300);
    }
    delay(1000);

    // === "Starting Up..." Animation ===
    u8g2.setFont(u8g2_font_t0_14_tr);
    for (int i = 0; i < 3; i++) {
        u8g2.clearBuffer();
        u8g2.setCursor(20, 40);
        u8g2.print("Starting Up");
        for (int j = 0; j <= i; j++) {
            u8g2.print(".");
        }
        u8g2.sendBuffer();
        delay(1000);
    }
    
    // === "WELCOME" Animation with 1 Blink ===
    char welcomeText[] = " WELCOME ";
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_logisoso16_tr);
    u8g2.setCursor(20, 40);
    u8g2.print(welcomeText);
    u8g2.sendBuffer();
    delay(500);

    // Blink Twice (ON → OFF → ON → OFF)
    u8g2.clearBuffer(); // OFF
    u8g2.sendBuffer();
    delay(250);
    
    u8g2.clearBuffer(); // ON
    u8g2.setCursor(20, 40);
    u8g2.print(welcomeText);
    u8g2.sendBuffer();
    delay(250);

    u8g2.clearBuffer(); // OFF
    u8g2.sendBuffer();
    delay(250);

    u8g2.clearBuffer(); // ON
    u8g2.setCursor(20, 40);
    u8g2.print(welcomeText);
    u8g2.sendBuffer();
    delay(1000);
}

void loop() {
    long duration, distance;

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;

    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_8x13_tr);

    if (distance >= 20) {
        u8g2.setCursor(10, 30);
        u8g2.print("Car is in");
        u8g2.setCursor(10, 50);
        u8g2.print("SAFE POSITION");
    } else {
        u8g2.setCursor(32, 20);
        u8g2.print("DISTANCE");
        u8g2.setCursor(45, 40);
        u8g2.print(distance);
        u8g2.print(" cm");
    }

    u8g2.sendBuffer();

    if (distance <= 10 && distance > 6) {
        tone(buzzerPin, 2000);
        delay(100);
        noTone(buzzerPin);
        delay(750);
    } else if (distance <= 6 && distance > 2) {
        tone(buzzerPin, 1700);
        delay(200);
        noTone(buzzerPin);
        delay(500);
    } else if (distance <= 2) {
        tone(buzzerPin, 1000);
        delay(1000);
        noTone(buzzerPin);
        delay(500);
    } else {
        noTone(buzzerPin);
    }

    delay(100);
}
