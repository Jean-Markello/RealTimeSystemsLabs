#include <Arduino_FreeRTOS.h>

// Pin Definitions
const int redLedPin = 3;    // Red LED
const int orangeLedPin = 5; // Orange (Yellow) LED
const int greenLedPin = 4;  // Green LED
const int PadestrianGreenLedPin = 7;
const int PadestrianRedLedPin = 6;
const int buttonPin = 2;

volatile int flag = 0;


// Task Handles
TaskHandle_t RedTaskHandle, OrangeTaskHandle, GreenTaskHandle, PadestrianGreenTaskHandle, PadestrianRedTaskHandle, ButtonPressedTaskHandle;

// Task Functions
void RedTask(void *pvParameters) {
  for (;;) {
    if (flag){
      vTaskDelay(3000 / portTICK_PERIOD_MS); // Wait for 12 seconds
      digitalWrite(redLedPin, HIGH);   // Red ON
      vTaskDelay(14000 / portTICK_PERIOD_MS); // Wait for 12 seconds
      digitalWrite(redLedPin, LOW);   // Red ON
    }
  }
}

void OrangeTask(void *pvParameters) {
  for (;;) {
    if (flag){
      digitalWrite(orangeLedPin, HIGH);
      vTaskDelay(3000 / portTICK_PERIOD_MS);
      digitalWrite(orangeLedPin, LOW);
      vTaskDelay(12000 / portTICK_PERIOD_MS); // Wait for 12 seconds
      digitalWrite(orangeLedPin, HIGH);
      vTaskDelay(2000 / portTICK_PERIOD_MS); // Wait for 12 seconds
      digitalWrite(orangeLedPin, LOW);
    }

  }
}

void GreenTask(void *pvParameters) {
  for (;;) {
    digitalWrite(greenLedPin, HIGH);
    if (flag){  
      digitalWrite(greenLedPin, LOW);
      vTaskDelay(17000 / portTICK_PERIOD_MS); // Wait for 12 seconds
      digitalWrite(greenLedPin, HIGH);   // Red ON
      flag = 0;
    }
  }
}

void PadestrianRedTask(void *pvParameters) {
  for (;;) {
    digitalWrite(PadestrianRedLedPin, HIGH);
    if (flag){  
      vTaskDelay(4000 / portTICK_PERIOD_MS); // Wait for 12 seconds
      digitalWrite(PadestrianRedLedPin, LOW);
      vTaskDelay(10000 / portTICK_PERIOD_MS); // Wait for 12 seconds
      digitalWrite(PadestrianRedLedPin, HIGH);
    }
  }
}

void PadestrianGreenTask(void *pvParameters) {
  for (;;) {
    if (flag){  
      digitalWrite(PadestrianGreenLedPin, LOW);
      vTaskDelay(4000 / portTICK_PERIOD_MS); // Wait for 12 seconds
      digitalWrite(PadestrianGreenLedPin, HIGH);
      vTaskDelay(10000 / portTICK_PERIOD_MS); // Wait for 12 seconds
      digitalWrite(PadestrianGreenLedPin, LOW);
      

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
  pinMode(PadestrianRedLedPin, OUTPUT);
  pinMode(PadestrianGreenLedPin, OUTPUT);

  Serial.begin(115200);
  Serial.println("Starting FreeRTOS...");

  // Create Tasks for each LED
  xTaskCreate(RedTask, "RedTask", 64, NULL, 1, &RedTaskHandle);
  xTaskCreate(OrangeTask, "OrangeTask", 64, NULL, 1, &OrangeTaskHandle);
  xTaskCreate(GreenTask, "GreenTask", 128, NULL, 1, &GreenTaskHandle);
  xTaskCreate(PadestrianGreenTask, "PadestrianGreenTask",64, NULL, 1, &PadestrianGreenTaskHandle);
  xTaskCreate(PadestrianRedTask, "PadestrianRedTask", 64, NULL, 1, &PadestrianRedTaskHandle);
  xTaskCreate(ButtonPressedTask, "ButtonPressedTask", 128, NULL, 2, &ButtonPressedTaskHandle);


  // Start the FreeRTOS Scheduler
  vTaskStartScheduler();

  // This point is only reached if the scheduler fails to start
  Serial.println("Scheduler failed to start!");
}

void loop() {
  // Empty because FreeRTOS handles the tasks
}
