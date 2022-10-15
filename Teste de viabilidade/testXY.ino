#include <driver/dac.h>
#include <soc/rtc.h>

//Variables
int           lastx,lasty;

//*****************************************************************************
// Dot 
//*****************************************************************************

inline void Dot(int x, int y)
{
    if (lastx!=x){
      lastx=x;
      dac_output_voltage(DAC_CHANNEL_1, x);
    }
    #if defined EXCEL
      Serial.print("0x");
      if (x<=0xF) Serial.print("0");
      Serial.print(x,HEX);
      Serial.print(",");
    #endif
    #if defined EXCEL
      Serial.print("0x");
      if (lasty<=0xF) Serial.print("0");
      Serial.print(lasty,HEX);
      Serial.println(",");
    #endif
    if (lasty!=y){
      lasty=y;
      dac_output_voltage(DAC_CHANNEL_2, y);
    }
    #if defined EXCEL
      Serial.print("0x");
      if (x<=0xF) Serial.print("0");
      Serial.print(x,HEX);
      Serial.print(",");
    #endif
    #if defined EXCEL
      Serial.print("0x");
      if (y<=0xF) Serial.print("0");
      Serial.print(y,HEX);
      Serial.println(",");
    #endif
}

// End Dot 
//*****************************************************************************
void Line(byte x1, byte y1, byte x2, byte y2)
{
    int acc;
    // for speed, there are 8 DDA's, one for each octant
    if (y1 < y2) { // quadrant 1 or 2
        byte dy = y2 - y1;
        if (x1 < x2) { // quadrant 1
            byte dx = x2 - x1;
            if (dx > dy) { // < 45
                acc = (dx >> 1);
                for (; x1 <= x2; x1++) {
                    Dot(x1, y1);
                    acc -= dy;
                    if (acc < 0) {
                        y1++;
                        acc += dx;
                    }
                }
            }
            else {   // > 45
                acc = dy >> 1;
                for (; y1 <= y2; y1++) {
                    Dot(x1, y1);
                    acc -= dx;
                    if (acc < 0) {
                        x1++;
                        acc += dy;
                    }
                }
            }
        }
        else {  // quadrant 2
            byte dx = x1 - x2;
            if (dx > dy) { // < 45
                acc = dx >> 1;
                for (; x1 >= x2; x1--) {
                    Dot(x1, y1);
                    acc -= dy;
                    if (acc < 0) {
                        y1++;
                        acc += dx;
                    }
                }
            }
            else {  // > 45
                acc = dy >> 1;
                for (; y1 <= y2; y1++) {
                    Dot(x1, y1);
                    acc -= dx;
                    if (acc < 0) {
                        x1--;
                        acc += dy;
                    }
                }
            }        
        }
    }
    else { // quadrant 3 or 4
        byte dy = y1 - y2;
        if (x1 < x2) { // quadrant 4
            byte dx = x2 - x1;
            if (dx > dy) {  // < 45
                acc = dx >> 1;
                for (; x1 <= x2; x1++) {
                    Dot(x1, y1);
                    acc -= dy;
                    if (acc < 0) {
                        y1--;
                        acc += dx;
                    }
                }
            
            }
            else {  // > 45
                acc = dy >> 1;
                for (; y1 >= y2; y1--) { 
                    Dot(x1, y1);
                    acc -= dx;
                    if (acc < 0) {
                        x1++;
                        acc += dy;
                    }
                }

            }
        }
        else {  // quadrant 3
            byte dx = x1 - x2;
            if (dx > dy) { // < 45
                acc = dx >> 1;
                for (; x1 >= x2; x1--) {
                    Dot(x1, y1);
                    acc -= dy;
                    if (acc < 0) {
                        y1--;
                        acc += dx;
                    }
                }

            }
            else {  // > 45
                acc = dy >> 1;
                for (; y1 >= y2; y1--) {
                    Dot(x1, y1);
                    acc -= dx;
                    if (acc < 0) {
                        x1--;
                        acc += dy;
                    }
                }
            }
        }
    
    }
}

// End Line 
//*****************************************************************************


void setup() {
  Serial.begin(115200);
  Serial.println("\nESP32 Oscilloscope test");
  //rtc_clk_cpu_freq_set(RTC_CPU_FREQ_240M);
  Serial.println("CPU Clockspeed: ");
  Serial.println(rtc_clk_cpu_freq_value(rtc_clk_cpu_freq_get()));
  
  dac_output_enable(DAC_CHANNEL_1);
  dac_output_enable(DAC_CHANNEL_2);

}
//testar ponto,reta,circulo,letra
float t;
void loop() {
  // put your main code here, to run repeatedly:
  // Line(300,0,600,600);
  // Line(900,0,600,600);
  // delay(1000);
   /*for(int i=0;i<10;i++){
    Dot(0,0);
    delay(100);
    Dot(600,600);
    delay(100);
   }*/
   t+=0.1;
   //circulo
 for(float f = 0; f < M_PI ; f+= 0.01) //mpi*2
  {
    dac_output_voltage(DAC_CHANNEL_1, sin(f) * 120 + 120);
    dac_output_voltage(DAC_CHANNEL_2, cos(f) * 120 + 120);
  }
}
