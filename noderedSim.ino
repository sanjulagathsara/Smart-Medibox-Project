// Medibox Project
// Name - Gathsara J.A.S
// Index Number - 210180L

// Libraries
#include <Wire.h>             // I2C Library
#include <Adafruit_GFX.h>     // Graphics Library
#include <Adafruit_SSD1306.h> // OLED Display Library
#include <WiFi.h>             // WiFi Library
#include <PubSubClient.h>     // For MQTT
#include <ESP32Servo.h>

// Defining OLED display parameters
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define DISPLAY_RESET -1
#define DISPLAY_ADDRESS 0x3C

// Declare objects
Adafruit_SSD1306 display(DISPLAY_WIDTH, DISPLAY_HEIGHT, &Wire, DISPLAY_RESET);
WiFiClient espClient;
PubSubClient mqttClient(espClient);
Servo servo;

// Define Pins
#define BUZZER 4
#define LED_1 15
#define LEFT_LDR 34
#define RIGHT_LDR 35

const int SERVO_PIN = 18;

// Image bitmap of pills
const unsigned char pills[] PROGMEM = {
    // 'pills, 64x64px
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x07, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x30, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x40, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xc0, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x30, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x80, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
    0x00, 0x03, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
    0x00, 0x04, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
    0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00,
    0x00, 0x30, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00,
    0x00, 0x70, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
    0x00, 0xfc, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x01, 0xf9, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00,
    0x03, 0xfb, 0xc0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x03, 0xf3, 0xf0, 0x03, 0x00, 0x00, 0x00, 0x00,
    0x07, 0xf7, 0xf8, 0x02, 0x00, 0x00, 0x00, 0x00, 0x07, 0xef, 0xfe, 0x06, 0x00, 0x00, 0x00, 0x00,
    0x0f, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x1f, 0xdf, 0xfc, 0x00, 0x01, 0xff, 0xff, 0xc0,
    0x1f, 0x9f, 0xf8, 0x00, 0x01, 0xff, 0xff, 0xe0, 0x3f, 0x3f, 0xe0, 0x00, 0x01, 0xff, 0xff, 0xf0,
    0x3f, 0x7f, 0xc0, 0x00, 0x01, 0xff, 0xff, 0xf8, 0x3e, 0x7f, 0xc0, 0x00, 0x00, 0x00, 0x0f, 0xf8,
    0x7e, 0xff, 0x80, 0x00, 0x01, 0xff, 0xff, 0xf8, 0x7f, 0xff, 0x80, 0x00, 0x01, 0xff, 0xff, 0xfc,
    0x7f, 0xff, 0x00, 0x00, 0x01, 0xff, 0xff, 0xfc, 0x7f, 0xff, 0x00, 0x00, 0x01, 0xff, 0xff, 0xfc,
    0x3f, 0xff, 0x00, 0x00, 0x01, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0x00, 0x00, 0x03, 0xff, 0xe0, 0xfc,
    0x3f, 0xff, 0x00, 0x00, 0x0e, 0x0f, 0x0e, 0x3c, 0x1f, 0xff, 0x80, 0x00, 0x10, 0x02, 0x7f, 0xcc,
    0x1f, 0xff, 0x80, 0x00, 0x20, 0x00, 0xff, 0xc4, 0x0f, 0xff, 0xc0, 0x00, 0x60, 0x01, 0xff, 0x90,
    0x07, 0xff, 0xc0, 0x00, 0xc0, 0x03, 0xff, 0xb8, 0x01, 0xff, 0xe0, 0x00, 0x80, 0x07, 0xff, 0x78,
    0x00, 0x7f, 0x30, 0x00, 0x80, 0x17, 0xfe, 0x7c, 0x00, 0x00, 0x1c, 0x01, 0x80, 0x27, 0xfc, 0xfc,
    0x00, 0x00, 0x07, 0xff, 0x00, 0x67, 0xf9, 0xfc, 0x00, 0x00, 0x00, 0x7f, 0x80, 0xc7, 0xf3, 0xfc,
    0x00, 0x00, 0x00, 0x01, 0x81, 0x87, 0xe7, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x83, 0x07, 0xef, 0xfc,
    0x00, 0x00, 0x00, 0x00, 0x86, 0x07, 0xdf, 0xf8, 0x00, 0x00, 0x00, 0x00, 0xc4, 0x03, 0x9f, 0xf8,
    0x00, 0x00, 0x00, 0x00, 0x68, 0x03, 0x3f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x7f, 0xe0,
    0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0xff, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x1e, 0x3f, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// Notes for buzzer
int n_notes = 8;
int C = 262;
int D = 294;
int E = 330;
int F = 349;
int G = 392;
int A = 440;
int B = 494;
int C_H = 523;

int notes[] = {C, D, E, F, G, A, B, C_H};

// const char* ssid = "Wokwi-GUEST";
// const char* password =  "";
const char *mqttServer = "192.168.1.118";
const int mqttPort = 1883;
const char *mqttUser = "ESP32Client-1002";
const char *mqttPassword = "mmcisawesome";

int servoAngle = 0;
int minAngle = 30;
float controlFactor = 0.75;

/****************************************************************************
-----------------------------   Setting Up  ----------------------------------
****************************************************************************/

void setup()
{
    // put your setup code here, to run once:
    pinMode(BUZZER, OUTPUT);
    pinMode(LED_1, OUTPUT);

    servo.attach(SERVO_PIN, 500, 2400);
    servo.write(0);
    analogReadResolution(10);

    Serial.begin(9600);
    Serial.println("MediBox is Initializing!");

    if (!display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDRESS))
    {
        Serial.println(F("Medibox SSD1306 Display Allocation Failed"));
        for (;;)
            ;
    }

    display.clearDisplay();
    display.drawBitmap(32, 0, pills, 64, 64, WHITE);

    display.display();
    delay(1000);
    WiFi.begin("Wokwi-GUEST", "", 6);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(250);
        display.clearDisplay();
        print_line("Connecting to WiFi", 0, 0, 2);
    }

    display.clearDisplay();
    print_line("Connected to WiFi", 0, 0, 2);

    delay(2000);
    display.clearDisplay();
    print_line("Welcome to MediBox!", 0, 0, 2);

    for (int i = 0; i < 5; i++)
    {
        tone(BUZZER, notes[i]); // Beep
        delay(100);
    }
    noTone(BUZZER);

    for (int i = 0; i < 180; i++)
    {
        servo.write(i);
        delay(15);
    }
    for (int i = 180; i >= 0; i--)
    {
        servo.write(i);
        delay(15);
    }

    setup_mqtt();
    Serial.println("MQTT Done");
}

/****************************************************************************
--------------------------   Looping the Code ------------------------------
****************************************************************************/

void loop()
{

    if (!mqttClient.connected())
    {
        connectToBroker();
    }

    // Reading LDR values
    int left_value = analogRead(LEFT_LDR);
    int right_value = analogRead(RIGHT_LDR);

    // Converting LDR values between 0 - 1
    float left_send = 1 - left_value / 1023.0;
    float right_send = 1 - right_value / 1032.0;

    // Displaying values in Serial Monitor
    Serial.print("left LDR : ");
    Serial.print(left_send);
    Serial.print("  Right LDR : ");
    Serial.print(right_send);
    Serial.print("  Motor Angle : ");
    Serial.println(servoAngle);

    display.clearDisplay();
    print_line("Left LDR : " + String(left_send), 0, 0, 1);
    print_line("RIGHT LDR : " + String(right_send), 0, 20, 1);
    print_line("Servo Angle : " + String(servoAngle), 0, 50, 1);

    float max_LDR;
    int max_side;
    int D;

    if (left_send > right_send)
    {
        max_LDR = left_send;
        max_side = 0;
        D = 1.5;
    }
    else
    {
        max_LDR = right_send;
        max_side = 1;
        D = 0.5;
    }

    mqttClient.loop();
    char bufValue[8];
    char bufSide[1];

    sprintf(bufValue, "%f", max_LDR);
    sprintf(bufSide, "%d", max_side);

    mqttClient.publish("MEDIBOX-SANJULA-MAXLDR", bufValue);
    mqttClient.publish("MEDIBOX-SANJULA-MAXSIDE", bufSide);

    servoAngle = min(int(minAngle * D + (180 - minAngle) * max_LDR * controlFactor), 180);
    servo.write(servoAngle);
    delay(500);
}

/****************************************************************************
--------------------   Function to printLine in OLED  -----------------------
****************************************************************************/

void print_line(String text, int column, int row, int text_size)
{

    display.setTextSize(text_size);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(column, row); // Column,Row
    display.println(text);

    display.display();
}

/****************************************************************************
---------------   Function to beep  -------------------
****************************************************************************/

void button_beep()
{
    tone(BUZZER, notes[0]); // Beep
    delay(10);
    noTone(BUZZER);
    delay(10);
}

/****************************************************************************
---------------   Function for MQTT -------------------
****************************************************************************/

void setup_mqtt()
{
    mqttClient.setServer("test.mosquitto.org", 1883);
    mqttClient.setCallback(receiveCallback);
}

void connectToBroker()
{
    while (!mqttClient.connected())
    {
        Serial.println("Attempting MQTT Connection...");
        // display.clearDisplay();
        // print_line("Attempting MQTT Connection",0,20,2);
        if (mqttClient.connect(mqttUser))
        {
            Serial.println("Connected");
            // print_line("Connected",0,50,2);
            digitalWrite(BUZZER, HIGH);
            delay(200);
            digitalWrite(BUZZER, LOW);
            mqttClient.subscribe("MEDIBOX-SANJULA-MINANGLE");
            mqttClient.subscribe("MEDIBOX-SANJULA-CONTFACTOR");
        }
        else
        {
            Serial.println("Failed");
            // print_line("Failed",0,50,2);
            Serial.println(mqttClient.state());
            char bufTemp[10];
            sprintf(bufTemp, "%d", mqttClient.state());
            // print_line(bufTemp,80,50,2);
            digitalWrite(BUZZER, HIGH);
            delay(1000);
            digitalWrite(BUZZER, LOW);
            delay(2000);
        }
    }
}

void receiveCallback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message Arrived [");
    Serial.print(topic);
    Serial.print("]");

    String topicS = String(topic);

    for (int i = 0; i < 3; i++)
    {
        digitalWrite(BUZZER, HIGH);
        delay(30);
        digitalWrite(BUZZER, LOW);
        delay(30);
    }

    char payloadCharAr[length];

    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
        payloadCharAr[i] = (char)payload[i];
    }
    Serial.println(" ");

    if (topicS == "MEDIBOX-SANJULA-MINANGLE")
    {
        Serial.print("Min Angle Updated - ");
        minAngle = atoi(payloadCharAr);
        Serial.println(minAngle);
    }
    else if (topicS == "MEDIBOX-SANJULA-CONTFACTOR")
    {
        Serial.print("Control Factor Updated - ");
        controlFactor = atof(payloadCharAr);
        Serial.println(controlFactor);
    }
}