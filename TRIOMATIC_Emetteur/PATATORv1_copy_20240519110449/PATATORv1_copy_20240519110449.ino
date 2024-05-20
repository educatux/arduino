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
#define RED     0xF800
#define BLUE    0x001F
#define YELLOW  0xFFE0

// Text areas
struct TouchArea {
  int x, y, w, h;
  const char *label;
  uint16_t backgroundColor;
  uint16_t textColor;
};

TouchArea touchAreas[3] = {
  {10, 100, 220, 50, "Ordures", BLACK, WHITE},
  {10, 160, 220, 50, "Plastiques", YELLOW, BLACK},
  {10, 220, 220, 50, "Papier", WHITE, BLACK}
};

void setup(void) {
  Serial.begin(9600);
  Wire.begin(); // Initialize as master
  Serial.println("Initializing screen...");
  tft.reset();
  tft.begin(0x9341);
  tft.fillScreen(BLACK);
  Serial.println("Screen initialized.");

  // Draw title
  drawTitle();
  Serial.println("Title displayed.");

  // Draw initial buttons
  for (int i = 0; i < 3; i++) {
    drawButton(touchAreas[i].x, touchAreas[i].y, touchAreas[i].w, touchAreas[i].h, touchAreas[i].label, touchAreas[i].backgroundColor, touchAreas[i].textColor);
    Serial.print("Button ");
    Serial.print(touchAreas[i].label);
    Serial.println(" displayed.");
  }
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
      if (p.x >= touchAreas[i].x && p.x <= (touchAreas[i].x + touchAreas[i].w) &&
          p.y >= touchAreas[i].y && p.y <= (touchAreas[i].y + touchAreas[i].h)) {
        Serial.print("Button ");
        Serial.print(touchAreas[i].label);
        Serial.println(" pressed.");

        const char* message = touchAreas[i].label;
        sendI2CMessage(message);

        tft.fillScreen(BLACK);
        drawTitle();
        tft.setCursor(10, 100);
        tft.setTextColor(WHITE);
        tft.setTextSize(3);
        tft.print("Opening ");
        tft.print(touchAreas[i].label);

        delay(1000); // Short delay to indicate action

        tft.fillScreen(BLACK);
        drawTitle();

        // Redraw buttons
        for (int j = 0; j < 3; j++) {
          drawButton(touchAreas[j].x, touchAreas[j].y, touchAreas[j].w, touchAreas[j].h, touchAreas[j].label, touchAreas[j].backgroundColor, touchAreas[j].textColor);
          Serial.print("Button ");
          Serial.print(touchAreas[j].label);
          Serial.println(" redisplayed.");
        }
      }
    }
  }
}

void drawTitle() {
  int x = 10, y = 10, w = 220, h = 40;
  tft.fillRect(x, y, w, h, BLUE);
  tft.drawRect(x, y, w, h, RED);
  tft.setCursor(x + 40, y + 10);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("PATATOR");
  Serial.println("Title PATATOR drawn.");
}

void drawButton(int x, int y, int w, int h, const char *label, uint16_t backgroundColor, uint16_t textColor) {
  tft.fillRect(x, y, w, h, backgroundColor);
  tft.drawRect(x, y, w, h, WHITE);
  tft.setCursor(x + 10, y + 15);
  tft.setTextColor(textColor);
  tft.setTextSize(2);
  tft.print(label);
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
