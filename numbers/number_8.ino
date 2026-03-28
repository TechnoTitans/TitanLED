// Number 8 LED
// uses piece-by-piece code per light for  VU meter effect
// colors: (r, b, g)

#include <Adafruit_NeoPixel.h>
#include <driver/i2s.h>

// Hardware ----------------------------------------------------
#define LED_PIN     21
#define NUM_LEDS    67
#define LED_TYPE    NEO_GBR + NEO_KHZ800
#define BRIGHTNESS  80

#define I2S_SCK     14          // CLK
#define I2S_WS      12          // WS
#define I2S_DIN     13          // SD
#define I2S_PORT    I2S_NUM_0
#define SAMPLE_RATE 44100
#define SAMPLES     256

#define BUTTON_PIN  0

// Chase tuning ------------------------------------------------
#define CHASE_SPEED_MS  20
#define CHASE_TAIL      20

// VU tuning ----------------------------------------------------
#define THRESH_LOW   20000
#define THRESH_HIGH  200000

#define ATTACK  0.50f
#define DECAY   0.85f

// Globals ------------------------------------------------------
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, LED_TYPE);

uint8_t  currentMode     = 0;
bool     lastButtonState = HIGH;

uint16_t chasePos        = 0;
uint32_t lastChaseFrame  = 0;

float smoothedPeak = 0;

// I2S init -----------------------------------------------------
void initMic() {
  i2s_config_t i2s_config = {
    .mode                 = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate          = SAMPLE_RATE,
    .bits_per_sample      = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format       = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags     = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count        = 8,
    .dma_buf_len          = 64,
    .use_apll             = false,
    .tx_desc_auto_clear   = false,
    .fixed_mclk           = 0
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num   = I2S_SCK,
    .ws_io_num    = I2S_WS,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num  = I2S_DIN
  };

  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_PORT, &pin_config);
  i2s_zero_dma_buffer(I2S_PORT);
}

// Read peak amplitude ---------------------------------------------
int32_t readPeak() {
  int32_t samples[SAMPLES];
  size_t  bytesRead = 0;
  i2s_read(I2S_PORT, samples, sizeof(samples), &bytesRead, portMAX_DELAY);

  int32_t peak  = 0;
  int     count = bytesRead / sizeof(int32_t);
  for (int i = 0; i < count; i++) {
    int32_t s = abs(samples[i] >> 11);
    if (s > peak) peak = s;
  }
  return peak;
}

// Helpers ----------------------------------------------------------
void fillRange(int from, int to, uint32_t col) {
  for (int i = from; i <= to; i++) strip.setPixelColor(i, col);
}

// Effect 0: Battery Saving Mode-------------------------------------
void effectLowBatteryMode() {
  strip.clear();
  strip.show();
}

// Effect #1: BlueWhiteChase ---------------------------------------
void effectBlueWhiteChase() {
  if (millis() - lastChaseFrame < CHASE_SPEED_MS) return;
  lastChaseFrame = millis();

  strip.clear();

  // Blue tail
  for (int t = 0; t < CHASE_TAIL; t++) {
    int   pos   = ((int)chasePos - t + NUM_LEDS) % NUM_LEDS;
    float frac  = 1.0f - (float)t / CHASE_TAIL;
    uint8_t bright = (uint8_t)(frac * frac * 255);

    uint8_t b = (uint8_t)((uint16_t)255 * bright / 255);
    strip.setPixelColor(pos, strip.Color(0, b, 0));
  }

  // White tail — offset halfway around the strip
  int whitePos = (chasePos + NUM_LEDS / 2) % NUM_LEDS;
  for (int t = 0; t < CHASE_TAIL; t++) {
    int   pos  = ((int)whitePos - t + NUM_LEDS) % NUM_LEDS;
    float frac = 1.0f - (float)t / CHASE_TAIL;
    uint8_t bright = (uint8_t)(frac * frac * 255);

    uint8_t w = (uint8_t)((uint16_t)255 * bright / 255);
    strip.setPixelColor(pos, strip.Color(w, w, w));
  }

  strip.show();
  chasePos = (chasePos + 1) % NUM_LEDS;
}

// Effect #2: VU meter -------------------------------------------
void effectFreqVU() {
  int32_t peak = readPeak();

  if (peak > smoothedPeak)
    smoothedPeak = smoothedPeak + (peak - smoothedPeak) * ATTACK;
  else
    smoothedPeak = smoothedPeak * DECAY;

  if (smoothedPeak < THRESH_LOW) smoothedPeak = 0;  

  Serial.println((int32_t)smoothedPeak);  // sends data to Serial Plotter

  // Map volume to 0-15 active layers
  const float LOG_MIN = log((float)THRESH_LOW + 1.0f);
  const float LOG_MAX = log((float)THRESH_HIGH + 1.0f);
  float logVal = log((float)smoothedPeak + 1.0f);
  float normalized = (logVal - LOG_MIN) / (LOG_MAX - LOG_MIN);

    int activeLayers = constrain((int)(normalized * 15.0f), 0, 15);

      struct Layer { int leftFrom, leftTo, middleLeft, middle, middleRight, rightFrom, rightTo; };

      Layer layers[15] = {
        {  5,  5, 37, 38, 39, 39, 40},   // layer 1  - 5 4 39 38 37
        {  6,  6, 35, 35, 35, 35, 35},   // layer 2  - 6 35
        {  7,  7, 34, 49, 50, 51, 51},   // layer 3  - 7 51 50 49 34
        {  8,  8, 33, 33, 43, 43, 48},   // layer 4  - 8 43 48 33
        {  9,  9, 32, 32, 44, 44, 47},   // layer 5  - 9 44 47 32
        { 10, 10, 31, 31, 45, 45, 46},   // layer 6  - 10 45 46 31
        { 11, 11, 11, 11, 30, 30, 30},   // layer 7  - 11 30
        { 12, 12, 12, 12, 29, 29, 29},   // layer 8  - 12 29
        { 13, 13, 13, 13, 28, 28, 28},   // layer 9  - 13 28
        { 14, 14, 27, 27, 64, 65, 66},   // layer 10 - 14 66 65 64 27
        { 15, 15, 26, 26, 63, 63, 63},   // layer 11 - 15 63 26
        { 16, 16, 25, 25, 58, 58, 62},   // layer 12 - 16 58 62 25
        { 17, 17, 24, 24, 59, 59, 61},   // layer 13 - 17 59 61 24
        { 18, 18, 18, 18, 23, 23, 23},   // layer 14 - 18 23
        { 19, 19, 20, 21, 22, 22, 22},   // layer 15 - 19 20 21 22
      };

      strip.clear();

    for (int i = 0; i < activeLayers; i++) {
        // (r, b, g)
        uint32_t layerColors[15] = {
          strip.Color(0,   40,  0),    // layer 1  - deep blue
          strip.Color(0,   70,  0),    // layer 2
          strip.Color(0,   100, 0),    // layer 3
          strip.Color(0,   130, 0),    // layer 4
          strip.Color(0,   160, 0),    // layer 5
          strip.Color(0,   190, 0),    // layer 6
          strip.Color(0,   220, 0),    // layer 7
          strip.Color(0,   255, 0),    // layer 8  - pure blue
          strip.Color(60,  255, 60),    // layer 9  - starts adding red
          strip.Color(110, 255, 110),    // layer 10
          strip.Color(150, 255, 150),    // layer 11
          strip.Color(185, 255, 185),    // layer 12
          strip.Color(215, 255, 215),    // layer 13
          strip.Color(240, 255, 240),    // layer 14
          strip.Color(255, 255, 255),    // layer 15 - pure white
        };

        uint32_t col = layerColors[i];
        fillRange(layers[i].leftFrom,   layers[i].leftTo,    col);
        fillRange(layers[i].middleLeft, layers[i].middleRight, col);
        fillRange(layers[i].rightFrom,  layers[i].rightTo,   col);
      }

      strip.show();
  }


//

// Effect #3: RedWhiteChase ---------------------------------------
void effectRedWhiteChase() {
  if (millis() - lastChaseFrame < CHASE_SPEED_MS) return;
  lastChaseFrame = millis();

  strip.clear();

  // Red Tail
  for (int t = 0; t < CHASE_TAIL; t++) {
    int   pos   = ((int)chasePos - t + NUM_LEDS) % NUM_LEDS;
    float frac  = 1.0f - (float)t / CHASE_TAIL;
    uint8_t bright = (uint8_t)(frac * frac * 255);

    uint8_t b = (uint8_t)((uint16_t)255 * bright / 255);
    strip.setPixelColor(pos, strip.Color(b, 0, 0));
  }

  // White tail — offset halfway around the strip
  int whitePos = (chasePos + NUM_LEDS / 2) % NUM_LEDS;
  for (int t = 0; t < CHASE_TAIL; t++) {
    int   pos  = ((int)whitePos - t + NUM_LEDS) % NUM_LEDS;
    float frac = 1.0f - (float)t / CHASE_TAIL;
    uint8_t bright = (uint8_t)(frac * frac * 255);

    uint8_t w = (uint8_t)((uint16_t)255 * bright / 255);
    strip.setPixelColor(pos, strip.Color(w, w, w));
  }

  strip.show();
  chasePos = (chasePos + 1) % NUM_LEDS;
}

// Effect #4: SolidBlueColor -----------------------------------------
void effectSolidBlueColor() {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(0, 255, 0));
  }
  strip.show();
} 

// Effect #5: SolidRedColor -----------------------------------------
void effectSolidRedColor() {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(255, 0, 0));
  }
  strip.show();
}

// Setup -----------------------------------------------------------
void setup() {
  Serial.begin(115200);
  Serial.println("Sparkle Motion Stick – starting up");

  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.clear();
  strip.show();

  initMic();
  Serial.println("Mic ready");

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.println("Mode 0: effectLowBatteryMode  |  Press BOOT to switch");
}

// Loop ------------------------------------------------------------
void loop() {
  bool btnState = digitalRead(BUTTON_PIN);
  if (btnState == LOW && lastButtonState == HIGH) {
    delay(50);
    currentMode = (currentMode + 1) % 6;
    strip.clear();
    strip.show();
    Serial.printf("Mode → %s\n",
      currentMode == 0 ? "Battery Saving Mode" :
      currentMode == 1 ? "Blue-White Chase" :
      currentMode == 2 ? "Frequency VU Meter" : 
      currentMode == 3 ? "Red-White Chase" : 
      currentMode == 4 ? "SolidBlueColor" : "SolidRedColor");

  }
  lastButtonState = btnState;

  switch (currentMode) {
    case 0: effectLowBatteryMode(); break;
    case 1: effectBlueWhiteChase(); break;
    case 2: effectFreqVU();         break;
    case 3: effectRedWhiteChase(); break;
    case 4: effectSolidBlueColor(); break;
    case 5: effectSolidRedColor(); break;
  }
}
