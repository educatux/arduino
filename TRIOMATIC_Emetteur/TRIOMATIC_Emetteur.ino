#include <Adafruit_GFX.h>
#include <Adafruit_TFTLCD.h>
#include <TouchScreen.h>
#include <Wire.h>

// Pins for TFT
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

// Touchscreen pins
#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

#define TS_MINX 100
#define TS_MAXX 920
#define TS_MINY 70
#define TS_MAXY 900
#define MINPRESSURE 10
#define MAXPRESSURE 1000

// TFT and TouchScreen objects
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Define colors
#define BLACK   0x0000
#define WHITE   0xFFFF
#define YELLOW  0xFFE0
#define GREEN   0x07E0
#define BLUE    0x001F
#define RED     0xF800

// Text areas
struct TouchArea {
  int x, y, w, h;
  const char *label;
  uint16_t backgroundColor;
  uint16_t textColor;
  int delayTime;
};

TouchArea touchAreas[3] = {
  {0, 60, 240, 80, "Ordures", RED, WHITE, 30000},
  {0, 140, 240, 80, "Plastiques", YELLOW, BLACK, 15000},
  {0, 220, 240, 80, "Papier", BLUE, BLACK, 45000}
};

void setup(void) {
  Serial.begin(9600);
  Wire.begin(); // Initialize as master
  Serial.println("Initializing screen...");
  tft.reset();
  tft.begin(0x9341);
  tft.setRotation(0); // Adjust screen orientation to portrait
  tft.fillScreen(BLACK);
  Serial.println("Screen initialized.");

  drawTitle();
  drawInitialButtons();
}

void loop(void) {
  TSPoint p = ts.getPoint();
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  if (p.z > MINPRESSURE && p.z < MAXPRESSURE) {
    p.x = map(p.x, TS_MINX, TS_MAXX, tft.width(), 0);
    p.y = map(p.y, TS_MINY, TS_MAXY, tft.height(), 0);
    Serial.print("Touch detected at (");
    Serial.print(p.x);
    Serial.print(", ");
    Serial.print(p.y);
    Serial.println(")");

    for (int i = 0; i < 3; i++) {
      if (isTouchWithinArea(p, touchAreas[i])) {
        Serial.print("Button ");
        Serial.print(touchAreas[i].label);
        Serial.println(" pressed.");

        handleButtonPress(touchAreas[i]);
      }
    }
  }
}

void drawTitle() {
  int x = 0, y = 0, w = 240, h = 50;
  tft.fillRect(x, y, w, h, GREEN);
  tft.drawRect(x, y, w, h, RED);
  tft.setCursor(x + 60, y + 20);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("TRIOMATIC");
  Serial.println("Title TRIOMATIC drawn.");
}

void drawInitialButtons() {
  for (int i = 0; i < 3; i++) {
    drawButton(touchAreas[i]);
    Serial.print("Button ");
    Serial.print(touchAreas[i].label);
    Serial.println(" displayed.");
  }
}

void drawButton(TouchArea area) {
  tft.fillRect(area.x, area.y, area.w, area.h, area.backgroundColor);
  tft.drawRect(area.x, area.y, area.w, area.h, WHITE);
  tft.setCursor(area.x + 10, area.y + 30);
  tft.setTextColor(area.textColor);
  tft.setTextSize(2);
  tft.print(area.label);
}

bool isTouchWithinArea(TSPoint p, TouchArea area) {
  return (p.x >= area.x && p.x <= (area.x + area.w) &&
          p.y >= area.y && p.y <= (area.y + area.h));
}

void handleButtonPress(TouchArea area) {
  sendI2CMessage(area.label);
  showOpeningScreen(area);
  delay(area.delayTime);
  tft.reset();
  delay(100);
  tft.begin(0x9341);
  tft.setRotation(0); // Adjust screen orientation to portrait
  tft.fillScreen(BLACK);
  delay(10000);
  returnToMainScreen();
}

void showOpeningScreen(TouchArea area) {
  tft.fillScreen(area.backgroundColor);
  tft.setCursor(20, 160);
  tft.setTextColor(WHITE);
  tft.setTextSize(3);
  tft.print("Opening ");
  tft.print(area.label);
  Serial.print("Showing opening screen for ");
  Serial.println(area.label);
}

void returnToMainScreen() {
  tft.fillScreen(BLACK);
  drawTitle();
  drawInitialButtons();
  Serial.println("Returned to main screen.");
}

void sendI2CMessage(const char* message) {
  Wire.beginTransmission(8); // Slave address
  Wire.write(message); // Send the message
  int error = Wire.endTransmission(); // End the transmission
  if (error == 0) {
    Serial.print("Transmission of '");
    Serial.print(message);
    Serial.println("' successful.");
  } else {
    Serial.print("Transmission error: ");
    Serial.println(error);
  }
}
