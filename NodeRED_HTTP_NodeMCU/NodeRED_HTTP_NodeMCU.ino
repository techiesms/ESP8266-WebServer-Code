
/* Comment this out to disable prints and save space */
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

/************************* Pin Definition *********************************/
2
//Relays for switching appliances
#define Relay1            D6
#define Relay2            D2
#define Relay3            D1
#define Relay4            D5

//DHT11 for reading temperature and humidity value
#define DHTPIN            D7

//buzzer to use it for alert
#define buzzer            D0

//Selection pins for multiplexer module to switch between different sensors and give data on a single analog pin.
#define S0                D3
#define S1                D4

//Analog pin to read the incoming analog value from different sensors.
#define analogpin         A0


String temp;
String hum;
int proximity;
int light;
int gas;
int motion;


// Uncomment whatever type you're using!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301



DHT dht(DHTPIN, DHTTYPE);

// Your WiFi credentials.
// Set password to "" for open networks.
const char* ssid = "SSID";
const char* password = "PASS";

ESP8266WebServer server(80);

void handleRoot() {

  server.send(200, "text/plain", "hello from esp8266!");

}

void handleNotFound() {

  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);

}


void setup()
{
  // Debug console
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  dht.begin();

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  pinMode(buzzer, OUTPUT);


  server.on("/", handleRoot); // http://localIPAddress/

  server.on("/dht-temp", []() // http://localIPAddress/dht-temp
  {
    int t = dht.readTemperature();
    temp = String(t);
    server.send(200, "text/plain", temp);
  });

  server.on("/dht-hum", []()  // http://localIPAddress/dht-hum
  {
    int h = dht.readHumidity();
    hum = String(h);
    server.send(200, "text/plain", hum);
  });

  server.on("/relay1/on", []()  // http://localIPAddress/relay1/on
  {
    server.send(200, "text/plain", "Relay 1 turned ON");
    digitalWrite(Relay1, HIGH);
  });

  server.on("/relay1/off", []()  // http://localIPAddress/relay1/off
  {
    server.send(200, "text/plain", "Relay 1 turned OFF");
    digitalWrite(Relay1, LOW);
  });

  server.on("/relay2/on", []()  // http://localIPAddress/relay2/on
  {
    server.send(200, "text/plain", "Relay 2 turned ON");
    digitalWrite(Relay2, HIGH);
  });

  server.on("/relay2/off", []()  // http://localIPAddress/relay2/off
  {
    server.send(200, "text/plain", "Relay 2 turned OFF");
    digitalWrite(Relay2, LOW);
  });

  server.on("/relay3/on", []()  // http://localIPAddress/relay3/on
  {
    server.send(200, "text/plain", "Relay 3 turned ON");
    digitalWrite(Relay3, HIGH);
  });

  server.on("/relay3/off", []()  // http://localIPAddress/relay3/off
  {
    server.send(200, "text/plain", "Relay 3 turned OFF");
    digitalWrite(Relay3, LOW);
  });

  server.on("/relay4/on", []()  // http://localIPAddress/relay4/on
  {
    server.send(200, "text/plain", "Relay 4 turned ON");
    digitalWrite(Relay4, HIGH);
  });

  server.on("/relay4/off", []()  // http://localIPAddress/relay4/off
  {
    server.send(200, "text/plain", "Relay 4 turned OFF");
    digitalWrite(Relay4, LOW);
  });

  server.on("/sens1", []() // http://localIPAddress/sens1
  {
    digitalWrite(S0, LOW);
    digitalWrite(S1, LOW);
    gas = analogRead(analogpin);
    String G = String(gas);
    server.send(200, "text/plain", G);
  });

  server.on("/sens2", []() // http://localIPAddress/sens2
  {
    digitalWrite(S0, HIGH);
    digitalWrite(S1, LOW);
    int raw_light = analogRead(analogpin);
    light = map(raw_light, 1024, 0, 0, 100);
    String L = String(light);
    server.send(200, "text/plain", L);
  });

  server.on("/sens3", []() // http://localIPAddress/sens3
  {
    digitalWrite(S0, LOW);
    digitalWrite(S1, HIGH);

    server.send(200, "text/plain", "Sensor 3 data");
  });

  server.on("/sens4", []() // http://localIPAddress/sens4
  {
    digitalWrite(S0, HIGH);
    digitalWrite(S1, HIGH);

    server.send(200, "text/plain", "Sensor 4 Data");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

}

void loop()
{

  server.handleClient();


}
