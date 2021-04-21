#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define debug true

#define wifi_ssid "Beeline_2G_F24922"
#define wifi_password "AHMEM9FNpY"

#define mqtt_server "sandbox.rightech.io"
#define mqtt_id "mqtt-iprofi_730109725-grv4sw"

#define BUZZER_PIN D6

long lastMsg = 0;
long FUEL = 5042;
double lat = 67.4;
double lon = 64.04;
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  connect_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
  if (now - lastMsg > 1000) {
    lastMsg = now;
    client.publish("adc", String(FUEL).c_str(), true);
    FUEL -= 20;
    if(FUEL < 100) {
      FUEL = 100;
    } else {
      lat += 0.001;
      lon += 0.001;
    }
    client.publish("lat", String(lat).c_str(), true);
    client.publish("lon", String(lon).c_str(), true);
  }
}

void connect_wifi() {
  WiFi.begin(wifi_ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  if(debug) Serial.println("WiFi OK");
}

void reconnect() {
  while (!client.connected()) {
    if(debug) Serial.print("Connecting to MQTT broker ...");
    if (client.connect(mqtt_id)) {
      if(debug) Serial.println("OK");
    } else {
      if(debug) Serial.print("error : ");
      if(debug) Serial.print(client.state());
      if(debug) Serial.println(" Wait 5 secondes before to retry");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  if(String(topic) == "buzzer" && length >= 1) {
    digitalWrite(BUZZER_PIN, payload[0] == '1');
  }
}
