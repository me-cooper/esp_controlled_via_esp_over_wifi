#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>

ESP8266WebServer server(80);
HTTPClient http;

const char* ssid = "WLAN_SSID";
const char* password = "WLAN_PASSWD";

// following for the name of: http://name_esp.local/ 
const char* dns_name = "sender";

void setup()
{
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

  server.begin();
  Serial.println("Webserver gestartet.");
}

void loop() {
  
  server.handleClient();
  
  if (WiFi.status() == WL_CONNECTED) {
 
    //Status des Relais vom Server abfragen
    http.begin("http://192.168.178.67/get_data?wasserventil");
    int http_code = http.GET();                                                                 
    String returned_data = http.getString();

    Serial.println(returned_data);
    Serial.println(http_code);
    
    if(returned_data == "1"){
      http.begin("http://192.168.178.67/set_data?wasserventil=off"); 
      int http_code = http.GET();   
    }

    if(returned_data == "0"){
      http.begin("http://192.168.178.67/set_data?wasserventil=on");
      int http_code = http.GET(); 
    }

    //http.begin("http://192.168.178.67/set_data?wasserventil=on"); 

    //Der Anfrage 500ms Zeit geben, bevor die Verbindung zum Server wieder beendet wird
    delay(500);
     
    http.end();
    
    
  }  

  //Nach dem Befehl 5 Sekunden warten bevor der nächste ausfegührt wird
  delay(5000);
       
}
