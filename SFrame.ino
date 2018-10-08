// Control variables

int XLPRESS_TIME = 10000; // 10s
int LPRESS_TIME = 2500; // 2500ms (2.5s)
int SPRESS_TIME = 250;
int XLONG_PRESS = 3;
int LONG_PRESS  = 1;
int SHORT_PRESS = 2;

int btnLight    = 3;
int btnSig      = 7;

uint8_t eMap = 2;
uint8_t brightness = 0;
// LED pin is 10

CRGB leds[NUM_LEDS];
CRGB targetColor;
CRGBPalette16 currentPalette;
TBlendType currentBlending;

LEDGrid grid;
VEqualizer vEQ;
Clock clk = Clock(11,19,true);

int lastState = LOW;
unsigned long lastTime = 0;
unsigned long pressTime = 0;

unsigned long dbTime = 0;
unsigned long dbDelay =  25;
  
int pinState(int input) {

  int value = digitalRead(input);
   
  if (value != lastState) {
    dbTime = millis();
  }

  if ((millis() - dbTime) > dbDelay) {
   if (value == HIGH) {
      lastTime = millis();
      if (pressTime == 0) {
        pressTime = millis();
      }
    } else if (lastTime > 0) {
        unsigned long curtime = millis();
        if (curtime - pressTime > LPRESS_TIME) {
          lastTime = 0;
          pressTime = 0;
//          Serial.print(millis());
//          Serial.print("LP\n");
          return LONG_PRESS;
        } else if (curtime - pressTime > SPRESS_TIME) {
          lastTime = 0;
          pressTime = 0;
//          Serial.print(millis());
//          Serial.print("P\n");   
          return SHORT_PRESS;
        } 
    } else {
      return 0;
    }
  }
  
  lastState = value;
}

void setup() {
    Serial.begin(115200);
    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness( 0 );

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
      if (eMap > 3) {
        eMap = 0;
      } else {
        eMap++;
      }
      
      grid.selectEffect(EffectsMap[eMap]);
   } else if (btnState == SHORT_PRESS) {
      brightness += 12;
      if (brightness > 60) {
        brightness = 0;
      }
      FastLED.setBrightness(brightness);
   }
}
