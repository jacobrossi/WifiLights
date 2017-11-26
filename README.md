# WifiLights
Web app and companion Arduino firmware for controlling addressable LED strips.

<img src="https://github.com/jacobrossi/WifiLights/raw/master/public/images/screenshot.jpg" width="300" alt="Screenshot of WifiLights app">

## Warning
Use of this project is at your own risk. Your safety is your own responsibility, including proper use of equipment and safety gear, and determining whether you have adequate skill and experience.

## Software setup
1. npm install
2. Edit WifiLightsConfig.h with your setup (SSID, Password, Server Hostname, LED protocol, etc.)
3. Install the Arduino IDE (recommended): https://www.arduino.cc/en/Main/Software
4. In the Arduino IDE, configure:
* File > Preferences -- add the ESP8266 board manager URL: http://arduino.esp8266.com/stable/package_esp8266com_index.json
* Sketch > Include Library > Manage Libraries -- install the latest FastLED and ArduinoJSON libraries

## Hardware setup

__Note:  Powering an LED strip with less than 10A is not recommended. If you're using more than couple hundred LEDs, you'll need even more power. Projects of this scale are potentially very dangerous without proper parts and experience in high-power, low-voltage systems. Always do the math. Always provide adequate power. Always take measurements to confirm operating values are as expected.__

### Example Parts List
Here's some suggested parts to get you started: http://a.co/6A6ZOjq

### Circuit

Tips for better results:
* Connecting a 1000uF capacitor between 5V and GND is recommended and can protect against power fluctuations
* Connecting a 300-500 Ohm resistor between your microcontroller's data pin and the data input pin of the strip is recommended to prevent voltage spikes from damaging your LED strips
* The microcontroller recommended in this project (ESP8266) operates at 3.3V while most LED strips operate at either 5V or 12V. You'll need to use a logic level shifter to step up the data voltage the the correct value for your strip.

<img src="https://github.com/jacobrossi/WifiLights/raw/master/Schematic.png" alt="Example Wiring Schematic">