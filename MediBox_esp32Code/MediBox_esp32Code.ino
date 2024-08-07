// Medibox Project
// Name - Gathsara J.A.S
// Index Number - 210180L

#include <WiFiManager.h>


// Libraries
#include <Wire.h> // I2C Library
#include <Adafruit_GFX.h> // Graphics Library
#include <Adafruit_SSD1306.h> // OLED Display Library
#include <DHTesp.h> // DHT Library
#include <WiFi.h> // WiFi Library

// Defining OLED display parameters
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define DISPLAY_RESET -1
#define DISPLAY_ADDRESS 0x3C

String WIFI_SSID = "ElectroCube Wifi2";
String WIFI_PASSWORD = "electrocubesanjula88";

// NTP Server details
#define NTP_SERVER   "pool.ntp.org"
int UTC_OFFSET = (5*3600)+1800;
int UTC_OFFSET_DST = 0;

// Declare objects
Adafruit_SSD1306 display(DISPLAY_WIDTH,DISPLAY_HEIGHT,&Wire,DISPLAY_RESET);
DHTesp dhtSensor;

const unsigned char pills [] PROGMEM = {
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
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Define Pins
#define BUZZER 18
#define LED_1 23
#define PB_CANCEL 36
#define PB_OK 34
#define PB_UP 39
#define PB_DOWN 35
#define DHTPIN 15

// Gllobal Variables
int days = 0;
int hours = 0;
int minutes = 0;
int seconds = 0;

unsigned long  time_now = 0;
unsigned long  time_last = 0;

bool alarm_enabled = true;
int n_alarms = 3;
int alarm_hours[] = {0,1,2};
int alarm_minutes[] = {1,10,33};
bool alarm_triggered[] = {false,false,false};

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

int notes[] = {C,D,E,F,G,A,B,C_H};

// Mode parameters
int current_mode = 0;
int max_modes = 5;
String modes[] = {"1 - Set Time Zone","2 - Set Alarm 1","3 - Set Alarm 2","4 - Set Alarm 3","5 - Disable Alarms"};

/****************************************************************************
-----------------------------   Setting Up  ----------------------------------
****************************************************************************/

void setup() {
  // put your setup code here, to run once:
  pinMode(BUZZER, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(PB_CANCEL, INPUT);
  pinMode(PB_OK,INPUT);
  pinMode(PB_UP,INPUT);
  pinMode(PB_DOWN,INPUT);

  Serial.begin(9600);
  Serial.println("MediBox is Initializing!");

  

  if(!display.begin(SSD1306_SWITCHCAPVCC,DISPLAY_ADDRESS)){
    Serial.println(F("Medibox SSD1306 Display Allocation Failed"));
    for(;;);
  }


  display.clearDisplay();
  display.drawBitmap(32, 0, pills, 64, 64, WHITE);
 
  display.display();

  WiFiManager wm;

  

  for(int i=0;i<5;i++){
    digitalWrite(LED_1,HIGH);
    delay(100);
    digitalWrite(LED_1,LOW);
    delay(100);
  }

  dhtSensor.setup(DHTPIN,DHTesp::DHT11);


  

  display.clearDisplay();
  print_line("Smart",30,10,2);
  print_line("Medibox",20,40,2);

  digitalWrite(BUZZER,HIGH);
  delay(500);
  digitalWrite(BUZZER,LOW);
  delay(500);

  WiFi.begin(WIFI_SSID,WIFI_PASSWORD,6);
  display.clearDisplay();
  print_line("Connecting to WiFi",10,10,1);
  print_line(WIFI_SSID,0,50,1);
  
  int tempConnectionCounter = 0;
  while(WiFi.status() != WL_CONNECTED){
    delay(250);
    //display.clearDisplay();
    //print_line("Connecting to WiFi",0,20,1);
    print_line(".",tempConnectionCounter,30,1);
    tempConnectionCounter += 4;
    if(tempConnectionCounter >= 127){
      //tempConnectionCounter = 0;
      //display.clearDisplay();
      //print_line("Connecting to WiFi",10,10,1);
      //print_line(WIFI_SSID,0,50,1);
      button_beep();
      break;
    }
  }

  display.clearDisplay();

  if(tempConnectionCounter >= 127){
    bool res = wm.autoConnect("MediBox","12345678");
    if(!res){
      Serial.println("Wifi AP failed");
      print_line("Connecting to WiFi",10,10,1);
      delay(1000);
      ESP.restart();
    }
    else{
      Serial.println("Wifi AP Done");
      print_line("Connecting to WiFi",10,10,1);
      delay(1000);
    }
  }

  display.clearDisplay();
  print_line("Connected to WiFi",0,20,2);

  configTime(UTC_OFFSET,UTC_OFFSET_DST,NTP_SERVER);

  // Displaying buffer on the screen
  //display.display();
  delay(2000);
  display.clearDisplay();
  print_line("Welcome to MediBox!",0,20,2);

  for(int i=0;i<5;i++){
    digitalWrite(BUZZER,HIGH);
    delay(150);
    digitalWrite(BUZZER,LOW);
    delay(50);
  }

  delay(1000);


}

/****************************************************************************
--------------------------   Looping the Code ------------------------------
****************************************************************************/


void loop() {
  // put your main code here, to run repeatedly:
  update_time_with_check_alarm();
  if(digitalRead(PB_OK) == LOW){
    button_beep();
    delay(200);
    go_to_menu();
  }
  check_temp();
  
}

/****************************************************************************
--------------------   Function to printLine in OLED  -----------------------
****************************************************************************/

void print_line(String text,int column,int row,int text_size){

  display.setTextSize(text_size);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(column,row); // Column,Row
  display.println(text);
  display.display();
}

/****************************************************************************
--------------------   Function to print Time in OLED  -----------------------
****************************************************************************/

void print_time_now(void){

  display.clearDisplay();
  print_line(String(days), 0, 0, 2);
  //print_line(" - ", 20, 0, 2);
  print_line(String (hours), 30, 30, 2);
  print_line(":", 50, 30, 2);
  print_line(String(minutes),60, 30, 2);
  print_line(" :", 70, 30, 2);
  print_line(String(seconds),90, 30, 2);

}

/****************************************************************************
-------------------------   Function to Update Time  -----------------------
****************************************************************************/

void update_time(void){

  struct tm timeinfo;
  getLocalTime(&timeinfo);

  char timeHour[3];
  strftime(timeHour,3,"%H",&timeinfo);
  hours = atoi(timeHour);

  char timeMinute[3];
  strftime(timeMinute,3,"%M",&timeinfo);
  minutes = atoi(timeMinute);

  char timeSecond[3];
  strftime(timeSecond,3,"%S",&timeinfo);
  seconds = atoi(timeSecond);

  char timeDay[3];
  strftime(timeDay,3,"%d",&timeinfo);
  days = atoi(timeDay);


}

/****************************************************************************
-----------   Function to Updating the time with Checking Alarm  -----------
****************************************************************************/

void update_time_with_check_alarm(void){
  print_time_now();
  update_time();


  if(alarm_enabled == true){
    for(int i=0;i<n_alarms;i++){
      if(alarm_triggered[i] == false && alarm_hours[i] == hours && alarm_minutes[i] == minutes){
        ring_alarm();
        alarm_triggered[i] = true;
      }
    }
  }

  delay(100);

}

/****************************************************************************
--------------------   Function to Ring the Alarm  -----------------------
****************************************************************************/

void ring_alarm(void){
  display.clearDisplay();
  print_line("Medicine Time!",0,20,2);
  digitalWrite(LED_1,HIGH);

  bool break_happened = false;

  while(digitalRead(PB_CANCEL) == HIGH && break_happened == false){
    // Ring the Buzzer
    for(int i=0;i<n_notes;i++){
      if(digitalRead(PB_CANCEL) == LOW){
        delay(200);
        break_happened = true;
        break;
      }
      tone(BUZZER,notes[i]);
      delay(500);
      noTone(BUZZER);
      delay(2);
    }
  }

  digitalWrite(LED_1,LOW);
  display.clearDisplay();

}

/****************************************************************************
--------------------------   Function to Go to Menu  -----------------------
****************************************************************************/

void go_to_menu(void){
  display.clearDisplay();
  print_line("Menu",30,25,3);
  delay(1000);

  while(digitalRead(PB_CANCEL) == HIGH){

    display.clearDisplay();
    print_line(modes[current_mode],0,2,2);
    delay(1000);

    int pressed = wait_for_button_press();

    if(pressed == PB_UP){
      delay(200);
      current_mode += 1;
      current_mode = current_mode % max_modes;
    }
    else if(pressed == PB_DOWN){
      delay(200);
      current_mode -= 1;
      if(current_mode <= -1){
        current_mode = max_modes - 1;
      }
    }
    else if(pressed == PB_OK){
      delay(200);
      run_mode(current_mode);
    }
    else if(pressed == PB_CANCEL){
      delay(200);
      break;
    }

  }
}

/****************************************************************************
------------------   Function to wait for button press  --------------------
****************************************************************************/

int wait_for_button_press(){
  while(true){
    if(digitalRead(PB_UP) == LOW){
      button_beep();
      delay(200);
      return PB_UP;
    }
    else if(digitalRead(PB_DOWN) == LOW){
      button_beep();
      delay(200);
      return PB_DOWN;
    }
    else if(digitalRead(PB_OK) == LOW){
      button_beep();
      delay(200);
      return PB_OK;
    }
    else if(digitalRead(PB_CANCEL) == LOW){
      button_beep();
      delay(200);
      return PB_CANCEL;
    }
  }
}

/****************************************************************************
---------------------------   Function to Run Mode  -----------------------
****************************************************************************/

void run_mode(int mode){
  if(mode == 0){
    set_time_zone();
  }
  else if(mode == 1 || mode == 2 || mode == 3){
    set_alarm(mode - 1);
  }
  else if(mode == 4){
    alarm_enabled = false;
  }
}

/****************************************************************************
-----------------------   Function to Set Time Zone   -----------------------
****************************************************************************/

void set_time_zone(){
  int temp_hour = 0;

  while(true){
    display.clearDisplay();
    print_line("Enter hour: " + String(temp_hour),0,0,2);

    int pressed = wait_for_button_press();

      if(pressed == PB_UP){
        delay(200);
        temp_hour += 1;
        temp_hour = temp_hour % 24;
      }
      else if(pressed == PB_DOWN){
        delay(200);
        temp_hour -= 1;
      }
      else if(pressed == PB_OK){
        delay(200);
        UTC_OFFSET = (temp_hour * 3600);
        break;
      }
      else if(pressed == PB_CANCEL){
        delay(200);
        break;
      }
  }

  int temp_minute = 0;

  while(true){
    display.clearDisplay();
    print_line("Enter Minute: " + String(temp_minute),0,0,2);

    int pressed = wait_for_button_press();

      if(pressed == PB_UP){
        delay(200);
        temp_minute += 30;
        temp_minute = temp_minute % 60;
      }
      else if(pressed == PB_DOWN){
        delay(200);
        temp_minute -= 30;
        if(temp_minute <= -1){
          temp_minute = 59;
        }
      }
      else if(pressed == PB_OK){
        delay(200);
        if(temp_hour >= 0){
          UTC_OFFSET = UTC_OFFSET+(temp_minute * 60);
        }
        else{
          UTC_OFFSET = UTC_OFFSET-(temp_minute * 60);
        }
        break;
      }
      else if(pressed == PB_CANCEL){
        delay(200);
        break;
      }
  }

  configTime(UTC_OFFSET,UTC_OFFSET_DST,NTP_SERVER);


  display.clearDisplay();
  print_line("Time Zone is set",0,0,2); // Time zone set
  delay(1000);

}

/****************************************************************************
------------------------   Function to Set Alarms  -----------------------
****************************************************************************/

void set_alarm(int alarm){
  int temp_hour = alarm_hours[alarm];

  while(true){
    display.clearDisplay();
    print_line("Enter hour: " + String(temp_hour),0,0,2); 

    int pressed = wait_for_button_press();

      if(pressed == PB_UP){
        delay(200);
        temp_hour += 1;
        temp_hour = temp_hour % 24;
      }
      else if(pressed == PB_DOWN){
        delay(200);
        temp_hour -= 1;
        if(temp_hour <= -1){
          temp_hour = 23;
        }
      }
      else if(pressed == PB_OK){
        delay(200);
        alarm_hours[alarm] = temp_hour;
        break;
      }
      else if(pressed == PB_CANCEL){
        delay(200);
        break;
      }
  }

  int temp_minute = alarm_minutes[alarm];

  while(true){
    display.clearDisplay();
    print_line("Enter Minute: " + String(temp_minute),0,0,2);

    int pressed = wait_for_button_press();

      if(pressed == PB_UP){
        delay(200);
        temp_minute += 1;
        temp_minute = temp_minute % 60;
      }
      else if(pressed == PB_DOWN){
        delay(200);
        temp_minute -= 1;
        if(temp_minute <= -1){
          temp_minute = 59;
        }
      }
      else if(pressed == PB_OK){
        delay(200);
        alarm_minutes[alarm] = temp_minute;
        break;
      }
      else if(pressed == PB_CANCEL){
        delay(200);
        break;
      }
  }

  display.clearDisplay();
  alarm_enabled = true;
  alarm_triggered[alarm] = false;
  print_line("Alarm "+String(alarm+1)+" is set",0,0,2);
  delay(1000);
}

/****************************************************************************
---------------   Function to beep when the button pressed -------------------
****************************************************************************/

void button_beep(){
  tone(BUZZER,notes[0]); // Beep
  delay(10);
  noTone(BUZZER);
  delay(10);
}

/****************************************************************************
---------------   Function to Check Temperature and Humidity  ---------------
****************************************************************************/

void check_temp(){
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  if(data.temperature > 32){
    display.clearDisplay();
    print_line(" TEMP HIGH",0,30,2);
    delay(1000);
  }
  if(data.temperature < 26){
    display.clearDisplay();
    print_line(" TEMP LOW",0,30,2);
    delay(1000);
  }
  if(data.humidity > 80){
    display.clearDisplay();
    print_line(" HUMIDITY",0,20,2);
    print_line("   HIGH",0,40,2);
    delay(1000);
  }
  if(data.humidity < 40 && false){
    display.clearDisplay();
    print_line(" HUMIDITY",0,20,2);
    print_line("   LOW",0,40,2);
    delay(1000);
  }
}

