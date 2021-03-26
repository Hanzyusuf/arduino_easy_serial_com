# Easy Serial Com (Library)

*Easy Serial Com* is a library for Arduino made to quickly get started with serial communication, and without getting the main code dirty by a bunch of incoming data parsing algorithm.

Works with both *HardwareSerial* and *SoftwareSerial* objects.

## Installation

### Steps:

1. Clone the project (or download and extract).

2. Copy **EasySerialCom.h** and **EasySerialCom.cpp** to the directory where your **main.cpp** is located.

4. Done!

## Usage

### Steps:

Perform the following steps in your main.cpp file:

1. Add an include:
  ```#include  <EasySerialCom.h>```


2. Create a function with these exact params (name anything you want):
    ```
	void  onCommandReceived(Stream& serialStream, char*  command, EasySerialCom::Error  error) {
	    // see working example...
	}
    ```


3. *OPTIONAL* - This step is recommended as it will allow you to switch the variable type between HardwareSerial and SoftwareSerial without having to change anything else in the whole code.
Create a variable referencing to the HardwareSerial or SoftwareSerial object (for eg: Arduino Uno uses RX pin 0 and TX pin 1 for it's only HardwareSerial which can be directly referred to by *Serial*):

	For HardwareSerial:
    ```
	// replace Serial with whichever HardwareSerial you want to use from your board.
	HardwareSerial& mySerial = Serial;
    ```
	  For SoftwareSerial:
    ```
	// replace 8,9 with whichever pins you want use from your board for SoftwareSerial.
	SoftwareSerial  mySerial(8, 9);
    ```


4. Create an EasySerialCom object:
```EasySerialCom  easySerialCom(mySerial, onCommandReceived, maxDataLength);```
	- replace *maxDataLength* with the length of the longest command which you will receive! (64 or 128 is usually sufficient)

5. Due to a specific limitation of some microcontrollers (including Arduino Uno), and to keep the library more versatile, we will have to set the baud rate in main.cpp itself:
    ```
    void setup() {
        mySerial.begin(9600);
    }
    ```

7. In loop() function, add:
    ```
    void loop() {
        easySerialCom.loop();
    }
    ```

8. That's it, now you will receive any data sent enclosed within the start and end tags in your *`onCommandReceived`* function!

### Working Example:

A working example is provided in [main.cpp](https://github.com/Hanzyusuf/arduino_easy_serial_com/blob/master/src/main.cpp).

#### Additional Notes:
- Default start and end tags are *'<' and '>'* respectively.
- Do not use the characters in between data which have been set as the start and end tags for that EasySerialCom object! If needed, [change the start and end tags](https://github.com/Hanzyusuf/arduino_easy_serial_com/blob/3be32be43df4a3c1134d2ce2ae9de4fd7b710593/src/main.cpp#L34-L38) accordingly.
- The **command** param in **onCommandReceived** function will not include the start and end tags.

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## License
[MIT](https://choosealicense.com/licenses/mit/)