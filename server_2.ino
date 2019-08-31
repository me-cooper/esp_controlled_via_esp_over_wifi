#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

ESP8266WebServer server(80);

const char* ssid = "WLAN_SSID";
const char* password = "WLAN_PASSWD";

// following for the name of: http://wasserventil.local/ 
const char* dns_name = "wasserventil";

int wasserventil = 5; //PIN D1
int data;

void setup()
{

  pinMode(wasserventil, OUTPUT);
  Serial.begin(115200);
  Serial.println("ESP Gestartet");

  WiFi.begin(ssid, password);

  Serial.print("Verbindung wird hergestellt ...");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Verbunden! IP-Adresse: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin(dns_name)) {
    Serial.println("DNS gestartet, erreichbar unter: ");
    Serial.println("http://" + String(dns_name) + ".local/");
  }

  server.onNotFound([](){
    server.send(404, "text/plain", "Link wurde nicht gefunden!");  
  });

  server.on("/", []() {
    server.send(200, "text/plain", "ESP-Startseite!");
  });

  server.on("/set_data", [](){

    set_Data();
    
  });

  server.on("/get_data", [](){

    get_Data();
    
  });

  server.begin();
  Serial.println("Webserver gestartet.");
}

void loop() {
  server.handleClient();  
}




//Funktion um die Variablen vom Sender / Client auszulesen
void set_Data() {

  String message;
  int number_of_variables = 0;
  
  //Variablen zählen
  number_of_variables = server.args();
  
  //Ausgabe im Webbrowser
    server.send(200, "text/plain","Anzahl der uebergebenen Variablen: " + String(number_of_variables));
  //Gleiche Ausgabe im seriellen Monitor
    Serial.println("Anzahl der uebergebenen Variablen:" + String(number_of_variables));  
  

  //Schleife um alle übergebenen Variablen nach der Reihe durchzugehen
  for (int i = 0; i < number_of_variables; i++) {
  
      // server.argName(i) = Name der Variable
      // server.arg(i) = Inhalt der Variable 


      if(server.argName(i) == "wasserventil"){
        
        // 192.168.178.67/set_data?wasserventil=on
        if(server.arg(i) == "on"){
         Serial.println("Wasserventil: 1");
         digitalWrite(wasserventil, 1);
        }

        // 192.168.178.67/set_data?wasserventil=off
        if(server.arg(i) == "off"){
         Serial.println("Wasserventil: 0");
         digitalWrite(wasserventil, 0);
        }
        
      }
      
  
  } 


}


void get_Data(){
  int number_of_variables = 0;
  number_of_variables = server.args();
  
  for (int i = 0; i < number_of_variables; i++) {
    

    if(server.argName(i) == "wasserventil"){
        data = digitalRead(wasserventil);
        server.send(200, "text/plain", String(data));
    }


    
  }
}
