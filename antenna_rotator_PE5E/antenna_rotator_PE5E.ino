

/* Antenna rotator by PE5E
 * 
 * This project is based upon the ESP8266 development board
 * There is a webserver at port 80 to show the user the status of the rotator and offer manual control
 * There is also a service listening on port 4533 for rotctl commands
 * 
 *  
 *  
 *  
 */

// define data here that needs to be used by multiple libs // // // 
float current_azimuth =      180  ;
float current_elevation =    0    ;
float requested_azimuth =    180  ;
float requested_elevation =  0    ;
int   webserver_address =    80   ;
int   rotctl_address =       4533 ; 

int   ledPin =               2    ; // built in led
bool  manual_control =       false;

enum moving_status {
  standstill,
  cw,  // to the right
  ccw, // to the left
  up,
  down
};
const char * moving_status_text[] = {"standing still", "going clockwise", "going counter clockwise", "going up", "going down"};
moving_status direction_status = standstill;

// until here is the shared data // // // // // // // // // // // //

#include "ESPAsyncWebServer.h"
#include "ESPAsyncTCP.h"           // used by rotctl
#include "index.h"                 // HTML webpage contents
#include "controlport_server.h"    // controlport server

 
const char* ssid = "Ziggo";
const char* password = "Tijdelijk_Netwerk_Voor_Gasten#";
 


AsyncWebServer http_server(webserver_address);
AsyncWebSocket ws("/ws"); // access at ws://[esp ip]/ws
AsyncEventSource events("/events"); // event source (Server-Sent events)

void onRequest(AsyncWebServerRequest *request){
  //Handle Unknown Request
  Serial.println("Received unknown request.");
  request->send(404);
}

// make HTML pages dynamic. replace variables like %variable% with server data
String processor(const String& var)
{
  if(var == "DIRECTION_STATUS"){
    return moving_status_text[direction_status];
  }

  if(var == "CURRENT_AZIMUTH"){
    return String(current_azimuth);
  }

  if(var == "CURRENT_ELEVATION"){
    return String(current_elevation);
  }

  if(var == "REQUESTED_AZIMUTH"){
    return String(requested_azimuth);
  }

  if(var == "REQUESTED_ELEVATION"){
    return String(requested_elevation);
  }
  
  if(var == "MANUAL_CONTROL_TEXT"){
      if(manual_control) {
        return String("manually");
      }
      else {
        return String("automatically");
      }
  }

  if(var == "CURRENT_ACTION_DEG"){
      switch(direction_status) {
        case standstill:
          return String("0");
          break;
        case cw:
          return String("90");
          break;
        case ccw:
          return String("270");
          break;
        case up:
          return String("0");
          break;
        case down:
          return String("180");
          break;
      }
  }
  
  return String();
}

void onEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){
  //Handle WebSocket event
  Serial.println("Websocket onEvent"); 
}

void processRequest(AsyncWebServerRequest *request){
    if(request->hasParam("direction")) {
      AsyncWebParameter* p = request->getParam("direction");
      String directionCommand = p->value().c_str();
      if(directionCommand == "UP") {
        direction_status = up;
      }
      else if(directionCommand == "DOWN") {
        direction_status = down;
      }
      else if(directionCommand == "CCW->LEFT") {
        direction_status = ccw;
      }
      else if(directionCommand == "CW->RIGHT") {
        direction_status = cw;
      }
      else if(directionCommand == "STOP") {
        direction_status = standstill;
      }
    }

    if(request->hasParam("controlmode")) {
      AsyncWebParameter* p = request->getParam("controlmode");
      String control_mode = p->value().c_str();
      if(control_mode == "MANUAL") {
        manual_control = true;
      }
      else if(control_mode == "AUTOMATIC") {
        manual_control = false;
      }
    }

    if(request->hasParam("azi")) {
      AsyncWebParameter* p = request->getParam("azi");
      String azi = p->value().c_str();
      requested_azimuth = azi.toFloat();
    }
    if(request->hasParam("ele")) {
      AsyncWebParameter* p = request->getParam("ele");
      String ele = p->value().c_str();
      requested_elevation = ele.toFloat();
    }
}

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH); // HIGH is off
  Serial.begin(115200);
  delay(10);

  // attach AsyncWebSocket
  ws.onEvent(onEvent);
  http_server.addHandler(&ws);

  // attach AsyncEventSource
  http_server.addHandler(&events);

  http_server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
    // request->send(SPIFFS, "/index.htm", String(), false, processor);
  });
  
  http_server.on("/action", HTTP_GET, [](AsyncWebServerRequest *request){
    processRequest(request); // do something with the request
    request->send_P(200, "text/html", index_html, processor);
  });

  AsyncServer* control_server = new AsyncServer(rotctl_address); // server for rotctl commands
  control_server->onClient(&handleNewClient, control_server);
 
// Connect to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
     Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the servers
  http_server.begin();
  control_server->begin();
  
  Serial.println("http server and control server started");
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}
 
void loop() {
  if(direction_status > 0) {
    digitalWrite(ledPin, LOW); // LOW is on
  }
  else {
    digitalWrite(ledPin, HIGH); // HIGH is off
  }

  delay(100); 
}
