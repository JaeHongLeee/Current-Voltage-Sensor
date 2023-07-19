#include <SoftwareSerial.h>
#include <MsTimer2.h>

#define CHANNEL_Current_1 A0
#define CHANNEL_Current_2 A1
#define CHANNEL_Current_3 A2

#define CHANNEL_Voltage_1 A3
#define CHANNEL_Voltage_2 A4
#define CHANNEL_Voltage_3 A5

bool print_flag = false;

// timer interrupt setting
volatile unsigned int timer_count = 0;

// current value
int Current_Value_0, Current_Value_1, Current_Value_2;

// current value
int start0, end0, minVal0, maxVal0;
int start1, end1, minVal1, maxVal1;
int start2, end2, minVal2, maxVal2;

// voltage value
int start3, end3, minVal3, maxVal3;
int start4, end4, minVal4, maxVal4;
int start5, end5, minVal5, maxVal5;

unsigned int serial_flag = 0;

// data distinct digit
int i = 0;

// Current variable
int a = 0, c = 0, e = 0;
int b, d, f;

// Voltage variable
int g = 0, l = 0, n = 0;
int h, m, o;

int Current_data_arr0[500], Current_data_arr1[500], Current_data_arr2[500];
int Current_data_mmd0[10], Current_data_mmd1[10], Current_data_mmd2[10];
int Voltage_data_arr0[500], Voltage_data_arr1[500], Voltage_data_arr2[500];
int Voltage_data_mmd0[10], Voltage_data_mmd1[10], Voltage_data_mmd2[10];

int Current_sum0 = 0, Current_sum1 = 0, Current_sum2 = 0;
int Current_mmd_sum0 = 0, Current_mmd_sum1 = 0, Current_mmd_sum2 = 0;
int Current_count0 = 0, Current_count1 = 0, Current_count2 = 0;
int Current_average0 = 0, Current_average1 = 0, Current_average2 = 0;

int Voltage_mmd_sum0 = 0, Voltage_mmd_sum1 = 0, Voltage_mmd_sum2 = 0;
int Voltage_count0 = 0, Voltage_count1 = 0, Voltage_count2 = 0;
int Voltage_average0 = 0, Voltage_average1 = 0, Voltage_average2 = 0;

SoftwareSerial SerialMega(10, 11);  // 아두이노 메가의 시리얼 통신을 위한 SoftwareSerial 설정

void onTimer0() { // interrupt function
  if (timer_count == 0) {
    Current_data_arr0[i] = analogRead(CHANNEL_Current_1);
    Current_data_arr1[i] = analogRead(CHANNEL_Current_2);
    Current_data_arr2[i] = analogRead(CHANNEL_Current_3);

    Voltage_data_arr0[i] = analogRead(CHANNEL_Voltage_1);
    Voltage_data_arr1[i] = analogRead(CHANNEL_Voltage_2);
    Voltage_data_arr2[i] = analogRead(CHANNEL_Voltage_3);

    i++;
  }
}

void onTimer1() {
  timer_count++;
}

void setup() {
  SerialMega.begin(115200); // 아두이노 메가와의 시리얼 통신을 위한 속도 설정
  pinMode(2, OUTPUT);

  MsTimer2::set(1, onTimer0); // 0.001 sec Sampling time
  MsTimer2::set(500, onTimer1); // 0.5 sec Timer (Mode Change timer)
  MsTimer2::start();

}

void loop() {
  if (timer_count != 0) {
    i = 0;
  }

  if (timer_count >= 1 && timer_count < 5.5) {  // 19.5
    // a, b 변수 사용 (channel_1 current)
    for (a = 0; a < 10; a++) {
      start0 = a * 50;
      end0 = start0 + 49;
      minVal0 = Current_data_arr0[start0];
      maxVal0 = Current_data_arr0[start0];
      for (b = start0 + 1; b <= end0; b++) {
        if (Current_data_arr0[b] < minVal0) {
          minVal0 = Current_data_arr0[b];
        }
        if (Current_data_arr0[b] > maxVal0) {
          maxVal0 = Current_data_arr0[b];
        }
      }
      Current_data_mmd0[a] = maxVal0 - minVal0;
      Current_mmd_sum0 += Current_data_mmd0[a];
      Current_count0++;
    }

    // g, h 변수 사용 (channel_1 voltage)
    for (g = 0; g < 10; g++) {
      start3 = g * 50;
      end3 = start3 + 49;
      minVal3 = Voltage_data_arr0[start3];
      maxVal3 = Voltage_data_arr0[start3];
      for (h = start3 + 1; h <= end3; h++) {
        if (Voltage_data_arr0[h] < minVal3) {
          minVal3 = Voltage_data_arr0[b];
        }
        if (Voltage_data_arr0[b] > maxVal3) {
          maxVal3 = Voltage_data_arr0[b];
        }
      }
      Voltage_data_mmd0[g] = maxVal3 - minVal3;
      Voltage_mmd_sum0 += Voltage_data_mmd0[g];
      Voltage_count0++;
    }

    // c, d 변수 사용 (channel_2 current)
    for (c = 0; c < 10; c++) {
      start1 = c * 50;
      end1 = start1 + 49;
      minVal1 = Current_data_arr1[start1];
      maxVal1 = Current_data_arr1[start1];
      for (d = start1 + 1; d <= end1; d++) {
        if (Current_data_arr1[d] < minVal1) {
          minVal1 = Current_data_arr1[d];
        }
        if (Current_data_arr1[d] > maxVal1) {
          maxVal1 = Current_data_arr1[d];
        }
      }
      Current_data_mmd1[c] = maxVal1 - minVal1;
      Current_mmd_sum1 += Current_data_mmd1[c];
      Current_count1++;
    }

    // l, m 변수 사용 (channel_2 voltage)
    for (l = 0; l < 10; l++) {
      start4 = l * 50;
      end4 = start4 + 49;
      minVal4 = Voltage_data_arr1[start4];
      maxVal4 = Voltage_data_arr1[start4];
      for (m = start4 + 1; m <= end4; m++) {
        if (Voltage_data_arr1[m] < minVal4) {
          minVal4 = Voltage_data_arr1[m];
        }
        if (Voltage_data_arr1[m] > maxVal4) {
          maxVal4 = Voltage_data_arr1[m];
        }
      }
      Voltage_data_mmd1[l] = maxVal4 - minVal4;
      Voltage_mmd_sum1 += Voltage_data_mmd1[l];
      Voltage_count1++;
    }

    // e, f 변수 사용 (channel_3 current)
    for (e = 0; e < 10; e++) {
      start2 = e * 50;
      end2 = start2 + 49;
      minVal2 = Current_data_arr2[start2];
      maxVal2 = Current_data_arr2[start2];
      for (f = start2 + 1; f <= end2; f++) {
        if (Current_data_arr2[f] < minVal2) {
          minVal2 = Current_data_arr2[f];
        }
        if (Current_data_arr2[f] > maxVal2) {
          maxVal2 = Current_data_arr2[f];
        }
      }
      Current_data_mmd2[e] = maxVal2 - minVal2;
      Current_mmd_sum2 += Current_data_mmd2[e];
      Current_count2++;
    }

    // n, o 변수 사용 (channel_3 voltage)
    for (n = 0; n < 10; n++) {
      start5 = n * 50;
      end5 = start5 + 49;
      minVal5 = Voltage_data_arr2[start5];
      maxVal5 = Voltage_data_arr2[start5];
      for (o = start5 + 1; o <= end5; o++) {
        if (Voltage_data_arr2[o] < minVal5) {
          minVal5 = Voltage_data_arr2[o];
        }
        if (Voltage_data_arr2[o] > maxVal5) {
          maxVal5 = Voltage_data_arr2[o];
        }
      }
      Voltage_data_mmd2[n] = maxVal5 - minVal5;
      Voltage_mmd_sum2 += Voltage_data_mmd2[n];
      Voltage_count2++;
    }

    Current_average0 = Current_mmd_sum0 / Current_count0;
    Current_average1 = Current_mmd_sum1 / Current_count1;
    Current_average2 = Current_mmd_sum2 / Current_count2;

    Voltage_average0 = Voltage_mmd_sum0 / Voltage_count0;
    Voltage_average1 = Voltage_mmd_sum1 / Voltage_count1;
    Voltage_average2 = Voltage_mmd_sum2 / Voltage_count2;

    print_flag = false;
  }

  else if (timer_count == 7) { // after 9.5sec  // 20
    memset(Current_data_arr0, 0, sizeof(Current_data_arr0));
    memset(Current_data_arr1, 0, sizeof(Current_data_arr1));
    memset(Current_data_arr2, 0, sizeof(Current_data_arr2));

    memset(Current_data_mmd0, 0, sizeof(Current_data_mmd0));
    memset(Current_data_mmd1, 0, sizeof(Current_data_mmd1));
    memset(Current_data_mmd2, 0, sizeof(Current_data_mmd2));

    memset(Voltage_data_arr0, 0, sizeof(Voltage_data_arr0));
    memset(Voltage_data_arr1, 0, sizeof(Voltage_data_arr1));
    memset(Voltage_data_arr2, 0, sizeof(Voltage_data_arr2));

    memset(Voltage_data_mmd0, 0, sizeof(Voltage_data_mmd0));
    memset(Voltage_data_mmd1, 0, sizeof(Voltage_data_mmd1));
    memset(Voltage_data_mmd2, 0, sizeof(Voltage_data_mmd2));

    Current_count0 = 0;
    Current_count1 = 0;
    Current_count2 = 0;
    Current_average0 = 0;
    Current_average1 = 0;
    Current_average2 = 0;
    Current_mmd_sum0 = 0;
    Current_mmd_sum1 = 0;
    Current_mmd_sum2 = 0;

    Voltage_count0 = 0;
    Voltage_count1 = 0;
    Voltage_count2 = 0;
    Voltage_average0 = 0;
    Voltage_average1 = 0;
    Voltage_average2 = 0;
    Voltage_mmd_sum0 = 0;
    Voltage_mmd_sum1 = 0;
    Voltage_mmd_sum2 = 0;

    timer_count = 0;
  }

  else if (timer_count == 6) { // 19
    if (!print_flag) {
      SerialMega.print(Current_average0);
      SerialMega.print("\t");
      SerialMega.print(Voltage_average0);
      SerialMega.print("\t");
      SerialMega.print(Current_average1);
      SerialMega.print("\t");
      SerialMega.println(Voltage_average1);
      //SerialMega.print("\n");
      //SerialMega.print(Current_average2);
      //SerialMega.print("\t");
      //SerialMega.println(Voltage_average2);

      SerialMega.print(Current_average0);

      print_flag = true;
    }
  }
}
