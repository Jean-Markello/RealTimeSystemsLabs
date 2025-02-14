#include <Arduino_FreeRTOS.h>

// Pin Definitions
const int redLedPin = 3;    // Red LED
const int orangeLedPin = 5; // Orange (Yellow) LED
const int greenLedPin = 4;  // Green LED
const int PadestrianGreenLedPin = 7;
const int PadestrianRedLedPin = 6;
const int buttonPin = 2;


// Task Handles
TaskHandle_t RedTaskHandle, OrangeTaskHandle, GreenTaskHandle, ButtonPressedTaskHandle;

// Task Functions
void RedTask(void *pvParameters) {
  for (;;) {
    if (flag){
      // Red ON (12 seconds)
      digitalWrite(redLedPin, HIGH);   // Red ON
      vTaskDelay(12000 / portTICK_PERIOD_MS); // Wait for 12 seconds
      // Red OFF
      digitalWrite(redLedPin, LOW);
      vTaskDelay(13000 / portTICK_PERIOD_MS);
    }
  }
}

void OrangeTask(void *pvParameters) {
  for (;;) {
    if (flag){
      // Wait for Red ON phase (10 seconds)
      vTaskDelay(10000 / portTICK_PERIOD_MS);

      // Orange ON (2 seconds for Red+Orange)
      digitalWrite(orangeLedPin, HIGH);
      vTaskDelay(2000 / portTICK_PERIOD_MS);

      // Orange OFF
      digitalWrite(orangeLedPin, LOW);

      // Wait for Green ON duration (10 seconds) before turning Orange ON again
      vTaskDelay(10000 / portTICK_PERIOD_MS);

      // Orange ON (3 seconds)
      digitalWrite(orangeLedPin, HIGH);
      vTaskDelay(3000 / portTICK_PERIOD_MS);

      // Orange OFF
      digitalWrite(orangeLedPin, LOW);
    }

  }
}

void GreenTask(void *pvParameters) {
  for (;;) {
    if (flag){
      // Wait for Red and Red+Orange phases (12 seconds)
      vTaskDelay(12000 / portTICK_PERIOD_MS);

      // Green ON (10 seconds)
      digitalWrite(greenLedPin, HIGH);
      vTaskDelay(10000 / portTICK_PERIOD_MS);

      // Green OFF
      digitalWrite(greenLedPin, LOW);

      // Wait for Orange phase (3 seconds) before restarting the cycle
      vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
  }

}

void ButtonPressedTask(void *pvParameters) {
  for (;;) {
    Serial.println("Task5: Waiting for button press...");
    while (digitalRead(buttonPin) == HIGH)
      ;
    flag = 1;
    Serial.println("Task5: Button pressed!");
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {
  // Initialize GPIO pins
  pinMode(redLedPin, OUTPUT);
  pinMode(orangeLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);

  Serial.begin(115200);
  Serial.println("Starting FreeRTOS...");

  // Create Tasks for each LED
  xTaskCreate(RedTask, "RedTask", 128, NULL, 1, &RedTaskHandle);
  xTaskCreate(OrangeTask, "OrangeTask", 128, NULL, 1, &OrangeTaskHandle);
  xTaskCreate(GreenTask, "GreenTask", 128, NULL, 1, &GreenTaskHandle);
  xTaskCreate(ButtonPressedTask, "ButtonPressedTask", 128, NULL, 1, &ButtonPressedTaskHandle);


  // Start the FreeRTOS Scheduler
  vTaskStartScheduler();

  // This point is only reached if the scheduler fails to start
  Serial.println("Scheduler failed to start!");
}

void loop() {
  // Empty because FreeRTOS handles the tasks
}
