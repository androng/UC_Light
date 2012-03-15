#include <EasyTransfer.h>

const char MESSAGE[] = "Arduino is a popular open-source single-board microcontroller, descendant of the open-source Wiring platform,designed to make the process of using electronics in multidisciplinary projects more accessible. The hardware consists of a simple open hardware design for the Arduino board with an Atmel AVR processor and on-board input/output support. The software consists of a standard programming language compiler and the boot loader that runs on the board.\nArduino hardware is programmed using a Wiring-based language (syntax and libraries), similar to C++ with some simplifications and modifications, and a Processing-based integrated development environment.\nCurrent versions can be purchased pre-assembled; hardware design information is available for those who would like to assemble an Arduino by hand. Additionally, variations of the Italian-made Arduino-with varying levels of compatibility-have been released by third parties; some of them are programmed using the Arduino software.\nThe Arduino project received an honorary mention in the Digital Communities category at the 2006 Prix Ars Electronica.\n-----------\n";
const int MESSAGE_LENGTH = 1125;
const int DATA_PACKET_SIZE = 200; /* Length of "Data" segment of EasyTransfer struct. */

//create object
EasyTransfer ET; 

struct SEND_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to send
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
    int blinks;
    int pause;
    char data[DATA_PACKET_SIZE];
};

//give a name to the group of data
SEND_DATA_STRUCTURE mydata;

void setup(){
    Serial.begin(9600);
    Serial1.begin(115200);
    //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc.
    ET.begin(details(mydata), &Serial1);
    
    pinMode(13, OUTPUT);
    
    randomSeed(analogRead(0));
    Serial.println("EasyTransfer_Sender");
}

void loop(){
    for(int messageIndex = 0; messageIndex < MESSAGE_LENGTH; messageIndex += DATA_PACKET_SIZE){ 
        /* Load new data */
        mydata.blinks = random(5);
        mydata.pause = random(5);
        /* Copy the next part of the paragraph into the data struct to be sent. */
        for(int b = 0; b < DATA_PACKET_SIZE; b++){
            /* Special case where we reached the end of the message */
            if(messageIndex + b >= MESSAGE_LENGTH){
                break;
            }
            
            mydata.data[b] = MESSAGE[messageIndex + b];
        }
        /* send the data */
        ET.sendData();
        
        delay(1000);
    }
    
    /* Just for fun, we will blink it out too */
//    for(int i = mydata.blinks; i>0; i--){
//        digitalWrite(13, HIGH);
//        delay(mydata.pause * 100);
//        digitalWrite(13, LOW);
//        delay(mydata.pause * 100);
//    }
     
    
    delay(5000);
}
