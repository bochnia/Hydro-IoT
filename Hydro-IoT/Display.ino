void Display(struct Parametros *Config);
void DisplayHeart(bool Smoke);
void BluetoothIcon(bool liga);
int DisplayHydro();
void printLocalTime(); 

void timeAvailable(struct timeval *t);
void DisplayClock(struct Parametros *Config);
void DisplayUpTime(struct Parametros *Config);
void DisplayMinMax(struct Parametros *Config);


// Matriz de bits para um caracteres especiais 
byte Heart[8] = {B00000,B01010,B11111,B11111,B11111,B01110,B00100,B00000};
byte Umidade[8] = {B00000,B01110,B11111,B00100,B00100,B10010,B10010,B01100};
byte Hydrio[8] = {B10010,B00100,B01000,B10010,B00000,B00101,B00111,B00101};
byte Porcentagem[8] = {B10010,B00100,B01000,B10010,B00000,B00000,B00000,B00000};
byte Temperatura[8] = {B01110,B01010,B01010,B01010,B01110,B11111,B11111,B01110};
byte Nivel[8] = {B10000,B11000,B10000,B11100,B10000,B11110,B10000,B11111};
byte Celsius[8] = {B11100,B10100,B11100,B00000,B00111,B00100,B00100,B00111};
byte BluetoothON[8] = {B00100,B10110,B01101,B00110,B00110,B01101,B10110,B00100};

void Display(struct Parametros *Config)
{
  lcd.createChar(0, Nivel);  
  lcd.createChar(1, Umidade);  
  lcd.createChar(2, Temperatura);  
  lcd.createChar(3, Celsius);  
  lcd.createChar(4, Hydrio);
  lcd.createChar(5, Porcentagem);
  lcd.createChar(6, Heart);
  lcd.createChar(7, BluetoothON);
  
  lcd.clear();      // Limpar o display LCD
  lcd.setCursor(15,1); lcd.write(byte(0));
  lcd.setCursor(15,2); lcd.print("H"); 
  lcd.setCursor(15,3); lcd.write(byte(2));
  lcd.setCursor(19,1); lcd.write(byte(5));
  lcd.setCursor(19,3); lcd.write(byte(3));
  lcd.setCursor(19,2); lcd.write(byte(4));

  lcd.setCursor(1, 1); lcd.print("Min  Max ");
  lcd.setCursor(3, 2); lcd.write(byte(4));
  lcd.setCursor(8, 2); lcd.write(byte(4));
  lcd.setCursor(3, 3); lcd.write(byte(3));
  lcd.setCursor(8, 3); lcd.write(byte(3));
}

void DisplayHeart(boolean Smoke)
{
  lcd.setCursor(13,3);
  if (Smoke)
    lcd.write(byte(6));
  else
    lcd.print(" ");
}

void BluetoothIcon(bool liga)
{
  lcd.setCursor(11,1);
  if (liga)
    lcd.write(byte(7));
  else
    lcd.print(" ");
}

int DisplayHydro(struct Parametros *Config)
{
  if (!Config->Hydro.Enabled) 
    return 0;

  // Apresenta da temperatura / Umidade
  if ( millis() - Config->Hydro.mSeg > Config->Hydro.Interval )
  {
    TempAndHumidity lastValue = dht.getTempAndHumidity();
    if (isnan(lastValue.temperature) || isnan(lastValue.humidity)) 
      {
        Serial.println("Falha ao ler do sensor DHT!");
        Config->Hydro.Enabled = false;
        return 0;
      }
    else
      {
        Config->Hydro.Value = lastValue.humidity;
        Config->Temperatura.Value = lastValue.temperature;

        // Umidade - Define Min e Max da Umidade
        if (Config->Hydro.Min > Config->Hydro.Value) 
        {
          Config->Hydro.Min = Config->Hydro.Value;
          preferences.putInt("H_Min", Config->Hydro.Min); 
        }

        if (Config->Hydro.Max < Config->Hydro.Value) 
        {
          Config->Hydro.Max = Config->Hydro.Value;
          preferences.putInt("H_Max", Config->Hydro.Min); 
        }

        // Temperatura - Define o Min e Max da Temperatura
        if (Config->Temperatura.Min > Config->Temperatura.Value) 
        {
          Config->Temperatura.Min = Config->Temperatura.Value;
          preferences.putInt("T_Min", Config->Temperatura.Min); 
        }

        if (Config->Temperatura.Max < Config->Temperatura.Value)
        {
          Config->Temperatura.Max = Config->Temperatura.Value;
          preferences.putInt("T_Max", Config->Temperatura.Max); 
        }

        lcd.setCursor(15,1); lcd.write(byte(0));
        lcd.setCursor(15,2); lcd.printf("H%3d",Config->Hydro.Value); lcd.write(byte(4));
        lcd.setCursor(15,3); lcd.write(byte(2)); lcd.printf("%3d",Config->Temperatura.Value); lcd.write(byte(3));
        lcd.setCursor(0, 2); lcd.printf("%3d",Config->Hydro.Min);
        lcd.setCursor(5, 2); lcd.printf("%3d",Config->Hydro.Max);
        lcd.setCursor(0, 3); lcd.printf("%3d",Config->Temperatura.Min);
        lcd.setCursor(5, 3); lcd.printf("%3d",Config->Temperatura.Max);
        lcd.setCursor(10,1);
        if (Config->Hydro.Show)
          {
            lcd.print("Auto");
            lcd.setCursor(11,2);
            lcd.printf("%3d",Config->Hydro.Auto);lcd.write(byte(4));
          }
        else
          {
            lcd.print("    ");
            lcd.setCursor(11,2);
            lcd.print("    ");
          }
      }
      Config->Hydro.mSeg = millis();
    }
  else 
    {
      if (Config->Hydro.Auto != Config->Hydro.Auto_OLD)
      {
        Config->Hydro.Auto_OLD = Config->Hydro.Auto;
        lcd.setCursor(11,2);
        lcd.printf("%3d",Config->Hydro.Auto);lcd.write(byte(4));
      }
    }
  return Config->Hydro.Value;
}


// Exibe o Relogio 
void DisplayClock(struct Parametros *Config)
{
  // apresentar o horario
  if ((Config->Clock.Show) && ( millis() - Config->Clock.mSeg > 250) )
  {
    lcd.setCursor(0,0);  lcd.print(printTimer(Config)); 
    Config->Clock.LastShow = true;
    Config->Clock.mSeg = millis();
  }
  if ((Config->Clock.LastShow) && (!Config->Clock.Show))
  {
    lcd.setCursor(0,0);  lcd.print("        "); 
    Config->Clock.LastShow = false;
  }
}

// Exibe o tempo de atividade do ESP32
void DisplayUpTime(struct Parametros *Config)
{
  if ((Config->UpTime.Show) && ( millis() - Config->UpTime.mSeg > 999) )
  {
    Config->UpTime.mSeg = millis();
    unsigned long sec = Config->UpTime.mSeg / 1000;
    unsigned long min = sec / 60;
    unsigned long totalHoras = min / 60;
    unsigned long hor = totalHoras % 24;
    float dia = totalHoras / 24;
    if (dia>0) // Quando a quantidade de dias for maior zero
      {        // O segundo sera suprimido e aparesntar a quantidade dias
        lcd.setCursor(10, 0);
        lcd.printf("%4.0f.%02d:%02d", dia, hor, min % 60); 
      }
    else
      { // Apresenta a HH:MM:SS quando a quantidade dias for zero.
        lcd.setCursor(12, 0);
        lcd.printf("%02d:%02d:%02d",hor, min % 60,sec % 60); 
      }
  }
}

void DisplayLevel(struct Parametros *Config)
{
  if (!Config->Level.Enabled) 
    return;

  if (lox.isRangeComplete()) 
  {
    int val = lox.readRange();

    val = map(val, 42, 128, 100, 0);

    if (val < 0) val = 0;
    if (val > 100) val = 100;

    Config->Level.Total += val;
    Config->Level.Count ++; 
    int rstLaser = Config->Level.Total / Config->Level.Count;

    if (rstLaser <   0) rstLaser =   0;
    if (rstLaser > 100) rstLaser = 100;

    if (Config->Level.Count > 300) 
    {  
      if (Config->Level.Max<rstLaser) {Config->Level.Max = rstLaser;}
      if (Config->Level.Min>rstLaser) {Config->Level.Min = rstLaser;}
      Config->Level.Low = (rstLaser < 16);
      if (!Config->Level.Low) 
      {
        lcd.setCursor(15,1); lcd.write(byte(0)); lcd.printf("%3.0f",(float) rstLaser); lcd.write(byte(5));
      }
      Config->Level.Count = 0;
      Config->Level.Total = 0;
    }

    if (Config->Level.Low) 
    {
      unsigned long sec = millis() / 1000;
      lcd.setCursor(15,1);
      if ((sec % 2) == 0)
        lcd.print("     ");
      else
        {lcd.write(byte(0)); lcd.printf("%3.0f",(float) rstLaser); lcd.write(byte(5));}
      Config->Level.mSeg = millis();
    }
    Config->Level.Value = rstLaser;
  }
}

void Splash(struct Parametros *Config)
{
  lcd.setCursor(5,0); lcd.print(DeviceName);
  lcd.setCursor(0,2); lcd.print("Ricardo B. dos Anjos"); 
  lcd.setCursor(0,3); lcd.print("Ver 1.0  -  10408408");   
}

void DisplayLed(struct Parametros *Config)
{
if (Config->LED.Show)
  {
    if (digitalRead(Config->LED.Pino)) 
    {
      if ( millis() - Config->LED.mSeg > Config->LED.ON_Time )  
      {
        digitalWrite(Config->LED.Pino, LOW); // Desliga o LED
        Config->LED.mSeg = millis();
      }  
    }
    else
    {
      if ( millis() - Config->LED.mSeg > Config->LED.OFF_Time ) 
      {
        digitalWrite(Config->LED.Pino, HIGH); // Liga o LED
        Config->LED.mSeg = millis();
      } 
    }
  }
}