#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

//char* ssid = "freebox_Avril";
//char* password = "0651113168";

char* ssid = "xxxxx"; // votre SSID
char* password = "yyyyyyy"; // votre mot de passe wifi
long port = 504;

MDNSResponder mdns;

ESP8266WebServer server(port);

const int led = 16;
const int relay =5;

void handle_app() {
  server.send(200, "text/plain", "opening");
open();
}

void handle_root() {

  char page[] = "<h1>Ouverture portail</h1><p><a href=\"open\"><button>Ouvrir</button></a></p>";

  server.send(200, "text/html", page);
  open();

}

void open() {
  digitalWrite(relay, 1);
  Serial.println("Opening");
  delay(1000);
  digitalWrite(relay, 0);
}


void setup(void) {
  
pinMode(relay, OUTPUT);
  pinMode(led, OUTPUT);
  
  digitalWrite(led, 1);
  digitalWrite(relay, 0);
  
  Serial.begin(115200);
  connect(ssid, password);
  Serial.println("");

  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

  server.on("/open", handle_app);

  server.on("/", handle_root);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  if (Serial.available()) handleSerial();
  if (WiFi.status() == WL_CONNECTED) digitalWrite(led,0);
  else digitalWrite(led,1);
}

void handleSerial() {

  delay(20);

  char buf[200];
  memset(buf, NULL, sizeof(buf));

  int i = 0;

  while (Serial.available()) buf[i++] = Serial.read();

  char *p;

  p = strtok(buf, ",");

  if (strstr(p, "$") != NULL) {
Serial.println("");
    Serial.println("New AP config received");

    char *_ssid = strtok(NULL, ",");
    char *_pwd = strtok(NULL, ",");

    connect(_ssid, _pwd);

  }

}

void connect(char *_SSID, char* _PWD) {

  Serial.println("");
  Serial.print("Connecting ");
  Serial.print(_SSID);
  Serial.print(" password = ");
  Serial.print( _PWD);

  //WiFi.disconnect();
  //delay(1500);
  WiFi.begin(_SSID, _PWD);
  Serial.println("");

int h=0;
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if(h++>40) {
      Serial.println();
      Serial.println("Failed to connect");
      WiFi.begin(ssid, password);
      return;
    }
    }
  
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}

