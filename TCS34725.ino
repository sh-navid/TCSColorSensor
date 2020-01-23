/**
   I forked this lib from this address:
   https://github.com/adafruit/Adafruit_TCS34725
   so for more info please see the link

   I'm working on textile and fabric color, shade, distance & ... detection; before this
   sensor i was testing the TCS230 sensor but the performance is so so poor so i decided
   to change the sensor to TCS34725
*/

#include <Wire.h>
#include "Adafruit_TCS34725.h"

/*Connect SCL to analog 5
  Connect SDA to analog 4
  Connect VDD to 3.3V DC
  Connect GROUND to common ground */

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);


int r = 0, g = 0, b = 0, c = 0, colorTemp = 0, lux = 0;
int rMAX = 0, gMAX = 0, bMAX = 0, cMAX = 0;
int rMIN = 0, gMIN = 0, bMIN = 0, cMIN = 0;

int rw = 255, gw = 255, bw = 255, cw = 255;
int rb = 0, gb = 0, bb = 0, cb = 0;

void color() {
  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);
}

void setup()
{
  Serial.begin(9600);
//  if (tcs.begin()) {
//    Serial.println("Found sensor");
//  } else {
//    Serial.println("No TCS34725 found ... check your connections");
//    while (1);
//  }
}

void makeSample() {
  int sample = 4;

  for (int i = 0; i < sample; i++) {
    delay(i * sample);
    color();

    if (i == 0) {
      rMAX = r;
      gMAX = g;
      bMAX = b;
      cMAX = c;

      rMIN = r;
      gMIN = g;
      bMIN = b;
      cMIN = c;
    }

    if (r > rMAX)rMAX = r;
    if (g > gMAX)gMAX = g;
    if (b > bMAX)bMAX = b;
    if (c > cMAX)cMAX = c;

    if (r < rMIN)rMIN = r;
    if (g < gMIN)gMIN = g;
    if (b < bMIN)bMIN = b;
    if (c < cMIN)cMIN = c;
  }

  r = (rMAX + rMIN) / 2;
  g = (gMAX + gMIN) / 2;
  b = (bMAX + bMIN) / 2;
  c = (cMAX + cMIN) / 2;
}

void printSample() {
  //  Serial.flush();

  String str = "~{";
  str += " 'LUX':" + String(lux);
  str += ",'TEMP':" + String(colorTemp);

  str += ",'RAW':{";
  str += " 'r':" + String(r);
  str += ",'g':" + String(g);
  str += ",'b':" + String(b);
  str += ",'c':" + String(c);
  str += "}";

  str += ",'MAX':{";
  str += " 'r':" + String(rMAX);
  str += ",'g':" + String(gMAX);
  str += ",'b':" + String(bMAX);
  str += ",'c':" + String(cMAX);
  str += "}";

  str += ",'MIN':{";
  str += " 'r':" + String(rMIN);
  str += ",'g':" + String(gMIN);
  str += ",'b':" + String(bMIN);
  str += ",'c':" + String(cMIN);
  str += "}";
  str += "}$";

  //  str += ",'MAP':{";
  //  str += " 'r':" + String(mp(r, rb, rw));
  //  str += ",'g':" + String(mp(g, gb, gw));
  //  str += ",'b':" + String(mp(b, bb, bw));
  //  str += ",'c':" + String(mp(c, cb, cw));
  //  str += "}";
  //  str += "}$";

  Serial.print(str);
}

//int mp(int X, float A, float B) {
//  float C = 0.0;
//  float D = 255.0;
//  return ((X - A) / (B - A)) * ((D - C) + C);
//}

void loop()
{
  char c = ' ';
  while (c = Serial.read()) {
    if (c == 'n')break;
    else if (c == 'w') {
      makeSample();
      rw = r;
      gw = g;
      bw = b;
      Serial.print("\nWHITE\n");
      printSample();
    } else if (c == 'b') {
      makeSample();
      rb = r;
      gb = g;
      bb = b;
      Serial.print("\nBLACK\n");
      printSample();
    }
  }

  Serial.print("\nSAMPLE\n");
  makeSample();
  printSample();

  delay(200);
}
