void DisplayMQTT(struct Parametros *Config)
{ // Publica a temperatura / Umidade / Nivel da água
  if ( !Config->MQTT.Enabled)
    return;

  if ( millis() - Config->MQTT.mSeg > Config->MQTT.Interval )
  {
    Config->MQTT.mSeg = millis();

    if (!clientMQTT.connected())
      reconnect();

    if (clientMQTT.connected())
    {
      clientMQTT.loop();
      
      char buffer1[60];
      char buffer2[60];
      char buffer3[20];
      char buffer4[20];
      sprintf(buffer1,  "T %3dcº - Min:%3dcº - Max:%3dcº", Config->Temperatura.Value, Config->Temperatura.Min, Config->Temperatura.Max);
      sprintf(buffer2,  "H %3d%% - Min:%3d%% - Max:%3d%%", Config->Hydro.Value, Config->Hydro.Min, Config->Hydro.Max);
      sprintf(buffer3,  "H %3d%%", Config->Hydro.Auto);
      sprintf(buffer4,  "N %3d%%", Config->Level.Value);

      clientMQTT.publish("Hydro-IoT/Temperatura",buffer1);
      clientMQTT.publish("Hydro-IoT/Umidade",buffer2);
      clientMQTT.publish("Hydro-IoT/Auto",buffer3);
      clientMQTT.publish("Hydro-IoT/Nivel",buffer4);
    }
  }
}

void reconnect()
{
  Serial.print("Tentando conexao com MQTT...");
  String clientId = DeviceName;
  clientId += String(random(0xffff), HEX);

  if (clientMQTT.connect(clientId.c_str(), Config.MQTT.User.c_str(), Config.MQTT.Password.c_str())) 
  {
    Serial.println("Conectado");
    clientMQTT.publish(DeviceName,"Conectado.");
    clientMQTT.subscribe("Hydro-IoT/Auto"); 
    Config.MQTT.Interval = 5000;
  }                              
  else
  {
    Serial.print("failed, rc=");
    Serial.print(clientMQTT.state());
    Serial.println(" try again in 30 seconds");
    Config.MQTT.Interval = 30000;
  }  
}

void callbackMQTT(char* topic, byte* payload, unsigned int length)
{
  String message = "";
  for (int i =0; i < length; i++)
  {
    message += (char)payload[i];
  }

  int numero = message.toInt();
  if ( (numero >= 10) && (numero <= 80) )
  {
    Config.Hydro.Auto = numero;
    char buffer[60];
    sprintf(buffer,  "H %3d%%", Config.Hydro.Auto);
    clientMQTT.publish("Hydro-IoT/Auto",buffer);
  }
}
