#include <SoftwareSerial.h>
#include <MsTimer2.h>
#include <math.h>

#define CHANNEL_Current_1 A0
#define CHANNEL_Current_2 A1
#define CHANNEL_Current_3 A2

#define CHANNEL_Voltage_1 A3
#define CHANNEL_Voltage_2 A4
#define CHANNEL_Voltage_3 A5

bool print_flag = false;
unsigned int serial_flag = 0;

// timer interrupt setting
unsigned int timer_count = 0;
int count = 0;
// data distinct digit
int i = 0;

unsigned int Current_data_arr0[500], Current_data_arr1[500], Current_data_arr2[500];
unsigned int Voltage_data_arr0[500], Voltage_data_arr1[500], Voltage_data_arr2[500];

// sin curve fitting value
float angle[500];
float fitting_sampling_time[500];

// sincurve fitting data variable
int Current0_theta = 0;
volatile unsigned long Current0_sum = 0, Current0_Odc = 0, Current0_X = 0;
unsigned int Current0_A1[500], Current0_A2[500];

int Current1_theta = 0;
volatile unsigned long Current1_sum = 0, Current1_Odc = 0, Current1_X = 0;
unsigned int Current1_A1[500], Current1_A2[500];

int Current2_theta = 0;
volatile unsigned long Current2_sum = 0, Current2_Odc = 0, Current2_X = 0;
unsigned int Current2_A1[500], Current2_A2[500];

int Voltage0_theta = 0;
volatile unsigned long Voltage0_sum = 0, Voltage0_Odc = 0, Voltage0_X = 0;
unsigned int Voltage0_A1[500], Voltage0_A2[500];

int Voltage1_theta = 0;
volatile unsigned long Voltage1_sum = 0, Voltage1_Odc = 0, Voltage1_X = 0;
unsigned int Voltage1_A1[500], Voltage1_A2[500];

// int Voltage2_theta = 0;
// volatile unsigned long Voltage2_sum = 0, Voltage2_Odc = 0, Voltage2_X = 0;
// unsigned int Voltage2_A1[500], Voltage2_A2[500];

long Irms0, Irms1, Irms2, Vrms0, Vrms1, Vrms2, Pf0, Pf1, Pf2;
unsigned long Channel1_Power = 0, Channel2_Power = 0, Channel3_Power = 0;

void onTimer0() { // interrupt function
  if (timer_count >= 0 && timer_count < 500){  // 0 ~ 0.5 sec
    noInterrupts();
    Current_data_arr0[i] = analogRead(CHANNEL_Current_1);
    Current_data_arr1[i] = analogRead(CHANNEL_Current_2);
    Current_data_arr2[i] = analogRead(CHANNEL_Current_3);

    Voltage_data_arr0[i] = analogRead(CHANNEL_Voltage_1);
    Voltage_data_arr1[i] = analogRead(CHANNEL_Voltage_2);
    Voltage_data_arr2[i] = analogRead(CHANNEL_Voltage_3);

    i++;
    interrupts();
  }
  
  timer_count ++; 
}

void setup() {
  Serial.begin(115200);

  MsTimer2::set(1, onTimer0); // 0.001 sec Timer
  MsTimer2::start();
  for(int t = 0; t < 500; t++){
    fitting_sampling_time[t] = 0.001 * (t+1);
    angle[t] = 2 * PI * 60 * fitting_sampling_time[t];
  }
}

void loop() {
  if (timer_count == 1000) {  // 1 second, 
    
    // sin curve fitting function
    for (int j = 0; j < 500; j++){
      
      noInterrupts();
      Current0_sum += Current_data_arr0[j];
      Current1_sum += Current_data_arr1[j];
      Current2_sum += Current_data_arr2[j];
      Voltage0_sum += Voltage_data_arr0[j];
      // Voltage1_sum += Voltage_data_arr1[j];
      // Voltage2_sum += Voltage_data_arr2[j]; 
      interrupts();
    
    }

    // Sincurve fitting 
    // Current 1
    Current0_Odc = Current0_sum/500;
    for(int a = 0; a < 500; a++){
      Current0_A1[a] = 2*Current0_Odc*sin(angle[a]);
      Current0_A2[a] = 2*Current0_Odc*cos(angle[a]);
      Current0_X = sqrt(sq(Current0_A1[a])+sq(Current0_A2[a]));
      Current0_theta = atan2(Current0_A2[a] ,Current0_A1[a]);
    }
    
    // Current 2
    Current1_Odc = Current1_sum/500;
    for(int b = 0; b < 500; b++){
      Current1_A1[b] = 2*Current1_Odc*sin(angle[b]);
      Current1_A2[b] = 2*Current1_Odc*cos(angle[b]);
      Current1_X = sqrt(sq(Current1_A1[b])+sq(Current1_A2[b]));
      Current1_theta = atan2(Current1_A2[b] ,Current1_A1[b]);
    }

    // Current 3
    Current2_Odc = Current2_sum/500;
    for(int c = 0; c < 500; c++){
      Current2_A1[c] = 2*Current2_Odc*sin(angle[c]);
      Current2_A2[c] = 2*Current2_Odc*cos(angle[c]);
      Current2_X = sqrt(sq(Current2_A1[c])+sq(Current2_A2[c]));
      Current2_theta = atan2(Current2_A2[c] ,Current2_A1[c]);
    }

    // Voltage 1
    Voltage0_Odc = Voltage0_sum/500;
    for(int d = 0; d < 500; d++){
      Voltage0_A1[d] = 2*Voltage0_Odc*sin(angle[d]);
      Voltage0_A2[d] = 2*Voltage0_Odc*cos(angle[d]);
      Voltage0_X = sqrt(sq(Voltage0_A1[d])+sq(Voltage0_A2[d]));
      Voltage0_theta = atan2(Voltage0_A2[d], Voltage0_A1[d]);
     }
    
    // Voltage 2
    Voltage1_Odc = Voltage1_sum/500;
    for(int e = 0; e < 500; e++){
      Voltage1_A1[e] = 2*Voltage1_Odc*sin(angle[e]);
      Voltage1_A2[e] = 2*Voltage1_Odc*cos(angle[e]);
      Voltage1_X = sqrt(sq(Voltage1_A1[e])+sq(Voltage1_A2[e]));
      Voltage1_theta = atan2(Voltage1_A2[e], Voltage1_A1[e]);
     }

    // Voltage 3
    // Voltage2_Odc = Voltage2_sum/500;
    // for(int f = 0; f < 500; f++){
    //   Voltage2_A1[f] = 2*Voltage2_Odc*sin(angle[f]);
    //   Voltage2_A2[f] = 2*Voltage2_Odc*cos(angle[f]);
    //   Voltage2_X = sqrt(sq(Voltage2_A1[f])+sq(Voltage2_A2[f]));
    //   Voltage2_theta = atan2(Voltage2_A2[f], Voltage2_A1[f]);
    //  }

    print_flag = true;

  Irms0 = Current0_X / sqrt(2);
  Irms1 = Current1_X / sqrt(2);
  Irms2 = Current2_X / sqrt(2);
  Vrms0 = Voltage0_X / sqrt(2);
  Vrms1 = Voltage1_X / sqrt(2);
  // Vrms2 = Voltage2_X / sqrt(2);

  Pf0 = cos(Current0_theta - Voltage0_theta);
  Pf1 = cos(Current0_theta - Voltage0_theta);
  // Pf2 = cos(Current0_theta - Voltage0_theta);

  // Channel 1
  Channel1_Power = Irms0 * Vrms0 * Pf0;

  // Channel 2
  Channel2_Power = Irms1 * Vrms1 * Pf1;

  // Channel 3
  // Channel3_Power = Irms2 * Vrms2 * Pf2;
  }

  else if (timer_count >= 10000) { // after 9.5sec, 10sec reset
    for(int k = 0; k < 500; k++){

      Current_data_arr0[k] = 0;
      Current_data_arr1[k] = 0;
      Current_data_arr2[k] = 0;

      Voltage_data_arr0[k] = 0;
      Voltage_data_arr1[k] = 0;
      Voltage_data_arr2[k] = 0;

      Current0_A1[k] = 0;
      Current0_A2[k] = 0; 
      Current1_A1[k] = 0; 
      Current1_A2[k] = 0; 
      Current2_A1[k] = 0; 
      Current2_A2[k] = 0;
      Voltage0_A1[k] = 0; 
      Voltage0_A2[k] = 0; 
      Voltage1_A1[k] = 0; 
      Voltage1_A2[k] = 0; 
      // Voltage2_A1[k] = 0; 
      // Voltage2_A2[k] = 0;
    }

    Current0_sum = 0, Current0_Odc = 0, Current0_X = 0, Current0_theta = 0;
    Current1_sum = 0, Current1_Odc = 0, Current1_X = 0, Current1_theta = 0;
    Current2_sum = 0, Current2_Odc = 0, Current2_X = 0, Current2_theta = 0;

    Voltage0_sum = 0, Voltage0_Odc = 0, Voltage0_X = 0, Voltage0_theta = 0;
    Voltage1_sum = 0, Voltage1_Odc = 0, Voltage1_X = 0, Voltage1_theta = 0;
    // Voltage2_sum = 0, Voltage2_Odc = 0, Voltage2_X = 0, Voltage2_theta = 0;
    
    Channel1_Power = 0, Channel2_Power = 0, Channel3_Power = 0;


    timer_count = 0;
    i = 0;
  }

  else if (timer_count == 9500) { // 9.5sec print
    if (print_flag == true) {
      // Serial.print(Irms0);
      // Serial.print("\t");
      // Serial.print(Vrms0);
      // Serial.print("\t");
      // Serial.print(cos(Current0_theta - Voltage0_theta));
      // Serial.print("\t");
      // Serial.println(Channel1_Power);
      
      Serial.println(fitting_sampling_time[300]);

      // Serial.print("\t");
      // Serial.print(Irms1);
      // Serial.print("\t");
      // Serial.print(Vrms1);
      // Serial.print("\t");
      // Serial.print(cos(Current1_theta - Voltage1_theta));
      // Serial.print("\t");
      // Serial.print(Channel2_Power);
      // Serial.print("\t");
      // Serial.print(Irms2);
      // Serial.print("\t");
      // Serial.print(Vrms2);
      // Serial.print("\t");
      // Serial.print(cos(Current2_theta - Voltage2_theta));
      // Serial.print("\t");
      // Serial.println(Channel3_Power);
      
      print_flag = false;
    }
  }
}
