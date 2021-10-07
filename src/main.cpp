#include <RCSwitch.h>
#include <ESP8266WiFi.h>
#include "rf_switch.h"
#include "actions.h"

const int ON_SWITCH = D1;

RCSwitch sender = RCSwitch();
RFSwitch aSocket(&sender, "000000FFFF0F", "000000FFFFF0");
RFSwitch bSocket(&sender, "00000F0FFF0F", "00000F0FFFF0");
RFSwitch cSocket(&sender, "00000FF0FF0F", "00000FF0FFF0");
RFSwitch* sockets[3] = { &aSocket, &bSocket, &cSocket };
const int socketCount = sizeof(sockets) / sizeof(*sockets);

#include "secrets.h"

// Create server, specify the port to listen on as an argument
WiFiServer server(80);

void connectToWifi();
String readFromWifi(WiFiClient);
Action *getRequestInfo(String);

void setup() {
#if OUTPUT_SERIAL
  Serial.begin(115200);
#endif

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(ON_SWITCH, OUTPUT);

  digitalWrite(ON_SWITCH, LOW);
  sender.enableTransmit(D3); //Pin D3
  sender.setProtocol(1);
  sender.setPulseLength(315);
  digitalWrite(ON_SWITCH, HIGH);

  connectToWifi();
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // read request
  String req = readFromWifi(client);
  if (req.length() == 0) {
    return;
  }
#if OUTPUT_SERIAL
  Serial.println(req);
#endif

  // get action to perform corresponding to request
  Action *action = getRequestInfo(req);
  action->performAction();
  client.print(action->getResponse());
  delete action;

  delay(1);
#if OUTPUT_SERIAL
  Serial.println("Client disonnected");
#endif
}

void connectToWifi() {
  
    // Connect to WiFi network
#if OUTPUT_SERIAL
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
#endif

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
#if OUTPUT_SERIAL
    Serial.print(".");
#endif
  }

#if OUTPUT_SERIAL
  Serial.println("");
  Serial.println("WiFi connected");
#endif
  
  // Start the server
  server.begin();

#if OUTPUT_SERIAL
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
#endif
}

String readFromWifi(WiFiClient client) {
  // Wait until the client sends some data
#if OUTPUT_SERIAL
  Serial.println("new client");
#endif

  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  return req;
}

Action *getRequestInfo(String req) {
  Action *result = NULL;
  if (req.indexOf("/switches/") != -1) {
    String substring = req.substring(req.indexOf("/switches/") + String("/switches/").length());
    if (substring.indexOf("/") != -1) {
      String indexPart = substring.substring(0, substring.indexOf("/"));
      String statePart = substring.substring(substring.indexOf("/") + 1, substring.indexOf(" "));

#if OUTPUT_SERIAL
      Serial.println("index: " + indexPart + ", state: " + statePart);
#endif

      int actionIndex = atoi(indexPart.c_str());
      if (actionIndex < socketCount) {
        SwitchAction *action = new SwitchAction(sockets[actionIndex]);
        action->setOn (statePart == "1");
        result = action;
      }
    }
  }
  if (result == NULL) {
    result = new InvalidAction();
  }
  return result;
}
