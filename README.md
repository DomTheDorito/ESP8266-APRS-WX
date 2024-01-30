# ESP8266-APRS-WX
This is an offshoot from my main project using an Arduino, ESP-01S, and a handful of sensors.

The ESP8266 code is modifed from https://lastminuteengineers.com/bme280-esp8266-weather-station/ , 
and formats the sensors into a JSON string that's displayed on the 404 page. Yes this is quick and 
dirty code, but it works until I can add other pages to the ESP code.

The C++ file does the same thing as it does in my Arduino based project; cURLs the JSON string,
formats it into an APRS packet, and generates a wxnow.txt.

Why didn't I just format the APRS string on the ESP? Outputting JSON provides more flexability 
for integrations.
