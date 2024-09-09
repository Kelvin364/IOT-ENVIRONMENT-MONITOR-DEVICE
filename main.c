// Turns the 'PRG' button into the power button, long press is off 
#define HELTEC_POWER_BUTTON   // must be before "#include <heltec_unofficial.h>"
#include <heltec_unofficial.h>
#include <Wire.h>
#include <DHT.h>

// Reduced pause time (in seconds)
#define PAUSE               10

// Frequency in MHz. Keep the decimal point to designate float.
// Check your own rules and regulations to see what is legal where you are.
#define FREQUENCY           866.3       // for Europe
// #define FREQUENCY           905.2       // for US

// LoRa bandwidth. Keep the decimal point to designate float.
// Allowed values are 7.8, 10.4, 15.6, 20.8, 31.25, 41.7, 62.5, 125.0, 250.0 and 500.0 kHz.
#define BANDWIDTH           250.0

// Number from 5 to 12. Higher means slower but higher "processor gain",
// meaning (in nutshell) longer range and more robust against interference. 
#define SPREADING_FACTOR    9

// Transmit power in dBm. 0 dBm = 1 mW, enough for tabletop-testing.
#define TRANSMIT_POWER      0

// Define sensor pins or addresses
#define CO_SENSOR_PIN 1
#define O2_SENSOR_PIN A2
#define METHANE_SENSOR_PIN 2
#define DHTPIN 3
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Structure to hold sensor data
struct SensorData {
  float co;
  float o2;
  float methane;
  float temperature;
};

SensorData sensorData;

String rxdata;
volatile bool rxFlag = false;
long counter = 0;
uint64_t last_tx = 0;
uint64_t tx_time;
uint64_t minimum_pause;

void setup() {
  heltec_setup();
  both.println("Radio init");
  RADIOLIB_OR_HALT(radio.begin());
  radio.setDio1Action(rx);
  
  both.printf("Frequency: %.2f MHz\n", FREQUENCY);
  RADIOLIB_OR_HALT(radio.setFrequency(FREQUENCY));
  both.printf("Bandwidth: %.1f kHz\n", BANDWIDTH);
  RADIOLIB_OR_HALT(radio.setBandwidth(BANDWIDTH));
  both.printf("Spreading Factor: %i\n", SPREADING_FACTOR);
  RADIOLIB_OR_HALT(radio.setSpreadingFactor(SPREADING_FACTOR));
  both.printf("TX power: %i dBm\n", TRANSMIT_POWER);
  RADIOLIB_OR_HALT(radio.setOutputPower(TRANSMIT_POWER));
  
  RADIOLIB_OR_HALT(radio.startReceive(RADIOLIB_SX126X_RX_TIMEOUT_INF));

  // Initialize sensors
  pinMode(CO_SENSOR_PIN, INPUT);
  pinMode(O2_SENSOR_PIN, INPUT);
  pinMode(METHANE_SENSOR_PIN, INPUT);
 
  dht.begin();
}

void loop() {
  heltec_loop();
  
  bool tx_legal = millis() > last_tx + minimum_pause;
  if ((PAUSE && tx_legal && millis() - last_tx > (PAUSE * 1000)) || button.isSingleClick()) {
    if (!tx_legal) {
      both.printf("Legal limit, wait %i sec.\n", (int)((minimum_pause - (millis() - last_tx)) / 1000) + 1);
      return;
    }

    readSensorData();
    displaySensorData();
    
    String dataString = String(counter++) + "," +
                        String(sensorData.co) + "," +
                        String(sensorData.o2) + "," +
                        String(sensorData.methane) + "," +
                        String(sensorData.temperature);

    //both.printf("TX [%s] ", dataString.c_str());
    radio.clearDio1Action();
    heltec_led(50);
    tx_time = millis();
    RADIOLIB(radio.transmit(dataString.c_str()));
    tx_time = millis() - tx_time;
    heltec_led(0);
    if (_radiolib_status == RADIOLIB_ERR_NONE) {
      //both.printf("OK (%i ms)\n", (int)tx_time);
    } else {
      both.printf("fail (%i)\n", _radiolib_status);
    }
    minimum_pause = tx_time * 100;
    last_tx = millis();
    radio.setDio1Action(rx);
    RADIOLIB_OR_HALT(radio.startReceive(RADIOLIB_SX126X_RX_TIMEOUT_INF));
  }

  if (rxFlag) {
    rxFlag = false;
    radio.readData(rxdata);
    if (_radiolib_status == RADIOLIB_ERR_NONE) {
      // both.printf("RX [%s]\n", rxdata.c_str());
      // both.printf("  RSSI: %.2f dBm\n", radio.getRSSI());
      // both.printf("  SNR: %.2f dB\n", radio.getSNR());
    }
    RADIOLIB_OR_HALT(radio.startReceive(RADIOLIB_SX126X_RX_TIMEOUT_INF));
  }
}

void readSensorData() {
  // Read data from sensors (replace with actual sensor reading code)
  sensorData.co = analogRead(CO_SENSOR_PIN);
  sensorData.co = (sensorData.co / 1960.0) * 100;
  sensorData.o2 = analogRead(O2_SENSOR_PIN);
  sensorData.o2 = sensorData.co * 0.428;
  sensorData.methane = analogRead(METHANE_SENSOR_PIN);
 sensorData.methane = (sensorData.methane / 1023.0) * 0.017;
  sensorData.temperature = dht.readTemperature();
  sensorData.temperature = sensorData.temperature*0.65;

  if (isnan(sensorData.temperature)) {
    both.println("Failed to read from DHT sensor!");
    sensorData.temperature = -999; // Use a value to indicate error
  }
  
  // Convert raw readings to actual values if necessary
}

void displaySensorData() {
  
both.printf("CO: %.2f %%\n", sensorData.co);          // Display CO with percentage
both.printf("O2: %.2f %%\n", sensorData.o2);          // Display O2 with percentage
both.printf("Methane: %.2f %%\n", sensorData.methane); // Display Methane with percentage
both.printf("Temperature: %.2f°C\n", sensorData.temperature); // Display Temperature in Celsius

  display.clear();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.drawString(0, 0, "CO: " + String(sensorData.co));
  display.drawString(0, 12, "O2: " + String(sensorData.o2));
  display.drawString(0, 24, "Methane: " + String(sensorData.methane));
  display.drawString(0, 36, "Temp: " + String(sensorData.temperature));
  display.display();
}

void rx() {
  rxFlag = true;
}