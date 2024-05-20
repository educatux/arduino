#include <Adafruit_GFX.h>    
#include <Adafruit_TFTLCD.h> 
#include <SD.h>
#include <SPI.h>

// Pins for TFT
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4
#define PIN_SD_CS 10 

// TFT object
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

#define BUFFPIXEL 60
#define BUFFPIXEL_X3 180

// Define colors
#define BLACK   0x0000
#define WHITE   0xFFFF

// Image files
const char *imageFiles[3] = {"plastiques.bmp", "papier.bmp", "ordures.bmp"};

// Function declarations
void bmpdraw(File f, int x, int y);
uint16_t read16(File f);
uint32_t read32(File f);
boolean bmpReadHeader(File f);

void setup(void) {
  Serial.begin(9600);
  tft.reset();
  tft.begin(0x9341); 
  tft.fillScreen(BLACK);

  if (!SD.begin(PIN_SD_CS)) {
    Serial.println("SD Card initialization failed!");
    return;
  }

  Serial.println("SD Card initialization done.");
}

void loop(void) {
  for (int i = 0; i < 3; i++) {
    File bmpFile = SD.open(imageFiles[i]);
    if (!bmpFile) {
      Serial.print("Failed to load image: ");
      Serial.println(imageFiles[i]);
      continue;
    }
    if (!bmpReadHeader(bmpFile)) {
      Serial.print("Invalid BMP header in file: ");
      Serial.println(imageFiles[i]);
      bmpFile.close();
      continue;
    }
    bmpdraw(bmpFile, 0, 0);
    bmpFile.close();
    delay(3000); // Display each image for 3 seconds
  }
}

void bmpdraw(File f, int x, int y) {
  f.seek(54);
  uint8_t sdbuffer[BUFFPIXEL_X3];
  for (int i = 0; i < 75; i++) { // Height of image
    for (int j = 0; j < (200 / BUFFPIXEL); j++) { // Width of image / BUFFPIXEL
      f.read(sdbuffer, BUFFPIXEL_X3);
      int offset_x = j * BUFFPIXEL;
      for (int k = 0; k < BUFFPIXEL; k++) {
        uint16_t color = (sdbuffer[3 * k + 2] >> 3) << 11 | (sdbuffer[3 * k + 1] >> 2) << 5 | (sdbuffer[3 * k] >> 3);
        tft.drawPixel(x + offset_x + k, y + i, color);
      }
    }
  }
}

uint16_t read16(File f) {
  uint16_t d;
  uint8_t b;
  b = f.read();
  d = f.read();
  d <<= 8;
  d |= b;
  return d;
}

uint32_t read32(File f) {
  uint32_t d;
  uint16_t b;
  b = read16(f);
  d = read16(f);
  d <<= 16;
  d |= b;
  return d;
}

boolean bmpReadHeader(File f) {
  if (read16(f) != 0x4D42) return false;
  read32(f);
  read32(f);
  uint32_t offset = read32(f);
  uint32_t headerSize = read32(f);
  int bmpWidth = read32(f);
  int bmpHeight = read32(f);
  if (bmpWidth != 200 || bmpHeight != 75) return false;
  if (read16(f) != 1) return false;
  if (read16(f) != 24) return false;
  if (read32(f) != 0) return false;
  return true;
}
