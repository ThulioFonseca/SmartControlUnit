#include <Arduino.h>
#include <ESP8266WiFi.h>          
#include <DNSServer.h>
#include <LittleFS.h>          
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer servidor(80);
const char* ssid     = "Oi velox";            // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "Oi130987";            // The password of the Wi-Fi network
int count = 0;       

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

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  
  NetConfig();
  ServerConfig();

  
  servidor.on("/",HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "ESP conectado ao wifi");
  });

  servidor.on("/home/",HTTP_POST, []( AsyncWebServerRequest *request){

      Serial.println("POST Recebido");

       String inputString;

       if(request->hasArg("valueSoundSys")){
          inputString = request->arg("valueSoundSys");

          if(inputString == "1"){
            digitalWrite(LED_BUILTIN, LOW);
            request->send(200, "text/plain", "Ligado");           
         }else{
            digitalWrite(LED_BUILTIN, HIGH);
            request->send(200, "text/plain", "Desligado");
         }
         
        //  else {
        //     request->send(406, "text/plain", "Argumento invalido");
        //   }

       }
  });

}

void loop() {
}

     
