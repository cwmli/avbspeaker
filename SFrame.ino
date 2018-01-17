// Control variables

int XLPRESS_TIME = 5000; // 5s
int LPRESS_TIME = 1000; // 1000ms (1s)
int XLONG_PRESS = 3;
int LONG_PRESS  = 1;
int SHORT_PRESS = 2;

int btnLight    = 3;
int btnSig      = 7;

uint8_t eMap = 2;
uint8_t brightness = 16;
// LED pin is 10

CRGB leds[NUM_LEDS];
CRGB targetColor;
CRGBPalette16 currentPalette;
TBlendType currentBlending;

LEDGrid grid;
VEqualizer vEQ;
Clock clk = Clock(11,19,true);

int pinState(int input) {
  static int lastState;
  static int lastTime = 0;
  static int pressTime;

  int value = digitalRead(input);
  //Serial.print(value);
  //Serial.print("\n");
  if (value == HIGH) {
    lastState = value;
    lastTime = millis();
    if (pressTime == 0) {
      pressTime = millis();
    }
  } else if (lastTime > 0 && millis() - lastTime > 50) {
      int curtime = millis();
      if (curtime - pressTime > XLPRESS_TIME) {
        lastState = LOW;
        lastTime = 0;
        pressTime = 0;
        Serial.print("XLP\n");
        return XLONG_PRESS;         
      } else if (curtime - pressTime > LPRESS_TIME) {
        lastState = LOW;
        lastTime = 0;
        pressTime = 0;
        Serial.print("LP\n");
        return LONG_PRESS;
      } else {
        lastState = LOW;
        lastTime = 0;
        pressTime = 0;
        Serial.print("P\n");   
        return SHORT_PRESS;
      } 
  } else {
    return 0;
  }
}

void setup() {
    Serial.begin(115200);
    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );

    //pinMode(btnLight, OUTPUT);
    pinMode(btnSig, INPUT);
    
    currentPalette = ForestColors_p;
    vEQ.enable();
    currentBlending = LINEARBLEND;
    grid.selectEffect(EffectsMap[eMap]);
}


void loop() { 
   if (grid.getEffect() == Effects::EQ) {
      vEQ.update();
   }
   grid.doEffect();
   
   int btnState = pinState(btnSig);
   if (btnState == LONG_PRESS) {
      if (eMap > 1) {
        eMap = 0;
      } else {
        eMap++;
      }
      
      grid.selectEffect(EffectsMap[eMap]);
   } else if (btnState == SHORT_PRESS) {
      if (brightness == 64) {
        brightness = 4;
      } else {
        brightness *= 2;
      } 
      FastLED.setBrightness(brightness);
   }
}
