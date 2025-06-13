#include "PS2Mouse.h"

PS2Mouse::PS2Mouse(int clk, int data){
  _ps2clk=clk;
  _ps2data=data;
  gohi(_ps2clk);
  gohi(_ps2data);  
}

void PS2Mouse::write(uint8_t data){
  uint8_t parity=1;
  
  gohi(_ps2data);
  gohi(_ps2clk);
  delayMicroseconds(300);
  golo(_ps2clk);
  delayMicroseconds(300);
  golo(_ps2data);
  delayMicroseconds(10);
  gohi(_ps2clk);
  
  while(digitalRead(_ps2clk)==HIGH);
  
  for(int i=0; i<8; i++){
    if(data&0x01) gohi(_ps2data);
    else golo(_ps2data); 
    while(digitalRead(_ps2clk)==LOW);
    while(digitalRead(_ps2clk)==HIGH);
    parity^=(data&0x01);
    data=data>>1;
  }
  
  if(parity) gohi(_ps2data);
  else golo(_ps2data);

  while(digitalRead(_ps2clk)==LOW);
  while(digitalRead(_ps2clk)==HIGH);
  
  gohi(_ps2data);
  delayMicroseconds(50);
  
  while(digitalRead(_ps2clk)==HIGH);
  while((digitalRead(_ps2clk)==LOW)||(digitalRead(_ps2data)==LOW));
  
  golo(_ps2clk);
}

uint8_t PS2Mouse::read(void){
  uint8_t data=0, bit=1;
 
  gohi(_ps2clk);
  gohi(_ps2data);
  delayMicroseconds(50);
  while(digitalRead(_ps2clk)==HIGH);
  
  delayMicroseconds(5);
  while(digitalRead(_ps2clk)==LOW);
  
  for(int i=0; i<8; i++){
    while(digitalRead(_ps2clk)==HIGH);
    bit=digitalRead(_ps2data);
    while(digitalRead(_ps2clk)==LOW);
    bitWrite(data,i,bit);
  }
  
  while(digitalRead(_ps2clk)==HIGH);
  while(digitalRead(_ps2clk)==LOW);
  while(digitalRead(_ps2clk)==HIGH);
  while(digitalRead(_ps2clk)==LOW);
  
  golo(_ps2clk);
  
  return data;
}

void PS2Mouse::writeAndRead(uint8_t data) {
  write(data);
  read();//Serial.println(read());
}

void PS2Mouse::begin(bool absolute){
  write(0xFF);
  for(int i=0; i<3; i++) read();
  writeAndRead(0xF0);
  if (absolute) {
    const uint8_t init[] = {
      0xE8, 0x03, 0xE8, 0x00, 0xE8, 0x00, 0xE8, 0x00, 0xF3, 0x14};
    for (int i=0;i<10; i++)
      writeAndRead(init[i]);
  }
  delayMicroseconds(100);
}

void PS2Mouse::getPosition(uint8_t &stat, int &x, int &y){
  writeAndRead(0xEB);
  stat=read();
  uint8_t _x=read();
  uint8_t _y=read();  

  bool negx=bitRead(stat,4);
  bool negy=bitRead(stat,5);
  x=twos(_x, negx);
  y=twos(_y, negy);
}

void PS2Mouse::getAbsPosition(uint8_t &stat, int &x, int &y, int& z) {
  writeAndRead(0xEB);
  stat=read() & 0b111111;
  uint16_t _xy1=read();
  z = read();  
  uint16_t _xy2=read();
  uint8_t _x=read();
  uint8_t _y=read();

  x=_x | ((_xy1 << 8) & 0xf00) | ((_xy2 << 8) & 0x1000);
  y=_y | ((_xy1 << 4) & 0xf00) | ((_xy2 << 7) & 0x1000);
}

void PS2Mouse::golo(int pin){
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void PS2Mouse::gohi(int pin){
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH);
}

const int m=0x100;
int PS2Mouse::twos(uint8_t value, bool sign){
  int v=(int)value;
  if(sign) v|=0xFF00;
  return v;
}

