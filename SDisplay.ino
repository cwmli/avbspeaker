 #include <HardwareSerial.h>
// Library components
#include <LEDGrid.h>

// SPECIAL CLASS FOR HANDLING VISUAL EQUALIZER

#define FHT_N   128
#define LOG_OUT 1

#include <FHT.h>

#define STEPS  7

// adjust for circuit noise
static const int noise[] = { 125, 65, 115, 95, 65, 105, 100, 70, 100 };

class VEqualizer {
  
    public:
        VEqualizer() {
            displayString = NULL;
            stringScroll = false;
            stringSpd = 0;
        }
    
        // enable() setup settings for a0
        void enable() {           
            //TIMSK0 = 0; // turn off timer0 for lower jitter
            ADCSRA = 0xe5; // set the adc to free running mode
            ADMUX = 0x40; // use adc0
            DIDR0 = 0x01; // turn off the digital input for adc0
        }
    
        // disable() reset to default settings
        void disable() {
              ADCSRA |= (0 << ADEN);

              ADMUX ^= B10000000;
              DIDR0 ^= B00000001;
        }
    
        // setText(string, scroll, scrollSpd) sets the display text options
        void setText(char *string, bool scroll, int scrollSpd) {
              displayString = string;
              stringScroll = scroll;
              stringSpd = scrollSpd;
        }

        char *getString() {
            return displayString;
        }
        
        bool getIfScroll() {
            return stringScroll;
        }
        
        int getStringSpd() {
            return stringSpd;
        }
        
        // update() updates the data inside LED Grid based on input from a0
        void update() {
            static unsigned long lastColorUpdate;
            static uint8_t startIndex = 0;
            
            updateFHTData();
            updateFrequencyData(); 
          
            unsigned long time = millis();
          
            // color the display ONLY for activeLeds set by updateFrequencyData()
            if (time - lastColorUpdate > 10) {
              lastColorUpdate = time;

                for (uint8_t c = 0; c < COLUMNS; c++) {
                    //startIndex++;
                    for (uint8_t r = 0; r < ROWS; r++) {
                        if (getActiveLed(r, c) == 2) {
                            // placeholder colorIndex value CHANGE
                            leds[gridIndex(r, c)] = ColorFromPalette(currentPalette,c * 30, 255, currentBlending);
                        } else if (getActiveLed(r, c) != 1) {
                            leds[gridIndex(r, c)].fadeToBlackBy(25);
                        }
                    }
                }
            }
        }

    private:
        char *displayString;
        bool stringScroll;
        int stringSpd;

        // void updateFHTData() updates fht data
        void updateFHTData() {
            cli();
            for (int i = 0; i < FHT_N; i++) {
                // Wait until last data registered before continuing
                while(!(ADCSRA & 0x10));            
                ADCSRA = 0xf5; // restart adc
                byte m = ADCL; // fetch adc data
                byte j = ADCH;
                int k = (j << 8) | m; // form into an int
                k -= 0x0200; // form into a signed int
                k <<= 6; // form into a 16b signed int
                fht_input[i] = k; // put real data into bins
            }
            
            // process the data
            fht_window();
            fht_reorder();
            fht_run();
            fht_mag_log();
            sei();
            /*for (int i = 0; i < FHT_N / 2; i++) {
              Serial.print(i);
              Serial.print(" : ");
              Serial.print(fht_log_out[i]);
              Serial.print("\n");
            }*/
        }

        // updateFrequencyData() sets active leds w/ value of 2 based on the
        // fht data supplied from updateFHTData()
        void updateFrequencyData() {
                  
            uint8_t intensity[COLUMNS];
        
            int intensity_avg = 0;
            // capture the average intensities
            for (int i = 0; i < FHT_N / 2; i++) {
                if (fht_log_out[i] > 50) { //base
                  intensity_avg += fht_log_out[i];
                }

                // working with 9 steps (FHT_N / 2) / COLUMNS
                if (i > 0 && i % STEPS == 0) {
                    /*Serial.print((i / STEPS) - 1);
                    Serial.print(" : ");
                    Serial.print((double)intensity_avg / STEPS);
                    Serial.print(" n ");
                    Serial.print(noise[(i / STEPS) - 1]);
                    Serial.print(" = ");*/
                    
                    double in = ((double)intensity_avg / STEPS) - (double)noise[(i / STEPS) - 1];
                    // adjust for circuit noise
                    double ain = in > 0 ? (in / 35) * 6 : 0;

                    //Serial.print(ain);
                    //Serial.print("\n");

                    intensity[(i / STEPS) - 1] = ain;
                    intensity_avg = 0;
                }
            }
        
            resetActiveLeds();
        
            // populate the LED grid
            for (uint8_t c = 0; c < COLUMNS; c++) {
                for (uint8_t r = 0; r < intensity[c]; r++) { 
                    setActiveLed(r, c, 2);
                }
            }
        }
};
