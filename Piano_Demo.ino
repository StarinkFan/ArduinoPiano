#include "pitches.h"
#include "U8glib.h"
#include <Adafruit_NeoPixel.h>
#include "LinkedList.h"
#define PIN_PIANO 11
#define BRIGHTNESS 6
#define NUM_LEDS 8
#define PIN_PIXELS 12
#define NUMPIXELS 55
#define DELAYVAL 10

#define WIDTH 7
#define HEIGHT 8



Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN_PIANO, NEO_GRBW + NEO_KHZ800);
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);

Adafruit_NeoPixel pixels(NUMPIXELS, PIN_PIXELS, NEO_GRB + NEO_KHZ800);

//const int lineColors[HEIGHT][3] = {
//  {46, 68, 243},
//  {71, 148, 255},
//  {91, 255, 253},
//  {210, 173, 255},
//  {245, 45, 255},
//  {255, 61, 97},
//  {255, 179, 90},
//  {255, 235, 74}
//};

//const int lineColors[HEIGHT][3] = {
//  {11, 17, 61},
//  {18, 37, 64},
//  {23, 64, 63},
//  {52, 43, 64},
//  {61, 11, 64},
//  {64, 15, 24},
//  {64, 45, 22},
//  {64, 59, 18}
//};

const int lineColors[HEIGHT][3] = {
  {64, 59, 18},
  {64, 45, 22},
  {64, 15, 24},
  {61, 11, 64},
  {52, 43, 64},
  {23, 64, 63},
  {18, 37, 64},
  {11, 17, 61}
};

LinkedList<int> records = LinkedList<int>();
int lastIndex = -1;

void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  pixels.begin();
  pixels.setBrightness(36);
//  for ( int i = 0; i < HEIGHT; i++ )
//  {
//    for (int j = 0; j < WIDTH; j++) {
//      pixels.setPixelColor((i + 1) * (j + 1) - 1, pixels.Color(lineColors[i][0], lineColors[i][1], lineColors[i][2]));
//    }
//  }

  for(int i = 0; i < 8; i++){
    records.add(-1);
  }

  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show();
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  u8g.setFont(u8g_font_osb18);
  u8g.setFontRefHeightText();
  u8g.setFontPosTop();
  u8g.firstPage();
  do {
    u8g.setPrintPos(0, 0);
    u8g.print("Piano");
  } while ( u8g.nextPage() );
}

void loop() {
  const int pianoColors[7][3] = {
    {255, 0, 0},
    {255, 60, 0},
    {255, 200, 0},
    {0, 255, 0},
    {0, 255, 255},
    {0, 0, 255},
    {255, 0, 255}
  };

  const String pitches[] = {
    "C", "D", "E", "F", "G" , "A", "B"
  };

  const int pitchValues[] = {
    NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5
  };



  int index = -1;
  for (int i = 0; i < 7; i++) {
    if (digitalRead(8 - i) == 1) {
      index = i;
    }
  }

  if(index >= 0){
    // strip.setPixelColor(strip.Color(pianoColors[index][0], pianoColors[index][1], pianoColors[index][2], 0));
    colorWipe(strip.Color(pianoColors[index][0], pianoColors[index][1], pianoColors[index][2], 0), 0);
    u8g.firstPage();
    do {
      u8g.setPrintPos(0, 45);
      u8g.print(pitches[index]);
    } while ( u8g.nextPage() );
    do {
      tone(10, pitchValues[index], 50);
    }
    while (digitalRead(8 - index) == 1);

    update(index);
  }
  
}

void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void update(int index) {
  pixels.clear();
  records.add(index);
  if (records.size() > 8) {
    records.shift();
  }
  for (int i = 0; i < records.size(); i++) {
    int value = records.get(i);
    if(value >= 0){
      pixels.setPixelColor(value * HEIGHT + (WIDTH - i), pixels.Color(lineColors[7 - i][0], lineColors[7 - i][1], lineColors[7 - i][2]));
    }
  }
  pixels.show();

}
