#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

HTTPClient sender;

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
  
  //Wenn eine Verbindung mit dem oben definierten Netzwerk besteht
  if (WiFi.status() == WL_CONNECTED) {
 
    //Status des Relais vom Server abfragen
    sender.begin("http://192.168.178.67/get_data?wasserventil");
    sender.GET();                                                                 
    String returned_data = sender.getString();

    Serial.println(returned_data);
    
    if(returned_data == "1"){
      sender.begin("http://192.168.178.67/set_data?wasserventil=off"); 
      sender.GET();   
    }

    if(returned_data == "0"){
      sender.begin("http://192.168.178.67/set_data?wasserventil=on");
      sender.GET(); 
    }

    //http.begin("http://192.168.178.67/set_data?wasserventil=on"); 

    //Der Anfrage 500ms Zeit geben, bevor die Verbindung zum Server wieder beendet wird
    delay(500);
     
    sender.end();
    
    
  }  

  //Nach dem Befehl 5 Sekunden warten bevor der nächste ausfegührt wird
  delay(5000);
       
}
