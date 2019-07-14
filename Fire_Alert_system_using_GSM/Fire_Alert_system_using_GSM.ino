#include <dht.h>
#include <SoftwareSerial.h>
SoftwareSerial gsm(9,8);
#define DHTxxPIN A1
dht DHT;
int p = A0;
int n = A2;
int ack;
int msgsend=0;
int th=28;    //SET THRESHOLD TEMPERATURE
unsigned long A = 1000L; 
unsigned long B = A * 60;
unsigned long C = B * 30 ;
void setup()
{
  Serial.begin(9600);
  gsm.begin(9600); 
  pinMode(p,OUTPUT);
  pinMode(n,OUTPUT);
  digitalWrite(p,1);
  digitalWrite(n,0);
  gsm.println("AT+CMGF=1");    
  delay(1000);  
  gsm.println("AT+CMGS=\"+91----------\"\r"); 
  delay(1000);
  gsm.println("---FIRE ALERT SYSTEM---\n---CMRIT---\n --- RAHUL AND PRIYANKA ---\n");
  delay(100);
  gsm.println((char)26);
  delay(1000);
}
void loop()
{
  top:
  msgsend=0;
  ack=0;
  int chk = DHT.read11(DHTxxPIN);
  switch (chk)
  {
    case DHTLIB_ERROR_CONNECT:
        ack=1;
        break;
  }
  if(ack==0)
  {
  Serial.print("Temperature(°C) = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity(%) = ");
  Serial.println(DHT.humidity);
  Serial.println("\n");
  delay(2000);
}
if(ack==1)
{
  goagain:
  msgsend=msgsend+1;
  Serial.print("NO DATA");
  Serial.print("\n\n");
  Serial.println("Sending SMS......\n");
  delay(500);
  gsm.println("AT+CMGF=1");    
  delay(1000);  
  gsm.println("AT+CMGS=\"+91----------\"\r"); 
  delay(1000);
  gsm.println("NO DATA FROM THE SENSOR/SENSOR DISCONNECTED");
  delay(100);
  gsm.println((char)26); 
  delay(1000);
  Serial.println("Message is sent\n");
  if(msgsend==2)
  {
    delay(C);
    goto top;
  }
  else
  {
    delay(10000);
    goto goagain;
  }
}
if(DHT.temperature>=th)
{
  doagain:
  msgsend=msgsend+1;
  Serial.println("Sending SMS......\n");
  gsm.println("AT+CMGF=1");    
  delay(1000);  
  gsm.println("AT+CMGS=\"+91----------\"\r"); 
  delay(1000);
  gsm.println("--FIRE ALERT--(THE TEMPERATURE IS):");
  gsm.print(DHT.temperature);
  gsm.print(" degree celsius");
  gsm.print("---THE TEMPERATURE IS MORE THAN THE PRESET THRESHOLD---\n----TAKE APPROPRIATE ACTION----\n----THE LOCATION IS==>  https://www.google.com/maps/place/12%C2%B057'57.6%22N+77%C2%B042'43.9%22E/@12.966,77.7100113,17z/data=!3m1!4b1!4m5!3m4!1s0x0:0x0!8m2!3d12.966!4d77.7122  ---");
  delay(100);
  gsm.println((char)26); 
  delay(1000);
  Serial.println("Message is sent\n");
  if(msgsend==2)
  {
    delay(C);
    goto top;
  }
  else
  {
    delay(10000);
    goto doagain;
  }
}
}
