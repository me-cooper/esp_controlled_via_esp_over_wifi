# esp_controlled_via_esp_oder_wifi
Two very simple ways to control a esp wirelessly with an esp over wifi. A simple way to switch a relay or even send sensor values wirelessly. 
No other hardware needed.

Example 1:

Very basic. You can control a relay which is connected to an esp8266 with a button switch wich is connected to another esp wirelessly.

Server-ESP: 
Relay connected to the ESP. The relay can be controlled over very basic weblinks.
http://ip-adress/relay_on - Turns the relay on
http://ip-adress/relay_off - Turns the relay off
http://ip-adress/state - Returns the current state of the relay.

Client-ESP: 
A Button is connected to the ESP. If the button is pressed, the client ESP gets the current state of the relay from http://ip-adress/state.
If the state is 0 (false), the client esp will call http://ip-adress/relay_on, and if the status is 1 (true) the client esp will call http://ip-adress/relay_off.

