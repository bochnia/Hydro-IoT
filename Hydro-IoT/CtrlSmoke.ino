extern boolean Smoke;

void CtrlSmoke(struct Parametros *Config)
{
  // Apresenta da temperatura / Umidade
  int HumidityValue = DisplayHydro(Config);

  if ((HumidityValue < Config->Hydro.Auto) && (!Config->Hydro.Smoke))
  { // Umidade menor que HydroMin LIGA
    Config->Hydro.Smoke = true; // Para ligar o Umidificardor um pulso HIGH e LOW
    digitalWrite(pin1, HIGH); 
    delay(50); // 50 milesegundos para enviar o pulso LOW
    digitalWrite(pin1, LOW); 
    DisplayHeart(Config->Hydro.Smoke);
  }
  if ((HumidityValue >= Config->Hydro.Auto) && Config->Hydro.Smoke)
  { // Umidade maior que HumidityValue Desliga
    Config->Hydro.Smoke = false; // Para desligar o Umidificador envia 4 pulsos
    digitalWrite(pin1, HIGH);
    delay(50);
    digitalWrite(pin1, LOW);
    delay(50);
    digitalWrite(pin1, HIGH);
    delay(50);
    digitalWrite(pin1, LOW);
    delay(50);
    DisplayHeart(Config->Hydro.Smoke);
  }
}