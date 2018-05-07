# React Protocol Buffers MQTT Potentiometer
Using React.js to control the brightness of the ESP8266 onboard led,
in this case, with Google Protocol Buffers over MQTT.

![Browser Slider](/screenshot.png?raw=true)

Also see [React JSON MQTT Potentiometer](https://github.com/RyleeHarrison/react-json-mqtt-esp8266)

## Hardwere
- WIFI Development Board [ESP8266 ESP-12E](https://www.banggood.com/Geekcreit-Doit-NodeMcu-Lua-ESP8266-ESP-12E-WIFI-Development-Board-p-985891.html?rmmds=search&cur_warehouse=USA)

## Documentation

- React documentation [Here](https://reactjs.org/docs).
- Google Protocol Buffers JS [Here](https://developers.google.com/protocol-buffers/docs/reference/javascript-generated)
- Nanopb - Protocol Buffers for Embedded Systems [Here](https://jpa.kapsi.fi/nanopb/docs/)
- PubSubClient - Arduino Client for MQTT[Here](https://pubsubclient.knolleary.net/api.html).

##  Installation
npm packages
```
$ cd react_app
$ npm install
$ npm start
```
Open the arduino sketch ([Arduino IDE](https://www.arduino.cc/en/Main/Software))

## Arduino libraries
You will need to install the following Arduino libraries.
*  [Nanopb](https://jpa.kapsi.fi/nanopb/download/)
*  [PubSubClient](https://github.com/knolleary/pubsubclient)
