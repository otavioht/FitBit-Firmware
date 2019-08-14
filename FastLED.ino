
void acende(uint8_t red, uint8_t green, uint8_t blue) {
  if (red + blue + green == 0) {
    for (uint16_t i = 0; i < NUMPIXELS; i += 2) {
      leds[i] = CRGB::Black;
    }
    FastLED.show();
  } else {
    for (uint16_t i = 0; i < NUMPIXELS; i += 2) {
      leds[i].r = red;
      leds[i].g = green;
      leds[i].b = blue;
    }
    FastLED.show();
  }
}


void pisca(uint16_t ms) {
  // Turn the first led red for 1 second
  uint8_t t = ms / 2000;
  for (uint8_t j = 0; j < t ; j++) {
    for (uint16_t i = 0; i < NUMPIXELS; i += 2) {
      leds[i] = CRGB::Green;
    }
    FastLED.show();
    delay(1000);
    // Set the first led back to black for 1 second
    for (uint16_t i = 0; i < NUMPIXELS; i += 2) {
      leds[i] = CRGB::Black;
    }
    FastLED.show();
    delay(1000);
  }
}
