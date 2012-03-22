/*
    Takes four bytes and outputs them on UART0, 1, 2, 3. 
    Outputs these bytes forever.  
*/

const byte BYTE0 = 0x00;
const byte BYTE1 = 0x01;
const byte BYTE2 = 0x02;
const byte BYTE3 = 0x03;

void setup(){
    Serial.begin(115200);
    Serial1.begin(115200);
    Serial2.begin(115200);
    Serial3.begin(115200);
}
void loop(){
    Serial.write(BYTE0);
    Serial1.write(BYTE1);
    Serial2.write(BYTE2);
    Serial3.write(BYTE3);
    delay(100);
}
