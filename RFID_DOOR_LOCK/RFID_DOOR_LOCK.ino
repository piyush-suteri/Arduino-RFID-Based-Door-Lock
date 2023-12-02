/*
 *--------------------------------------------------------------------------------------------------------------
 * RFID and Keypad based based door lock using Arduino Uno and OLED 128X64 display
 *--------------------------------------------------------------------------------------------------------------
 *
 * Author: Piyush Suteri -- https://www.instagram.com/piyush.suteri
 * This code was originally created for youtube video, link -- https://www.youtube.com/@piyushsuteri
 * Other links * Instructables -- https://www.instructables.com/member/piyushsuteri
 *
 *--------------------------------------------------------------------------------------------------------------
 * This code makes the use of MFRC522 library for RFID Module, https://github.com/miguelbalboa/rfid
 * This code also uses SPI library that is included with arduino ide and SSD1306 library which can be found at 
   library manager in arduino ide and Servo library, also included with arduino ide
 * This code demonstrates project on digital door lock system using arduino uno. Components used for this are--
   * RS522 RFID reader and writter module with RFID tags
   * 4X4 Keypad module
   * 128X64 OLED Display
   * SG90 Servo motor
   * Arduino Uno R3
 *--------------------------------------------------------------------------------------------------------------
*/


//*======================= Include necessary libraries ============*
#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>
#include <Keypad.h>
//*================================================================*


/*
 * Pinout of RFID on some of arduino boards is as follows, this is provided for spi protocol
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          A3            A3         A3         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 *
 * Pinout for other parts is in circuit diagram, see the youtube video to found it
*/


//*======================= Pinout ================================*
#define RST_PIN A3         // Reset pin RFID
#define SS_PIN 10          // SS pin RFID
#define SERVO A2           // Signal pin Servo
#define BUZZER A0          // Buzzer pin
#define LED_P A1         // Led pin
#define RESET LED_BUILTIN  // Reset for OLED


//*======================= Variables =============================*
const byte ROWS = 4;  // Four rows
const byte COLS = 4;  // Four columns

byte rowPins[ROWS] = { 2, 3, 4, 5 };  // {R1, R2, R3, R4}
byte colPins[COLS] = { 6, 7, 8, 9 };  // {C1, C2, C3, C4}

byte readCard[4];

String MasterTag = "D36A819";  // Your RFID Tag UID
String tagID = "";             // Scanned tag UID

int key_count = 0;    // Number of times any key in keypad was pressed
int wrong_pass = 0;   // Remembers number of times wrong password was entered
int oled_x_int = 70;  // Used to print password in oled display

int servo_u = 0;
int servo_v = 180;

char PASSWARD[5] = { '5', '2', '8', '6' };  // Correct passward (this is needed for keypad)
char password[5] = { '0', '0', '0', '0' };  // Stores passward entered by user

// Place keys according to order in your keypad
char Keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

//*===============================================================*
//*===============================================================*


//*========================Create instances=======================*
MFRC522 mfrc522(SS_PIN, RST_PIN);

Keypad keypad = Keypad(makeKeymap(Keys), rowPins, colPins, ROWS, COLS);

Adafruit_SSD1306 oled(RESET);

Servo myservo;
//*===============================================================*


//*===============================================================*
void setup() {
  myservo.attach(SERVO);

  pinMode(LED_P, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  //*********** initallize ***********
  Serial.begin(9600);
  SPI.begin();
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  mfrc522.PCD_Init();
  //**********************************

  //********* ready the oled *********
  oled.setTextColor(WHITE);
  oled.clearDisplay();
  oled.setCursor(0, 0);
  oled.print("## Access  Control ##");
  oled.setCursor(0, 10);
  oled.print("UID Card >>");
  oled.setCursor(0, 20);
  oled.print("Passcode >>");
  oled.display();
  delay(500);
  //**********************************

  //**** ready the serial monitor ****
  Serial.print("Access Control\n");
  Serial.print("Scan Card or enter password >>\n");
  //**********************************

  myservo.write(servo_u);

  //* indicate completetion of setup *
  digitalWrite(BUZZER, HIGH);
  delay(300);
  digitalWrite(BUZZER, LOW);
  //**********************************
}
//*===============================================================*


//*===============================================================*
void loop() {

  if (wrong_pass >= 5) {
    Serial.print("\n You have No trials left, because of wrong trials, you have to wait for 1 minute");

    oled.clearDisplay();
    oled.setCursor(0, 0);
    oled.print("You have No trials left, because of 5 wrong trials, you have to wait for 1 minute");
    oled.display();

    int i = 0;
    while (i < 20) {
      digitalWrite(BUZZER, HIGH);
      digitalWrite(LED_P, HIGH);
      delay(100);
      digitalWrite(BUZZER, LOW);
      digitalWrite(LED_P, LOW);
      delay(50);
      i++;
    }

    delay(60000);

    wrong_pass = 0;

    oled.setTextColor(WHITE);
    oled.clearDisplay();
    oled.setCursor(0, 0);
    oled.print("## Access  Control ##");
    oled.setCursor(0, 10);
    oled.print("UID Card >>");
    oled.setCursor(0, 20);
    oled.print("Passcode >>");
    oled.display();
  }

  char customKey = keypad.getKey();

  if (customKey) {

    password[key_count] = customKey;

    Serial.print(customKey);
    oled.setCursor((oled_x_int), 20);
    oled.write(customKey);
    oled.display();

    key_count++;
    oled_x_int += 7;

    if (key_count == 4) {

      key_count = 0;
      oled_x_int = 70;

      oled.clearDisplay();
      Serial.print("\n");

      if (password[0] == PASSWARD[0]
          && password[1] == PASSWARD[1]
          && password[2] == PASSWARD[2]
          && password[3] == PASSWARD[3]
          && password[4] == PASSWARD[4]) {

        wrong_pass = 0;

        digitalWrite(BUZZER, HIGH);
        digitalWrite(LED_P, HIGH);
        myservo.write(servo_v);

        delay(500);

        digitalWrite(BUZZER, LOW);
        digitalWrite(LED_P, LOW);

        Serial.print("Access Granted!\n");

        oled.setCursor(0, 0);
        oled.print("Access Granted!");
        oled.display();

        delay(1000);
      } else {

        digitalWrite(BUZZER, HIGH);
        digitalWrite(LED_P, HIGH);
        delay(100);
        digitalWrite(BUZZER, LOW);
        digitalWrite(LED_P, LOW);
        delay(50);
        digitalWrite(BUZZER, HIGH);
        digitalWrite(LED_P, HIGH);
        delay(100);
        digitalWrite(BUZZER, LOW);
        digitalWrite(LED_P, LOW);

        wrong_pass++;

        myservo.write(servo_u);

        Serial.println("Access denied\n");
        Serial.print("Wrong - password - , you have done ");
        Serial.print(wrong_pass);
        Serial.print(" wrong trials out of 5");

        oled.setCursor(0, 0);
        oled.print("Wrong - password - , you have done ");
        oled.print(wrong_pass);
        oled.print(" wrong trials out of 5");
        oled.display();
      }

      delay(2000);

      myservo.write(servo_u);

      Serial.print("Access control\n");

      oled.setTextColor(WHITE);
      oled.clearDisplay();
      oled.setCursor(0, 0);
      oled.print("## Access  Control ##");
      oled.setCursor(0, 10);
      oled.print("UID Card >>");
      oled.setCursor(0, 20);
      oled.print("Passcode >>");
      oled.display();

      Serial.print("\n");
    }
  }

  //Wait until new tag is available
  if (getID()) {

    oled.clearDisplay();
    oled.setCursor(0, 0);

    // Serial.print(tagID); // Uncomment this line to see your RFID tag's UID

    if (tagID == MasterTag) {

      wrong_pass = 0;

      digitalWrite(BUZZER, HIGH);
      digitalWrite(LED_P, HIGH);

      myservo.write(servo_v);

      delay(500);

      digitalWrite(BUZZER, LOW);
      digitalWrite(LED_P, LOW);

      Serial.print("Access Granted!\n");
      oled.setCursor(0, 0);
      oled.print("Access Granted!");
      oled.display();

      delay(1000);

    } else {

      digitalWrite(BUZZER, HIGH);
      digitalWrite(LED_P, HIGH);
      delay(100);
      digitalWrite(BUZZER, LOW);
      digitalWrite(LED_P, LOW);
      delay(50);
      digitalWrite(BUZZER, HIGH);
      digitalWrite(LED_P, HIGH);
      delay(100);
      digitalWrite(BUZZER, LOW);
      digitalWrite(LED_P, LOW);

      myservo.write(servo_u);

      wrong_pass++;

      Serial.println("Access denied\n");
      Serial.print("Wrong - card - , you have done ");
      Serial.print(wrong_pass);
      Serial.print(" wrong trials out of 5");

      oled.print("Wrong - card - , you have done ");
      oled.print(wrong_pass);
      oled.print(" wrong trials out of 5");
      oled.display();

      delay(1000);
    }

    Serial.print("ID : \t");
    Serial.print(tagID);

    oled.setTextColor(WHITE);
    oled.clearDisplay();
    oled.setCursor(0, 0);
    oled.print("## Access  Control ##");
    oled.setCursor(0, 10);
    oled.print("UID Card >>");
    oled.setCursor(0, 20);
    oled.print("Passcode >>");
    oled.display();

    oled.setCursor(70, 10);
    oled.print(tagID);
    oled.display();

    delay(2000);

    myservo.write(servo_u);

    Serial.print("\nAccess control \n");

    oled.setTextColor(WHITE);
    oled.clearDisplay();
    oled.setCursor(0, 0);
    oled.print("## Access  Control ##");
    oled.setCursor(0, 10);
    oled.print("UID Card >>");
    oled.setCursor(0, 20);
    oled.print("Passcode >>");
    oled.display();

    Serial.print("\n");
  }
}
//*===============================================================*


//*===============================================================*
boolean getID() {

  //*------------------------------------------------*
  if (!mfrc522.PICC_IsNewCardPresent()) {
    //If a new PICC placed to RFID reader continue
    return false;
  }
  //*------------------------------------------------*
  if (!mfrc522.PICC_ReadCardSerial()) {
    //Since a PICC placed get Serial and continue
    return false;
  }
  //*------------------------------------------------*


  tagID = "";


  //*------------------------------------------------*
  // The MIFARE PICCs that we use have 4 byte UID
  for (uint16_t i = 0; i < 4; i++) {
    // Adds the 4 bytes in a single String variable
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  //*------------------------------------------------*

  tagID.toUpperCase();
  mfrc522.PICC_HaltA();  // Stop reading

  return true;
}
//*===============================================================*