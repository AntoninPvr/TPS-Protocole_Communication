#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_MCP9808.h"
// Code ESP32 pour configurer un point d'accès WiFi hébergeant une page web simple
#include <WiFi.h>
#include <WebServer.h>
#include "index.h"                      // index.h contient le contenu des pages HTML

//---Déclaration des identifiants du point d'accès WiFi
const char* ssid     = "NodeMCUx_Anto";      //Nom du réseau WiFi que vous allez créer
const char* password = "123456789";     //mot de passe du réseau (8 caractères min)

// Create the MCP9808 temperature sensor object
Adafruit_MCP9808 tempsensor_0 = Adafruit_MCP9808();
Adafruit_MCP9808 tempsensor_1 = Adafruit_MCP9808();

// WiFi
WebServer server(80);   

// Timing
// =================
#define DELAY_TEMP 2000

uint32_t previous_millis_temp = 0;

// Config
// =================
union AlertConfig {
  uint16_t config;
  uint8_t bytes[2];
  struct {
    uint8_t alert_mode : 1;
    uint8_t alert_polarity : 1;
    uint8_t alert_select : 1;
    uint8_t alert_control : 1;
    uint8_t alert_status : 1;
    uint8_t interrupt_clear : 1;
    uint8_t window_locked : 1;
    uint8_t critical_locked : 1;
    uint8_t shutdown : 1;
    uint8_t t_hyst : 2;
    uint8_t reserved : 5;
  };
};

// Global variables
// =================

// Prototypes
// =================
void enable_alert(uint16_t temp);
uint16_t temp_2_bit(uint16_t temp);
//------------Déclaration des gestionnaires des requêtes HTTP------------
void handleRoot();
void handleADC(); 
void handleMCP0();
void handleMCP1();
void handleADC_Javascript();
void handleNotFound(); //Texte brute envoyé en cas de page inconnue


void setup() {
  Serial.begin(115200);
  if (!tempsensor_0.begin(0x18)) {
    Serial.println("Couldn't find MCP9808! Check your connections and verify the address is correct.");
    while (1);
  }
  if (!tempsensor_1.begin(0x19)) {
    Serial.println("Couldn't find MCP9808! Check your connections and verify the address is correct.");
    while (1);
  }

  Serial.println("Found MCP9808!");
  tempsensor_0.setResolution(3);
  tempsensor_1.setResolution(3);
  // Mode Resolution SampleTime
  //  0    0.5°C       30 ms
  //  1    0.25°C      65 ms
  //  2    0.125°C     130 ms
  //  3    0.0625°C    250 ms
  enable_alert(27);
  WiFi.mode(WIFI_AP);                   //Access Point mode
  WiFi.softAP(ssid,password);           //Init du Point d'Acces  WiFi
  Serial.print(WiFi.softAPIP());        //Affichage de l'adresse IP du Point d'Acces WiFi
  server.on("/", handleRoot);           //Redirection vers la fonction gestionnaire de la page web racine
  server.on("/ADC", handleADC);     //Redirection vers la fonction gestionnaire de la page web /hello
  server.on("/MCP0", handleMCP0);     //Redirection vers la fonction gestionnaire de la page web /hello
  server.on("/MCP1", handleMCP1);     //Redirection vers la fonction gestionnaire de la page web /hello
  server.on("/readADC", handleADC_Javascript);     //Redirection vers la fonction gestionnaire de la page web /hello
  server.onNotFound(handleNotFound);    //Redirection vers la fonction gestionnaire de page web invalide
  server.begin();      
}

void loop() {
  // put your main code here, to run repeatedly:
  tempsensor_0.wake();
  tempsensor_1.wake();
  uint32_t current_millis = millis();
  if (current_millis - previous_millis_temp >= DELAY_TEMP) {
    previous_millis_temp  = current_millis;
    float c_0 = tempsensor_0.readTempC();
    Serial.print("Temp 0: ");
    Serial.print(c_0);
    Serial.print("C\n");
    float c_1 = tempsensor_1.readTempC();
    Serial.print("Temp 1: ");
    Serial.print(c_1);
    Serial.print("C\n");
    Serial.print("Delta: ");
    Serial.print(c_0 - c_1);
    Serial.print("C\n");
  }
  server.handleClient();   
}

void enable_alert(uint16_t temp) {
  // Set the high and low temp
  union AlertConfig config;
  config.alert_mode = 0;
  config.alert_polarity = 0;
  config.alert_select = 1;
  config.alert_control = 1;
  config.alert_status = 0;
  config.interrupt_clear = 0;
  config.window_locked = 0;
  config.critical_locked = 0;
  config.shutdown = 0;
  config.t_hyst = 0;

  tempsensor_0.write16(MCP9808_REG_CONFIG, config.config);
  tempsensor_0.write16(MCP9808_REG_CRIT_TEMP, temp_2_bit(temp));
}

uint16_t temp_2_bit(uint16_t temp) {
  return temp << 4;
}

//------------Déclaration des gestionnaires des requêtes HTTP------------
void handleRoot() 
 {
 String s = MAIN_page;                  //Page HTML décrite dans index.h
 s.replace("XXX",String(millis()));     //Mise à jour de l'heure à afficher sur la page
 server.send(200, "text/html", s);      //Send web page
 }
void handleNotFound() {server.send(404, "text/plain", "404: Not found");} //Texte brute envoyé en cas de page inconnue

void handleADC()
{
  String s = MesureADC;                  //Page HTML décrite dans index.h
  server.send(200, "text/html", s);      //Send web page
}

void handleMCP0()
{
  String s = MesureMCP0;                  //Page HTML décrite dans index.h
  s.replace("XX",String(tempsensor_0.readTempC()));     //Mise à jour de l'heure à afficher sur la page
  server.send(200, "text/html", s);      //Send web page
}

void handleMCP1()
{
  String s = MesureMCP1;                  //Page HTML décrite dans index.h
  s.replace("XX",String(tempsensor_1.readTempC()));     //Mise à jour de l'heure à afficher sur la page
  server.send(200, "text/html", s);      //Send web page
}

void handleADC_Javascript()
{
int Vpot = random(0,4095);
String ADCdata =String("<?xml version = \"1.0\" ?><inputs><reading>")+String(Vpot)+String("</reading></inputs>");
server.send(200,"text/xml",ADCdata);
}
//-----------------------------------------------------------------------