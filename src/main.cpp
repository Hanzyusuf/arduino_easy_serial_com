// working example for the library 'EasySerialCom'

#include <Arduino.h>
#include <EasySerialCom.h>
#include <SoftwareSerial.h>

// function declaration for the arduino build environment to create prototypes of these functions.
// prototypes of the main functions (setup and loop) are already created by the arduino build environment.
void onCommandReceived(Stream& serialStream, char* command, EasySerialCom::Error error);
void writeToSerial(Stream& serialStream, char* msg);

// max serial command length
const byte maxDataLength = 64;

// in this case, this is optional since 'Serial' is already an object.
HardwareSerial& mySerialUSB = Serial;

// in this case, this is necessary since we are creating an actual object and not referencing an already created one.
SoftwareSerial mySerialBLE(8, 9); // board RX & ble TX = 8, board TX & ble RX = 9

// create 'EasySerialCom' objects for both serial devices
EasySerialCom easySerialCom_USB(mySerialUSB, onCommandReceived, maxDataLength);
EasySerialCom easySerialCom_BLE(mySerialBLE, onCommandReceived, maxDataLength);

// a green led connected on pin number 2
const byte ledGreen = 2;

// set to false when the current command has not been parsed and processed
// the further incoming data will be stored in the arduino buffer and not be read until set to true
bool bListenForNewCommands = true;

void setup() {

  // (OPTIONAL) set a different start and end tags for BLE (can be same too)
  easySerialCom_BLE.setStartTag('{');
  easySerialCom_BLE.setEndTag('}');
  // - mySerialBLE will trigger onCommandReceived() function when received data is enclosed within ‘{’ and ‘}’ tags, as these have been changed.
  // - mySerialUSB will trigger onCommandReceived() function when received data is enclosed within '<' and '>' tags, as these are the default tags.

  // set baud rate on all serial objects
  mySerialUSB.begin(9600);
  mySerialBLE.begin(9600);

  // set pin mode
  pinMode(ledGreen, OUTPUT);

}

void loop() {

  // IMPORTANT - call loop on all EasySerialCom objects, else it won't work
  // you can also omit the boolean check below if you don't mind receiving more commands before the current one has finished processing.
  if(bListenForNewCommands) {
    easySerialCom_USB.loop();
    easySerialCom_BLE.loop();
  }

}

void onCommandReceived(Stream& serialStream, char* command, EasySerialCom::Error error) {
  
  bListenForNewCommands = false;

  char* responseChar = "<Response:Success>";

  // if no error in data transmission
  if(error == EasySerialCom::Error::None) {

    // -- handle commands specific to usb serial
    if(&serialStream == &mySerialUSB) {

    }

    // -- handle commands specific to ble
    if(&serialStream == &mySerialBLE) {

    }

    // -- handle commands irrespective of which serial com it came from

    if(strcmp(command, "TurnLedOn:Green") == 0) {
      digitalWrite(ledGreen, HIGH);
    }
    else if(strcmp(command, "TurnLedOff:Green") == 0) {
      digitalWrite(ledGreen, LOW);
    }
    else {
      responseChar = "<Response:Fail>";
      writeToSerial(serialStream, "<Error:unrecognized command>");
    }

  }
  // else data transmission error occurred
  else {
    
    responseChar = "<Response:Fail>";

    // if data was too long (and is most likely corrupted?)
    if(error == EasySerialCom::Error::DataExceededMaxBuffer) {
      writeToSerial(serialStream, "Error:command exceeded max data length, data may be corrupt!");
    }

  }

  // send response (success or fail)
  writeToSerial(serialStream, responseChar);

  bListenForNewCommands = true;

}

void writeToSerial(Stream& t_serial, char* msg) {

  t_serial.write(msg);
  t_serial.write("\n");

}