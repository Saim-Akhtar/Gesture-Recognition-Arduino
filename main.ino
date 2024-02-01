#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <esp_now.h>
#include <WiFi.h>
#include <saim2781-project-1_inferencing.h>

// Constants
#define MAX_ACCEPTED_RANGE  2.0f        
#define SCALE_FACTOR 0.08

// MPU6050 and ESP-NOW objects
Adafruit_MPU6050 mpu;
esp_now_peer_info_t peerInfo;

// Data Structure for ESP-NOW
struct struct_message {
    int b;
} myData;

// Broadcast Address for ESP-NOW
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP32");
    return;
  }
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("Error initializing MPU6050!");
    return;
  } else {
    Serial.println("MPU6050 initialized");
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
  mpu.setFilterBandwidth(MPU6050_BAND_94_HZ);
}

void loop() {
  // Prepare buffer for sensor data
  float buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE] = { 0 };

  for (size_t ix = 0; ix < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE; ix += 3) {
    uint64_t next_tick = micros() + (EI_CLASSIFIER_INTERVAL_MS * 1000);

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    buffer[ix] = a.acceleration.x * SCALE_FACTOR;
    buffer[ix + 1] = a.acceleration.y * SCALE_FACTOR;
    buffer[ix + 2] = a.acceleration.z * SCALE_FACTOR;

    delayMicroseconds(next_tick - micros());
  }

  // Convert buffer to signal for classification
  signal_t signal;
  int err = numpy::signal_from_buffer(buffer, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);
  if (err != 0) {
    Serial.println("Failed to create signal from buffer");
    return;
  }

  // Run the classifier
  ei_impulse_result_t result = { 0 };
  err = run_classifier(&signal, &result, false);
  if (err != EI_IMPULSE_OK) {
    Serial.print("Failed to run classifier: ");
    Serial.println(err);
    return;
  }

  
  if (result.classification[0].value > 0.7) {
        Serial.println(result.classification[0].label);
    }
   else if(result.classification[1].value > 0.7) {
        Serial.println(result.classification[1].label);
  }
  // Send data over ESP-NOW
  esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  // Small delay to stabilize loop
  delay(150);
}
