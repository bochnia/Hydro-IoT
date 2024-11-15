String printTimer(struct Parametros *Config);
String TempoDeAtividade(bool completo);

void printLocalTime() 
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) 
    {
      Serial.println("No time available (yet)");
    }
  else
    { 
      char buffer[64];
      strftime(buffer, sizeof(buffer), "%d.%m.%Y %H:%M:%S", &timeinfo);
      String formattedTime = String(buffer);
      Serial.println(formattedTime);
    }
}


String printTimer(struct Parametros *Config) 
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) 
  {
    return "";
  }
  
  char buffer[21];

  // Formatação da data e hora
  // strftime(buffer, sizeof(buffer), "%d.%m.%Y  %H:%M:%S", &timeinfo);
  if (Config->Clock.Full)   
    strftime(buffer, sizeof(buffer), "%H:%M:%S", &timeinfo);
  else
    strftime(buffer, sizeof(buffer), "%H:%M   ", &timeinfo);
  return buffer; // dd.mm.yyyy hh:mm:ss
}

// Callback function (gets called when time adjusts via NTP)
void timeAvailable(struct timeval *t) 
{
  Serial.println(F("Relogio sincronizado com sucesso via NTP!"));
  printLocalTime();
}

String TempoDeAtividade(bool completo)
{
  char buffer[40];
  unsigned long ShowUpTime = millis();
  unsigned long sec = ShowUpTime / 1000;
  unsigned long min = sec / 60;
  unsigned long totalHoras = min / 60;
  unsigned long hor = totalHoras % 24;
  float dia = totalHoras / 24;
  if (completo)
    sprintf(buffer,"%4.0f.%02d:%02d:%02d", dia, hor, min % 60, sec % 60); 
  else
  {
    if (dia>0) // Quando a quantidade de dias for maior zero
      {        // O segundo sera suprimido e aparesntar a quantidade dias
        sprintf(buffer,"%4.0f.%02d:%02d", dia, hor, min % 60); 
      }
    else
      { // Apresenta a HH:MM:SS quando a quantidade dias for zero.
        sprintf(buffer,"%02d:%02d:%02d",hor, min % 60,sec % 60); 
      }    
  }
  return buffer;
}  