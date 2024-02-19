#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHTesp.h>
#include <WiFi.h>

// Defining OLED display parameters
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define DISPLAY_RESET -1
#define DISPLAY_ADDRESS 0x3C

// NTP Server details
#define NTP_SERVER   "pool.ntp.org"
int UTC_OFFSET = 0;
int UTC_OFFSET_DST = 0;

// Declare objects
Adafruit_SSD1306 display(DISPLAY_WIDTH,DISPLAY_HEIGHT,&Wire,DISPLAY_RESET);
DHTesp dhtSensor;

// Define Pins
#define BUZZER 5
#define LED_1 15
#define PB_CANCEL 34
#define PB_OK 32
#define PB_UP 35
#define PB_DOWN 33
#define DHTPIN 12

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
int max_modes = 4;
String modes[] = {"1 - Set Time Zone","2 - Set Alarm 1","3 - Set Alarm 2","4 - Set Alarm 3","5 - Disable Alarm"};

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

  dhtSensor.setup(DHTPIN,DHTesp::DHT22);


  Serial.begin(9600);
  Serial.println("MediBox is Initializing!");

  if(!display.begin(SSD1306_SWITCHCAPVCC,DISPLAY_ADDRESS)){
    Serial.println(F("Medibox SSD1306 Display Allocation Failed"));
    for(;;);
  }

  WiFi.begin("Wokwi-GUEST","",6);
  while(WiFi.status() != WL_CONNECTED){
    delay(250);
    display.clearDisplay();
    print_line("Connecting to WiFi",0,0,2);
  }

  display.clearDisplay();
  print_line("Connected to WiFi",0,0,2);

  configTime(UTC_OFFSET,UTC_OFFSET_DST,NTP_SERVER);

  // Displaying buffer on the screen
  //display.display();
  delay(2000);
  display.clearDisplay();
  print_line("Welcome to MediBox!",0,0,2);

  delay(2000);


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
   // Clearing the buffer
  //display.clearDisplay();


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
  print_line(":", 20, 0, 2);
  print_line(String (hours), 30, 0, 2);
  print_line(":", 50, 0, 2);
  print_line(String(minutes),60, 0, 2);
  print_line(" :", 80, 0, 2);
  print_line(String(seconds),90, 0, 2);

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

}

/****************************************************************************
--------------------   Function to Ring the Alarm  -----------------------
****************************************************************************/

void ring_alarm(void){
  display.clearDisplay();
  print_line("Medicine Time!",0,0,2);
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
  print_line("Menu",0,2,2);
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

    int pressed = wait_for_button_press(); //Waiting until the button is pressed

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

  configTime(UTC_OFFSET,UTC_OFFSET,NTP_SERVER);


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
    print_line("Enter hour: " + String(temp_hour),0,0,2); #Hour Entering

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
  print_line("Alarm "+String(alarm)+" is set",0,0,2);
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
    print_line("TEMP HIGH",0,40,2);
    delay(1000);
  }
  if(data.temperature < 26){
    display.clearDisplay();
    print_line("TEMP LOW",0,40,2);
    delay(1000);
  }
  if(data.humidity > 80){
    display.clearDisplay();
    print_line("HUMIDITY HIGH",0,50,1);
    delay(1000);
  }
  if(data.humidity < 60){
    display.clearDisplay();
    print_line("HUMIDITY LOW",0,50,1);
    delay(1000);
  }
}