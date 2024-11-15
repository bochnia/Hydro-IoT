/*
  Autor: Ricardo Bochnia dos Anjos
  Data:  Out/2024
  Ver:   1.0
  Local: São Paulo - SP - Brasil
*/

#include <WiFi.h>              // necessário para conectar ao WiFi
#include <WebServer.h>         // necessário para criar um servidor web simples
#include <WebSocketsServer.h>  // necessário para comunicação instantânea entre cliente e servidor através de Websockets
#include <ArduinoJson.h>       // necessário para encapsulação JSON (enviar várias variáveis com uma string)
#include <HTML.h>
#include <style.h>
#include <script.h>
#include <icone.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiAP.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <Preferences.h>
#include <esp_sntp.h>
#include <DHTesp.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_VL53L0X.h>
#include <Classes.h>
#include <Const.h>
#include <PubSubClient.h>

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

//RTC_DS1307 rtc;  //OBJETO DO TIPO RTC_DS1307
DHTesp dht;
#define pin1 4

LiquidCrystal_I2C lcd(0x27, 20, 4);  // Chamada da funcação LiquidCrystal para ser usada com o I2C

const char *ntpServer1 = "pool.ntp.org";
const char *ntpServer2 = "time.nist.gov";
const long gmtOffset_sec = -5400;
const int daylightOffset_sec = -5400;

const char *DeviceName = "Hydro-IoT";

//const char *time_zone = "CET-1CEST,M3.5.0,M10.5.0/3";  // TimeZone rule for Europe/Rome including daylight adjustment rules (optional)
//const char *time_zone = "BRT3BRST,M10.3.0/0,M2.3.0/0"; // Com horario de Verão
const char *time_zone = "BRT3";  // Sem horario de verão <-- ***
/*
1 - BRT3BRST:
    - BRT: Abreviação para o horário padrão de Brasília (Brasília Time).
    - 3: Diferença de horas em relação ao UTC (UTC-3).
    - BRST: Abreviação para o horário de verão de Brasília (Brasília Summer Time).
2 - M10.3.0/0:
    - M10: Indica o mês de outubro.
    - 3: Indica a terceira semana do mês.
    - 0: Indica o domingo (0 = domingo, 1 = segunda-feira, etc.).
    - /0: Indica a hora do dia em que a mudança ocorre (0 = meia-noite).
  Isso significa que o horário de verão começa no terceiro domingo de outubro à meia-noite.
3 - M2.3.0/0:
    - M2: Indica o mês de fevereiro.
    - 3: Indica a terceira semana do mês.
    - 0: Indica o domingo.
    - /0: Indica a hora do dia em que a mudança ocorre.
  Isso significa que o horário de verão termina no terceiro domingo de fevereiro à meia-noite.
*/


// Funções Externas
extern void CtrlSmoke(struct Parametros *Config);
extern void Display(struct Parametros *Config);
extern void DisplayClock(struct Parametros *Config);
extern void DisplayHeart(boolean Smoke);
extern int DisplayHydro(struct Parametros *Config);
extern void DisplayIPv4(struct Parametros *Config);
extern void DisplayLed(struct Parametros *Config);
extern void DisplayLevel(struct Parametros *Config);
extern void DisplayMinMax(struct Parametros *Config);
extern void DisplayTime(struct Parametros *Config);
extern void DisplayUpTime(struct Parametros *Config);
extern void DisplayMQTT(struct Parametros *Config);
extern void callbackMQTT(char* topic, byte* payload, unsigned int length);
extern void printLocalTime();
extern String printTimer(struct Parametros *Config);
extern void Splash(struct Parametros *Config);
extern void timeAvailable(struct timeval *t);
extern void Webserver();
extern void ConfigDefault(struct Parametros *Config);
extern bool StartConfig(struct Parametros *Config);
extern bool SaveConfig(struct Parametros *Config);
extern void StartPartition(struct Parametros *Config);
extern bool CreateConfig(struct Parametros *Config);
extern void ShowConfig(struct Parametros *Config);
extern void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length);
std::vector<DeviceInfo> devices;
Preferences preferences;

Parametros Config;

WiFiUDP udp;
const char* broadcastAddress = "255.255.255.255";
const unsigned int localUdpPort = 4210;  // Porta para ouvir
char incomingPacket[255];  // Buffer para dados recebidos
bool udpInitialized = false;

WiFiClient espClient;
PubSubClient clientMQTT(espClient);

// Initialization of webserver and websocket
WebServer server(80);                                 // the server uses port 80 (standard port for websites
WebSocketsServer webSocket = WebSocketsServer(81);    // the websocket uses port 81 (standard port for websockets

void setup() {
  ConfigDefault(&Config);

  pinMode(pin1, OUTPUT);

  digitalWrite(pin1, LOW);

  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  randomSeed(micros());

  preferences.begin("Hydro-IoT", false);

  Config.BootCount = preferences.getInt("bootCount", 0);
  Config.BootCount++;
  preferences.putInt("bootCount", Config.BootCount);  

  lcd.init();       // Iniciar a comunicação com o display já conectado
  lcd.backlight();  // Ligar a luz do display
  lcd.clear();      // Limpar a tela do display

  Splash(&Config);

  Serial.begin(115200);  // Output status on Uno serial monitor

  Serial.printf("\n%s\n",DeviceName);

  StartPartition(&Config);  // Ativa a Partição de arquivos
  StartConfig(&Config);     // Iniciar a leitura das configurações

  dht.setup(Config.Hydro.Pino, DHTesp::DHT22);

  
  if (!lox.begin()) 
  {
    Serial.println("** Falha ao iniciar o Sensor de Nivel **");
    Config.Level.Enabled = false;
  }
  else
  {
    // start continuous ranging
    lox.startRangeContinuous();
    Config.Level.Enabled = true;
  }

  if ( (Config.Hardware.SSID == "") || (Config.Hardware.PassWord == ""))
    {
      Serial.printf("Connecting to WiFi %s - Password: 123456789\n", DeviceName);
      WiFi.softAP(DeviceName,"12345678");
      IPAddress myIP = WiFi.softAPIP();
      Serial.print("AP IP address: ");
      Serial.println(myIP);
      if (MDNS.begin(DeviceName))
      {
        Serial.printf("MDNS esponder started '%s'\n",DeviceName);
      }
    }
  else
    {
      Serial.printf("Connecting to WiFi %s\n", Config.Hardware.SSID);
      Serial.printf("Connecting to Password %s\n", Config.Hardware.PassWord);
      WiFi.begin(Config.Hardware.SSID, Config.Hardware.PassWord);

      /**
      * O endereço do servidor NTP pode ser adquirido via DHCP,
      *
      * NOTA: Esta chamada deve ser feita ANTES do ESP32 adquirir o endereço IP via DHCP,
      * caso contrário, a opção 42 do SNTP será rejeitada por padrão.
      * NOTA: A chamada da função configTime() se feita APÓS a execução do cliente DHCP
      * irá SOBRESCREVER o endereço do servidor NTP adquirido
      */

      esp_sntp_servermode_dhcp(1);  // (optional)

      if (WiFi.status() != WL_CONNECTED) 
        {
          Serial.println("Conectado");
          Serial.println(WiFi.macAddress());
        } 
      else 
        {
          Serial.println("Não Conectado");
        }
      WiFi.setHostname(Config.Hardware.HostName.c_str());  //define hostname
    
      Serial.print("Default Hostname: ");
      Serial.println(WiFi.getHostname());
      
      // set notification call-back function
      sntp_set_time_sync_notification_cb(timeAvailable);

      // Isso definirá os servidores NTP configurados e o fuso horário/deslocamento "TimeZone/daylightOffset" de horário de verão constante.
      // Deve estar OK se o seu fuso horário não precisar ajustar o deslocamento "daylightOffset" de horário de verão duas vezes por ano,
      // nesse caso, o ajuste de horário não será tratado automaticamente.
      configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);  

      // Uma abordagem mais conveniente para lidar com fusos horários "TimeZones" com deslocamento de horário de verão "daylightOffset"
      // seria especificar uma variável de ambiente com a definição do fuso horário "TimeZone", incluindo as regras de ajuste de horário de verão.
      // Uma lista de regras para sua zona pode ser obtida em https://github.com/esp8266/Arduino/blob/master/cores/esp8266/TZ.h
      configTzTime(time_zone, ntpServer1, ntpServer2);
    }

  // Compactar o HTML e armazenar na variável global
  //index_html_gz = gzip_compress((uint8_t*)index_html, sizeof(index_html), &index_html_gz_len);

  // Define aqui o que o servidor web precisa fazer
  // ele precisa enviar a string HTML "webpage" para o cliente
  // Definir o manipulador para a página inicial 
  server.on("/", []() { 
                        server.sendHeader("Cache-Control", "no-cache"); 
                        server.sendHeader("X-Content-Type-Options", "nosniff");
                        server.send_P(200, "text/html; charset=utf-8", index_html); 
                      });
  // Definir o manipulador para o arquivo CSS
  server.on("/style.css", []() { server.sendHeader("X-Content-Type-Options", "nosniff");  
                                 server.send_P(200, "text/css; charset=utf-8", style_css); 
                                }); 
  // Definir o manipulador para o arquivo JavaScript 
  server.on("/script.js", []() { 
                                 server.sendHeader("X-Content-Type-Options", "nosniff"); 
                                 server.send_P(200, "application/javascript; charset=utf-8", script_js); 
                                });
  // Define o favicon.inco para envio ao cliente web
  server.on("/favicon.ico", []() { 
                                   server.sendHeader("X-Content-Type-Options", "nosniff"); 
                                   server.send_P(200, "image/x-icon", Favicon, sizeof(Favicon)); 
                                  });


  // iniciar servidor
  server.begin();                            

  // iniciar websocket
  webSocket.begin();                         

  // definir uma função de callback -> o que o ESP32 precisa fazer quando 
  // um evento do websocket é recebido? -> executar a função "webSocketEvent()"
  webSocket.onEvent(webSocketEvent);         
  
  delay(1000);

  Display(&Config);

  Config.Hydro.Enabled = true;
  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  clientMQTT.setServer(Config.MQTT.Server.c_str(), Config.MQTT.Port);
  clientMQTT.setCallback(callbackMQTT);

  // Inicializa o UDP para ouvir na porta especificada      
  udp.begin(localUdpPort);  
  udpInitialized = true;
}

void loop() 
{
  DisplayLed(&Config);  
  DisplayClock(&Config);
  DisplayLevel(&Config);
  DisplayUpTime(&Config);
  DisplayHydro(&Config);
  CtrlSmoke(&Config);
  WebServer(&Config);
  Scan(&Config);
  DisplayMQTT(&Config);
}
