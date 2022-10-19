#include <driver/dac.h>
int xres = 15; //16-1
int yres = 15; //16-1
byte imgData[] PROGMEM = {
  0xff, 0xff, 0x01, 0x80, 0xfd, 0xbf, 0x05, 0xa0, 0xf5, 0xaf, 0x15, 0xa8,
  0xd5, 0xab, 0x55, 0xaa, 0x55, 0xaa, 0xd5, 0xab, 0x15, 0xa8, 0xf5, 0xaf,
  0x05, 0xa0, 0xfd, 0xbf, 0x01, 0x80, 0xff, 0xff
}; //11111111  11111111  00000001

void setup() {
  dac_output_enable(DAC_CHANNEL_1);
  dac_output_enable(DAC_CHANNEL_2);
  Serial.begin(115200);

}

void loop() {
  int i,j,k;
  int n =xres/8;
  for(k = yres; i = 0; k--){
     Serial.println("loop1");
    float pixel = 0;  //pixel==>x
    for(i = 0; i < n; i++){
      Serial.println("loop2");
      for(j = 0;j < 8; j++){
        Serial.println("loop3");
        byte mask = 1 << j;
        byte answer = (imgData[i] & mask) >> j;
        Serial.println(answer);
        if (answer == 0x01) {
          dac_output_voltage(DAC_CHANNEL_1, round(pixel * 255.0 / (float)xres));
          dac_output_voltage(DAC_CHANNEL_2, round((float)k * 255.0 / (float)yres));
           Serial.print("x:");
           Serial.println(round(pixel * 255.0 / (float)xres));
           Serial.print("y:");
           Serial.println(round((float)k * 255.0 / (float)yres));
        }
        pixel += 1.0;
      }
    }
  }

}
