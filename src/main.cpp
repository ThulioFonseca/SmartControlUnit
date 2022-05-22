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
  String FitaLedStatus = "teste";
  String LedPcStatus = "teste";



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

void NetConfig(){

  WiFi.begin(ssid, password);                                   // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.print(" ");

  while (WiFi.status() != WL_CONNECTED) {                       // Wait for the Wi-Fi to connect
    Spinner(count);
    count++;
   }
   
  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address..............: ");
  Serial.println(WiFi.localIP());                 

}

void ServerConfig(){
  servidor.serveStatic("/home", LittleFS, "/");
  LittleFS.begin();                                              // Inicializa o sistema de arquivos
  Serial.println("LittleFS................: Running");
  servidor.begin();                                              // Inicializa o servidor web
  Serial.println("WEB-Server..............: Running");
  
}


void setup() {

  NetConfig();
  ServerConfig();

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  

  String json = "{\"CaixaDeSomStatus\":\"" + CaixaDeSomStatus + "\"}";
  
  
  // servidor.on("/home",HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send(LittleFS,"index.html");
  // });



  servidor.on("/home/",HTTP_POST, [&]( AsyncWebServerRequest *request){

      Serial.println("POST Recebido");

       String inputString;

       if(request->hasArg("CaixaDeSom")){
          inputString = request->arg("CaixaDeSom");

          if(inputString == "1"){
            CaixaDeSomStatus = "Ligado";
            digitalWrite(LED_BUILTIN, LOW);
            request->send(200, "text/plain", "Caixa de Som - Ligada");           
         }else if(inputString = "0"){
            CaixaDeSomStatus = "Desligado";
            digitalWrite(LED_BUILTIN, HIGH);
            request->send(200, "text/plain", "Caixa de Som - Desligada");
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
  
