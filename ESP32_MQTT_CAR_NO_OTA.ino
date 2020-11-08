#include <WiFi.h>
#include <PubSubClient.h>

// WIFI and MQTT broker configuration

const char* ssid = "wifi_name";
const char* password = "password";
const char* mqtt_server = "ip_of_server";
const char* client_name = "MQTT Car ESP32";
unsigned long intervals[] = {75, 40, 70, 40, 70, 40, 70, 40, 1000, 0}; // Pattern in which the inbuilt LED on the ESP32 should flash
const byte NUM_OF_INTERVALS = sizeof(intervals) / sizeof(unsigned long);
byte currentInterval = 0;
unsigned long previousMillis = 0;
unsigned long pins[] = {15, 23, 18, 19, 4, 13, 21, 22};
int d = 0, c = 0, b = 0, a = sizeof(pins);
//OPTIONAL PARAMETERS
//const int mqttPort = 1883;
//const char* mqttUser = "YourMqttUser";
//const char* mqttPassword = "YourMqttUserPassword";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {
  delay(10);// We start by connecting to a WiFi network
  Serial.println();
  ledblink();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}



void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  /* for (int i = 0; i < length; i++) {
     Serial.print((char)payload[i]);
    }*/
  char message[length + 1];
  for (int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0';
  Serial.println(message);
  if ( strcmp (message, "FORWARD") == 0) {
    forward();
    c = 0;
  }
  if ( strcmp (message, "BACKWARD") == 0) {
    backward();
    c = 0;
  }
  if ( strcmp (message, "LEFT") == 0) {
    left();
    c = 0;
  }
  if ( strcmp (message, "RIGHT") == 0) {
    right();
    c = 0;
  }
  if ( strcmp (message, "STOP") == 0) {
    cancel();
    c = 0;
  }
  if ( strcmp (message, "AUTO") == 0) {
    c = 1;
  }
  if ( strcmp (message, "CRAZY") == 0) {
    crazy();
    c = 0;
  }
  if ( strcmp (message, "CRAZY2") == 0) {
    crazy2();
    c = 0;
  }
}

void reconnect() {
  // Loop until we're reconnected
  if (WiFi.status() != WL_CONNECTED)
  {
    setup_wifi();
  }
  while (!client.connected()) {
    digitalWrite(2, HIGH);
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    Serial.println(clientId);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("mqttcar/status", " MQTT CAR Connected");
      // ... and resubscribe
      client.subscribe("mqttcar/input");
      digitalWrite(2, LOW);
    } else {


      ESP.restart();


      /*if (WiFi.status() != WL_CONNECTED)
        {
        setup_wifi();
        }
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 1 seconds");
        // Wait 1 seconds before retrying
        delay(1000);*/
    }
  }
}



void setup()
{
  pinMode(15, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(21, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
  //Car Online Beacon
  long now = millis();
  if (now - lastMsg > 1000) {
    lastMsg = now;
    d = WiFi.RSSI();
    snprintf (msg, 75, "I'm Online! RSSI: %ld :)", d);
    client.publish("mqttcar/status", msg);
  }
  ledblink();
  if (c == 1)
  {
    autonomous();
  }
}


void ledblink()
{
  unsigned long currentMillis = millis();          //get current value of millisecond counter
  if (currentMillis - previousMillis >= intervals[currentInterval])  //if the time interval has elapsed
  {
    currentInterval = currentInterval + 1;     // select the next interval in the list
    if (currentInterval >= NUM_OF_INTERVALS)
    {
      currentInterval = 0;
    }
    digitalWrite(2, not digitalRead(2));    //change state of the LED
    previousMillis = currentMillis;                //save the time of change
  }
}


void forward()
{
  digitalWrite(15, HIGH);
  digitalWrite(18, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(21, HIGH);
  digitalWrite(23, LOW);
  digitalWrite(19, LOW);
  digitalWrite(13, LOW);
  digitalWrite(22, LOW);

}

void backward()
{
  digitalWrite(15, LOW);
  digitalWrite(18, LOW);
  digitalWrite(4, LOW);
  digitalWrite(21, LOW);
  digitalWrite(23, HIGH);
  digitalWrite(19, HIGH);
  digitalWrite(13, HIGH);
  digitalWrite(22, HIGH);
}


void cancel()
{
  for (b = 0; b <= a; b++)
  {
    digitalWrite(pins[b], LOW);
  }
}

void right()
{
  digitalWrite(15, HIGH);
  digitalWrite(18, LOW);
  digitalWrite(4, HIGH);
  digitalWrite(21, LOW);
  digitalWrite(23, LOW);
  digitalWrite(19, HIGH);
  digitalWrite(13, LOW);
  digitalWrite(22, HIGH);

}

void left()
{
  digitalWrite(15, LOW);
  digitalWrite(18, HIGH);
  digitalWrite(4, LOW);
  digitalWrite(21, HIGH);
  digitalWrite(23, HIGH);
  digitalWrite(19, LOW);
  digitalWrite(13, HIGH);
  digitalWrite(22, LOW);

}

void autonomous()
{
  if (analogRead(34) >= 100 && analogRead(35) >= 100)
  {
    forward();
  }
  if (analogRead(34) <= 100 && analogRead(35) >= 100)
  {
    backward();
  }
  if (analogRead(34) >= 100 && analogRead(35) <= 100)
  {
    forward();
  }
  if (analogRead(34) <= 100 && analogRead(35) <= 100)
  {
    cancel();
  }
}

void crazy()
{
  digitalWrite(15, HIGH);
  digitalWrite(18, LOW);
  digitalWrite(4, LOW);
  digitalWrite(21, HIGH);
  digitalWrite(23, LOW);
  digitalWrite(19, HIGH);
  digitalWrite(13, HIGH);
  digitalWrite(22, LOW);

}

void crazy2()
{
  digitalWrite(15, HIGH);
  digitalWrite(18, HIGH);
  digitalWrite(4, LOW);
  digitalWrite(21, LOW);
  digitalWrite(23, LOW);
  digitalWrite(19, LOW);
  digitalWrite(13, HIGH);
  digitalWrite(22, HIGH);

}




























/* if(c==49){forward();}
  if(c==50){backward();}
  if(c==51){left();}
  if(c==52){right();}
  if(c==53){cancel();}*/
