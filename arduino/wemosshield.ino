#include <Wire.h>
#define SLAVE_ADDRESS_1 0x32
//Set Commands
#define MYSLAVE_SET_REG 0x01
//GET commands
#define MOTOR_GET_COUNT 0x02
#define MOTOR_SET_COUNT 0x03
#define MOTOR_SET_TARGET 0x04
#define MOTOR_GET_TARGET 0x05
#define MOTOR_SET_TICKS 0x06
#define MOTOR_SET_DIR_RES 0x7
void setup() {
 Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output

}

int32_t read_wms( uint8_t command, int address)
{ int32_t pos;
  char *pointer;
  char c;
  pointer=(char*)(&pos);

 Wire.beginTransmission(address); 
  Wire.write(command);             
  Wire.endTransmission();  
    
  delay(5);
  
   Wire.requestFrom(address, 4); 
    while (Wire.available()) { 
    char c = Wire.read();
    (*pointer++)=c;
    }
    return pos;
}


void write_wms( uint8_t command, int address,int32_t value)
{ 
  char *pointer;
  pointer=(char*)(&value);
 Wire.beginTransmission(address); 
  Wire.write( command);
  Wire.write(pointer,4);
  Wire.endTransmission();    

}
void loop() {

 //goto from 0 to 1000 
 int32_t pos=0;//set motor counter to 0
 int32_t target= 1000;//set desired position 1000
 int32_t speed=3000;//20,8 us * 300 =0.624s  ->16,02 microsteps/s 
 write_wms(MOTOR_SET_COUNT,SLAVE_ADDRESS_1,pos);//send counter
 write_wms(MOTOR_SET_TARGET,SLAVE_ADDRESS_1,target);//target set
 write_wms(MOTOR_SET_TICKS,SLAVE_ADDRESS_1,speed);//command speed
 while ( (pos=read_wms( MOTOR_GET_COUNT ,SLAVE_ADDRESS_1))!=target)
 {
  delay (100);
  Serial.println(pos);}

//motor will turn an stop until reach 1000 counter value

//now we command back 0 pos
  target= 0;//set desired position 1000
 speed=-3000;//20,8 us * 300 =0.624s  ->-16,02 microsteps/s 
 int32_t resolution=2; //motor will no run on 16 msteps 
 write_wms(MOTOR_SET_DIR_RES,SLAVE_ADDRESS_1,resolution);
 write_wms(MOTOR_SET_TARGET,SLAVE_ADDRESS_1,target);//target set
 write_wms(MOTOR_SET_TICKS,SLAVE_ADDRESS_1,speed);//command speed
 while ( (pos=read_wms( MOTOR_GET_COUNT ,SLAVE_ADDRESS_1))!=target)
 {
  delay (100);
  Serial.println(pos);}
 

}
