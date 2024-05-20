#include <Adafruit_TFTLCD.h> 
#include <Adafruit_GFX.h>    
#include <TouchScreen.h>

#define LCD_CS A4
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A5

#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

void setup() {
  Serial.begin(9600);
  Serial.print("Starting...");
  
  tft.reset();
  tft.begin(0x9341);
  tft.setRotation(1);
  
  tft.fillScreen(BLACK);


  tft.setCursor(100,30);
  tft.setTextColor(WHITE);
  tft.setTextSize(4);
  tft.print("Hello");
}

void loop() {
  // Rien à faire ici
}