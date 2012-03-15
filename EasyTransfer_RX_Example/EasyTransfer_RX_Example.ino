#include <EasyTransfer.h>

//create object
EasyTransfer ET; 

/* Length of "Data" segment of EasyTransfer struct. Cannot be greater than 255. */
const int DATA_PACKET_SIZE = 200; 

struct RECEIVE_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
    char data[DATA_PACKET_SIZE + 1];
};

//give a name to the group of data
RECEIVE_DATA_STRUCTURE mydata;

void setup(){
    Serial.begin(9600);
    Serial1.begin(115200);
    //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc. 
    ET.begin(details(mydata), &Serial1);
    
    pinMode(13, OUTPUT);
  
    Serial.println("EasyTransfer_Receiver");
}

void loop(){
    //check and see if a data packet has come in. 
    if(ET.receiveData()){
        /* this is how you access the variables. [name of the group].[variable name] */
        /* since we have data, we will blink it out.  */
//        for(int i = mydata.blinks; i>0; i--){
//            digitalWrite(13, HIGH);
//            delay(mydata.pause * 100);
//            digitalWrite(13, LOW);
//            delay(mydata.pause * 100);
//        }
        /* Output received data on serial port */
//        Serial.print("Blinks: ");
//        Serial.print(mydata.blinks);
//        Serial.print(", Pause: ");
//        Serial.println(mydata.pause);
        Serial.write(mydata.data);
    }
    
    
    /* you should make this delay shorter then your transmit delay or else messages could be lost */
//    delay(250);
}
