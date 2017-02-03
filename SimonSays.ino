/*

   A simple Simon Says game that I created with some bits code taken from:
   http://linksprite.com/wiki/index.php5?title=16_X_2_LCD_Keypad_Shield_for_Arduino

   Isaac Sullivan

*/

#include <LiquidCrystal.h>   // include LCD library
/*--------------------------------------------------------------------------------------
  Defines
  --------------------------------------------------------------------------------------*/
// Pins in use
#define BUTTON_ADC_PIN           A0  // A0 is the button ADC input
#define LCD_BACKLIGHT_PIN         10  // D10 controls LCD backlight
// ADC readings expected for the 5 buttons on the ADC input
#define RIGHT_10BIT_ADC         815  // right
#define UP_10BIT_ADC            930  // up
#define DOWN_10BIT_ADC          903  // down
#define LEFT_10BIT_ADC          856  // left
#define SELECT_10BIT_ADC        615  // select
#define BUTTONHYSTERESIS        10  // hysteresis for valid button sensing window
//return values for ReadButtons()
#define BUTTON_NONE               0  // 
#define BUTTON_RIGHT              3  // 
#define BUTTON_UP                 1  // 
#define BUTTON_DOWN               2  // 
#define BUTTON_LEFT               4  // 
#define BUTTON_SELECT             5  // 
//some example macros with friendly labels for LCD backlight/pin control, tested and can be swapped into the example code as you like
#define LCD_BACKLIGHT_OFF()     digitalWrite( LCD_BACKLIGHT_PIN, LOW )
#define LCD_BACKLIGHT_ON()      digitalWrite( LCD_BACKLIGHT_PIN, HIGH )
#define LCD_BACKLIGHT(state)    { if( state ){digitalWrite( LCD_BACKLIGHT_PIN, HIGH );}else{digitalWrite( LCD_BACKLIGHT_PIN, LOW );} }
/*--------------------------------------------------------------------------------------
  Variables
  --------------------------------------------------------------------------------------*/
byte buttonJustPressed  = false;         //this will be true after a ReadButtons() call if triggered
byte buttonJustReleased = false;         //this will be true after a ReadButtons() call if triggered
byte buttonWas          = BUTTON_NONE;   //used by ReadButtons() for detection of button events

int arr[20]; //size of generated array
int user[20]; //size of user imput array

int valPos = 0; // variable to store the position in the user created array
int lvl = 0; // variable to store the current level
int i;

long randNumber;
byte button;
static int count = 0; //variable to store the player score

const int UP = 1;
const int DOWN = 2;
const int RIGHT = 3;
const int LEFT = 4;

const int SELU = 5;
const int SELD = 6;
const int SELR = 7;
const int SELL = 8;

LiquidCrystal lcd( 8, 9, 4, 5, 6, 7 );   //Pins for the freetronics 16x2 LCD shield. LCD: ( RS, E, LCD-D4, LCD-D5, LCD-D6, LCD-D7 )

void setup() {
  Serial.begin(9600);

  randomSeed(analogRead(1));

  //button adc input
  pinMode( BUTTON_ADC_PIN, INPUT );         //ensure A0 is an input
  digitalWrite( BUTTON_ADC_PIN, LOW );      //ensure pullup is off on A0
  //lcd backlight control
  digitalWrite( LCD_BACKLIGHT_PIN, HIGH );  //backlight control pin D3 is high (on)
  pinMode( LCD_BACKLIGHT_PIN, OUTPUT );     //D3 is an output
  //set up the LCD number of columns and rows:
  lcd.begin( 16, 2 );

  // welcoming message
  lcd.setCursor(2, 0);
  lcd.print("WELCOME TO:");
  lcd.setCursor(2, 1);
  lcd.print("SIMON SAYS!");
  delay(1500);
  lcd.clear();

  lcd.setCursor(2, 0);
  lcd.print("Press Select");
  lcd.setCursor(4, 1);
  lcd.print("to Start");
  delay(1500);
  lcd.clear();

  lcd.setCursor( 4, 1);
  lcd.print("SCORE:0");
  ArrayGen();
}

// function to generate a random array
int ArrayGen() {

  for ( i = 0; i < 20; i++)
  {
    randNumber = random(1, 4);
    arr[i] = randNumber;
  }
  return (arr[i]);
}

// function to display the array on the LCD
void PrintArray() {

  for ( i = 0 ; i < lvl + 1 ; i++)
  {
    delay(750); //delay inbetween selection flash
    if (arr[i] == 1)
    {
      lcd.setCursor(14, 0);
      lcd.write(SELU);
      delay(300);  //delay for duration of selection flash
      lcd.setCursor(14, 0);
      lcd.write(UP);
    }
    else if (arr[i] == 2)
    {
      lcd.setCursor(14, 1);
      lcd.write(SELD);
      delay(300);
      lcd.setCursor(14, 1);
      lcd.write(DOWN);
    }
    else if (arr[i] == 3)
    {
      lcd.setCursor(15, 1);
      lcd.write(SELR);
      delay(300);
      lcd.setCursor(15, 1);
      lcd.write(RIGHT);
    }
    else if (arr[i] == 4)
    {
      lcd.setCursor(13, 1);
      lcd.write(SELL);
      delay(300);
      lcd.setCursor(13, 1);
      lcd.write(LEFT);
    }
  }
  lvl++;
}

void loop() {

  //custom arrow sprites
  byte simonUp[8] = {
    0b00000,
    0b00100,
    0b01110,
    0b11111,
    0b01110,
    0b01110,
    0b01110,
    0b00000
  };

  byte simonDown[8] = {
    0b00000,
    0b01110,
    0b01110,
    0b01110,
    0b11111,
    0b01110,
    0b00100,
    0b00000
  };

  byte simonRight[8] = {
    0b00000,
    0b00100,
    0b11110,
    0b11111,
    0b11110,
    0b00100,
    0b00000,
    0b00000
  };

  byte simonLeft[8] = {
    0b00000,
    0b00100,
    0b01111,
    0b11111,
    0b01111,
    0b00100,
    0b00000,
    0b00000
  };

  byte selectUP[8] = {
    0b00000,
    0b00100,
    0b01010,
    0b10001,
    0b01010,
    0b01010,
    0b01110,
    0b00000
  };

  byte selectDown[8] = {
    0b00000,
    0b01110,
    0b01010,
    0b01010,
    0b10001,
    0b01010,
    0b00100,
    0b00000
  };

  byte selectRight[8] = {
    0b00000,
    0b00100,
    0b11010,
    0b10001,
    0b11010,
    0b00100,
    0b00000,
    0b00000
  };

  byte selectLeft[8] = {
    0b00000,
    0b00100,
    0b01011,
    0b10001,
    0b01011,
    0b00100,
    0b00000,
    0b00000
  };

  lcd.createChar(UP, simonUp);
  lcd.createChar(DOWN, simonDown);
  lcd.createChar(RIGHT, simonRight);
  lcd.createChar(LEFT, simonLeft);

  lcd.createChar(SELU, selectUP);
  lcd.createChar(SELD, selectDown);
  lcd.createChar(SELR, selectRight);
  lcd.createChar(SELL, selectLeft);

  lcd.setCursor(1, 0);
  lcd.write(UP);
  lcd.setCursor(1, 1);
  lcd.write(DOWN);
  lcd.setCursor(2, 1);
  lcd.write(RIGHT);
  lcd.setCursor(0, 1);
  lcd.write(LEFT);

  lcd.setCursor(14, 0);
  lcd.write(UP);
  lcd.setCursor(14, 1);
  lcd.write(DOWN);
  lcd.setCursor(15, 1);
  lcd.write(RIGHT);
  lcd.setCursor(13, 1);
  lcd.write(LEFT);

  //get the latest button pressed, also the buttonJustPressed, buttonJustReleased flags
  button = ReadButtons();
  //blank the demo text line if a new button is pressed or released, ready for a new label to be written
  if ( buttonJustPressed || buttonJustReleased )
  {
    lcd.setCursor(10, 1);
    lcd.print(count);
  }

  if (button == BUTTON_RIGHT)
  {
    //Selection Display Animation
    lcd.setCursor(2, 1);
    lcd.write(SELR);
    delay(300);
    lcd.setCursor(2, 1);
    lcd.write(RIGHT);
    StoreUserArray(3);
    Compare();
  }
  else if (button == BUTTON_LEFT)
  {
    //Selection Display Animation
    lcd.setCursor(0, 1);
    lcd.write(SELL);
    delay(300);
    lcd.setCursor(0, 1);
    lcd.write(LEFT);
    StoreUserArray(4);
    Compare();
  }
  else if (button == BUTTON_UP)
  {
    //Selection Display Animation
    lcd.setCursor(1, 0);
    lcd.write(SELU);
    delay(300);
    lcd.setCursor(1, 0);
    lcd.write(UP);
    StoreUserArray(1);
    Compare();
  }
  else if (button == BUTTON_DOWN)
  {
    //Selection Display Animation
    lcd.setCursor(1, 2);
    lcd.write(SELD);
    delay(300);
    lcd.setCursor(1, 2);
    lcd.write(DOWN);
    StoreUserArray(2);
    Compare();
  }
  else if (button == BUTTON_SELECT)
    PrintArray();

  //clear the buttonJustPressed or buttonJustReleased flags, they've already done their job now.
  if ( buttonJustPressed )
    buttonJustPressed = false;
  if ( buttonJustReleased )
    buttonJustReleased = false;
}

// function to compare the user created array to the randomly generated array
void Compare() {

  int k = memcmp(user, arr, valPos * 2);
  if ((lvl == valPos) && (k == 0))
  {
    count++;
    valPos = 0;
    PrintArray();
  }

  if (k != 0)
  {
    lcd.setCursor( 4, 0);
    lcd.print("YOU LOSE");
  }
}

// function to store the input from the user into an array
void StoreUserArray(int val) {
  user[valPos] = val;
  valPos++;
}

/*--------------------------------------------------------------------------------------
  ReadButtons()
  Detect the button pressed and return the value
  Uses global values buttonWas, buttonJustPressed, buttonJustReleased.
  --------------------------------------------------------------------------------------*/
byte ReadButtons() {

  unsigned int buttonVoltage;
  byte button = BUTTON_NONE;   // return no button pressed if the below checks don't write to btn

  //read the button ADC pin voltage
  buttonVoltage = analogRead( BUTTON_ADC_PIN );
  //sense if the voltage falls within valid voltage windows
  if ( buttonVoltage <= ( RIGHT_10BIT_ADC + BUTTONHYSTERESIS )
       && buttonVoltage >= ( RIGHT_10BIT_ADC - BUTTONHYSTERESIS ) )
  {
    button = BUTTON_RIGHT;
  }
  else if (   buttonVoltage >= ( UP_10BIT_ADC - BUTTONHYSTERESIS )
              && buttonVoltage <= ( UP_10BIT_ADC + BUTTONHYSTERESIS ) )
  {
    button = BUTTON_UP;
  }
  else if (   buttonVoltage >= ( DOWN_10BIT_ADC - BUTTONHYSTERESIS )
              && buttonVoltage <= ( DOWN_10BIT_ADC + BUTTONHYSTERESIS ) )
  {
    button = BUTTON_DOWN;
  }
  else if (   buttonVoltage >= ( LEFT_10BIT_ADC - BUTTONHYSTERESIS )
              && buttonVoltage <= ( LEFT_10BIT_ADC + BUTTONHYSTERESIS ) )
  {
    button = BUTTON_LEFT;
  }
  else if (   buttonVoltage >= ( SELECT_10BIT_ADC - BUTTONHYSTERESIS )
              && buttonVoltage <= ( SELECT_10BIT_ADC + BUTTONHYSTERESIS ) )
  {
    button = BUTTON_SELECT;
  }
  //handle button flags for just pressed and just released events
  if ( ( buttonWas == BUTTON_NONE ) && ( button != BUTTON_NONE ) )
  {
    //the button was just pressed, set buttonJustPressed, this can optionally be used to trigger a once-off action for a button press event
    //it's the duty of the receiver to clear these flags if it wants to detect a new button change event
    buttonJustPressed  = true;
    buttonJustReleased = false;
  }
  if ( ( buttonWas != BUTTON_NONE ) && ( button == BUTTON_NONE ) )
  {
    buttonJustPressed  = false;
    buttonJustReleased = true;
  }

  //save the latest button value, for change event detection next time round
  buttonWas = button;

  return ( button );
}
