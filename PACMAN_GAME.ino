#include <LiquidCrystal.h>
#include <EEPROM.h>

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0


int tempo = 105;

int buzzer = 13;

int numOfLoops = 0;

int noteDuration1 = 1000/8;

int pauseBetweenNotes = noteDuration1 * 0.2;

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth
int melody[] = {

  NOTE_B4, 16, NOTE_B5, 16, NOTE_FS5, 16, NOTE_DS5, 16, //1
  NOTE_B5, 32, NOTE_FS5, -16, NOTE_DS5, 8, NOTE_C5, 16,
  NOTE_C6, 16, NOTE_G6, 16, NOTE_E6, 16, NOTE_C6, 32, NOTE_G6, -16, NOTE_E6, 8,

  NOTE_B4, 16,  NOTE_B5, 16,  NOTE_FS5, 16,   NOTE_DS5, 16,  NOTE_B5, 32,  //2
  NOTE_FS5, -16, NOTE_DS5, 8,  NOTE_DS5, 32, NOTE_E5, 32,  NOTE_F5, 32,
  NOTE_F5, 32,  NOTE_FS5, 32,  NOTE_G5, 32,  NOTE_G5, 32, NOTE_GS5, 32,  NOTE_A5, 16, NOTE_B5, 8
};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
static const int SPRITE_PACMAN_OPEN = 0;
static const int SPRITE_PACMAN_CLOSED = 1;
static const int SPRITE_GHOST = 2;
static const int SPRITE_HEART = 3;
static const int SPRITE_SMILEY = 4;
static const int TYPE_NONE = 0;
static const int TYPE_HEART = 1;
static const int TYPE_GHOST = 2;
static const int MAX_SPRITES = 10;
static const int STATE_INTRO = 0;
static const int STATE_PLAY = 1;
static const int STATE_GAMEOVER = 2;
static const int HIGHSCORE_ADDRESS = 0;
static const int TOP = 0;
static const int BOTTOM = 1;
struct sprite {
  int x;
  int y;
  int type;
};
struct sprite sprites[MAX_SPRITES];
long timeToMove, timeToAnimate, timeToDebounce;
int state, score, highScore, gameSpeed, pacmanX, pacmanY, ghostOdds;
boolean mouthState, smile;
static byte spriteBitmaps[8][8] = {
  {0x7, 0xF, 0x1E, 0x1C, 0x1C, 0x1E, 0xF, 0x7},
  {0x0, 0xF, 0x1F, 0x1F, 0x1E, 0x1F, 0xF, 0x0},
  {0x19, 0x1F, 0x15, 0x1F, 0x11, 0x1F, 0x1D, 0xC},
  {0x0, 0x0, 0x0, 0xA, 0x15, 0x11, 0xA, 0x4},
  {0x0, 0xA, 0x0, 0x0, 0x11, 0xE, 0x0, 0x0},
  {0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x0, 0x0, 0x0},
  {0x0, 0x0, 0x0, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F},
  {0x1F, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}
};


void setup() {
  if (checkButton()) {
    EEPROM.write(HIGHSCORE_ADDRESS, 0);
  }
  for (int i = 0; i < (sizeof(spriteBitmaps) / 8); i++) {
    lcd.createChar(i, spriteBitmaps[i]);
  }
  delay(500);
  lcd.begin(16, 2);
  randomSeed(analogRead(1));

  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, melody[thisNote], noteDuration * 0.9);

    lcd.clear(); lcd.setCursor(3, 0); lcd.print("WELCOME TO"); lcd.setCursor(1, 1); lcd.print(" PACMAN GAME! ");

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(buzzer);
  }
  waitButton();
  
  state = STATE_INTRO;
  initVars();
}
void loop() {
  switch (state) {
    case STATE_INTRO: intro(); break;
    case STATE_PLAY: play(); break;
    case STATE_GAMEOVER: gameover(); break;
  }
}
void initVars() {
  for (int i = 0; i < MAX_SPRITES; i++) {
    sprites[i] = {0, 0, TYPE_NONE};
  }
  highScore = EEPROM.read(HIGHSCORE_ADDRESS);
  timeToMove = 0;
  timeToAnimate = 0;
  timeToDebounce = 0;
  score = 0;
  gameSpeed = 600;
  ghostOdds = 6;
  pacmanX = 1;
  pacmanY = 0;
  mouthState = false;
  smile = false;
}
void intro() {
  lcd.clear(); lcd.setCursor(0, 0); lcd.print(" Some rules to"); lcd.setCursor(1, 1); lcd.print("   follow :");
  waitButton();
  lcd.clear(); lcd.setCursor(0, 0); lcd.print("Press the button"); lcd.setCursor(1, 1); lcd.print("to move pacman");
  waitButton();
  lcd.clear(); lcd.setCursor(6, 0); lcd.print("");
  lcd.clear(); lcd.setCursor(3, 0); lcd.print("COLLECT: "); lcd.write(byte(SPRITE_HEART)); lcd.setCursor(4, 1); lcd.print("AVOID: "); lcd.write(byte(SPRITE_GHOST));
  waitButton(); lcd.clear(); lcd.setCursor(1, 0); lcd.print("ARE YOU READY?"); lcd.setCursor(0, 1); lcd.print("   Good luck!");
  waitButton();
  animation(1);
  state = STATE_PLAY;
}
void play() {
  drawScreen();
  long now = millis();
  if (checkButton()) {
    hidePacman();
    pacmanY = !pacmanY;
  }
  if (now > timeToMove) {
    moveLeft();
    if (!random(0, ghostOdds)) {
      spawn(TYPE_GHOST);
    }
    if (!random(0, 3)) {
      spawn(TYPE_HEART);
    }
    timeToMove = now + gameSpeed;
  }
  int c = collision();
  if (c == TYPE_HEART) {
    eatHeart();
    increaseScore();
  } else if (c == TYPE_GHOST) {
    state = STATE_GAMEOVER;
  }
}
void gameover() {
  animation(0);
  lcd.setCursor(3, 0); lcd.print("GAME OVER");
  for (numOfLoops = 0; numOfLoops < 4; numOfLoops++) {
    
    //Play low to high
    for (int i=25; i< 30; i++) {
      tone(buzzer, 20*i, noteDuration1);
      delay(pauseBetweenNotes);
    }
  
    //Play high to low
    for (int i=30; i>=25; i--) {
      tone(buzzer, 20*i, noteDuration1);
      delay(pauseBetweenNotes);}
    }
  
  delay (500);
  tone(buzzer, 500, 200) ;
  delay (200);
  tone(buzzer, 1200, 200) ;
  delay (200);
  tone(buzzer, 300, 200) ;
  delay(200);
  tone(buzzer, 1000, 200) ;
  delay (200);
  tone(buzzer, 400, 200);
  delay (200);
  tone(buzzer, 1100, 200);
  delay(200);
  
  lcd.setCursor(0, 1); lcd.print("How did you do?");
  waitButton();
  lcd.clear(); lcd.setCursor(2, 0); lcd.print("YOUR SCORE:"); lcd.setCursor(7, 1); lcd.print(score);
  waitButton();
  if (score > highScore) {
    lcd.clear(); lcd.setCursor(1, 0); lcd.print("NEW HIGHSCORE!");
    EEPROM.write(HIGHSCORE_ADDRESS, score);
    highScore = score;
    waitButton();
  }
  lcd.clear(); lcd.setCursor(3, 0); lcd.print("TRY AGAIN"); lcd.setCursor(2, 1); lcd.print("Highscore: "); lcd.print(highScore);
  waitButton();
  initVars();
  state = STATE_PLAY;
  lcd.clear();
}
void drawScreen() {
  for (int i = 0; i < MAX_SPRITES; i++) {
    drawSprite(i);
  }
  drawPacman();
}
void eatHeart() {
  for (int i = 0; i < MAX_SPRITES; i++) {
    if (sprites[i].x == pacmanX && sprites[i].y == pacmanY && sprites[i].type == TYPE_HEART) {
      smile = true;
      tone(buzzer, NOTE_B5, 200);
      deleteSprite(i);
      return;
    }
  }
}
void increaseScore() {
  score++;
  if (!(score % 10)) {
    gameSpeed = gameSpeed - 30;
    if (ghostOdds > 1) {
      ghostOdds--;
    }
  }
}
void spawn(int type) {
  int x = 15;
  int y = random(0, 2);
  for (int i = 0; i < MAX_SPRITES; i++) {
    int spriteType = sprites[i].type;
    if (spriteType == TYPE_NONE) {
      if (((type == TYPE_GHOST) && okayToSpawnGhost(y)) || ((type == TYPE_HEART) && okayToSpawnHeart(y))) {
        createSprite(i, x, y, type);
      }
      return;
    }
  }
}
int at(int x, int y) {
  for (int i = 0; i < MAX_SPRITES; i++) {
    if (sprites[i].x == x && sprites[i].y == y && sprites[i].type != TYPE_NONE) {
      return sprites[i].type;
    }
  }
  return TYPE_NONE;
}
int okayToSpawnGhost(int pos) {
  if (at(15, pos) != TYPE_NONE) {
    return 0;
  } else if (at(15, !pos) == TYPE_GHOST) {
    return 0;
  } else if ((pos == TOP) && (at(14, BOTTOM) == TYPE_GHOST)) {
    return 0;
  } else if ((pos == BOTTOM) && (at(14, TOP) == TYPE_GHOST)) {
    return 0;
  }
  return 1;
}
int okayToSpawnHeart(int pos) {
  if (at(15, pos) != TYPE_NONE) {
    return 0;
  }
  return 1;
}
void moveLeft() {
  for (int i = 0; i < MAX_SPRITES; i++) {
    if (sprites[i].type != TYPE_NONE) {
      int x = sprites[i].x - 1;
      int y = sprites[i].y;
      moveSprite(i, x, y);
    }
  }
}
void createSprite(int s, int x, int y, int type) {
  sprites[s].x = x;
  sprites[s].y = y;
  sprites[s].type = type;
}
void drawSprite(int s) {
  int spriteType = sprites[s].type;
  if (spriteType == TYPE_NONE) {
    return;
  }
  int x = sprites[s].x;
  int y = sprites[s].y;
  if (!((x == pacmanX) && (y == pacmanY))) {
    lcd.setCursor(x, y);
    switch (spriteType) {
      case TYPE_GHOST: lcd.write(byte(SPRITE_GHOST)); break;
      case TYPE_HEART: lcd.write(byte(SPRITE_HEART)); break;
      default: lcd.write(byte(32)); break;
    }
  }
}
void hideSprite(int s) {
  lcd.setCursor(sprites[s].x, sprites[s].y);
  lcd.write(byte(32));
}
void drawPacman() {
  if ((millis() > timeToAnimate) || (smile)) {
    int wait = 350;
    lcd.setCursor(pacmanX, pacmanY);
    if (smile) {
      lcd.write(byte(SPRITE_SMILEY));
      wait = 600;
      smile = false;
    } else if (mouthState) {
      lcd.write(byte(SPRITE_PACMAN_OPEN));
    } else {
      lcd.write(byte(SPRITE_PACMAN_CLOSED));
    }
    mouthState = !mouthState;
    timeToAnimate = millis() + wait;
  }
}
void hidePacman() {
  lcd.setCursor(pacmanX, pacmanY);
  lcd.write(byte(32));
}
void deleteSprite(int s) {
  hideSprite(s);
  sprites[s].x = 0;
  sprites[s].y = 0;
  sprites[s].type = TYPE_NONE;
}
void moveSprite(int s, int x, int y) {
  if ((x < 0) || (x > 15)) {
    deleteSprite(s);
  } else {
    hideSprite(s);
    sprites[s].x = x;
    sprites[s].y = y;
  }
}
int collision() {
  return at(pacmanX, pacmanY);
}
void animation(int direction) {
  byte animationOpen[6] = {255, 255, 5, 6, 7, 95};
  byte animationClose[6] = {7, 95, 5, 6, 255, 255};
  byte animationChars[6];
  if (direction) {
    memcpy(animationChars, animationOpen, sizeof animationChars);
  } else {
    memcpy(animationChars, animationClose, sizeof animationChars);
  }
  lcd.clear();
  for (int frame = 0; frame < 3; frame++) {
    lcd.setCursor(0, 0);
    for (int i = 0; i < 16; i++) lcd.write(animationChars[frame * 2]);
    lcd.setCursor(0, 1);
    for (int i = 0; i < 16; i++) lcd.write(animationChars[(frame * 2) + 1]);
    delay(300);
    lcd.clear();
  }
}
int checkButton() {
  long now = millis();
  if (now > timeToDebounce) {
    int b = analogRead(A0);
    if (b < 850) {
      timeToDebounce = now + 300;
      return 1;
    }
  }
  return 0;
}
void waitButton() {
  while (!checkButton()) delay(50);
}
