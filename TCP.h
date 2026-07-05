#ifndef TCP_H
#define TCP_H
#include <WiFi.h>
#define pass (void)0

class tcp {
const char* ssid;
const char* password;
uint8_t count = 0;
public:
  tcp(const char* ssid, const char* password) {
    this-> ssid = ssid;
    this->password = password;
  }
  bool connect() {
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED && count < 50) {
      Serial.println(WiFi.status());
      count ++;
      delay(500);
    }
    return WiFi.status() == WL_CONNECTED;
  }

  void StartServer(){
    server.begin();
    Serial.println("IP: ");
    Serial.println(WiFi.localIP());
  }

  void send(float val, long time){
    if(!client.connected()|| !client)  
      client = server.available();
    
    if(client.connected()){
      client.print(val);
      client.print(",");
      client.println(float(time/1000000.0f));
    }
    else{
      pass;
    }
  }

  void sendText(const char* val){
    if(!client.connected()|| !client)  
      client = server.available();
    
    if(client.connected()){
      client.println(val);
    }
    else{
      pass;
    }
  }

  
private:
  WiFiServer server = WiFiServer(1234);
   WiFiClient client;
};
#endif