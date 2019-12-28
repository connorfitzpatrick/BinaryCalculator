#include <LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);
const int switchPin1 = 6;
const int switchPin2 = 7;
const int brightPin = 9;
int switchState1 = 0;
int switchState2 = 0;
const int prevSwitchState1 = 0;
const int prevSwitchState2 = 0;
boolean loopy = false;
int brightness = 20;
void setup() {
  // Welcome Screen & Contrast are dealt with in setup()
  Serial.begin(9600);
  lcd.begin(16,2);
  pinMode(switchPin1, INPUT);
  pinMode(switchPin2, INPUT);
  pinMode(brightPin, OUTPUT);
  analogWrite(9,brightness);
  lcd.print("Welcome!");
  lcd.setCursor(0,1);
  lcd.print("Press both: Exit");
  switchState1 = digitalRead(switchPin1);
  switchState2 = digitalRead(switchPin2);
  
  // while both buttons aren't being pressed
  while(loopy == false) {
    loopy = bothButtons();
  } // while()

  // releaseButtons prevents action due to button
  // state changing upon release
  releaseButtons();
  loopy = false;
  lcd.clear();
  contrast();
} // setup()

void loop() {
  // Calculator runs in loop
  // EntryScreen 1() ---> EntryScreen2() ---> Calculate()
  releaseButtons();
  entryScreen1();
  releaseButtons();
  lcd.clear();
  delay(500);
} // loop()

void releaseButtons() {
  // Makes sure the program only reads one action when button 
  // is pressed. Makes sure button can't be held down
  while(switchState1 == 1 || switchState2 == 1){
        switchState1 = digitalRead(switchPin1);
        switchState2 = digitalRead(switchPin2);
  } // while()
}

boolean bothButtons() {
  switchState1 = digitalRead(switchPin1);
  switchState2 = digitalRead(switchPin2);
 
  // Since the computer wont read the buttons as being 
  // pressed at the exact same moment, provide delay if at 
  // least one if them is pressed, so other button's state
  // can be read in time. If both are pressed, return true,
  // else return false
  if (switchState1 != prevSwitchState1) {
    delay(10);
    if (switchState2 != prevSwitchState2) {
      return loopy = true;
    } // if()
  } // if()
  if (switchState2 != prevSwitchState2) {
   delay(10);
   if (switchState1 != prevSwitchState1) {
      return loopy = true;
   } // if()
  } //if()
  return loopy = false;
} // bothButtons()

void contrast() {
  reprint();
  brightness = 0;
  
  // while both buttons have not been pressed
  while(loopy == false) {
      switchState1 = digitalRead(switchPin1);
      switchState2 = digitalRead(switchPin2);
     loopy = bothButtons();
     
      
      // IF Sw1 is pressed
      if((switchState1 != prevSwitchState1) && (brightness < 150)) {
        brightness += 5;
        reprint();
        delay(90);
      } // if()
      
      // IF Sw2 is pressed
      if((switchState2 != prevSwitchState2) && (brightness > 10)) {
        brightness -= 5;
        reprint();
        delay(90);
      } // if()
    analogWrite(9,brightness);
  } // while()
  lcd.clear();
} // contrast()

// Following method reprints contrast screen
void reprint() {
  lcd.clear();
  lcd.print("Sw1: -, Sw2: +");
  lcd.setCursor(0,1);
  lcd.print("Press both: Exit");
} // reprint()

// Following method reprints entry Screen with desired output
void printScreen(int array[], int eNum) {
  lcd.clear();
  lcd.print("Sw1:+/- Sw2:-->");
  lcd.setCursor(0,1);
  lcd.print(eNum);
  lcd.print(": ");
  // Following for loop prints one array value at a time
  for(int i = 0; i < 8; i++) {
    lcd.print(array[i]);
  } // for
} // printScreen()

// Following method allows the user to input his entry
// into calculator.
int entryScreen1() {
  int eNum = 1;
  int entry1[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  printScreen(entry1, eNum);

  // Following for loop runs iterates once everytime Sw2 is pressed
  for(int i = 0; i < 8; i++) {
    releaseButtons();

    // while loop does not end until Sw2 is pressed
    while(switchState2 == prevSwitchState2) {
     
      // IF Sw1 is pressed, value changes to opposite binary
      // value
      if(switchState1 != prevSwitchState1) {
        releaseButtons();
        if(entry1[i] == 0) {
          entry1[i] = 1;
        }
        else {
          entry1[i] = 0;
        }
        printScreen(entry1, eNum);
      } // if()
      // move cursor every time Sw2 pressed
      int cursor = i+3;
      lcd.setCursor(cursor, 1);
      delay(20);
      lcd.blink();
      switchState1 = digitalRead(switchPin1);
      switchState2 = digitalRead(switchPin2);
    } // while()
  } // for()
  releaseButtons();
  // once cursor shifts past eighth value, go to entryScreen2
  entryScreen2(entry1);
} // entryScreen1()

// Following method allows the user to input his entry
// into calculator.
void entryScreen2(int entry1[]) {
  int eNum = 2;
  int entry2[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  printScreen(entry2, eNum);
  
  // Following for loop runs iterates once everytime Sw2 is pressed
  for(int i = 0; i < 8; i++) {
    releaseButtons();
    
    // while loop does not end until Sw2 is pressed
    while(switchState2 == prevSwitchState2) {
      
      // IF Sw1 is pressed, value changes to opposite binary
      // value
      if(switchState1 != prevSwitchState1) {
        releaseButtons();
        if(entry2[i] == 0) {
          entry2[i] = 1;
        }
        else {
          entry2[i] = 0;
        }
        // printScreen to update entry
        printScreen(entry2, eNum);
      } // if()
      // move cursor every time Sw2 pressed
      int cursor = i+3;
      lcd.setCursor(cursor, 1);
      delay(20);
      lcd.blink();
      switchState1 = digitalRead(switchPin1);
      switchState2 = digitalRead(switchPin2);
    } // while()
  } // for()
  releaseButtons();
  lcd.clear();
  delay(100);
  // once cursor shifts past eighth value, go to entryScreen2
  calculate(entry1, entry2);
} // entryScreen2()


// In calculate method, answer is calculated and displayed
// on LCD screen
void calculate(int entry1[], int entry2[]) {
  String answer = "";
  int temp1[8] = {0,0,0,0,0,0,0,0};
  int temp2[8] = {0,0,0,0,0,0,0,0};

  // for loop reverses the array to make calculation
  // simpler
  
  for (int i = 0; i < 8; i++) {
    temp1[i] = entry1[7-i];
    temp2[i] = entry2[7-i];
  } // for()

  String temp = "";
  int carry = 0;
  
  // Calculation happens in the following for loop
  for(int i = 0; i < 8; i++) {
    if(temp1[i] + temp2[i] + carry == 0) {
      answer += "0";
      carry = 0;
    } // if()
    else if(temp1[i] + temp2[i] + carry == 1) {
      answer += "1";
      carry = 0; 
    } // if()
    else if(temp1[i] + temp2[i] + carry == 2) {
      answer += "0";
      carry = 1; 
    } // if()
    else if(temp1[i] + temp2[i] + carry == 3) {
      answer += "1";
      carry = 1; 
    } // if()

    // Following if: creates space between first and second
    // four bits
    if(i==3) {
      answer += " ";
    } // if()
    
    // Following if: deals with overflow. If carry equals
    // 1 at end of entry, add a one as the ninth number value
    if((i == 7) && (carry == 1)) {
      answer+=" 1";
    } // if()
  }

  // Following for loop reverses the string, since entries
  // were reversed at begining of calculation
  for(int i=answer.length()-1;i>=0;i--) {
    temp+=answer[i];
  } // for()
  
  answer=temp;

  lcd.clear();
  lcd.print("Sum: ");
  lcd.setCursor(0,1);
  lcd.noBlink();

  boolean oneYet = false;

  // Zero case
  if(answer == "0000 0000") {
    lcd.print("0");
  } // if
  else {
    // Following for loop eliminates leading zeroes. As long as
    // a zero has not appeared in answer, do not start printing yet
    for (int i = 0; i < answer.length(); i++) {
      if(answer[i] == '1') {
        oneYet = true;
      } // if()
      
      if(oneYet == false) {
      } // if()
      else {
        lcd.print(answer[i]);
      } // else()
    } // for()
  } // else()
  delay(100);
  loopy = false;
  
  // Hold screen until both buttons are pressed
  while(loopy == false) {
    loopy = bothButtons();
  } // while()
  lcd.clear();
} // calculate()
