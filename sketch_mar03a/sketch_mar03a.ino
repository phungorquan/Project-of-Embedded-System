#include <SPI.h>
//SRAM opcodes
#define RDSR 5
#define WRSR 1
#define READ 3
#define WRITE 2
 
#define CSPIN 10
 
uint8_t Spi23LC1024Read8(uint32_t address)
{
    uint8_t read_byte;
    digitalWrite(CSPIN, LOW); //chọn ic này
    SPI.transfer(READ);
    SPI.transfer((uint8_t)(address >> 16) & 0xff);
    SPI.transfer((uint8_t)(address >> 8) & 0xff);
    SPI.transfer((uint8_t)address);
    read_byte = SPI.transfer(0x00);
    digitalWrite(CSPIN, HIGH);
 
    return read_byte;
}
 
void Spi23LC1024Write8(uint32_t address, uint8_t data_byte)
{
    digitalWrite(CSPIN, LOW); // chọn ic này
    SPI.transfer(WRITE);
    SPI.transfer((uint8_t)(address >> 16) & 0xff);
    SPI.transfer((uint8_t)(address >> 8) & 0xff);
    SPI.transfer((uint8_t)address);
    SPI.transfer(data_byte);
    digitalWrite(CSPIN, HIGH);
}

template <class T> int RAM_WRITE(uint32_t adress, const T& value)
{
    const byte* p = (const byte*)(const void*)&value;//truy xuat gia tri tu dia chi ao cua value
    int i;
    for (i = 0; i < sizeof(value); i++)
        Spi23LC1024Write8(adress + i, *p++);
    return i;
}
 
template <class T> int RAM_READ(uint32_t adress, T& value)
{
    byte* p = (byte*)(void*)&value;
    int i;
    for (i = 0; i < sizeof(value); i++)
        *p++ = Spi23LC1024Read8(adress + i);
    return i;
}
 
void setup()
{
    Serial.begin(9600);
    SPI.begin();
    //thiết lập pin CS
    pinMode(CSPIN, OUTPUT);
    digitalWrite(CSPIN, HIGH);
}
int INT = -1233, i = 0;
float FLOAT = 134.344, f = 0;
double DOUBLE = 123.966, d = 0;
long LONG = 123343545, l = 0;
byte BYTE = 93, b = 0;
char CHAR = 'g', c = 0;
char chuoigui[] = "Tuan Vo badass", chuoinhan[100];
void loop()
{
    delay(1000);
 
    /*//ghi
    RAM_WRITE(0, INT); //2 byte (ghi 2 ô :0,1)
    RAM_WRITE(2, FLOAT); //4 byte (ghi 4 ô: 2,3,4,5)
    RAM_WRITE(6, DOUBLE); //4 byte (ghi 4 ô: 6,7,8,9)
    RAM_WRITE(10, LONG); //4 byte(ghi 4 ô: 10 -> 13)
    RAM_WRITE(14, BYTE); //1 byte(ghi 1 ô:14)
    RAM_WRITE(15, CHAR); //1 byte(ghi 4 ô: 15)
    //đọc
    RAM_READ(0, i);
    RAM_READ(2, f);
    RAM_READ(6, d);
    RAM_READ(10, l);
    RAM_READ(14, b);
    RAM_READ(15, c);
    //in
 
    Serial.println(i)
    Serial.println(f, 5);
    Serial.println(d, 5);
    Serial.println(l);
    Serial.println(b);
    Serial.println(c);
    Serial.println("_____");*/
    //RAM_WRITE(0,chuoigui);
    RAM_READ(0,chuoinhan);
    Serial.println(chuoinhan);
    Serial.println("_____");
}
