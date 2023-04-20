#include <Arduino.h>
#include <FastLED.h>


// =========================================
// ============= CONFIGURATION =============
// =========================================

// Number of leds
#define LED_COUNT 50

// LED Strip pin
#define LED_PIN 13

// Button pin
#define BUTTON_PIN 12

// First encoder pin
#define ENCODER_1_PIN A0

// Second encoder pin
#define ENCODER_2_PIN A1

// Third encoder pin
#define ENCODER_3_PIN A2

// Double click protection
#define DOUBLE_CLICK_DELAY 500


// =========================================
// ================= SETUP =================
// =========================================

CRGB leds[LED_COUNT];

void setup()
{
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    pinMode(ENCODER_1_PIN, INPUT);
    pinMode(ENCODER_2_PIN, INPUT);
    pinMode(ENCODER_3_PIN, INPUT);

    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_COUNT);

    for (int i = 0; i < LED_COUNT; i++) {
        leds[i] = CRGB::Black;
    }
    FastLED.show();
}


// ==========================================
// ================== MAIN ==================
// ==========================================

void loop()
{
    uint64_t now = millis();

    static int curEffect = 0;
    static uint64_t lastClickTime = 0;

    int sensorVal = digitalRead(BUTTON_PIN);
    if ((sensorVal == LOW) && ((now - lastClickTime) > DOUBLE_CLICK_DELAY)) {
        curEffect++;
        if (curEffect > 14) {
            curEffect = 0;
        }
        lastClickTime = now;
    }


    if (curEffect == 0) {
        yellowBrightness(1);
    } else if (curEffect == 1) {
        yellowBrightness(10);
    } else if (curEffect == 2) {
        yellowBrightness(50);
    } else if (curEffect == 3) {
        yellowBrightness(100);
    } else if (curEffect == 4) {
        yellowBrightness(255);
    } else if (curEffect == 5) {
        whiteBrightness(1);
    } else if (curEffect == 6) {
        whiteBrightness(10);
    } else if (curEffect == 7) {
        whiteBrightness(50);
    } else if (curEffect == 8) {
        whiteBrightness(100);
    } else if (curEffect == 9) {
        whiteBrightness(255);
    } else if (curEffect == 10) {
        movingColors(now);
    } else if (curEffect == 11) {
        fillByColor();
    } else if (curEffect == 12) {
        simpleBlink(now);
    } else if (curEffect == 13) {
        runningPoint(now);
    } else {
        showColors(now);
    }
}


// ===========================================
// ================= EFFECTS =================
// ===========================================

void whiteBrightness(int brightness)
{
    FastLED.setBrightness(brightness);
    for (int i = 0; i < LED_COUNT; i++) {
        leds[i].setRGB(255, 255, 255);
    }
    FastLED.show();
}

void yellowBrightness(int brightness)
{
    FastLED.setBrightness(brightness);
    for (int i = 0; i < LED_COUNT; i++) {
        leds[i].setRGB(255, 255, 0);
    }
    FastLED.show();
}

void movingColors(uint64_t now)
{
    static uint64_t last = 0;

    int colorR, colorG, colorB;
    static int j = 10;

    int delay = map(analogRead(ENCODER_1_PIN), 0, 1023, 10, 1000);

    if ((now - last) > delay) {
        for (int i = 0; i < LED_COUNT; i++) {
            if (((i + j) % 10) == 0) {
                colorR = 246;
                colorG = 119;
                colorB = 199;
            } else if (((i + j) % 10) == 1) {
                colorR = 229;
                colorG = 10;
                colorB = 149;
            } else if (((i + j) % 10) == 2) {
                colorR = 226;
                colorG = 8;
                colorB = 22;
            } else if (((i + j) % 10) == 3) {
                colorR = 243;
                colorG = 81;
                colorB = 22;
            } else if (((i + j) % 10) == 4) {
                colorR = 252;
                colorG = 229;
                colorB = 21;
            } else if (((i + j) % 10) == 5) {
                colorR = 99;
                colorG = 245;
                colorB = 37;
            } else if (((i + j) % 10) == 6) {
                colorR = 70;
                colorG = 255;
                colorB = 255;
            } else if (((i + j) % 10) == 7) {
                colorR = 24;
                colorG = 80;
                colorB = 236;
            } else if (((i + j) % 10) == 8) {
                colorR = 123;
                colorG = 24;
                colorB = 236;
            } else if (((i + j) % 10) == 9) {
                colorR = 182;
                colorG = 87;
                colorB = 238;
            }
            leds[i].setRGB(colorR, colorG, colorB);
        }
        j--;
        if (j == 0) {
            j = 10;
        }
        last = now;
    }
    FastLED.show();
}

void fillByColor()
{
    int colorR = map(analogRead(ENCODER_1_PIN), 0, 1023, 0, 255);
    int colorG = map(analogRead(ENCODER_2_PIN), 0, 1023, 0, 255);
    int colorB = map(analogRead(ENCODER_3_PIN), 0, 1023, 0, 255);

    for (int i = 0; i < LED_COUNT; i++) {
        leds[i].setRGB(colorR, colorG, colorB);
    }
    FastLED.show();
}

void simpleBlink(uint64_t now)
{
    static uint8_t state = 0;
    static uint64_t last = 0;

    int delay = map(analogRead(ENCODER_1_PIN), 0, 1023, 100, 1000);

    if ((now - last) > delay) {
        if (state == 0) {
        for (int i = 0; i < LED_COUNT; i++) {
            if (i % 7 == 0) {
             leds[i] = CRGB::Red;
            } else if (i % 7 == 1) {
                leds[i] = CRGB::Orange;
            } else if (i % 7 == 2) {
                leds[i] = CRGB::Yellow;
            } else if (i % 7 == 3) {
                leds[i] = CRGB::Green;
            } else if (i % 7 == 4) {
                leds[i] = CRGB::LightBlue;
            } else if (i % 7 == 5) {
                leds[i] = CRGB::Blue;
            } else {
                leds[i] = CRGB::Violet;
            }
        }
        FastLED.show();
        } else {
            for (int i = 0; i < LED_COUNT; i++) {
                leds[i] = CRGB::Black;
            }
            FastLED.show();
        }
        last = now;
        state = (state + 1) % 2;
    }
}

void showColors(uint64_t now)
{
    static uint8_t state = 0;
    static uint16_t curColor = 0;
    static uint64_t last = 0;

    if ((now - last) > 50) {
        curColor++;
        if (curColor >= 256) {
            curColor = 0;
            state++;
            if (state >= 4) {
                state = 0;
            }
        }

        for (int i = 0; i < LED_COUNT; i++) {
            if (state == 0) {
                leds[i].setRGB(curColor, curColor, curColor);
            } else if (state == 1) {
                leds[i].setRGB(curColor, 0, 0);
            } else if (state == 2) {
                leds[i].setRGB(0, curColor, 0);
            } else if (state == 3) {
                leds[i].setRGB(0, 0, curColor);
            }
        }
        FastLED.show();

        last = now;
    }
}

void runningPoint(uint64_t now)
{
  static uint64_t last = 0;
  static uint8_t direction = 0;
  static int16_t curLed = 0;

  static int colorR = rand() % 255;
  static int colorG = rand() % 255;
  static int colorB = rand() % 255;

  if ((now - last) > 50) {
    leds[curLed] = CRGB::Black;

    if (direction == 0) {
      curLed++;
      if (curLed >= LED_COUNT) {
        curLed -= 2;
        direction = 1;
      }
    } else {
      curLed--;
      if (curLed <= -1) {
        curLed += 2;
        direction = 0;
      }
    }

    leds[curLed].setRGB(colorR, colorG, colorB);
    colorR = (colorR + 1) % 255;
    colorG = (colorG + 1) % 255;
    colorB = (colorB + 1) % 255;

    FastLED.show();

    last = now;
  }
}
