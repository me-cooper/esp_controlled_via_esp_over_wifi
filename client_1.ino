#include <ESP8266WiFi.h>

#include <ESP8266HTTPClient.h>
HTTPClient sender;

//Wlan-Daten eures Netzwerks eintragen
const char* ssid = "WLAN_SSID";
const char* password = "WLAN_PASSWD";


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
}

void loop() {
  
  if (WiFi.status() == WL_CONNECTED) {
 
    //Status des Relais vom Server abfragen
    sender.begin("http://192.168.178.56/state");
    sender.GET();
    
    //Inhalt der Webseite http://192.168.178.56/state wird in die Variable returned_data geschrieben                                                                 
    String returned_data = sender.getString();

    //Ausgabe nur zu debuggin-Zwecke im seriellen Monitor
    Serial.println(returned_data);

    //Wenn Status = 1 ist, dann Relais über den Link ausschalten
    if(returned_data == "1"){
      sender.begin("http://192.168.178.56/off"); 
      sender.GET();   
    }

    //Wenn Status = 0 ist, dann Relais über den Link anschalten
    if(returned_data == "0"){
      sender.begin("http://192.168.178.56/on");
      sender.GET(); 
    }


    //Der Anfrage 1000ms Zeit geben, bevor die Verbindung zum Server wieder beendet wird
    delay(1000);
    sender.end();
    
    
  }  

  //Nach dem Befehl 5 Sekunden warten bevor der nächste ausfegührt wird
  delay(5000);
       
}
