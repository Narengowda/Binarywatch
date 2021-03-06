/*
Name: Binary watch
Author: Narendra L

Output:
2 LEDs to blink fo rseconds
4 LEDs to display hour
5 LEDs to display minute

Features:
Auto screen off
Set time using up down buttons
*/

const int leds[2]  = {2,3};
const int ledm[6]  = {8,9,10,11,12,13};
const int ledh[4]  = {4,5,6,7};

int sec = 0;
int minute = 0;
int hour = 0;

int screentimeout = 30;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin outputs.
  for (int i = 0; i <= 5; i++) {
    pinMode(ledm[i], OUTPUT);
    digitalWrite(ledm[i], LOW);
  }
  for (int i = 0; i <= 3; i++) {
    pinMode(leds[i], OUTPUT);
    pinMode(ledh[i], OUTPUT);
    digitalWrite(leds[i], LOW);
    digitalWrite(ledh[i], LOW);
  }
  
  // Input buttons
  pinMode(14, INPUT);
  pinMode(15, INPUT);
  pinMode(16, INPUT);  
  Serial.begin(9600);
}

void led(int pin, int value) {
  if (screentimeout > 0){
    digitalWrite(pin, value);
  } else {
    digitalWrite(pin, 0);
    screentimeout = 0;
  }
}

void display_hr(int value){
    int temp = value;
    for (int i = 0; i <= 3; i++) {
    if (temp > 1) {
      led(ledh[i], temp % 2);
      temp = temp / 2;
    } else if(temp == 1){
      led(ledh[i], HIGH);
      temp = 0;
    } else {
      led(ledh[i], LOW);         
    }
  }
}

void display_min(int value){
    int temp = value;
    for (int i = 0; i <= 5; i++) {
    if (temp > 1) {
      led(ledm[i], temp % 2);
      temp = temp / 2;
    } else if(temp == 1){
      led(ledm[i], HIGH);
      temp = 0;
    } else {
      led(ledm[i], LOW);         
    }
  }
}

void clock(){
  int temp = sec;
  led(leds[0], temp % 2);
  led(leds[1], (temp+1) % 2);
  delay(1000);
  sec += 1;
  if (screentimeout > 0) {
    screentimeout -= 1;
  }
  if (sec >= 60) {
    if (screentimeout == 0){
      // wake screen for three seconds every 1 minute.
      screentimeout = 3;
    }
    sec = 0;
    minute += 1;
    if (minute >= 60){
      hour += 1;
      hour = hour % 12;
      minute = 0;
    }
  }
  display_min(minute);
  display_hr(hour);
}

void loop() {
  int menu = digitalRead(14);
  int up = digitalRead(15);
  int down = digitalRead(16);
    
  if (up == 1 | down == 1){
    screentimeout = 30;
  }
  
  Serial.print("main loop\n");
  if(menu == 1){
    while (1) {
      // Set hours
      // Let screen be awake all the time in setting mode.
      screentimeout = 1;
      digitalWrite(leds[0], HIGH);
      digitalWrite(leds[1], LOW);      
      delay(500);
      int up = digitalRead(15);
      int down = digitalRead(16);
      int menu = digitalRead(14);
      if(up == 1){
        hour += 1;
      }
      if(down == 1){
        hour -= 1;
      }
      if (hour > 12){hour = 0;}
      if (hour < 0){hour = 12;}
      display_hr(hour);
      if (menu == 1){
        menu = 0;
        break;
      }      
    }    
    
    while (1) {
      // Let screen be awake all the time in setting mode.      
      screentimeout = 1;
      digitalWrite(leds[0], LOW);      
      digitalWrite(leds[1], HIGH);      
      delay(500);
      int up = digitalRead(15);
      int down = digitalRead(16);
      int menu = digitalRead(14);
      if(up == 1){
        minute += 1;
      }
      if(down == 1){
        minute -= 1;
      }
      if (minute > 60){minute = 0;}
      if (minute < 0){minute = 60;}
      display_min(minute);
      if (menu == 1){
        menu = 0;
        break;
      }
    }
    delay(1000);
    screentimeout = 30;
  } else {
    // Run clock
    clock();
  }
}
