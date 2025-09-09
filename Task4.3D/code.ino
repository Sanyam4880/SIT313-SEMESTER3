/****************************************************************************************************************************
  Task 4.3D: Multiple Inputs with Timer Interrupts for Arduino Nano 33 IoT
  Based on working example from SAMD_TimerInterrupt library
  Modified for DHT22 temperature and humidity sensor
*****************************************************************************************************************************/

// Required board validation
#if !( defined(ARDUINO_SAMD_ZERO) || defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_SAMD_MKRWIFI1010) \
      || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_SAMD_MKRFox1200) || defined(ARDUINO_SAMD_MKRWAN1300) || defined(ARDUINO_SAMD_MKRWAN1310) \
      || defined(ARDUINO_SAMD_MKRGSM1400) || defined(ARDUINO_SAMD_MKRNB1500) || defined(ARDUINO_SAMD_MKRVIDOR4000) \
      || defined(ARDUINO_SAMD_CIRCUITPLAYGROUND_EXPRESS) || defined(_SAMD51) || defined(SAMD51J20A_) \
      || defined(_SAMD51J19A) || defined(SAMD51G19A) || defined(SAMD51P19A_)  \
      || defined(_SAMD21E15A) || defined(SAMD21E16A) || defined(SAMD21E17A) || defined(SAMD21E18A_) \
      || defined(_SAMD21G15A) || defined(SAMD21G16A) || defined(SAMD21G17A) || defined(SAMD21G18A_) \
      || defined(_SAMD21J15A) || defined(SAMD21J16A) || defined(SAMD21J17A) || defined(SAMD21J18A_) )
  #error This code is designed to run on SAMD21/SAMD51 platform! Please check your Tools->Board setting.
#endif

// These define's MUST be placed at the beginning before #include "SAMDTimerInterrupt.h"
#define TIMER_INTERRUPT_DEBUG         0
#define TIMERINTERRUPT_LOGLEVEL     0

// Select only one to be true for SAMD21 - MUST be placed before the include
#define USING_TIMER_TC3         true
#define USING_TIMER_TC4         false
#define USING_TIMER_TC5         false
#define USING_TIMER_TCC         false
#define USING_TIMER_TCC1        false
#define USING_TIMER_TCC2        false

// Library includes - MUST be in this order
#include "SAMDTimerInterrupt.h"
#include "SAMD_ISR_Timer.h"
#include <DHT.h>  // Include DHT sensor library

// Pin definitions
#define BUTTON_PIN     2    // Push button (external interrupt)
#define PIR_PIN        3    // PIR sensor (external interrupt)
#define LED1_PIN       4    // LED1 controlled by button
#define LED2_PIN       5    // LED2 controlled by PIR sensor
#define LED3_PIN       6    // LED3 controlled by timer
#define DHT_PIN        7    // DHT22 data pin (was TRIG_PIN)
#define BUILTIN_LED    13   // Built-in LED

// DHT sensor type
#define DHT_TYPE DHT22

// Temperature thresholds (°C)
#define TEMP_LOW   20.0    // Low temperature
#define TEMP_MID   25.0    // Medium temperature  
#define TEMP_HIGH  30.0    // High temperature

// Hardware timer interval
#define HW_TIMER_INTERVAL_MS      10

// Software timer intervals
#define TIMER_INTERVAL_SENSOR     2000L  // Read DHT every 2 seconds (DHT22 needs more time)
#define TIMER_INTERVAL_STATUS     500L   // Update status every 500ms

///////////////////////////////////////////////

// Timer selection
#if (TIMER_INTERRUPT_USING_SAMD21)
  #if USING_TIMER_TC3
    #define SELECTED_TIMER      TIMER_TC3
  #else
    #error You have to select TC3 for SAMD21
  #endif
#else
  #define SELECTED_TIMER      TIMER_TC3
#endif

// Init selected SAMD timer
SAMDTimer ITimer(SELECTED_TIMER);

// Init SAMD_ISR_Timer
SAMD_ISR_Timer ISR_Timer;

// DHT sensor object
DHT dht(DHT_PIN, DHT_TYPE);

// Global variables
volatile bool buttonPressed = false;
volatile bool motionDetected = false;
float temperature = 0;
float humidity = 0;
bool sensorError = false;

// Function prototypes
void TimerHandler(void);
void buttonISR();
void pirISR();
void readDHT();
void updateStatus();
void handleButton();
void handlePIR();

void setup()
{
  Serial.begin(115200);
  while (!Serial && millis() < 5000);

  Serial.print(F("\nStarting Task 4.3D with DHT22 Sensor on ")); Serial.println(BOARD_NAME);
  Serial.println(SAMD_TIMER_INTERRUPT_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));

  // Configure pins
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(PIR_PIN, INPUT);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(BUILTIN_LED, OUTPUT);

  // Initialize DHT sensor
  dht.begin();

  // Initialize LEDs
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
  digitalWrite(BUILTIN_LED, LOW);

  // Attach external interrupts
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), pirISR, RISING);

  // Initialize hardware timer
  if (ITimer.attachInterruptInterval_MS(HW_TIMER_INTERVAL_MS, TimerHandler))
  {
    Serial.print(F("Starting ITimer OK, millis() = ")); Serial.println(millis());
  }
  else
    Serial.println(F("Can't set ITimer. Select another freq. or timer"));

  // Set up software timers
  ISR_Timer.setInterval(TIMER_INTERVAL_SENSOR, readDHT);
  ISR_Timer.setInterval(TIMER_INTERVAL_STATUS, updateStatus);

  // Startup sequence
  for(int i = 0; i < 3; i++) {
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, HIGH);
    digitalWrite(LED3_PIN, HIGH);
    digitalWrite(BUILTIN_LED, HIGH);
    delay(150);
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);
    digitalWrite(LED3_PIN, LOW);
    digitalWrite(BUILTIN_LED, LOW);
    delay(150);
  }

  Serial.println(F("=== Task 4.3D Initialized ==="));
  Serial.println(F("Button -> LED1, PIR -> LED2, Temperature -> LED3"));
  Serial.println(F("Temperature thresholds:"));
  Serial.println(F("- LED3 ON: Temperature >= 30°C"));
  Serial.println(F("- LED3 blinking: 25°C <= Temperature < 30°C"));  
  Serial.println(F("- LED3 OFF: Temperature < 25°C"));
  Serial.println(F("=============================================="));
}

void loop()
{
  // Handle external interrupts in main loop
  if (buttonPressed) {
    handleButton();
    buttonPressed = false;
  }
  
  if (motionDetected) {
    handlePIR();
    motionDetected = false;
  }
}

// Timer interrupt service routine
void TimerHandler(void)
{
  ISR_Timer.run();
}

// External interrupt service routines
void buttonISR() {
  buttonPressed = true;
}

void pirISR() {
  motionDetected = true;
}

// Handle button press
void handleButton() {
  digitalWrite(LED1_PIN, !digitalRead(LED1_PIN));
  Serial.println(F("Button pressed - LED1 toggled"));
}

// Handle PIR motion detection  
void handlePIR() {
  digitalWrite(LED2_PIN, !digitalRead(LED2_PIN));
  Serial.println(F("Motion detected - LED2 toggled"));
}

// Read DHT22 sensor values
void readDHT() {
  // Read temperature and humidity
  float newTemp = dht.readTemperature();
  float newHumidity = dht.readHumidity();
  
  // Check if any reads failed
  if (isnan(newTemp) || isnan(newHumidity)) {
    if (!sensorError) {
      Serial.println(F("Failed to read from DHT sensor!"));
      sensorError = true;
    }
    return;
  }
  
  // Clear error flag if reading is successful
  if (sensorError) {
    sensorError = false;
    Serial.println(F("DHT sensor reading recovered"));
  }
  
  // Update global variables
  temperature = newTemp;
  humidity = newHumidity;
  
  // Print values to serial monitor
  Serial.print(F("Temperature: ")); Serial.print(temperature); Serial.print(F("°C"));
  Serial.print(F(", Humidity: ")); Serial.print(humidity); Serial.println(F("%"));
}

// Update status based on temperature
void updateStatus() {
  if (sensorError) {
    // Blink LED3 to indicate sensor error
    digitalWrite(LED3_PIN, !digitalRead(LED3_PIN));
    return;
  }
  
  // Control LED3 based on temperature ranges
  if (temperature >= TEMP_HIGH) {
    digitalWrite(LED3_PIN, HIGH);  // High temperature - LED ON
    Serial.println(F("Temperature HIGH - LED3 ON"));
  } 
  else if (temperature >= TEMP_MID) {
    digitalWrite(LED3_PIN, !digitalRead(LED3_PIN));  // Medium temperature - LED blinking
    Serial.println(F("Temperature MEDIUM - LED3 blinking"));
  } 
  else {
    digitalWrite(LED3_PIN, LOW);  // Low temperature - LED OFF
    Serial.println(F("Temperature LOW - LED3 OFF"));
  }
  
  // Blink built-in LED to show system is running
  digitalWrite(BUILTIN_LED, !digitalRead(BUILTIN_LED));
}
