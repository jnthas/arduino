#define VIBRATION_DIGITAL_IN_PIN 8
#define DIGITAL_OUT_PIN_1 13
#define DIGITAL_OUT_PIN_2 12

int KNOCK_THRESHOLD = 200;
int NOISE_DURATION = 10;

int knock_count = 0;
int current_millis = 0;
int last_knock_millis= 0;
int interval = 0;

void setup() {
  Serial.begin(9600);

  pinMode(VIBRATION_DIGITAL_IN_PIN, INPUT);
  pinMode(DIGITAL_OUT_PIN_1, OUTPUT);
  pinMode(DIGITAL_OUT_PIN_2, OUTPUT);
}

void loop() {
  
  current_millis = millis();
  interval = current_millis - last_knock_millis;
  
  if (digitalRead(VIBRATION_DIGITAL_IN_PIN) == HIGH) {

    if (interval > NOISE_DURATION && interval <= KNOCK_THRESHOLD) {
      knock_count++;
      Serial.print("Knock count: "); Serial.println(knock_count);
    }
    
    last_knock_millis = current_millis;      
  }

  if (interval > KNOCK_THRESHOLD + NOISE_DURATION) {

    if (knock_count == 1) {
      digitalWrite(DIGITAL_OUT_PIN_1, !digitalRead(DIGITAL_OUT_PIN_1));
    } else if (knock_count == 2) {
      digitalWrite(DIGITAL_OUT_PIN_2, !digitalRead(DIGITAL_OUT_PIN_2));
    }

    knock_count = 0;    
  }
  
}
