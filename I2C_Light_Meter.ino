// --------------------------------------------------------------------------------
// An Arduino sketch to display visible light and lux levels.
//
// MIT license.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
//
//      ******************************************************
//      Designed for the Adafruit TSL2561 Digital Light Sensor,
//      http://www.adafruit.com/products/439
//      4-channel I2C Safe Bi-directional Logic Level Converter,
//      http://www.adafruit.com/products/757
//      16×2 LCD,
//      http://www.adafruit.com/products/181
//      and LCD I2C Backpack
//      http://www.adafruit.com/products/292
//      ******************************************************
//
//
// --------------------------------------------------------------------------------
// Dependencies
// --------------------------------------------------------------------------------
// Adafruit Industries's Liquid Crystal library:
//       https://github.com/adafruit/LiquidCrystal
// Adafruit Industries's TSL2561 library:
//       https://github.com/adafruit/TSL2561-Arduino-Library
// --------------------------------------------------------------------------------
#include "Wire.h"
#include "LiquidCrystal.h"
#include "TSL2561.h"

//! LCD display configured to default i2c address.
LiquidCrystal lcd(0);

//! Light sensor configured to default i2c address.
TSL2561 tsl(TSL2561_ADDR_FLOAT); 

//! Buffer for printing light readings to the LCD.
char output_buffer[6];

//! Initialize the sketch.
void setup() {
  // Initialize the serial connection for debugging.
  Serial.begin(9600);
  Serial.println(F("Serial initialized"));
  
  // Initialize the LCD.
  lcd.begin(16, 2);
  Serial.println(F("LCD initialized"));
  lcd.setBacklight(HIGH);
  
  // Print the top line label.
  lcd.setCursor(0, 0);
  lcd.print(F("Vis:"));
  
  // Print the bottom line label.
  lcd.setCursor(0, 1);
  lcd.print(F("Lux:"));
    
  // Initialize the light sensor.
  if (tsl.begin()) {
    Serial.println(F("Sensor initialized"));
  } else {
    Serial.println(F("Sensor failed to initialize"));
  }

  // Measuring relatively bright light, so gain of 0 is reasonable
  tsl.setGain(TSL2561_GAIN_0X);
  
  // Measuring relatively bright light, so short sampling time is reasonable
  tsl.setTiming(TSL2561_INTEGRATIONTIME_13MS);
  
  // For low light level, comment out the above configuration and uncomment the below lines
  // tsl.setGain(TSL2561_GAIN_16X);
  // tsl.setTiming(TSL2561_INTEGRATIONTIME_402MS);
}

//! Main execution loop.
void loop() {
  // From the Adafruit TSL2561 Example sketch, get both IR and
  // full spectrum values in one read.
  uint32_t full_luminosity = tsl.getFullLuminosity();
  uint16_t ir_spectrum = full_luminosity >> 16;
  uint16_t full_spectrum = full_luminosity & 0xFFFF;
  
  // Set the cursor to the data column on the top line.
  lcd.setCursor(5, 0);
    
  // Format the output using a format string stored in program memory.
  snprintf_P(output_buffer, 6, PSTR("%5d"), (full_spectrum - ir_spectrum));
  
  // Print to the LCD.
  lcd.print(output_buffer);
  
  // Set the cursor to the data column on the bottom line.
  lcd.setCursor(5, 1);
  
  // Format the output using a format string stored in program memory.
  snprintf_P(output_buffer, 6, PSTR("%5d"), tsl.calculateLux(full_spectrum, ir_spectrum));

  // Print to the LCD.
  lcd.print(output_buffer);
}

