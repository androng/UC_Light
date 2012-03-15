/*
    Menu system for transmitting ASCII strings to different I2C slaves. 
    Must have "newline" selected in serial monitor. 
*/
#include <Wire.h>

const int NUM_SLAVES = 2;
/* Stores the address of each slave. The maximum number of slaves is 2^7 = 128 */
const byte I2C_SLAVES[NUM_SLAVES] = {1,2};
/* 
    States:
    0: Enter slave address
    1: Enter slave sub address
    2: Enter string to send, terminated by newline
*/
const int STATE_ENTER_SLAVE_ADDRESS = 0;
const int STATE_ENTER_SLAVE_SUB_ADDRESS = 1;
const int STATE_ENTER_DATA = 2;

void setup()
{
    Wire.begin(); // join i2c bus (address optional for master)
    Serial.begin(9600);
    Serial.println("Master writer");                    
}
void loop()
{
    boolean outputToUser = true;
    int menuState = 0;
    int slaveAddressForTx = -1;
    
    while(true){
        /* Output system */
        if(outputToUser){
            switch(menuState){
                case STATE_ENTER_SLAVE_ADDRESS:
                    Serial.print("Enter slave address (0-127): ");
                    break;
                case STATE_ENTER_DATA:
                    Serial.print("Enter string followed by ENTER: ");
                    break;
            }
            outputToUser = false;
        }
        
        /* Input system */
        /* Read until buffer is empty */
        while(Serial.available() > 0) {
            switch(menuState){
            case STATE_ENTER_SLAVE_ADDRESS:{
                /* Input slave address to trasmit to */
                int slaveAddress = readSerialNumber();
                Serial.println(slaveAddress);
                
                /* If invalid input, output to user that it was invalid and output prompt again. */
                if(slaveAddress < 0 || slaveAddress > 127){
                    Serial.println("ERROR: Invalid address. (range 0-127)");
                } 
                /* else it was valid. Store address and change state. */
                else {
                    slaveAddressForTx = slaveAddress;
                    menuState = STATE_ENTER_DATA;
                }
                break;
            }
            case STATE_ENTER_DATA:{
                /* Input data 200 bytes at a time or until newline to store in buffer. */
                const int BUF_LEN = 200;
                char buffer[BUF_LEN] = {0};
                int i = 0;
                do{
                    buffer[i] = Serial.read();
//                    int n = buffer[i];
//                    Serial.println(buffer[i], HEX);
                    if(buffer[i] == '\n'){
                        break;
                    }
                    /* If valid data read, increment index */
                    if(buffer[i] != -1){
                        i++;
                    }
                } while(i < BUF_LEN);
                Serial.write(buffer);
                
                Serial.println("Transmitting..");
                /* Transmit message over I2C */
                Wire.beginTransmission(slaveAddressForTx); 
                Wire.write(buffer);
                Wire.endTransmission();
                
                /* Change state */
                menuState = STATE_ENTER_SLAVE_ADDRESS;
                break;
            }
            }
            outputToUser = true;
        }
    }
  
//    Wire.beginTransmission(I2C_SLAVES[0]); // transmit to device 
//    Wire.write("x is ");        // sends five bytes
//    Wire.write(x);              // sends one byte  
//    Wire.endTransmission();    // stop transmitting
//  
//    x += 2;
//    
//    Wire.beginTransmission(I2C_SLAVES[1]); // transmit to device 
//    Wire.write("y is ");        // sends five bytes
//    Wire.write(y);              // sends one byte  
//    Wire.endTransmission();    // stop transmitting
//  
//    y += 2;
}
/* Reads in numbers from serial until a newline character and returns the number. */
int readSerialNumber(){
    int num = 0;
    int incomingByte = 0;
    
    if(Serial.available() == 0){
        return num;
    }
    do{
        /* Read the incoming byte: */
        incomingByte = Serial.read();
//        Serial.print("Reading digit ");
//        Serial.println(incomingByte, HEX);
        /* Convert ASCII to real number */
        int convertedDigit = incomingByte - 48;
        /* Check if read byte a digit from 0-9  */
        if(convertedDigit >= 0 && convertedDigit <= 9){
//            Serial.print("Adding digit ");
//            Serial.println(convertedDigit);
            /* Multiply the value by 10 to make room in ones place */
            num *= 10;
            /* Store the non-ASCII version of the digit in ones place */
            num += convertedDigit;
        }
    } while (incomingByte != 10);
    
    return num;
}
