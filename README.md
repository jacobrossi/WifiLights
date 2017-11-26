# WifiLights
Web app and companion Arduino firmware for controlling addressable RGB LED strips. It was designed with Christmas lights in mind, but you can use it for all kinds of fun projects.

<img src="https://github.com/jacobrossi/WifiLights/raw/master/public/images/screenshot.jpg" width="300" alt="Screenshot of WifiLights app">

## Warning
Use of this project is at your own risk. Your safety is your own responsibility, including proper use of equipment and safety gear, and determining whether you have adequate skill and experience.

## Hardware setup

:zap: __Important__ :zap:  Powering an LED strip with less than 10A is not recommended. Unless you just have a handful of LED pixels, you cannot power it directly from a microcontroller. If you're using more than couple hundred LEDs, you will likely need more advanced power supply than the one in the exampleparts list. __Projects of this scale are potentially very dangerous without proper parts and experience in high-power, low-voltage systems.__ Always do the math. Always provide adequate power. Always take measurements to confirm operating values are as expected.

### Example Parts List
Here's some suggested parts to get you started: http://a.co/6A6ZOjq

### Example Circuit

Tips for better results:
* Connecting a 1000uF capacitor between 5V and GND is recommended and can protect against power fluctuations
* Connecting a 300-500 Ohm resistor between your microcontroller's data pin and the data input pin of the strip is recommended to prevent voltage spikes from damaging your LED strips
* The microcontroller recommended in this project (ESP8266) operates at 3.3V while most LED strips operate at either 5V or 12V. You'll need to use a logic level shifter to step up the data voltage the the correct value for your strip.

<img src="https://github.com/jacobrossi/WifiLights/raw/master/Schematic.png" alt="Example Wiring Schematic">

## Software setup

### Web app
1. npm install
2. npm start

This starts the web app. By default, it starts on port 3000 (e.g. http://localhost:3000). Setting a PORT env variable can override this.

### ESP8266 Firmware

:zap: __Important!__ :zap: Always ensure the power supply is connected to the circuit before plugging in the FTDI cable. The FTDI cable will supply power from your USB port - but not enough to power your LEDs. __Plugging in the FTDI cable without the power supply connected could overload your USB port and damage it.__

1. Install the Arduino IDE (recommended): https://www.arduino.cc/en/Main/Software
2. Edit WifiLightsConfig.h with your setup (SSID, Password, Server Hostname, LED protocol, etc.)
3. In the Arduino IDE, configure:
* File > Preferences -- add the ESP8266 board manager URL: http://arduino.esp8266.com/stable/package_esp8266com_index.json
* Tools > Board > Board Manager -- install the ESP8266 package
* Tools > Board -- select Adafruit HUZZAH ESP8266
* Sketch > Include Library > Manage Libraries -- install the latest FastLED and ArduinoJSON libraries
4. Attach the FTDI programmer cable (be sure to correctly align the GND pin)
5. Hold down the GPIO0 button on the ESP8266, then press the RESET button, then release GPIO0 (the red LED should be dimly lit)
5. Click Upload

### Debugging
You can get basic debug info from the ESP8266 by connecting the FTDI cable (ensure the GND pin is correctly aligned) and opening the Serial Monitor in the Arduino IDE. Set the baud rate to 115200. The firmware will output basic logs about connecting to your WiFi network, polling the node app's API, and parsing the results of the node API.
