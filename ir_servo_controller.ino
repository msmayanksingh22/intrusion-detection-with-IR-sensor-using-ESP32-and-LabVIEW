#include <WiFi.h>
#include <Servo.h>

#define ir 23
#define servo 22

#define id "TheCosmos"
#define pass "thedivinefun"

Servo myservo;
int pos = 0;
int inputVal=0;

WiFiServer server(80);

void setup() {
  myservo.attach(servo);
  pinMode(ir,INPUT);
  Serial.begin(9600);
  Serial.print("\nConnecting to ");
  Serial.println(id);

  WiFi.begin(id, pass);
  while (WiFi.status() != WL_CONNECTED) { // Waiting for successful connection
        delay(500);
        Serial.print(".");
  }
  Serial.print("WiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void servo180(){ //to close
  for (pos = 0; pos <= 180; pos += 1) {
    myservo.write(pos);
  }
}

void servo0(){ //to open
  for (pos = 180; pos >= 0; pos -= 1) {
    myservo.write(pos);
  }
}

void lab(){
  if(digitalRead(ir)==1){
      if(pos==0){
        servo180();
      }
      else{
        pos=180;
      }
    }
    if (digitalRead(ir)==0){
      if(pos==180){
        servo0();
      }
      else{
        pos=0;
      }
    }
}

void loop() {
  WiFiClient client = server.available();
  
  if (client){
    if(client.connected()){
      Serial.println("Client Connected");
    }
    while(client.connected()){      
      while(client.available()>0)
      {        
        uint8_t data =client.read(); 
        Serial.write(data);
        
        switch(data){
          case 'a':
            lab();
            Serial.println(digitalRead(ir));
            client.print(digitalRead(ir));
            break;
          
          case 'c':
            Serial.println(digitalRead(ir));
            client.print(digitalRead(ir));
            if(pos==0){
              servo180();
            }
            else{
              pos=180;
            }
            break;

          case 'o':
            Serial.println(digitalRead(ir));
            client.print(digitalRead(ir));
            if(pos==180){
              servo0();
            }
            else{
              pos=0;
            }
            break;
        }
        //Serial.println(digitalRead(ir));
        //client.print(digitalRead(ir));
      }
    }
    client.stop();
    Serial.println("Client disconnected");
  }
  delay(100);
}
