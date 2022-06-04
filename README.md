# PacmanGame
Overall, the project implements a version of the Pacman game using an Arduino UNO ATmega 328P, a Shield specifically for Arduino with LCD and Keyboard (LCD 1602 with Keypad) and moreover, a Buzzer connected to a Breadboard using wires.
Technically, the whole idea behind this Arduino game consists of three main states STATE_INTRO, STATE_PLAY and STATE_GAMEOVER, represented by three essential functions accordingly, which will be described below.
The intro() function:
  - displays the intro text on the screen and waits for the player's response (pushing of the button). It also calls the animation function which, according to the set direction is used for either the beginning or the end of the game to write the specific vectors on the LCD screen. In the end, the state is set to STATE_PLAY and the play function from the loop is called.
  -  
