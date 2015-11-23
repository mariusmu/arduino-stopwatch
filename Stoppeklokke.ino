/**
 * ------ A counter program for Arduino ------
 * @author: Marius Munthe-Kaas
 * @email: mariusmu@mail.uio.no
 */

#include <LiquidCrystal.h>

const int BUTTON1 = 12;     //The pin for button1
const int BUTTON2 = 13;     //The pin for button2

long lastPressedBtn1 = 0;    //Last millisecond we pressed button1
long lastPressedBtn2 = 0;    //Last millisecond we pressed button2;

boolean run = false;         //Should the system run?

int button1State = LOW;      //The value the button1 reads
int button1CurrState = LOW;  //The last value of button1
int button2State = LOW;      //The value of button1
int button2CurrState = LOW;  //The last value of button2

LiquidCrystal lcd (9, 8, 5, 4, 3, 2); //Assign correct Arduino digital-port

long timer = 0; //The timer

int second = 0;
int minute = 0;
int tenth = 0;

/**
 * SETUP
 */
 
void setup() {
  pinMode(BUTTON1, INPUT);
  pinMode(BUTTON2, INPUT);
  lcd.setCursor(0,1);
  lcd.print("00:00:0");

}

/**
 * This is the timer function
 * For each tenth of a second call the tick() function
 */
void tickClock() {
  Serial.println(millis() /10);
  if((timer - millis()/100) >= 100 || timer == 0) {
    tick();
    timer = millis()/100;
  }
}

/**
 * Loop
 */
void loop() {
  tickClock(); //Start ticking the clock
  button1State = digitalRead(BUTTON1);
  button2State = digitalRead(BUTTON2); 
  checkStart();
  checkClear();     
}

/**
 * Check for button1 press
 * If the first button is pressed, 
 * be sure that there is at least 100 ms since the last press
 */
void checkStart() {
  if(button1State != button1CurrState) {    //Check if the button state has changed
    if(button1State == HIGH && (millis()-lastPressedBtn1) > 100 || lastPressedBtn1 == 0) {
      lastPressedBtn1 = millis();
      run = !run; //Switch the running state
    }    
  }
  button1CurrState = button1State;    //Set the current state equals the button state
}

/**
 * Check for button 2 press
 * If the second button is pressed, 
 * be sure that there is at least 100 since the last press
 */
void checkClear() {
  if(button2State != button2CurrState) {
    if(button2State == HIGH && (millis()-lastPressedBtn2) > 100 || lastPressedBtn2 == 0) {  
      lastPressedBtn2 = millis();
      clearAll(); //Clear the display
    }    
  }
  button2CurrState = button2State;   //Set btn2 current state equals the btn2State
}

/** 
 * Tick one tenth of a second if it should be running
 */
void tick() {
  if(run) {
    updateLCD();
    
    
    
    if(tenth == 9) {
      tenth = 0;
      
      if(second == 59) {
        second = 0;
        minute++;
        
      } else {
        second++;
      }
    } else {
    tenth++;
    }
  }
}
/** 
 * Update the LCD-display
 * The extra 0s and : is for the sake of beauty :)
 */
void updateLCD() {
  lcd.setCursor(0,1);
  if(minute < 10) {      // If hour does not have 2 digits
    lcd.print("0");
  }
  lcd.print(minute, DEC);
  lcd.print(":");

  if(second < 10) {    // If minute does not have 2 digits
    lcd.print("0");
  }
  lcd.print(second, DEC);
  lcd.print(":");

  lcd.print(tenth, DEC); 
}

/**
 * Clear the display
 * Set all time variables equal 0
 */
void clearAll() {
  run = false;
  second = 0;
  minute = 0;
  tenth = 0;
  updateLCD(); 

}
