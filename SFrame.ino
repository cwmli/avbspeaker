// Control variables

int LPRESS_TIME = 1000; // 1000ms (1s)
int LONG_PRESS  = 1;
int SHORT_PRESS = 2;

int plusSw      = 12;
int minSw       = 11;

CRGB leds[NUM_LEDS];
CRGB targetColor;
CRGBPalette16 currentPalette;
TBlendType currentBlending;

LEDGrid grid;
VEqualizer vEQ;
Clock clk = Clock(11,19,true);

bool pinState(int input) {
  static int lastState;
  static int pressTime;

  int value = digitalRead(input);
  
  if (value == HIGH) {
    if (lastState == LOW) {
      lastState = value;
      pressTime = millis();
    } else if (millis() - pressTime > LPRESS_TIME) {
      // delay 500ms (.5s)
      pressTime += 500;
      return LONG_PRESS;
    }
  } else {
    if (lastState == HIGH) {
      return SHORT_PRESS;
    }
    
    lastState = value;
    return 0;
  }
}

void setup() {
    Serial.begin(115200);
    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );

    pinMode(plusSw, INPUT);
    pinMode(minSw, INPUT);
    
    currentPalette = ForestColors_p;
    vEQ.enable();
    currentBlending = LINEARBLEND;
    grid.selectEffect(Effects::EQ);
}


void loop() { 
   vEQ.update();
   grid.doEffect();

   int plusSWState = pinState(plusSw);
   int minSWState = pinState(minSw);

   if (plusSWState == LONG_PRESS) {
     // increase brightness
     // reserve short press for LED disp switching 
   }
}
