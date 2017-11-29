# include <ArduinoJson.h>
# include <SPI.h>
# include <WiFi101.h>
# include <OneWire.h>
# include <DallasTemperature.h>
# include <Servo.h>

// Author : Lam Ngo
# define ONE_WIRE_BUS 11
# define TEMPERATURE PRECISION 9
# define LIGHT_ONE A4
# define LIGHT_TWO A0
# define LIGHT_THREE A2
# define LIGHT_FOUR 1

char ssid[] = ”AndroidAP”; //my hotspot SSID
char pass [ ] = ”” ; //I set it as an open network for testing
int keyIndex = 0;
int lightCell1 = A1;
int lightCell2 = A3;

const int breakbeam = 10 ;
const int digital = 9;
bool triggered = false;
Servo myservo;
int carcount = 0 ;
int status = WL_IDLE_STATUS ;
char server [] = ”q16j6d1ynd.execute-api.us-east-1.amazonaws.com”;

char c;
String response;
int t0 = 0;
int t1 = 0;
WiFiSSLClient client;
DynamicJsonBuffer jsonBuffer;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress lightTemp1 , lightTemp2;
void setup ( ) {
    WiFi.setPins(8,7,4,2);
    Serial.begin(9600);
    sensors.begin();
    sensors.getAddress(lightTemp1,0);
    sensors.getAddress(lightTemp2,1);
    sensors.setResolution(lightTemp1,TEMPERATURE_PRECISION) ;
    sensors.setResolution(lightTemp2,TEMPERATURE_PRECISION) ;
    //Setting up pins
    pinMode(breakbeam,INPUT);
    pinMode(digital,INPUT);
    pinMode (LIGHT_ONE, OUTPUT) ;
    pinMode (LIGHT_TWO, OUTPUT) ;
    pinMode (LIGHT_THREE, OUTPUT) ;
    pinMode (LIGHT_FOUR, OUTPUT) ;
    digitalWrite(breakbeam,HIGH) ; // turn on the pullup
    attachInterrupt(breakbeam,breakbeamupdate,CHANGE) ;
    attachInterrupt(digital,digitalsensorupdate,CHANGE) ;
    myservo.attach(12);
    myservo.write(30);
    initiateNetWork();
    t0 = millis();
}

//Int e r rupt breakbeam
void breakbeamupdate ( ) {
    myservo.write(90);
    carcount = carcount + 1;
    triggered = true;
}

//Interrupt digital sensor
void digitalsensorupdate(){
    myservo.write(30);
    if (carcount > 0) {
        carcount = carcount 􀀀 1 ;
    }
}


void loop(){
    t1 = millis();
    if (t1 - t0 > 10000){
      //Update table if 10 seconds have passed
      Serial.println(”doing this”);
      sensors.requestTemperatures();
      addTempAndLight(sensors.getTempC(lightTemp1) , sensors.getTempC(lightTemp2) ,analogRead(A1)
      t0 = millis();
      no = true;
      delay(1000);
    }
    else{
      sendJSON(”GetState”,analogRead(A1),analogRead(A3),carcount);
      //Let the cloud decide the state.
      response = ””;
      while (client.available()) {
        //CLOUD INTERACTION
        //Recieve and save the response from the lambda function
        c = client.read();
        response += c;
        Serial.write(c);
    }
    String state = response.substring(response.length() - 2,response.length() - 1);
    //Processing response from the cloud
    if (state.equals(”0”)){
        Serial.println(”Recieve 0 from the cloud, turning light off”);
        digitalWrite(LIGHT_ONE,LOW);
        digitalWrite(LIGHT_TWO,LOW);
        digitalWrite(LIGHT_THREE,LOW);
        digitalWrite(LIGHT_FOUR,LOW);
     } else if(state.equals(”1”)){
        Serial.println(”Recieve 1 from the cloud, turning light on”);
        digitalWrite(LIGHT_ONE,HIGH);
        digitalWrite(LIGHT_TWO,HIGH);
        digitalWrite(LIGHT_THREE,HIGH);
        digitalWrite(LIGHT_FOUR,HIGH);
    }
    client.stop();
    if (triggered) {
      //update VehicleCount table
      Serial.println(carcount);
      addCarCount(carcount,millis());
      go1 = false;
    }
  }
}

void initiateNetWork() {
    //check for the presence of the shield:
    if (WiFi.status () == WL_NO_SHIELD) {
        Serial.println(”WiFi shield not present”);
        //don’t continue:
        while (true);
    }
    // attempt to connect to Wifi network:
    // CLOUD INTERACTION
    while (status != WLCONNECTED) {
        Serial.print (”Attempting to connect to SSID : ” );
        Serial.println(ssid);
        //Connect to WPA/WPA2 network . Change this line if using open or WEP network :
        status = WiFi.begin(ssid);
        // wait 10 seconds for connection:
        delay(10000);
    }
    Serial.println(”Connected to wifi”);
    printWifiStatus();
}

void addCarCount (int carCount , int time) {
    //Send a HTTP Post to the server
    //CLOUD INTERACTION HERE
    Serial.println( ”\n Starting connection to server ...”);
    // if you get a connection, report back via serial:
    if (client.connectSSL(server, 443)){
        Serial.println(”connected to server”);
        // Make a HTTP reques t :
        String req = ”{\” operationn” : n”addn” , n” carcount n” : 20 , n” time n” : 1000g ” ;
        S t r ing reques t = ”” ;
        J sonObjec t& root = j sonBuf f e r . par seObjec t ( req ) ;
        root [ ” carcount ” ] = carCount ;
        root [ ” time ” ] = t i me ;
        root . pr intTo ( reques t ) ;
        c l i e n t . pr int ln ( ”POST /Tes t ing/vehi c l e s count HTTP/1.1 ” ) ;
        c l i e n t . pr int ln ( ”Host : q16j6d1ynd . execute􀀀api . us􀀀eas t 􀀀1.amazonaws . com” ) ;
        // c l i e n t . pr int ln ( ”x􀀀api􀀀key : FvNDOWhGNA8EwRSSpIfCO2Nbe50kt6Yt1kkwKWTX” ) ;
        22
        c l i e n t . pr int ( ”Content􀀀Length : ” ) ;
        c l i e n t . pr int ( reques t . length ( ) ) ;
        c l i e n t . pr int ( ”nnnn” ) ;
        c l i e n t . pr int ( reques t ) ;
        g e l s e f
        S e r i a l . pr int ln ( ”nnCan ’ t connect ” ) ;
g
delay ( 2 0 0 0 ) ;
g
void addTempAndLight ( i n t temp1 , i n t temp2 , i n t l ight 1 , i n t l ight 2 , i n t t i me )f
// Send a HTTP Post to the s e rve r
// CLOUD INTERACTION HERE
S e r i a l . pr int ln ( ”n nSt a r t ing connect ion to s e rve r . . . ” ) ;
// i f you get a connection , r epor t back via s e r i a l :
i f ( c l i e n t . connectSSL ( server , 4 4 3 ) ) f
S e r i a l . pr int ln ( ” connected to s e rve r ” ) ;
// Make a HTTP reques t :
S t r ing req = ”fn” operat ion n” : n”addn” , n”temp1n” : 20 , n”temp2n” : 25 , n” l i g h t 1 n” : 1 , n” l S t r ing reques t = ”” ;
J sonObjec t& root = j sonBuf f e r . par seObjec t ( req ) ;
root [ ”temp1” ] = temp1 ;
root [ ”temp2” ] = temp2 ;
root [ ” l i g h t 1 ” ] = l i g h t 1 ;
root [ ” l i g h t 2 ” ] = l i g h t 2 ;
root [ ” temp time ” ] = t i me ;
root . pr intTo ( reques t ) ;
c l i e n t . pr int ln ( ”POST /Tes t ing/tempandlight HTTP/1.1 ” ) ;
c l i e n t . pr int ln ( ”Host : q16j6d1ynd . execute􀀀api . us􀀀eas t 􀀀1.amazonaws . com” ) ;
// c l i e n t . pr int ln ( ”x􀀀api􀀀key : FvNDOWhGNA8EwRSSpIfCO2Nbe50kt6Yt1kkwKWTX” ) ;
c l i e n t . pr int ( ”Content􀀀Length : ” ) ;
c l i e n t . pr int ( reques t . length ( ) ) ;
c l i e n t . pr int ( ”nnnn” ) ;
c l i e n t . pr int ( reques t ) ;
g e l s e f
S e r i a l . pr int ln ( ”nnCan ’ t connect ” ) ;
g
delay ( 2 0 0 0 ) ;
g
void sendJSON( S t r ing type , i n t value1 , i n t value2 , i n t value3 )f
// Send a HTTP Post to the s e rve r
// CLOUD INTERACTION HERE
S e r i a l . pr int ln ( ”n nSt a r t ing connect ion to s e rve r . . . ” ) ;
// i f you get a connection , r epor t back via s e r i a l :
23
i f ( c l i e n t . connectSSL ( server , 4 4 3 ) ) f
S e r i a l . pr int ln ( ” connected to s e rve r ” ) ;
// Make a HTTP reques t :
S t r ing req = ”fn” idn” : n” de f aul t n” , n” l i g h t 1 n” : 20 , n” l i g h t 2 n” : 25 , n”incomingn” : 1g” ;
S t r ing reques t = ”” ;
J sonObjec t& root = j sonBuf f e r . par seObjec t ( req ) ;
root [ ” id” ] = type ;
root [ ” l i g h t 1 ” ] = value1 ;
root [ ” l i g h t 2 ” ] = value2 ;
root [ ”incoming” ] = value3 ;
root . pr intTo ( reques t ) ;
c l i e n t . pr int ln ( ”POST /Tes t ing/ l i g h t s t a t e HTTP/1.1 ” ) ;
c l i e n t . pr int ln ( ”Host : q16j6d1ynd . execute􀀀api . us􀀀eas t 􀀀1.amazonaws . com” ) ;
// c l i e n t . pr int ln ( ”x􀀀api􀀀key : FvNDOWhGNA8EwRSSpIfCO2Nbe50kt6Yt1kkwKWTX” ) ;
c l i e n t . pr int ( ”Content􀀀Length : ” ) ;
c l i e n t . pr int ( reques t . length ( ) ) ;
c l i e n t . pr int ( ”nnnn” ) ;
c l i e n t . pr int ( reques t ) ;
g e l s e f
S e r i a l . pr int ln ( ”nnCan ’ t connect ” ) ;
g
delay ( 2 0 0 0 ) ;
g
void pr intWi f i S t a tus ( ) f
// pr int the SSID of the network you ’ re at tached to :
S e r i a l . pr int ( ”SSID : ” ) ;
S e r i a l . pr int ln (WiFi . SSID ( ) ) ;
// pr int your WiFi shield ’ s IP address :
IPAddress ip = WiFi . l o c a l IP ( ) ;
S e r i a l . pr int ( ” IP Address : ” ) ;
S e r i a l . pr int ln ( ip ) ;
// pr int the received s igna l s t r ength :
long r s s i = WiFi . RSSI ( ) ;
S e r i a l . pr int ( ” s igna l s t r ength ( RSSI ) : ” ) ;
S e r i a l . pr int ( r s s i ) ;
S

