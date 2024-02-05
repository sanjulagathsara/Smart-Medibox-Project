#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Defining OLED display parameters
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define DISPLAY_RESET -1
#define DISPLAY_ADDRESS 0x3C

Adafruit_SSD1306 display(DISPLAY_WIDTH,DISPLAY_HEIGHT,&Wire,DISPLAY_RESET);

#define BUZZER 5
#define LED_1 15

int days = 0;
int hours = 0;
int minutes = 0;
int seconds = 0;

unsigned long  time_now = 0;
unsigned long  time_last = 0;

bool alarm_enabled = true;
int n_alarms = 2;
int alarm_hours[] = {0,1};
int alarm_minutes[] = {1,10};
bool alarm_triggered[] = {false,false};

int n_notes = 8;
int C = 262;
int D = 294;
int E = 330;
int F = 349;
int G = 392;
int A = 440;
int B = 494;
int C_H = 523;

int notes[] = {C,D,E,F,G,A,B,C_H};

void setup() {
  // put your setup code here, to run once:
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_1, OUTPUT);


  Serial.begin(9600);
  Serial.println("MediBox is Initializing!");

  if(!display.begin(SSD1306_SWITCHCAPVCC,DISPLAY_ADDRESS)){
    Serial.println(F("Medibox SSD1306 Display Allocation Failed"));
    for(;;);
  }

  // Displaying buffer on the screen
  display.display();
  delay(2000);

  print_line("Welcome to MediBox!",0,0,2);

  delay(2000);


}

void loop() {
  // put your main code here, to run repeatedly:
  update_time_with_check_alarm();
  
}

void print_line(String text,int column,int row,int text_size){
   // Clearing the buffer
  //display.clearDisplay();


  display.setTextSize(text_size);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(column,row); // Column,Row
  display.println(text);

  display.display();
}

void print_time_now(void){

  display.clearDisplay();
  print_line(String(days), 0, 0, 2);
  print_line(":", 20, 0, 2);
  print_line(String (hours), 30, 0, 2);
  print_line(":", 50, 0, 2);
  print_line(String(minutes),60, 0, 2);
  print_line(" :", 80, 0, 2);
  print_line(String(seconds),90, 0, 2);

}

void update_time(void){
  time_now = millis()/1000;

  seconds = time_now - time_last;

  if(seconds >= 60){
    minutes += 1;
    time_last += 60;
  }

  if(minutes == 60){
    hours += 1;
    minutes = 0;
  }

  if(hours == 24){
    days += 1;
    hours = 0;
  }

}

void update_time_with_check_alarm(void){
  print_time_now();
  update_time();


  if(alarm_enabled == true){
    for(int i=0;i<n_alarms;i++){
      if(alarm_triggered[i] == false && alarm_hours[i] == hours && alarm_minutes[i] == minutes){
        ring_alarm();
      }
    }
  }

}

void ring_alarm(void){
  display.clearDisplay();
  print_line("Medicine Line!",0,0,2);
  digitalWrite(LED_1,HIGH);

  // Ring the Buzzer
  for(int i=0;i<n_notes;i++){
    tone(BUZZER,notes[i]);
    delay(500);
    noTone(BUZZER);
    delay(2);
  }
  digitalWrite(LED_1,LOW);
  display.clearDisplay();

}