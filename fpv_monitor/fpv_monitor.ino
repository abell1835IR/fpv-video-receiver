#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "rx5808.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define RSSI_PIN A3
#define BUTTON1 12
#define BUTTON2 14
#define LED_PIN 0 

#define TOTAL_BANDS 17
#define TOTAL_CHANNELS 8

// Channel configuration
struct ChannelInfo {
  uint16_t frequency;
  char band;
  uint8_t channel;
};

volatile uint8_t currentChannel = 1;
volatile uint8_t currentBand = 4;

// Display refresh control
unsigned long lastUpdate = 0;
const unsigned long updateInterval = 200; // 5Hz refresh rate


void handleButton1() {
  if (currentBand < TOTAL_BANDS - 1) {
    currentBand++;
  } else {
    currentBand = 0;
  }
} 

void handleButton2() {
  if (currentChannel < TOTAL_CHANNELS) {
    currentChannel++;
  } else {
    currentChannel = 1;
  }
} 

void drawSignalIndicator(int rssi) {
  const int startX = 80;
  const int startY = 10;
  const int columnWidth = 9;
  const int spacing = 2;
  
  int level;
  if (rssi >= -60) {
    level = 4;
  }
  else if (rssi >= -70) {
    level = 3;
  }
  else if (rssi >= -80) {
    level = 2;
  }
  else if (rssi >= -90) {
    level = 1;
  }
  else {
    level = 0;
  }
  

  for(int i = 0; i < 4; i++) {
    int height = (i + 1) * 12;
    int x = startX + (i * (columnWidth + spacing));
    
    display.drawRect(x, startY + (40 - height), columnWidth, height, SSD1306_WHITE);
    
    if(i < level) {
      display.fillRect(x, startY + (40 - height), columnWidth, height, SSD1306_WHITE);
    }
  }

  display.setTextSize(1);
  display.setCursor(startX, 54);
  display.print(rssi);
  display.print("dBm");
  display.setCursor(startX + 30, 54);
}

void updateDisplay(bool updateChannel) {
  display.clearDisplay();
  
  int rssi = map(analogRead(RSSI_PIN), 0, 360, -120, 0);

  uint16_t index = currentBand * TOTAL_CHANNELS + (currentChannel - 1);
  uint16_t freq = pgm_read_word(&channelTable[index]);
  char band = pgm_read_byte(&bands[currentBand]);
  
  if (updateChannel) {
    set_5ghz_channel_freq(freq);
  }

  display.setTextSize(2);
  display.setCursor(0, 0);                  
  display.print(freq); 
  display.print("MHz");

  display.setTextSize(5);
  display.setCursor(15, 25);
  display.print(band);
  display.setTextSize(5);
  display.print(currentChannel);

  drawSignalIndicator(rssi);

  display.display();
}

void setup() {
  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.setTextColor(SSD1306_WHITE);
  display.setRotation(2);

  manual_spi_init();

  pinMode(12, INPUT);
  pinMode(14, INPUT);
  pinMode(0, OUTPUT);

  pinMode(BUTTON1, INPUT_PULLUP); 
  pinMode(BUTTON2, INPUT_PULLUP); 
  pinMode(LED_PIN, OUTPUT);   
  
  pinMode(RSSI_PIN, INPUT);
  
  char initialBand = pgm_read_byte(&bands[currentBand]);
  updateDisplay(true);
}

void loop() {
  if(millis() - lastUpdate > updateInterval) {
    updateDisplay(false);
    lastUpdate = millis();
  }

  bool stateButton1 = digitalRead(BUTTON1); 
  bool stateButton2 = digitalRead(BUTTON2);
  
  if (stateButton1 == LOW) {
    handleButton1();
    digitalWrite(LED_PIN, HIGH);
    updateDisplay(true);
    delay(30);
  } 
  else if (stateButton2 == LOW) {
    handleButton2();
    digitalWrite(LED_PIN, HIGH);
    updateDisplay(true);
    delay(30);
  } 
  else {
    digitalWrite(LED_PIN, LOW);
  }
  
  delay(100);

}