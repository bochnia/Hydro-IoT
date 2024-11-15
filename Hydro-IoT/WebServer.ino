extern struct Parametros Config;

void WebServer(struct Parametros *Config)
{
  // Necessário para o servidor web lidar com todos os clientes
  server.handleClient();                              
  // Função de atualização para os WebSockets
  webSocket.loop();                                   

  // verificar se "interval" ms se passaram desde a última vez que os clientes foram atualizados
  if ((Config->Web.Agora) || ((unsigned long)(millis() - Config->Web.mSeg) > Config->Web.Interval))
  {
    Config->Web.mSeg = millis(); 
    if (Config->Web.Agora || ConfigNewOld(Config))
    {
      Config->Web.Agora = false;
      // criar uma string JSON para enviar dados ao cliente
      String jsonString = "";                           
      // criar um contêiner JSON
      StaticJsonDocument<200> doc;                      
      // criar um Objeto JSON
      JsonObject object2 = doc.to<JsonObject>();   
      JsonObject object = object2.createNestedObject("Dados");      

      // escrever dados no objeto JSON 
      object["H_Min"] = Config->Hydro.Min;
      object["H_Max"] = Config->Hydro.Max;
      object["H_AlertMin"] = Config->Hydro.Alert.Min;
      object["H_AlertMax"] = Config->Hydro.Alert.Max;
      object["H_Value"] = Config->Hydro.Value;
      
      object["H_Auto"] = Config->Hydro.Auto;

      object["H_Smoke"] = (Config->Hydro.Smoke ? "Ativado" : "");

      object["T_Min"] = Config->Temperatura.Min;
      object["T_Max"] = Config->Temperatura.Max;
      object["T_AlertMin"] = Config->Temperatura.Alert.Min;
      object["T_AlertMax"] = Config->Temperatura.Alert.Max;
      object["T_Value"] = Config->Temperatura.Value;

      object["L_Value"] = Config->Level.Value;
      object["L_Min"] = Config->Level.Min;
      object["L_Max"] = Config->Level.Max;
      object["L_AlertMin"] = Config->Level.Alert.Min;

      time_t now;
      time(&now);
      object["ESP_Clock"] = now;
    
      object["ESP_Uptime"] = Config->Web.mSeg;
    
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
     // Serial.println(jsonString);                       

      // enviar string JSON para os clientes
      webSocket.broadcastTXT(jsonString);  

      ConfigNewToOld(Config);

    }
  }
}  

void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length)
{ 
  switch (type) 
  {
  case WStype_DISCONNECTED:                         
    Serial.println("Client " + String(num) + " disconnected");
    break;

  case WStype_CONNECTED:
    Serial.println("Client " + String(num) + " connected");
    ConfigSend(&Config);
    Config.Web.Agora = true;
    break;

  case WStype_TEXT:
    StaticJsonDocument<200> doc;                    
    DeserializationError error = deserializeJson(doc, payload);
    if (error) 
      {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }
    else 
      {
        // Serializa o JSON para uma string 
        serializeJson(doc, Serial);

        const char* tipo = doc["Tipo"]; 
      //    Serial.printf("Tipo de dado %s\n", tipo);
        if (strcmp(tipo, "Update") == 0)
        {
          Config.Hydro.Auto = doc["H_Auto"];
          Config.Hydro.Alert.Min = doc["H_AlertMin"];
          Config.Hydro.Alert.Max = doc["H_AlertMax"];
          Config.Temperatura.Alert.Min = doc["T_AlertMin"];
          Config.Temperatura.Alert.Max = doc["T_AlertMax"];
          Config.Level.Min = doc["L_Min"];
          Config.Clock.Full = (doc["ClockFormat"] == "1");
          Config.Clock.Show = doc["ClockShow"];
          Config.Hardware.HostName = doc["DeviceName"].as<String>();

          Config.MQTT.Server = doc["ServerMQTT"].as<String>(); 
          Config.MQTT.User = doc["UserMQTT"].as<String>(); 
          Config.MQTT.Password = doc["PassMQTT"].as<String>(); 
          Config.MQTT.Port = doc["PortMQTT"];
          Config.MQTT.Enabled = doc["EnabledMQTT"];
          SaveConfig(&Config);
        }

        if (strcmp(tipo, "Reset") == 0)
        {
          Config.Hydro.Max = 0;
          Config.Hydro.Min = 100;
          Config.Temperatura.Max = 0;
          Config.Temperatura.Min = 100;
        }
        // Recebe o comando para disparar o Scan de device na rede
        if (strcmp(tipo, "Scan") == 0)
        {
          Config.Devices.Scan = true;
        }
      }
    Serial.println("");
    break;
  }
}