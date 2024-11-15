/*
	Gerencia o arquivo de configurações do sistema!
*/
#include "SPIFFS.h"

bool StartConfig(struct Parametros *Config);
void StartPartition(struct Parametros *Config);
bool CreateConfig(struct Parametros *Config);
void ShowConfig(struct Parametros *Config);
void ConfigOld(struct Parametros *Config);
bool ConfigNewOld(struct Parametros *Config);


void StartPartition(struct Parametros *Config)
{
  // Inicializa o SPIFFS - Sistema de Arquivos no ESP32
  if (!SPIFFS.begin(false))
    {
      Serial.println(Msg_ParticaoNaoMontada);
      Serial.println(Msg_ParticaoFormatando);
      if (!SPIFFS.begin(true))
        {
          Serial.println(Msg_ParticaoNaoMontada);
        }
      else
        {
          Config->Particao = true;
          Serial.println(Msg_ParticaoMontada);
        }
    }
  else
    {
      Config->Particao = true;
      Serial.println(Msg_ParticaoMontada);
    }  
}

bool StartConfig(struct Parametros *Config)
{
  Config->Hydro.Min = preferences.getInt("H_Min", 100);
  Config->Hydro.Max = preferences.getInt("H_Max", 0); 
  Config->Hydro.Auto = preferences.getInt("H_Auto", 60); 
  Config->Temperatura.Min = preferences.getInt("T_Min", 100);
  Config->Temperatura.Max = preferences.getInt("T_Max", 0); 
  Config->Hardware.HostName = preferences.getString("Hostname","Kydro-IoT");
  Config->Hardware.SSID = preferences.getString("SSID","");
  Config->Hardware.PassWord = preferences.getString("Password","");

  Config->Clock.Full = preferences.getBool("Clock.Full",true);
  Config->DHCP = preferences.getBool("DHCP",true);

  Config->Localizacao.UF = preferences.getString("UF","SP");
  Config->Localizacao.Municipio = preferences.getString("Municipio","São Paulo");
  Config->Localizacao.Bairro = preferences.getString("Bairro","Vila Romana");
  Config->Localizacao.Logradouro = preferences.getString("Logradouro","Rua Cipião");
  Config->Localizacao.Numero = preferences.getString("Numero","###");
  Config->Localizacao.Complemento = preferences.getString("Complemento","Apto ###");
  Config->Localizacao.Andar = preferences.getString("Andar","## Andar");
  Config->Localizacao.Conjunto = preferences.getString("Conjunto","Area TI");
  Config->Localizacao.Local = preferences.getString("Local","Analise e Desenvolvimento de Sistemas");
  Config->Localizacao.CEP = preferences.getString("CEP","05047-060");
  Config->Localizacao.Fone = preferences.getString("Fone","(11) 9####-###8");
  Config->Localizacao.Ramal = preferences.getString("Ramal","1234");
  Config->Localizacao.Email = preferences.getString("Email","10408408@mackenzista.com.br");

  Config->MQTT.Server = preferences.getString("MQTTServer","broker.hivemq.com");
  Config->MQTT.User = preferences.getString("MQTTUser","");
  Config->MQTT.Password = preferences.getString("MQTTPassword","");
  Config->MQTT.Port = preferences.getInt("MQTTPort",1883);
  Config->MQTT.Enabled = preferences.getBool("MQTTEnabled",false);

  return true;
}

bool SaveConfig(struct Parametros *Config)
{
  preferences.putInt("H_Min", Config->Hydro.Min);
  preferences.putInt("H_Max", Config->Hydro.Max);
  preferences.putInt("H_Auto", Config->Hydro.Auto);
  preferences.putInt("T_Min", Config->Temperatura.Min);
  preferences.putInt("T_Max", Config->Temperatura.Max);
  preferences.putString("Hostname", Config->Hardware.HostName);
  preferences.putString("SSID", Config->Hardware.SSID);
  preferences.putString("Password", Config->Hardware.PassWord);

  preferences.putBool("Clock.Full", Config->Clock.Full);
  preferences.putBool("DHCP", Config->DHCP);

  preferences.putString("MQTTServer", Config->MQTT.Server);
  preferences.putString("MQTTUser", Config->MQTT.User);
  preferences.putString("MQTTPassword", Config->MQTT.Password);
  preferences.putInt("MQTTPort", Config->MQTT.Port);
  preferences.putBool("MQTTEnabled", Config->MQTT.Enabled);

  return true;
}

void ConfigNewToOld(struct Parametros *Config)
{
    Config->OldValue.Hydro.Value = Config->Hydro.Value;
    Config->OldValue.Hydro.Auto = Config->Hydro.Auto;
    Config->OldValue.Hydro.Min = Config->Hydro.Min;
    Config->OldValue.Hydro.Max = Config->Hydro.Max;
    Config->OldValue.Hydro.Alert.Min = Config->Hydro.Alert.Min;
    Config->OldValue.Hydro.Alert.Max = Config->Hydro.Alert.Max;

    Config->OldValue.Temperatura.Value = Config->Temperatura.Value;
    Config->OldValue.Temperatura.Min = Config->Temperatura.Min;
    Config->OldValue.Temperatura.Max = Config->Temperatura.Max;
    Config->OldValue.Temperatura.Alert.Min = Config->Temperatura.Alert.Min;
    Config->OldValue.Temperatura.Alert.Max = Config->Temperatura.Alert.Max;

    Config->OldValue.Level.Value = Config->Level.Value;
    Config->OldValue.Level.Min = Config->Level.Min;
    Config->OldValue.Level.Max = Config->Level.Max;
    Config->OldValue.Level.Alert.Min = Config->Level.Alert.Min;  
    Config->OldValue.Hardware.HostName = Config->Hardware.HostName;
}

bool ConfigNewOld(struct Parametros *Config)
{
  if (Config->OldValue.Hydro.Value != Config->Hydro.Value) {return true;}
  if (Config->OldValue.Hydro.Auto != Config->Hydro.Auto) {return true;}
  if (Config->OldValue.Hydro.Min != Config->Hydro.Min) {return true;}
  if (Config->OldValue.Hydro.Max != Config->Hydro.Max) {return true;}
  if (Config->OldValue.Hydro.Alert.Min != Config->Hydro.Alert.Min) {return true;}
  if (Config->OldValue.Hydro.Alert.Max != Config->Hydro.Alert.Max) {return true;}
  if (Config->OldValue.Temperatura.Value != Config->Temperatura.Value) {return true;}
  if (Config->OldValue.Temperatura.Min != Config->Temperatura.Min) {return true;}
  if (Config->OldValue.Temperatura.Max != Config->Temperatura.Max) {return true;}
  if (Config->OldValue.Temperatura.Alert.Min != Config->Temperatura.Alert.Min) {return true;}
  if (Config->OldValue.Temperatura.Alert.Max != Config->Temperatura.Alert.Max) {return true;}
  if (Config->OldValue.Level.Value != Config->Level.Value) {return true;}
  if (Config->OldValue.Level.Min != Config->Level.Min) {return true;}
  if (Config->OldValue.Level.Max != Config->Level.Max) {return true;}
  if (Config->OldValue.Level.Alert.Min != Config->Level.Alert.Min) {return true;}
  if (Config->OldValue.Hardware.HostName != Config->Hardware.HostName) {return true;}
  return false;
}  


void ConfigSend(struct Parametros *Config)
{
  // Necessário para o servidor web lidar com todos os clientes
  server.handleClient();                              
  // Função de atualização para os WebSockets
  webSocket.loop();                                   

  // criar uma string JSON para enviar dados ao cliente
  String jsonString = "";                           
  // criar um contêiner JSON
  StaticJsonDocument<200> doc;                      
  // criar um Objeto JSON
  JsonObject object2 = doc.to<JsonObject>();   
  JsonObject object = object2.createNestedObject("Config");      

  // escrever dados no objeto JSON 
  object["H_Min"] = Config->Hydro.Min;
  object["H_Max"] = Config->Hydro.Max;
  object["H_AlertMin"] = Config->Hydro.Alert.Min;
  object["H_AlertMax"] = Config->Hydro.Alert.Max;
  object["H_Value"] = Config->Hydro.Value;
  
  object["H_Auto"] = Config->Hydro.Auto;

  object["T_Min"] = Config->Temperatura.Min;
  object["T_Max"] = Config->Temperatura.Max;
  object["T_AlertMin"] = Config->Temperatura.Alert.Min;
  object["T_AlertMax"] = Config->Temperatura.Alert.Max;
  object["T_Value"] = Config->Temperatura.Value;

  object["L_Value"] = Config->Level.Value;
  object["L_Min"] = Config->Level.Min;
  object["L_Max"] = Config->Level.Max;
  object["L_AlertMin"] = Config->Level.Alert.Min;

  object["ClockShow"] = Config->Clock.Show;
  object["ClockFormat"] = Config->Clock.Full;
  object["ShowUptime"] = Config->UpTime.Show;

  object["HostName"] = Config->Hardware.HostName;
  object["SSID"] = Config->Hardware.SSID;
  object["Pwd"] = Config->Hardware.PassWord;
  object["BootCount"] = Config->BootCount;

  object["ServerMQTT"] = String(Config->MQTT.Server);
  object["UserMQTT"] = String(Config->MQTT.User);
  object["PassMQTT"] = String(Config->MQTT.Password);
  object["PortMQTT"] = Config->MQTT.Port;
  object["EnabledMQTT"] = Config->MQTT.Enabled;


  object["UF"] = Config->Localizacao.UF;
  object["Municipio"] = Config->Localizacao.Municipio;
  object["Bairro"] = Config->Localizacao.Bairro;
  object["Logradouro"] = Config->Localizacao.Logradouro;
  object["Numero"] = Config->Localizacao.Numero;
  object["Complemento"] = Config->Localizacao.Complemento;
  object["Andar"] = Config->Localizacao.Andar;
  object["Conjunto"] = Config->Localizacao.Conjunto;
  object["Local"] = Config->Localizacao.Local;
  object["CEP"] = Config->Localizacao.CEP;
  object["Email"] = Config->Localizacao.Email;
  object["Fone"] = Config->Localizacao.Fone;
  object["Ramal"] = Config->Localizacao.Ramal;
      
  // converter objeto JSON para string
  serializeJson(doc, jsonString);                   
  
  // imprimir string JSON no console para fins de depuração (você pode comentar isso)
  //Serial.println(jsonString);                       

  // enviar string JSON para os clientes
  webSocket.broadcastTXT(jsonString);  
}