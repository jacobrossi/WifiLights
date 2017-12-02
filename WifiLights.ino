// WifiLights
// Portions of the animation routines from FastLED DemoReel100 (see LICENSE)
// Configure settings in WifiLightsConfig.h before flashing
#define FASTLED_ALLOW_INTERRUPTS 0
#include "FastLED.h"
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include "WifiLightsConfig.h"

FASTLED_USING_NAMESPACE

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(115200);
  delay(100);

  // Connect to WIFI
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFISSID);
  
  WiFi.begin(WIFISSID, PWD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Setup LEDs
  delay(2400); //Delay for recovery 
  
  // Configure FastLED
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm, solid, twinkle };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
uint32_t color1 = 255; //Blue
uint32_t color2 = 65280; //Green

void loop()
{
  gPatterns[gCurrentPatternNumber](); // Call the current pattern function once, updating the 'leds' array
  
  FastLED.show();  // send the 'leds' array out to the actual LED strip
  FastLED.delay(1000/FRAMES_PER_SECOND); // insert a delay to keep the framerate modest
  
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  EVERY_N_SECONDS( 10 ) { pollService(); } // poll service for latest pattern setting
}

void pollService()
{
  Serial.print("connecting to ");
  Serial.println(HOST);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(HOST, PORT)) {
    Serial.println("connection failed - incrementing pattern");
    nextPattern();
    return;
  }
  
  // We now create a URI for the request
  Serial.print("Requesting URL: ");
  Serial.println(PATH);
  
  // This will send the request to the server
  client.print(String("GET ") + PATH + " HTTP/1.1\r\n" +
               "Host: " + HOST + "\r\n" + 
               "Connection: close\r\n\r\n");

  //Trim Headers
  char endOfHeaders[] = "\r\n\r\n";
  client.find(endOfHeaders) || Serial.println("Invalid response");

  //Allocate JSON Buffer
  const size_t BUFFER_SIZE = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
  DynamicJsonBuffer jsonBuffer(BUFFER_SIZE);

  //Parse
  JsonObject& root = jsonBuffer.parseObject(client);
  if (!root.success()) Serial.println("Parsing failed!");
  
  int pattern = root["pattern"];
  color1 = root["color1"];
  color2 = root["color2"];
  Serial.println("pattern: " + String(pattern));
  Serial.println("color1: " + String(color1));
  Serial.println("color2: " + String(color2));
  
 
  // Set pattern based on response
  switch(pattern) {
    case 0:
      Serial.println("Setting LEDs to OFF");
      FastLED.setBrightness(0);
      break;
    case 1:
      Serial.println("Setting LEDs to RANDOM");
      FastLED.setBrightness(BRIGHTNESS);
      nextPattern();
      break;
    case 2:
      Serial.println("Setting LEDs to RAINBOW");
      FastLED.setBrightness(BRIGHTNESS);
      gCurrentPatternNumber = 0;
      break;
    case 3:
      Serial.println("Setting LEDs to RAINBOW WITH GLITTER");
      FastLED.setBrightness(BRIGHTNESS);
      gCurrentPatternNumber = 1;
      break;     
    case 4:
      Serial.println("Setting LEDs to CONFETTI");
      FastLED.setBrightness(BRIGHTNESS);
      gCurrentPatternNumber = 2;
      break;   
    case 5:
      Serial.println("Setting LEDs to SINELON");
      FastLED.setBrightness(BRIGHTNESS);
      gCurrentPatternNumber = 3;
      break;
    case 6:
      Serial.println("Setting LEDs to JUGGLE");
      FastLED.setBrightness(BRIGHTNESS);
      gCurrentPatternNumber = 4;
      break;
    case 7:
      Serial.println("Setting LEDs to BPM");
      FastLED.setBrightness(BRIGHTNESS);
      gCurrentPatternNumber = 5;
      break;
    case 8:
      Serial.println("Setting LEDs to SOLID");
      gCurrentPatternNumber = 6;
      break;
    case 9:
      Serial.println("Setting LEDs to TWINKLE");
      gCurrentPatternNumber = 7;
      break;
    default:
      //Unrecognized response, just increment to the next pattern
      Serial.println("Unrecognized command. Incrementing pattern.");
      FastLED.setBrightness(BRIGHTNESS);
      nextPattern();
      break;
  }
  
}

// LED ANIMATION PATTERNS===========================================================================================

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void twinkle()
{
  // themed colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += color1; 
  pos = random16(NUM_LEDS);
  leds[pos] += color2;
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void solid() {
  fill_solid(leds, NUM_LEDS, color1);
}
