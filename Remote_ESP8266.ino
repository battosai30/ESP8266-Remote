#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

char* ssid = "xxxxx"; // votre SSID
char* password = "yyyyyyy"; // votre mot de passe wifi
long port = 504;

MDNSResponder mdns; // serveur mDNS

ESP8266WebServer server(port); // serveur HTTP

const int led = 16; // led integree au NodeMCU, attention logique inverse
const int relay = 5; // relais connecte au GPIO5

void setup(void) {

/* Configuration des entree/sortie */
  pinMode(relay, OUTPUT);
  pinMode(led, OUTPUT);

  digitalWrite(led, 1);
  digitalWrite(relay, 0);

  Serial.begin(115200); // initialisation du port serie
  
  connect(ssid, password); // connexion au reseau Wifi
  
  Serial.println("");

/* demarrage du serveur mDNS sur esp8266.local */
  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

/* ajout des actions du serveur */
  server.on("/", handle_root); 
  server.on("/open", handle_open); 
 
  server.begin(); // demarrage du serveur
  
  Serial.println("HTTP server started");
}

void loop(void) {
  
  server.handleClient(); // gestion du serveur

/* Si connecté au wifi alors la LED s'allume */
  if (WiFi.status() == WL_CONNECTED) digitalWrite(led, 0);
  else digitalWrite(led, 1);
  
}

/* Routine pour se connecter à un reseau wifi */
void connect(char *_SSID, char* _PWD) {

  Serial.println("");
  Serial.print("Connecting ");
  Serial.print(_SSID);
  Serial.print(" password = ");
  Serial.print( _PWD);

  WiFi.begin(_SSID, _PWD);
  Serial.println("");

  int h = 0;
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    
    if (h++ > 40) { // si trop long on abandonne
      Serial.println();
      Serial.println("Failed to connect");
      return;
    }
    
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

}

/* Action serveur sur /open */
void handle_open() {
  server.send(200, "text/plain", "opening"); // repond "opening"
  open(); // actionne le relais
}

void handle_root() {

/* page[] contient notre page web et renvois vers le domaine /open si on appuie sur le bouton */
  char page[] = "<h1>Ouverture portail</h1><p><a href=\"open\"><button>Ouvrir</button></a></p>";

  server.send(200, "text/html", page); // repond avec la page web codee en HTML

}

void open() {
  
  digitalWrite(relay, 1);
  Serial.println("Opening");
  delay(1000);
  digitalWrite(relay, 0);
  
}

