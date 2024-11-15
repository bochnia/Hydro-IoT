void Scan(struct Parametros *Config)
{
  // Verificar se a variável de controle está ativa para enviar a mensagem de descoberta
  if (Config->Devices.Scan)
  {
    // Desativar a variável de controle após o envio
    Config->Devices.Scan = false;
    Config->Devices.NewDevice = false;

    const char* message = "Hydro-IoT on the network respond?";
    Serial.println("Broadcast enviando...");
    udp.beginPacket(broadcastAddress, localUdpPort);
    udp.write(reinterpret_cast<const uint8_t*>(message), strlen(message));
    udp.endPacket();
    Serial.printf("Broadcast enviado %s\n",message);
    Config->Devices.mSeg = millis();
  }

  // Verificar se há pacotes recebidos
  if (udpInitialized) // UDP foi inicializado
  {
    int packetSize = udp.parsePacket();
    if (packetSize)
    {
      int len = udp.read(incomingPacket, 255);
      if (len > 0)
      {
        incomingPacket[len] = 0;
      }
      Serial.printf("Received packet: %s\n", incomingPacket);

      const char* message = "Hydro-IoT on the network respond?";
      if  (strcmp(incomingPacket, message) == 0)
      {
        Serial.println(Config->Localizacao.Local);
        // Responder ao remetente com o IP do ESP32
        IPAddress remoteIP = udp.remoteIP();
        udp.beginPacket(remoteIP, udp.remotePort());
        udp.printf("Yes Hydro-IoT '%s' '%s' '%s' ", WiFi.localIP().toString().c_str(),Config->Hardware.HostName,Config->Localizacao.Local.c_str());
        udp.endPacket();
        Serial.printf("Yes Hydro-IoT '%s' '%s' '%s'\n", WiFi.localIP().toString().c_str(),Config->Hardware.HostName,Config->Localizacao.Local.c_str());
      }
                                    
      if (strncmp(incomingPacket, "Yes Hydro-IoT", 13) == 0) 
      {
        Serial.println(incomingPacket);

        // Extrai o IP
        char* token = strtok(incomingPacket, "'");
        token = strtok(NULL, "'");
        char ip[20];
        strncpy(ip, token, sizeof(ip) - 1);
        ip[sizeof(ip) - 1] = '\0';

        // Extrai o Nome
        token = strtok(NULL, "'");
        token = strtok(NULL, "'");
        char name[50];
        strncpy(name, token, sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0';

        // Extrai o Local
        token = strtok(NULL, "'");
        token = strtok(NULL, "'");
        char location[50];
        strncpy(location, token, sizeof(location) - 1);
        location[sizeof(location) - 1] = '\0';
        
        struct tm dateTime;
        bool found = false;
        for (const auto& device : devices) 
        {
          if (device.IP == String(ip))
          {
            found = true;
            break;
          }
        }
        if (!found)
        {
          if (!getLocalTime(&dateTime)) 
            {
              devices.push_back({name, ip, location, {0}});
            }
          else
            {
              devices.push_back({name, ip, location, dateTime});
            }
          Config->Devices.NewDevice = true;  
          Config->Devices.mSeg = millis(); 
        }
      }
    }
  }
  if ((Config->Devices.NewDevice) && (devices.size() > 0)) {ScanSend(Config);}
}

void convertFromJson(JsonVariantConst src, tm& dst) {
    strptime(src.as<const char*>(), "%FT%TZ", &dst);
}

bool convertToJson(const tm& src, JsonVariant dst) 
{
  char buf[32];
  strftime(buf, sizeof(buf), "%FT%TZ", &src);
  return dst.set(buf);
}

void ScanSend(struct Parametros *Config)
{
  if ((Config->Devices.NewDevice) && ((unsigned long)(millis() - Config->Devices.mSeg) > Config->Devices.Interval))
  {
    // criar uma string JSON para enviar dados ao cliente
    String jsonString = "";                           
    // criar um contêiner JSON
    StaticJsonDocument<200> doc;                      
    // criar um Objeto JSON
    JsonArray array = doc.createNestedArray("Devices");      
    for (const auto& device : devices) 
    {
      char buffer[30];
      strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", &device.DateTime);
      JsonObject object = array.createNestedObject();
      object["DeviceName"] = device.DeviceName;
      object["IP"] = device.IP;
      object["Local"] = device.Local;
      object["Status"] = true;
      object["Data"] = buffer; //object["Data"] = strptime(src.as<const char*>(), "%FT%TZ", &device.DateTime);
    }      

    // converter objeto JSON para string
    serializeJson(doc, jsonString);                   
    
    // imprimir string JSON no console para fins de depuração (você pode comentar isso)
    Serial.println(jsonString);                       

    // enviar string JSON para os clientes
    webSocket.broadcastTXT(jsonString);    
    Config->Devices.NewDevice = false;
    Config->Devices.mSeg = millis(); 
  }
}