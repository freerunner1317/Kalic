//#include <ESP8266WiFi.h>
//#include <ESPAsyncTCP.h>
//#include <ESPAsyncWebServer.h>
//#include <AsyncElegantOTA.h>
#include <WiFiManager.h>
#include <OTA.h>


const char* ssid = "DIR-825-135";
const char* password = "11223360";

//AsyncWebServer server(80);
WiFiManager wm;

void Init_OTA(){
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP   


    wm.setConfigPortalBlocking(false);
    wm.setConfigPortalTimeout(5);
    //automatically connect using saved credentials if they exist
    //If connection fails it starts an access point with the specified name
    if(wm.autoConnect("AutoConnectAP")){
        Serial.println("connected...yeey :)");
    }
    else {
        Serial.println("Configportal running");
    }
    //WiFi.setSleepMode(WIFI_LIGHT_SLEEP, 3);
    // WiFi.mode(WIFI_STA);
    // WiFi.begin(ssid, password);
    // Serial.println("");

    // // Wait for connection
    // while (WiFi.status() != WL_CONNECTED) {
    //     delay(500);
    //     Serial.print(".");
    // }
    // Serial.println("");
    // Serial.print("Connected to ");
    // Serial.println(ssid);
    // Serial.print("IP address: ");
    // Serial.println(WiFi.localIP());

    // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    //     request->send(200, "text/plain", "Hi! I am ESP8266.");
    // });

    // AsyncElegantOTA.begin(&server);    // Start ElegantOTA
    // server.begin();
    //Serial.println("HTTP server started");
}