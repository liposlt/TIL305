//By liposlt
//TI TIL305测试程序

#include "font.h"

#define HC595_DS    10    //HC595 PIN14 DS
#define HC595_ST_CP 9     //HC595 PIN12 ST_CP 
#define HC595_SH_CP 8     //HC595 PIN11 SH_CP 

#define HC595_DS_RESET    digitalWrite(HC595_DS, LOW)     
#define HC595_DS_SET      digitalWrite(HC595_DS, HIGH)

#define HC595_ST_CP_RESET digitalWrite(HC595_ST_CP, LOW)     
#define HC595_ST_CP_SET   digitalWrite(HC595_ST_CP, HIGH)

#define HC595_SH_CP_RESET digitalWrite(HC595_SH_CP, LOW)    
#define HC595_SH_CP_SET   digitalWrite(HC595_SH_CP, HIGH)

void setup() {
  // put your setup code here, to run once:
  pinMode(HC595_DS,  OUTPUT);
  pinMode(HC595_ST_CP, OUTPUT);
  pinMode(HC595_SH_CP, OUTPUT);
  }
  
void loop() {
  // put your main code here, to run repeatedly:
  //
//  HC595SendData_TIL305_char('9');
//  HC595SendData_TIL305_bmp();
//  HC595SendData_TIL305_rotate_bmp(bmp_rotate+16, 0);
//  HC595SendData_TIL305_rotate_onechar('4', 1);                //4
//  HC595SendData_TIL305_rotate_twochar('9', '8', 1);             //9 8
  HC595SendData_TIL305_rotate_twochar(0x30 + 11, 0x30 + 12, 1); //heart
}

/*-------------------------data format-------------------------------
TIL305
    COL   B0    B1    B2    B3    B4    B5
          PIN5  PIN1  PIN8  PIN14 PIN13 PIN7(Decimal Point) 
ROW   
A0(PIN2)    +     +     +     +     +     +
A1(PIN12)
A2(PIN3)
A3(PIN4)
A4(PIN11)
A5(PIN10)
A6(PIN9)

data0 == X    A6    A5    A4    A3    A2    A1    A0
data1 == X    X     B5    B4    B3    B2    B1    B0
-------------------------------------------------------------------*/
void HC595SendData_two(char data0, char data1)
{
  uint8_t i = 0;
  for (i = 0; i < 8; i++)
  {
    if (data0 & 0x80) {HC595_DS_SET;}
    else              {HC595_DS_RESET;}
    data0 <<= 1;
    HC595_SH_CP_SET;  HC595_SH_CP_RESET;
  } 
  for (i = 0; i < 8; i++) 
  {   
    if (data1 & 0x80) {HC595_DS_SET;}
    else              {HC595_DS_RESET;}
    data1 <<= 1;
    HC595_SH_CP_SET;  HC595_SH_CP_RESET;
  }
  HC595_ST_CP_SET;
  HC595_ST_CP_RESET;
}

//竖着显示一个字符
void HC595SendData_TIL305_char(uint8_t chr){
  uint8_t c = 0, i = 0;
  uint8_t delay_t = 10;
  c = chr - ' ';
  
  for(i = 0; i < 7; i++){
    HC595SendData_two(1 << i, (((F5x7[c][0] >> i) & 0x01) << 0) + \
                              (((F5x7[c][1] >> i) & 0x01) << 1) + \
                              (((F5x7[c][2] >> i) & 0x01) << 2) + \
                              (((F5x7[c][3] >> i) & 0x01) << 3) + \
                              (((F5x7[c][4] >> i) & 0x01) << 4));
    delayMicroseconds(delay_t*10);    
  }
}

//竖着显示一个图案
void HC595SendData_TIL305_bmp(void)
{
  uint8_t c = 0; 
  uint32_t i = 0;
  uint32_t j = 100;   //循环时间间隔
  uint8_t delay_t = 10;
  uint32_t m = 0;
  
  for(c = 0; c < 27; c++){
    for(i = 0; i < j; i++){
     for(m = 0; m < 7; m++){
        HC595SendData_two(1 << m, (((bmp[c + 0] >> m) & 0x01) << 0) + \
                                  (((bmp[c + 1] >> m) & 0x01) << 1) + \
                                  (((bmp[c + 2] >> m) & 0x01) << 2) + \
                                  (((bmp[c + 3] >> m) & 0x01) << 3) + \
                                  (((bmp[c + 4] >> m) & 0x01) << 4));
        delayMicroseconds(delay_t*10);    
      } 
    }
  }
}



//横着显示一个图案
void HC595SendData_TIL305_rotate_bmp(uint8_t *p, uint8_t rotate)
{
  uint32_t i = 0;
  uint32_t j = 0;
  uint32_t k = 0;
  
  uint32_t m = 11;      //字符长度
  uint32_t n = 100;   //循环时间间隔
  
  uint8_t delay_t = 10;
  uint8_t *data;
  
  data = p;
  
  if(0 == rotate) {//not rotate
    for(i = 0; i < m; i++){
      for(j = 0; j < n; j++){
        for(k = 0; k < 7; k++){
          HC595SendData_two(1 << k,data[i + k]);
          delayMicroseconds(delay_t*10);    
        } 
      }
    }
  }
  if(1 == rotate) {      //rotate 180
    for(i = 0; i < m; i++){
      for(j = 0; j < n; j++){
        for(k = 0; k < 7; k++){
          HC595SendData_two(0x40 >> k,data[i + k]);
          delayMicroseconds(delay_t*10);   
        } 
      }
    }
  }
}

//横着显示一个字符
void HC595SendData_TIL305_rotate_onechar(uint8_t chr, uint8_t rotate)
{
  uint8_t c = 0;
  uint8_t delay_t = 10;
  c = chr - '0';
  if(0 == rotate) {     //not rotate
    HC595SendData_two(0x01, F3x5[c][0]);delayMicroseconds(delay_t*10);
    HC595SendData_two(0x02, F3x5[c][1]);delayMicroseconds(delay_t*10);
    HC595SendData_two(0x04, F3x5[c][2]);delayMicroseconds(delay_t*10);
  }
  if(1 == rotate) {      //rotate 180
    HC595SendData_two(0x40, F3x5_rotate[c][0]);delayMicroseconds(delay_t*10);
    HC595SendData_two(0x20, F3x5_rotate[c][1]);delayMicroseconds(delay_t*10);
    HC595SendData_two(0x10, F3x5_rotate[c][2]);delayMicroseconds(delay_t*10);
  }
}

//横着显示两个字符
void HC595SendData_TIL305_rotate_twochar(uint8_t chr0, uint8_t chr1, uint8_t rotate)
{
  uint8_t c0 = 0,c1 = 0;
  uint8_t delay_t = 10;
  c0 = chr0 - '0';
  c1 = chr1 - '0';
  uint32_t i = 0;
  uint32_t j = 800;   //循环时间间隔
  
  if(0 == rotate) {       //not rotate
    for(i = 0; i < j; i++){
      HC595SendData_two(0x01, F3x5[c0][0]);delayMicroseconds(delay_t*10);
      HC595SendData_two(0x02, F3x5[c0][1]);delayMicroseconds(delay_t*10);
      HC595SendData_two(0x04, F3x5[c0][2]);delayMicroseconds(delay_t*10);
      HC595SendData_two(0x08, F3x5[c0][3]);delayMicroseconds(delay_t*10);
      HC595SendData_two(0x10, F3x5[c1][0]);delayMicroseconds(delay_t*10);
      HC595SendData_two(0x20, F3x5[c1][1]);delayMicroseconds(delay_t*10);
      HC595SendData_two(0x40, F3x5[c1][2]);delayMicroseconds(delay_t*10);
    }
  }
  if(1 == rotate) {      //rotate 180
    for(i = 0; i < j; i++){
      HC595SendData_two(0x40, F3x5_rotate[c0][0]);delayMicroseconds(delay_t*10);
      HC595SendData_two(0x20, F3x5_rotate[c0][1]);delayMicroseconds(delay_t*10);
      HC595SendData_two(0x10, F3x5_rotate[c0][2]);delayMicroseconds(delay_t*10);
      HC595SendData_two(0x08, F3x5_rotate[c0][3]);delayMicroseconds(delay_t*10);
      HC595SendData_two(0x04, F3x5_rotate[c1][0]);delayMicroseconds(delay_t*10);
      HC595SendData_two(0x02, F3x5_rotate[c1][1]);delayMicroseconds(delay_t*10);
      HC595SendData_two(0x01, F3x5_rotate[c1][2]);delayMicroseconds(delay_t*10);
    }
  }
}
