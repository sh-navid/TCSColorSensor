/*
  // TCS230 color recognition

  Color Sensor      Arduino
  -----------      --------
  VCC               5V
  GND               GND
  s0                8
  s1                9
  s2                10
  s3                11
  OUT               12
  OE                GND
*/
const int s0 = 8;
const int s1 = 9;
const int s2 = 10;
const int s3 = 11;
const int out = 12;

int r = 0, g = 0, b = 0, c = 0;
int rMAX = 0, gMAX = 0, bMAX = 0, cMAX = 0;
int rMIN = 0, gMIN = 0, bMIN = 0, cMIN = 0;

int rw = 255, gw = 255, bw = 255, cw = 255;
int rb = 0, gb = 0, bb = 0, cb = 0;

void color() {
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);

  noInterrupts();
  r = pulseIn(out, HIGH);
  interrupts();

  digitalWrite(s3, HIGH);
  noInterrupts();
  b = pulseIn(out, HIGH);
  interrupts();

  digitalWrite(s2, HIGH);
  noInterrupts();
  g = pulseIn(out, HIGH);
  interrupts();

  digitalWrite(s3, LOW);
  noInterrupts();
  c = pulseIn(out, HIGH);
  interrupts();
}

void setup()
{
  Serial.begin(9600);
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT);
  digitalWrite(s0, HIGH);
  digitalWrite(s1, LOW);
}

void makeSample() {
  int sample = 6;

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

  String str = "";
  str += "~{'RAW':{";
  str += "'r':" + String(r);
  str += ",'g':" + String(g);
  str += ",'b':" + String(b);
  str += ",'c':" + String(c);
  str += "}";

  str += ",'MAX':{";
  str += "'r':" + String(rMAX);
  str += ",'g':" + String(gMAX);
  str += ",'b':" + String(bMAX);
  str += ",'c':" + String(cMAX);
  str += "}";

  str += ",'MIN':{";
  str += "'r':" + String(rMIN);
  str += ",'g':" + String(gMIN);
  str += ",'b':" + String(bMIN);
  str += ",'c':" + String(cMIN);
  str += "}";
  str += "}$";

//  str += ",'MAP':{";
//  str += "'r':" + String(mp(r, rb, rw));
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
