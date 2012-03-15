/*
    Receives data over the I2C bus, puts it in a buffer, and keeps sending that buffer out over
    a serial port with EasyTransfer.
*/

#include <Wire.h>
#include <EasyTransfer.h>

const byte I2C_ADDRESS = 1;
/* Length of "Data" segment of EasyTransfer struct. Cannot be greater than 255. */
const int DATA_PACKET_SIZE = 200; 

char i2cBuffer[DATA_PACKET_SIZE + 1] = {0};
int i2cBufferIndex = 0;
EasyTransfer ET; 

struct SEND_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to send
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
    
    char data[DATA_PACKET_SIZE + 1]; /* +1 for the \0 */
};
SEND_DATA_STRUCTURE mydata;

void setup()
{
    Wire.begin(I2C_ADDRESS);                // join i2c bus with address
    Wire.onReceive(receiveEvent); // register event
    Serial.begin(9600);           // start serial for output
    Serial.print("Slave receiver and EasyTransfer Sender: I2C address ");
    Serial.println(I2C_ADDRESS);
    
    Serial1.begin(115200);
    ET.begin(details(mydata), &Serial1);
}

void loop()
{
    /* Keep sending out data until new data comes out over the I2C bus. */
    Serial.print("Sending: ");
    Serial.write(mydata.data);
    ET.sendData();
    delay(100);
}

void receiveEvent(int howMany)
{
    while(Wire.available())
    {
        i2cBuffer[i2cBufferIndex] = Wire.read(); // receive byte as a character
//        Serial.write(c);         // print the character
        /* If end-of-line character, copy i2cBuffer to Serial buffer. */
        if(i2cBuffer[i2cBufferIndex] == '\n'){
            i2cBuffer[i2cBufferIndex + 1] = '\0';
//            Serial.print("Received: ");
//            Serial.write(i2cBuffer);
    
            for(int i = 0; i <= i2cBufferIndex + 1; i++){
                mydata.data[i] = i2cBuffer[i];
            }
            i2cBufferIndex = 0;
            break;
        }
        i2cBufferIndex++;
    }

}
