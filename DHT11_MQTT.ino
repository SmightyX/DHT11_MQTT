#include <ESP8266WiFi.h> //Library ESP8266
#include <PubSubClient.h>
#include "DHT.h" //Library Sensor DHT11
#define DHTPIN D5 //Pin Data Pada DHT11 ke ESP8266/ESP32
#define DHTTYPE DHT11 //Tipe Sensor DHT

// Koneksi WiFi
const char *ssid = " "; // Enter your WiFi name
const char *password = " ";  // Enter WiFi password

// Koneksi MQTT Broker
const char *mqtt_broker = " "; //Sesuaikan dengan MQTT Broker
const char *topic1= "Temperature";
const char *topic2= "Humidity";
const char *mqtt_username = ""; // Disesuaikan dengan username pada MQTT Kalian
const char *mqtt_password = ""; // Disesuaikan dengan password pada MQTT Kalian
const int mqtt_port = 1883;

DHT dht(DHTPIN, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
  }
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  
  while (!client.connected()) {
      String client_id = "esp8266-client-";
      client_id += String(WiFi.macAddress());
      Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
      if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
          Serial.println("Public emqx mqtt broker connected");
      } else {
          Serial.print("failed with state ");
          Serial.print(client.state());
          delay(2000);
      }
  }
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic1);
  Serial.println(topic2);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
      Serial.print((char) payload[i]);
  }
  Serial.println("-----------------------");
}

void loop()
{
  delay(1000);
 // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  //client.publish("inTopic/humidity",sprintf(buf, "%f", h));
//  Serial.print("Humidity: ");
//  Serial.print(h);
//  Serial.print(" %\t");
//  Serial.print("Temperature: ");
//  Serial.print(t);
//  Serial.print(" *C ");

  Serial.print("Temperature: ");
  Serial.println(String(t).c_str());
  client.publish(topic1,String(t).c_str());

  Serial.print("Humidity: ");
  Serial.println(String(h).c_str());
  client.publish(topic2,String(h).c_str());
  client.loop();
}
