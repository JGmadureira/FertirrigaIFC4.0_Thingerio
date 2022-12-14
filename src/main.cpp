#include <Arduino.h>


//#define DISABLE_TLS 
#define THINGER_SERIAL_DEBUG
#define THINGER_FREE_RTOS
#define THINGER_INSECURE_SSL
#define _DEBUG_
#include <ThingerESP32OTA.h>

#include <ThingerESP32.h>
#include "ThingerClient.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "math.h"
//#include <ThingerCore32.h>
#include "arduino_secrets.h"
// Requires WifiManager from Library Manager or https://github.com/tzapu/WiFiManager
// At least version 2.0 (available on master)
//#include <ThingerESP32WebConfig.h>

//ThingerESP32WebConfig thing;
//1. Connect to Thinger-Device WiFi with your computer or phone, using thinger.io as WiFi password

//ThingerESP32 thingOn(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);
ThingerESP32 thingS(USERNAME, DEVICE_IDS, DEVICE_CREDENTIALS);

// https://docs.thinger.io/coding/coding-guide
//Link dashboard https://backend.thinger.io/dashboards/ControleDigitais?authorization=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJqdGkiOiJEYXNoYm9hcmRfQ29udHJvbGVEaWdpdGFpcyIsInN2ciI6InVzLWVhc3QuYXdzLnRoaW5nZXIuaW8iLCJ1c3IiOiJNb3JhbmdvRmVydGlycmlkYSJ9.t1lVv4o8OLcMgrfjKf9rFHYQNQAKZoIsnQ8eXTI3-RY
// Initialize ESP32OTA OTA
// use Thinger.io VSCode Studio extension + Platformio to upgrade the device remotelly
//ThingerESP32OTA ota(thing);
//#define MODO_SENSORES true
//#define MODO_RELES false
//#define MODO_DEBUG false


#define NUM_SENSOR 6
String Medicoes="";
int SENSORES_ADC[NUM_SENSOR] = {32, 33, 34, 35, 36, 39};
bool vartest_S =0;
bool ledresposta_S=0;


#define DHTPIN 4
#define DHTTYPE DHT11           //Tipo do DHT
DHT dht(DHTPIN, DHTTYPE);     // Criando objeto dht

int t =0.0;
int h =0.0;
/*
#define NUM_RELAYS 12
#define ValvulaLigada LOW
#define ValvulaDesligada HIGH
int relayGPIOs[NUM_RELAYS] =          {2, 13, 14, 27, 26, 25, 33, 32, 16, 17, 4, 15};
bool vartest =0;
bool ledresposta=0;
int i = 0;
*/
void setup() {
  //pinMode(2, OUTPUT);
  //pinMode(0, INPUT);

  // open serial for debugging
  Serial.begin(115200);
  thingS.add_wifi(SSID, SSID_PASSWORD);
  dht.begin();
  for (int s=0; s<= NUM_SENSOR; s++) { 
    pinMode(SENSORES_ADC[s], INPUT);
  }
  
  /*
  pinMode(relayGPIOs[0], OUTPUT);
  for (i ; i <= 12; i++) {
    pinMode(relayGPIOs[i], OUTPUT); 
    digitalWrite(relayGPIOs[i], ValvulaDesligada); //Inicia todas portas em HIGH pro sistema desligar os reles. HIGH relé DESLIGADO
  };

  thingOn.add_wifi(SSID, SSID_PASSWORD);
 
  // digital pin control example (i.e. turning on/off a light, a relay, configuring a parameter, etc)
  thingOn["VAgua"] << inverted_digital_pin(relayGPIOs[0]);
  thingOn["VRetAdb1"] << inverted_digital_pin(relayGPIOs[1]);
  thingOn["VAdb1"] << inverted_digital_pin(relayGPIOs[2]);
  thingOn["VRetAdb2"] << inverted_digital_pin(relayGPIOs[3]);
  thingOn["VAdb2"] << inverted_digital_pin(relayGPIOs[4]);
  thingOn["VL6"] << inverted_digital_pin(relayGPIOs[5]);
  thingOn["VL1"] << inverted_digital_pin(relayGPIOs[6]);
  thingOn["VL2"] << inverted_digital_pin(relayGPIOs[7]);
  thingOn["VL3"] << inverted_digital_pin(relayGPIOs[8]);
  thingOn["VL4"] << inverted_digital_pin(relayGPIOs[9]);
  thingOn["VL5"] << inverted_digital_pin(relayGPIOs[10]);
  thingOn["VBomba"] << inverted_digital_pin(relayGPIOs[11]);
  // resource output example (i.e. reading a sensor value)
  thingOn["ledresposta"] >> outputValue(ledresposta);
  thingOn["vartest"] << inputValue(vartest);

  */
  // resource output example (i.e. reading a sensor value)
  // thingS["SL1"] >> outputValue(analogRead(SENSORES_ADC[0]));
 


  thingS["ledresposta"] >> outputValue(ledresposta_S);
  thingS["vartest"] << inputValue(vartest_S);

 //Habilita o watchdog configurando o timeout para 4 segundos
  esp_task_wdt_init(8, true);
  esp_task_wdt_add(NULL);
  
  // more details at http://docs.thinger.io/arduino/
}


void loop() {
  Medicoes = "";

  for (int s=0 ; s <= 6; s++) {
    Medicoes.concat(analogRead(SENSORES_ADC[s])); 
    Medicoes.concat(" ");
  };

  
  h = dht.readHumidity()*100;    // função que lê e guarda a humidade
  t = dht.readTemperature()*100; // função que lê e guarda a temperatura



  if (isnan(h) || isnan(t))
  {
    Serial.println(F("Falha ao ler o sensor DHT"));
    Serial.println("\n");
    delay(2000);
  }

  //digitalWrite(2, digitalRead(0));
  
  //thingOn.handle();
  //ledresposta = vartest ;
  ledresposta_S = vartest_S ;
  for(int t=0; t<=NUM_SENSOR; t++){
    int teste = analogRead(SENSORES_ADC[t]);
    Serial.println(teste);
    
  }
  delay(2000);
  thingS["Leituras"] >> [](pson & out){
    out["SL1"] = analogRead(SENSORES_ADC[0]);
    out["SL2"] = analogRead(SENSORES_ADC[1]);
    out["SL3"] = analogRead(SENSORES_ADC[2]);
    out["SL4"] = analogRead(SENSORES_ADC[3]);
    out["SL5"] = analogRead(SENSORES_ADC[4]);
    out["SL6"] = analogRead(SENSORES_ADC[5]);
    out["TA"] = (float)t/100.0;
    out["UA"] = (float)h/100.0;
    //out["Last"] = Medicoes;
  };
  thingS.handle();
  //Reseta o temporizador do watchdog
  esp_task_wdt_reset();

  
}

/*
 * Alteração na lib ThigerClient.h
 * linha 598
 * inline void inverted_digital_pin(protoson::pson& in, int pin, bool& current_state){
    if(in.is_empty()) {
        in = current_state; ///!current_state; /// removi o ! para funcionar com a lógia invertida
    }
    else{
        current_state = in;
        digitalWrite(pin, current_state ? LOW : HIGH);
    }
}
#endif
*/

  