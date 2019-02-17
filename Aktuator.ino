#include <PubSubClient.h>
#include <ESP8266WiFi.h>


const char* ssid = "";
const char* password = "";
const char* mqtt_server = "";

WiFiClient espClient;
PubSubClient client(espClient);


void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  //WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
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

   String callTopic = "";
   int i = 0;

   while((char)topic[i] != 0){

    callTopic += (char)topic[i];
    i++;
   }

   if(callTopic.equals("inputkendali/kipas")){

    if((char)payload[0] == '1'){

      digitalWrite(D2, HIGH);
      
    }
    else if((char)payload[0] == '0'){

      digitalWrite(D2, LOW);
      
    }
    
   }

   if(callTopic.equals("inputkendali/pompa")){

    if((char)payload[0] == '1'){

      digitalWrite(D3, HIGH);
      
    }

    else if((char)payload[0] == '0'){

      digitalWrite(D3, LOW);
      
    }
    
   }
}


void reconnect() {

  while (!client.connected()) {

    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX); 

    if (client.connect(clientId.c_str())) { 

      Serial.println("connected");
      client.subscribe("inputkendali/kipas");
      client.subscribe("inputkendali/pompa");
      

    } else {
      
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      delay(5000);
      
    }
  }
}

void setup() {

  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
}

void loop() {

  if (!client.connected()) {
    
    reconnect();
  }

  client.loop();

  
}
