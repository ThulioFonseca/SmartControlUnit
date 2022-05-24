#include <Arduino.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <ESP8266WiFi.h>          
#include <DNSServer.h>
#include <LittleFS.h>          
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer servidor(80);
const char* ssid     = "Oi velox";            // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "Oi130987";            // The password of the Wi-Fi network
int count = 0;
String CaixaDeSomStatus = "Desligado";
String FitaLedStatus = "Desligado";
String LedPcStatus = "Desligado";



void Spinner(int counter)                         //Load Console Spinner
    {
        switch (counter % 4)
        {
            case 0: Serial.print("/"); break;
            case 1:  Serial.print("-"); break;
            case 2:  Serial.print("\\"); break;
            case 3:  Serial.print("|"); break;
        }
        delay(150);
        Serial.print("\b \b");
    }

void NetConfig(){                                 //Configure Wifi Network

  Serial.println("\n");
  Serial.println("=====================[ Advanced Control Unit ]=====================");
  Serial.println("\n");                            
  WiFi.begin(ssid, password);                     // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.print(" ");

  while (WiFi.status() != WL_CONNECTED) {         // Wait for the Wi-Fi to connect
    Spinner(count);
    count++;
   }
  Serial.print("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b                      ");
  Serial.println("Connection established!");  
  Serial.println('\n');
  Serial.print("IP address..............: ");
  Serial.println(WiFi.localIP());                 

}

void ServerConfig(){                              //Configure Web Server

  servidor.serveStatic("/home", LittleFS, "/");
  LittleFS.begin();                                              // Inicializa o sistema de arquivos
  Serial.println("LittleFS................: Running");
  servidor.begin();                                              // Inicializa o servidor web
  Serial.println("WEB-Server..............: Running");
  Serial.println('\n');
  
}

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  
  NetConfig();
  ServerConfig();

  Serial.println("System Log:");
  Serial.println("\n");
  
  servidor.on("/home/",HTTP_POST, [&]( AsyncWebServerRequest *request){

      Serial.println("-> POST Request on /home/ - " + request->params());    //System Log

      String CaixaDeSomCMD;
      String FitaLedCMD;
      String LedPcCMD;

       if(request->hasArg("CaixaDeSom")){

          CaixaDeSomCMD = request->arg("CaixaDeSom");

          if(CaixaDeSomCMD == "1")
          {
            CaixaDeSomStatus = "Ligado";
            digitalWrite(LED_BUILTIN, LOW);
            request->send(200, "text/plain", "Caixa de Som - Ligada");

          }else if(CaixaDeSomCMD = "0")
          {
            CaixaDeSomStatus = "Desligado";
            digitalWrite(LED_BUILTIN, HIGH);
            request->send(200, "text/plain", "Caixa de Som - Desligada");
          }         
         else {
            request->send(406, "text/plain", "Argumento invalido");
          }

        }
        if(request->hasArg("FitaLed")){

          FitaLedCMD = request->arg("FitaLed");

          if(FitaLedCMD == "1")
          {
            FitaLedStatus = "Ligado";
            request->send(200, "text/plain", "Fita Led - Ligada");

          }else if(FitaLedCMD = "0")
          {
            FitaLedStatus = "Desligado";
            request->send(200, "text/plain", "Fita Led - Desligada");
          }         
         else {
            request->send(406, "text/plain", "Argumento invalido");
          }

        }
        if(request->hasArg("LedPc")){

          LedPcCMD = request->arg("LedPc");

          if(LedPcCMD == "1")
          {
            LedPcStatus = "Ligado";
            request->send(200, "text/plain", "Led PC - Ligada");

          }else if(LedPcCMD = "0")
          {
            LedPcStatus = "Desligado";
            request->send(200, "text/plain", "Led PC - Desligada");
          }         
         else {
            request->send(406, "text/plain", "Argumento invalido");
          }

        }
  });

    servidor.on("/home/", HTTP_GET, [&](AsyncWebServerRequest *request){

    
        AsyncResponseStream *response = request->beginResponseStream("application/json");
        DynamicJsonDocument doc(1024);    
        JsonObject obj = doc.to<JsonObject>();   
        obj[String("CaixaDeSomStatus")] = CaixaDeSomStatus;
        obj[String("FitaLedStatus")] = FitaLedStatus;
        obj[String("LedPcStatus")] = LedPcStatus;
        serializeJson(doc, *response);  
        request->send(response);  


  });


}

void loop() {
}
  
