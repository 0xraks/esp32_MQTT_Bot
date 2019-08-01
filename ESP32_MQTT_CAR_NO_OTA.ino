

#include <WiFi.h>
#include <PubSubClient.h>



// WIFI configuration

const char* ssid = "YOUR SSID";
const char* password = "YOUR WIFI PASSWORD";
const char* mqtt_server = "MQTT BROKER";

unsigned long intervals[] = {75,40,70,40,70,40,70,40,1000,0};
const byte NUM_OF_INTERVALS = sizeof(intervals) / sizeof(unsigned long);
byte currentInterval = 0;
unsigned long previousMillis = 0;
unsigned long pins[] = {15,23,18,19,4,13,21,22};
int c=0, b=0,a=sizeof(pins);

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {
  delay(10);// We start by connecting to a WiFi network
  Serial.println();
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
  c=payload[0];
  Serial.print(c);
  if(c==49){forward();}
  if(c==50){backward();}
  if(c==51){left();}
  if(c==52){right();}
  if(c==53){cancel();}
  
  
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "Connected");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}



void setup() 
{
  pinMode(15,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(18,OUTPUT);
  pinMode(19,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(13,OUTPUT);
  pinMode(21,OUTPUT);
  pinMode(22,OUTPUT);
  pinMode(23,OUTPUT);
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

  long now = millis();
  if (now - lastMsg > 1000) {
    lastMsg = now;
    snprintf (msg, 75, "hello world ");
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", msg);
    Serial.println(c);
  }
 ledblink();
}













void ledblink()
{
  unsigned long currentMillis = millis();          //get current value of millisecond counter
  if (currentMillis - previousMillis >= intervals[currentInterval])  //if the time interval has elapsed
  {
    currentInterval = currentInterval + 1;     // select the next interval in the list
    if (currentInterval >= NUM_OF_INTERVALS)
      {currentInterval = 0;}
    digitalWrite(2, not digitalRead(2));    //change state of the LED
    previousMillis = currentMillis;                //save the time of change
  }
}

 
void forward()
{
  digitalWrite(15,HIGH);
  digitalWrite(18,HIGH);
  digitalWrite(4,HIGH);
  digitalWrite(21,HIGH);
  digitalWrite(23,LOW);
  digitalWrite(19,LOW);
  digitalWrite(13,LOW);
  digitalWrite(22,LOW);      
  
}

void backward()
{
  digitalWrite(15,LOW);
  digitalWrite(18,LOW);
  digitalWrite(4,LOW);
  digitalWrite(21,LOW);
  digitalWrite(23,HIGH);
  digitalWrite(19,HIGH);
  digitalWrite(13,HIGH);
  digitalWrite(22,HIGH); 
}


void cancel()
{
  for(b=0;b<=a;b++)
  {
    digitalWrite(pins[b],LOW);
  }
}

void right()
{
  digitalWrite(15,HIGH);
  digitalWrite(18,LOW);
  digitalWrite(4,HIGH);
  digitalWrite(21,LOW);
  digitalWrite(23,LOW);
  digitalWrite(19,HIGH);
  digitalWrite(13,LOW);
  digitalWrite(22,HIGH); 
  
}

void left()
{
  digitalWrite(15,LOW);
  digitalWrite(18,HIGH);
  digitalWrite(4,LOW);
  digitalWrite(21,HIGH);
  digitalWrite(23,HIGH);
  digitalWrite(19,LOW);
  digitalWrite(13,HIGH);
  digitalWrite(22,LOW); 
  
}
