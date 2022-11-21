#include <WiFi.h>
#include <driver/dac.h>
#include "FastDAC.h"
#include "ImageData.h"

void copy(byte* src, byte* dst, int len) {
    memcpy(dst, src, sizeof(src[0])*len);
}

const char* ssid     = "******";  //YOUR SSID
const char* password = "*******"; //YOUR PASSWORD
int xres = 71; //16-1
int yres = 71; //16-1
int xo=255.0;
int yo=255.0;
hw_timer_t * timer = NULL;
byte PROGMEM imgData[648]; 

WiFiServer server(80);

void IRAM_ATTR displayOsci(byte * frame, int xres, int yres, double xd, double yd){
  DACPrepare(true);
  int i,j,k,z,yi=yres,y2=(xres+1)*(yres+1)/8,stp=y2/(yres+1);
  for(k =y2; k >= stp; k-=stp){
      z=k;
      for(j = 0;j <=xres ; j++){
        i=j;
        if(j>=8){
          i=j-8*(int)(j/8.0);
          z=k-(int)(j/8.0) ;
        }
        byte mask = 0x01 << i;
        byte answer = (frame[y2-z] & mask) >> i;
        if (answer != 0x01) {
          DAC1Write(round((double)j * xd / (double)xres));
          DAC2Write(round((double)yi * yd / (double)yres));
        } 
      }
    yi=yi-1;
  }
   DACUnprepare(true);
  
}
void IRAM_ATTR irq_handler() {
  displayOsci(imgData, xres, yres,xo,yo);
}


void timer_start_task(void *argm) {
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &irq_handler, true);
  timerAlarmWrite(timer, 20500, true); 
  timerAlarmEnable(timer);

    vTaskDelete(NULL);
}

void setup()
{
  copy(imgData0, imgData, 648);
    Serial.begin(115200);
    dac_output_enable(DAC_CHANNEL_1);
  dac_output_enable(DAC_CHANNEL_2);
   rtc_clk_cpu_freq_set(RTC_CPU_FREQ_240M);
  Serial.println("CPU Clockspeed: ");
  Serial.println(rtc_clk_cpu_freq_value(rtc_clk_cpu_freq_get()));
  

    delay(10);

    // connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.begin();

    // create timer interrupt
    xTaskCreatePinnedToCore(timer_start_task, "timer_start", 4096, NULL, 0, NULL,1);//All ESP-IDF protocol stuff (UART, WiFi, BLE, etc.) run in CPU 0 (PRO_CPU_NUM)

}

void loop(){
 WiFiClient client = server.available();

  if (client) {                           
    String currentLine = "";               
    while (client.connected()) {            
      if (client.available()) {             
        char c = client.read();             
               if (c == '\n') { 
            if (currentLine.length() == 0) { client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("<div style='text-align:center; font-size:20px'><h1 >Escolha uma foto </h1>"); 
            client.print("<a href=\'/1\'><img style='border:7px solid #FFFFFF' alt='cachorro' src='https://images.pexels.com/photos/8131025/pexels-photo-8131025.jpeg?auto=compress&cs=tinysrgb&w=1260&h=750&dpr=1'width=192 height=192><br></a>");
            client.print("<a href=\'/2\'><img style='border:7px solid #FFFFFF' alt='flor' src='https://images.pexels.com/photos/8480217/pexels-photo-8480217.jpeg?auto=compress&cs=tinysrgb&w=1260&h=750&dpr=1'width=192 height=192><br></a>");
            client.print("<a href=\'/3\'><img style='border:7px solid #FFFFFF' alt='pessoa' src='https://images.pexels.com/photos/91227/pexels-photo-91227.jpeg?auto=compress&cs=tinysrgb&w=1260&h=750&dpr=1'width=240 height=192></a><br> <a style='font-size:20px' href=\'/4\'>TIRAR FOTO<br></a></div></div>");
           client.println();
            break;
          } else {    
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;      
        }
        if (currentLine.endsWith("GET /1")) {
          xres=71;               
          yres=71;
          xo=255.0;
          yo=255.0;
          copy(imgData2, imgData, 648);
        }
        if (currentLine.endsWith("GET /2")) {
          xres=71;               
          yres=71;
          xo=255.0;
          yo=255.0;
          copy(imgData1, imgData, 648);
        }
        if (currentLine.endsWith("GET /3")) {
          xres=79;               
          yres=63;
          xo=255.0;
          yo=191.25;
         copy(imgData3, imgData, 640);
        }
         if (currentLine.endsWith("GET /4")) {
          xres=63;               
          yres=63;
          xo=255.0;
          yo=255.0;
          copy(imgData4, imgData, 512);
        }
      }
       
    }
    // close the connection:
    client.stop();
    //Serial.println("Client Disconnected.");
  }
 
}
