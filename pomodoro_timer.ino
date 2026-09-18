#include <LiquidCrystal.h>

// LCD Pin Variables
int rs = 3;
int en = 5;
int d4 = 7;
int d5 = 9;
int d6 = 11;
int d7 = 13;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Button Pin Variables
int buttonPin = 22;

// LED Pin Variables
int redPin = 45;
int greenPin = 46;
int bluePin = 47;

// Buzzer Pin Variable
int buzzerPin = 23;

// Event Times
unsigned int lastButtonPress = 0;
unsigned int buttonPressTime = 0;

// Even Durations
unsigned long shortButtonPressDuration = 200;
unsigned long longButtonPressDuration = 1000;

// Pin States
int currentShortButtonState = LOW;
int pastShortButtonState = LOW;

int currentLongButtonState = LOW;
int pastLongButtonState = LOW;

// Function Prototypes
bool check_short_button_press();
bool check_long_button_press();

int get_time(char *, int);
void run_countdown(long);
void done_countdown();


void setup()
{
  lcd.begin(16,2);
  Serial.begin(9600);
  
  // initalizing pins
  pinMode(buttonPin, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  // Prints Welcome Message
  lcd.setCursor(0,0);
  lcd.print("Welcome to your");
  lcd.setCursor(1,1);
  lcd.print("Pomodoro Timer");
  delay(3000);
  lcd.clear();
  
}

void loop()
{
  // calls function to get number of study sessions, study times, short break times and long break times, second argument indicates if the question is asking for
  // 0 -- number of sessions, 1 -- number of hours, 2 -- number of minutes
  // turns the hours and minutes into the total seconds before assigning to variable
  int numStudySessions = get_time("Enter number of sessions: ", 0);
  long studyTimeSeconds = ((get_time("Enter study hours: ", 1) * 3600) + (get_time("Enter study minutes: ", 2) * 60));
  long shortBreakTimeSeconds = ((get_time("Enter short break hours: ", 1) * 3600) + (get_time("Enter short break minutes: ", 2) * 60));
  long longBreakTimeSeconds = ((get_time("Enter long break hours: ", 1) * 3600) + (get_time("Enter long break minutes: ", 2) * 60));

  // loops for the number of study sessions
  for (int i = 1; i <= numStudySessions; i++) {

    // Study session times
    // Turns led on to green 
    analogWrite(redPin, 0);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 0);

    // Displays study session stargin message on lcd
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Starting Study");
    lcd.setCursor(4,1);
    lcd.print("Sessions");
    delay(2000);

    // calls function to start timer
    run_countdown(studyTimeSeconds);

    // Displays study session ending meddage on lcd
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Study Session is");
    lcd.setCursor(6,1);
    lcd.print("Over");
    delay(2000);
    lcd.clear();

    // Turns led off
    analogWrite(redPin, 0);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
    
    // if the session session that just finish is odd, it does a short break, such that the first break is alwasy a short break
    // if the study session was the last study session then no break
    if (i % 2 == 1 && i < numStudySessions) {

      // turns led on to light blue 
      analogWrite(redPin, 0);
      analogWrite(greenPin, 150);
      analogWrite(bluePin, 150);

      // Displays short break starting message on lcd
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("Starting Short");
      lcd.setCursor(3,1);
      lcd.print("Break Time");
      delay(2000);

      // calls function to start timer
      run_countdown(shortBreakTimeSeconds);

      // Display short break ending message on lcd
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Short Break Time");
      lcd.setCursor(6,1);
      lcd.print("Over");
      delay(2000);
      lcd.clear();

      // Turns led off
      analogWrite(redPin, 0);
      analogWrite(greenPin, 0);
      analogWrite(bluePin, 0);

    } 
    // if the session session that just finish is even, it does a long break
    // if the study session was the last study session then no break
    else if (i % 2 == 0 && i < numStudySessions){

      // Turns led on to purple
      analogWrite(redPin, 150);
      analogWrite(greenPin, 0);
      analogWrite(bluePin, 150);

      // Display long break starting message
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("Starting Long");
      lcd.setCursor(3,1);
      lcd.print("Break Time");
      delay(2000);

      // calls function to start timer
      run_countdown(longBreakTimeSeconds);

      // Display long break ending message on lcd
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Long Break Time");
      lcd.setCursor(6,1);
      lcd.print("Over");
      delay(2000);
      lcd.clear();

      // turns led off
      analogWrite(redPin, 0);
      analogWrite(greenPin, 0);
      analogWrite(bluePin, 0);
    }
  }

  // Displays final ending message after all study sessions are complete
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("All Done!! Great");
  lcd.setCursor(1,1);
  lcd.print("Job Studying!!");
  while(true); // waits until arduino is reset to start again
}

// Function runs the countdown timer
void run_countdown(long totalSeconds){
  lcd.clear();
  
  while (totalSeconds >= 0) {

    // Converts total seconds back into back to hours, minutes, and second
    int h = totalSeconds / 3600;
    int m = (totalSeconds % 3600) / 60;
    int s = totalSeconds % 60;

    // Displays remaining time on lcd
    // Adds 0 to the front of singl edigit number so they look nice
    lcd.setCursor(4, 0);
    if (h < 10) lcd.print("0");
    lcd.print(h);
    lcd.print(":");
    if (m < 10) lcd.print("0");
    lcd.print(m);
    lcd.print(":");
    if (s < 10) lcd.print("0");
    lcd.print(s);

    // Pauses timer for 1 second at a time
    if (totalSeconds != 0){
      delay(1000);
    }

    // decremenets the total seconds
    totalSeconds--;
  }
  
  // Pauses slihtly when timer hits 0 seconds
  delay(2000);
  lcd.clear();

  // Calls function to to display the timer ending actions
  done_countdown();
}


// Function blinks the led red and sounds the buzzer when the countdown comes to and end
// NOTE: for reasons I have yet to understand the buzzer I used which is a 2 pin active buzzer from a  kit I bought, is really, really loud, like I'm using a 
// 10k ohm resistor and it is still like fire alarm loud so be warned.
void done_countdown(){

  // Flashes the led red and buzzer sound 3 times
  for(int i = 0; i < 3; i++){
    analogWrite(redPin, 255);
  	analogWrite(greenPin, 0);
  	analogWrite(bluePin, 0);
    digitalWrite(buzzerPin, HIGH);
    delay(1000);
    
    analogWrite(redPin, 0);
  	analogWrite(greenPin, 0);
  	analogWrite(bluePin, 0);
    digitalWrite(buzzerPin, LOW);
    delay(1000);
  }
}

// Function to get the number of session, number of hours, and number of minutes of each element
int get_time(char *message, int type){
  int time = 0, i;

  // creates a two element array initaized to 0 to hold the row and col of the cursor position
  int cursorPosition[2] = {0};
  
  lcd.clear();
  lcd.setCursor(0,0);

  // Displays the message in the lcd screen, moves from first row of screen to second row of screen once first row is filled
  for(i = 0; i < strlen(message) ; i++){
    if(i == 16){
      lcd.setCursor(0,1);
      cursorPosition[0] = 0;
      cursorPosition[1]++;
    }
    lcd.print(message[i]);
    cursorPosition[0]++; 
  }

  // if first row wasn't previous filled, moves cursor to second row for getting the input
  if(i <= 16){
    lcd.setCursor(0,1);
    cursorPosition[0] = 0;
    cursorPosition[1] = 1;
  }
  
  // Calls function that checks for a long button press, if false, chekcs for short button press
  // Short button press increments the numbers till the end of the range
  // Long button press moves on to the next category
  while(!check_long_button_press()){

    // Moves cursor back to inital position when starting this for if the user goes out of range and is sent back to the beginning
    lcd.setCursor(cursorPosition[0], cursorPosition[1]);
    
    // Displays current time on lcd
    lcd.print(time);

    // calls function that checks if user did a short pressed button if they did increment the time
  	if(check_short_button_press()){
      time++;
    }
    
    // Checks if time has be incremented out of accetable rane if so returns time back to 0, and clears previous time from lcd screen
    if((type == 0 && time > 6) || (type == 1 && time > 12) || (type == 2 && time > 59)){
      lcd.setCursor(cursorPosition[0], cursorPosition[1]);
      lcd.print("     ");
      time = 0; 
    }
    
  }
  

  // returns the inputed time
  return time;
}
  
// Function that checks if user did a short button press
bool check_short_button_press(){
  // finds current button state
  currentShortButtonState = digitalRead(buttonPin);
  
  // Edge Detection
  bool risingEdge = (currentShortButtonState == HIGH && pastShortButtonState == LOW);
  bool fallingEdge = (currentShortButtonState == LOW && pastShortButtonState == HIGH);
  
  // checks if button has been pressed
  if(risingEdge){
    lastButtonPress = millis();
  }
  
  // Checks if button has been released
  if(fallingEdge){
    buttonPressTime = millis() - lastButtonPress; 
    
    //If button was pressed for a sufficent amount of time, updates states and returns 1
    if(buttonPressTime >= shortButtonPressDuration && buttonPressTime < longButtonPressDuration){
      pastShortButtonState = currentShortButtonState;
      return 1;
    }
  }
  
  pastShortButtonState = currentShortButtonState;

  // If button was not pressed for long enough returns 0
  return 0;
}


// Function that ckeck is user did a long button press
bool check_long_button_press(){
  // finds current button
  currentLongButtonState = digitalRead(buttonPin);
  
  // Edge Detection
  bool risingEdge = (currentLongButtonState == HIGH && pastLongButtonState == LOW);
  bool fallingEdge = (currentLongButtonState == LOW && pastLongButtonState == HIGH);
  
  // checks if button has been pressed
  if(risingEdge){
    lastButtonPress = millis();
  }
  
  // Checks if button has been released
  if(fallingEdge){
    buttonPressTime = millis() - lastButtonPress; 
    
    //If button was pressed for a sufficent amount of time, updates states returns 1
    if(buttonPressTime >= longButtonPressDuration){
      pastLongButtonState = currentLongButtonState;
      return 1;
    }
  }
  
  pastLongButtonState = currentLongButtonState;

  //If button was not pressed for a long enough amount of time returns 0
  return 0;
}



