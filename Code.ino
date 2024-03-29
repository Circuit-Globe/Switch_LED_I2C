#include<avr/io.h>
void i2c_init (void){
TWSR = 0X00; // set prescaler bits to zero
TWBR = 0x47; // SCL frequency is 50K for XTAL = 8Mhz
TWCR = 0x04; // enable TWI module
}
void i2c_start(void){
TWCR = ( 1 << TWINT ) | ( 1 << TWSTA ) | ( 1 << TWEN );
while ((TWCR & ( 1 << TWINT ) ) == 0) ;
}
void i2c_write(unsigned char data){
TWDR = data ;
TWCR = (1<<TWINT) | (1<<TWEN); // enable the transmission
while ((TWCR & ( 1 << TWINT ) ) == 0);
}
unsigned char i2c_read(unsigned char last){
if ( last == 0 ) // If want to read more than 1 byte
 TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
else // else read only one byte
TWCR = (1<<TWINT) | (1<<TWEN);
while ((TWCR & ( 1 << TWINT ) ) == 0);
return TWDR ;
}
void i2c_stop(){
TWCR = ( 1 <<TWINT )|( 1 << TWEN) | ( 1<< TWSTO);
}
int main(void){
 volatile uint8_t rcvData=0;

 while(1){

 i2c_start();
 i2c_write(0x40); // slave address + write
 i2c_write(rcvData|0x0F); // Write lower nibble to HIGH and higher nibble to rcvData
 i2c_stop();

 i2c_start();
 i2c_write(0x41); // slave address + read
 rcvData=i2c_read(1); // Read the input of lower nibble
 i2c_stop();

 rcvData <<= 4; //Shift the inputs right to the output
 rcvData = ~rcvData;

 delayMicroseconds(100);
 }
 return 0;
}
