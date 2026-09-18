# arduino-pomodoro-timer

An interactive, Pomodoro Timer built with Arduino Mega, a 16x2 LCD Display, an RGB LED, an Active Buzzer, and a push-button. This device guides you through customizable study sessions, short breaks, and long breaks with visual and audio feedback.

## Features

* Interactive Setup: Configure the number of study sessions, study hours/minutes, short break hours/minutes, and long break hours/minutes directly using a single push-button interface.
* Visual Status Indicators (RGB LED):
  * Green: Active Study Session
  * Light Blue: Short Break Time
  * Purple: Long Break Time
  * Red Flashing: Session/Break ending alert
* Audio Alerts: An active buzzer sounds at the end of each timer countdown (warning: it can be quite loud!).
* Real-time Countdown: Displays formatted HH:MM:SS countdowns on a 16x2 LCD screen.


## Pin Configuration / Wiring Map

* LCD Screen: RS = Pin 3, En = Pin 5, D4 = Pin 7, D5 = Pin 9, D6 = Pin 11, D7 = Pin 13
* RGB LED: Red Pin = 45, Green Pin = 46, Blue Pin = 47
* Pushbutton: Pin 22
* Active Buzzer: Pin 23


## How It Works

1. Welcome Screen: Upon boot, the LCD displays a welcome message for 3 seconds.
2. Configuration Phase:
   * Use short presses to increment values (number of sessions, hours, minutes).
   * Use a long press to lock in your choice and move to the next setting.
3. Timer Execution: The system loops through your study sessions, alternating automatically between short breaks (after odd sessions) and long breaks (after even sessions).
4. Completion: Once all sessions are finished, a congratulatory message is displayed, and the program halts until the Arduino is reset.
