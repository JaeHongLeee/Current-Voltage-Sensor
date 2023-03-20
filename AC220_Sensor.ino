#define ANALOG_PIN_TIMER_INTERVAL 1000 // 1ms
#define CHANNEL_1 36 

int Channel_1_arr_size;

volatile int interruptCounter;
int totalInterruptCounter;
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

static long analogPinTimer = 0;
unsigned long thisMillis_old;
int sensorValue;

int swtich_number;  // '1': Data collection, '2': Data Calculation

unsigned long pastMillis = 0; 
unsigned long pastMillis_2 = 0;
int timerflag = 0;  // 초기 상태

int i = 0;  // 데이터 구별 숫자
int Data_arr1[500];
int Data_num1;

void IRAM_ATTR onTimer_1() { // interrupt function
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);
}

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);

  timer = timerBegin(0, 80, true);  // run for 1 second
  timerAttachInterrupt(timer, &onTimer_1, true);
  timerAlarmWrite(timer, 1000000*60*5, true); // run for 5 minutes
  timerAlarmEnable(timer);
}

void loop() {
  
  unsigned long deltaMillis = 0;
  unsigned long thisMillis = millis();

/*
  if(thisMillis - pastMillis >= 500){ // Doing 0.5sec
        pastMillis = thisMillis;
        timerflag = 1;
  }

  if(timerflag == 1){
        digitalWrite(2, !(digitalRead(2)));
        timerflag = 0;
  }
*/

  if(interruptCounter > 0){ //interrupt function, 5분 마다 동작
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);

    totalInterruptCounter++;

    Serial.print("An interrupt as occurred. Total number: ");
    Serial.println(totalInterruptCounter);
  }
  
  switch(timerflag){
    case 0:
      if (thisMillis != thisMillis_old) {
        deltaMillis = thisMillis-thisMillis_old;
        thisMillis_old = thisMillis;
      }
    
      analogPinTimer -= deltaMillis;

      if(analogPinTimer <= 0) {
        analogPinTimer += ANALOG_PIN_TIMER_INTERVAL;
        sensorValue = analogRead(CHANNEL_1);

        Data_arr1[i] = sensorValue;
            
        Serial.print(thisMillis/1000.0);  // Sampleing Time 1ms setting
        Serial.print(",");
        Serial.print(sensorValue);
        Serial.print(",");
        Serial.print(Data_arr1[i]);
        Serial.print(",");
        Serial.println(i);

        i++;

        if(thisMillis - pastMillis >= 3000){ // Doing 0.5 second
          pastMillis = thisMillis;
          //i = 0;
          Serial.print("on");
          Serial.print(",");
          Serial.println(i);
          timerflag = 1;
        }

        if((thisMillis%300000) == 0){ // Operations per 5 minutes
          Serial.print("5555555555555555555555555555555555555");
        } 
      }
    
      break;
    
  
    case 1:
      if(thisMillis - pastMillis_2 >= 9500 && timerflag == 1){ // 0.5초가 지났을때 수행할 작업 함수 작성
      pastMillis_2 = thisMillis;
      digitalWrite(2, !(digitalRead(2)));
      delay(1000);
      timerflag = 0;
      }
      break;
 }
}

/*
  if(thisMillis - pastMillis >= 3000){ // Doing 0.5 second
  pastMillis = thisMillis;
  //i = 0;
  Serial.print("on");
  Serial.print(",");
  Serial.println(i);
  timerflag = 1;

  }

  if(thisMillis - pastMillis_2 >= 9500 && timerflag == 1){ // 0.5초가 지났을때 수행할 작업 함수 작성
    pastMillis_2 = thisMillis;
    digitalWrite(2, !(digitalRead(2)));
    delay(1000);
    timerflag = 0;
  }
}
*/