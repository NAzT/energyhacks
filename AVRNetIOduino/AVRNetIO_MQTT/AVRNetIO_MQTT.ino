/* AVRNetIO using MQTT to post sensor readings to the FLM broker */
#include <SPI.h>
#include <UIPEthernet.h>  // https://github.com/ntruchsess/arduino_uip
#include <PubSubClient.h>  // https://github.com/knolleary/pubsubclient

#define ledPin 1
#define AVG 50

//Global variables for Interrupt computation
unsigned long sum = 0, nHz = 0, microseconds = 0;
byte cnt = 0;

// The AVRNetIO's MAC as provided by Pollin - use your own unique address
byte mac[]    = { 
  0x00, 0x22, 0xF9, 0x01, 0x2E, 0x49 };
// The FLM's IP address / the MQTT broker's address
// The FLM itself publishes on topic /sensor/#, so this is used also to get
// a generic display capability via the FLM MQTT panel
// https://github.com/gebhardm/energyhacks/tree/master/RaspberryPi/panel
byte flm[] = { 
  192, 168, 0, 50 };

void callback(char* topic, byte* payload, unsigned int length) {
  if (length>0) {
    // do something with the payload
  }
}

EthernetClient ethClient;
PubSubClient client(flm, 1883, callback, ethClient);

void setup()
{
  //  initialize ethernet controller by DHCP
  if(Ethernet.begin(mac) == 0) {
    // something went wrong
    while(true)
    { 
	  // let LED blink to show error condition
      for (int i=0;i<3;i++){
        digitalWrite(ledPin, HIGH);
        delay(150);
        digitalWrite(ledPin, LOW);
        delay(150);
      };
      delay(700);
    }
  } 
  else {
    IPAddress myip = Ethernet.localIP();
  };
  // attach INT0 to the net frequency detection routine
  attachInterrupt(0, interrupt, FALLING);
}

// create topic payload in compliance with FLM topic in JSON
char* createPayload(long value, char* unit)
{
  char val[10], payload[60];
  // build payload [<value>,"<unit>"]
  ltoa(value, val, 10);
  strcpy(payload,"");
  strcat(payload,"[");
  strcat(payload,val);
  strcat(payload,",\"");
  strcat(payload,unit);
  strcat(payload,"\"]");
  return payload;
}

void loop()
{
  client.loop();
  if (client.connected()) {
  // now publish the readings; gauge is generically used for "gauge display", so this is reused
    client.publish("/sensor/ADC1/gauge",createPayload((analogRead(A1)*(500000/1023)/100),"mV"));
    client.publish("/sensor/ADC2/gauge",createPayload((analogRead(A2)*(500000/1023)/100),"mV"));
    client.publish("/sensor/ADC3/gauge",createPayload((analogRead(A3)*(500000/1023)/100),"mV"));
    client.publish("/sensor/ADC4/gauge",createPayload((analogRead(A4)*(500000/1023)/100),"mV"));
    client.publish("/sensor/Net/gauge",createPayload( (long) (1000000000L / nHz),"mHz"));
  }
  else {
    // reconnect on lost connection
    client.connect("arduino");
  }
  // blink for convenience
  digitalWrite(ledPin, HIGH);
  delay(250);
  digitalWrite(ledPin, LOW);
  delay(750);
}

void interrupt( void )
{
  // attach an AC signal via diode and 10k resistor to INT0
  sum += ( micros() - microseconds );
  microseconds = micros();  
  cnt++;
  // calculate the average net frequency detected during AVG reads
  if (cnt == AVG) {
    nHz = sum / cnt;
    cnt = 0;
    sum = 0;  
  }
}