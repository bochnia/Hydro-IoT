struct Local 
{
  String CEP;  
  String UF;
  String Municipio;
  String Bairro;
  String Logradouro;
  String Numero;
  String Complemento;
  String Andar;
  String Conjunto;
  String Local;
  String Email;
  String Fone;
  String Ramal;
};

struct Hardware 
{
  String SSID;
  String PassWord;
  String HostName;  
};

struct Alert
{
  signed char Min;           // Valor minimo da registrado para disparo de Alerta
  signed char Max;           // Valor maximo da registrado para disparo de Alerta
};

struct Hydro 
{
  signed char Value;         // Valor corretne da umidade
  signed char Min;           // Valor minimo da umidade registrado
  signed char Max;           // Valor maximo da umidade registrado
  signed char Auto;          // Valor configurado para o umidificar ligar 
  signed char Auto_OLD;      // Armazena o valor configurado anterior
  bool Show;                 // Opção para exibir no LED a informação do valor configurado
  bool Smoke;                // Utilizado para controlar a exibitação do icone do umidificador on/off
  unsigned long mSeg;        // Utilizado para controlar o tempo de execução. 
  const unsigned int Interval = 5000; // Intervalo de tempo em mSeg para ativar a leitura do sensor
  const signed char Pino = 14;     // Pino de trabalho do sensor
  const signed char ValorMin = 30;
  const signed char ValorMax = 80;
  struct Alert Alert;
  bool Enabled;
};

struct MQTT
{
  unsigned long mSeg;
  unsigned int Interval = 30000; // Intervalo de tempo em mSeg para ativar a leitura do sensor
  String Password;
  String Server;
  String User;
  int Port = 1883;
  bool Enabled = false;
};

struct Temperatura 
{
  signed char Value;
  signed char Min;
  signed char Max;
  bool Show;
  struct Alert Alert;
};

struct Clock 
{
  bool Show;
  bool LastShow;
  bool Full;
  unsigned long mSeg;
};

struct LED 
{
  bool LED;
  const unsigned int ON_Time = 50;
  const unsigned int OFF_Time = 1500;  
  const signed char Pino = 2;
  bool Show;  
  unsigned long mSeg;
};

struct Level
{
  bool Low;
  bool High;
  unsigned long Count;
  unsigned long Total;
  signed char Min;
  signed char Max;
  int Value;
  unsigned long int mSeg;
  struct Alert Alert;
  bool Enabled;
};

struct OldValue 
{
  struct Hydro Hydro;
  struct Temperatura Temperatura;
  struct LED LED;
  //struct Bluetooth Bluetooth;
  struct Level Level;
  struct Hardware Hardware;
  bool Enabled;
};

struct Web
{
  unsigned long mSeg;       // Utilizado para controlar o tempo de execução. 
  bool Agora = false;
  const int Interval = 5000; // Intervalo de tempo em mSeg para ativar a leitura do sensor
};

struct Devices
{
  bool Scan = false;
  bool NewDevice = false;
  unsigned long mSeg;       // Utilizado para controlar o tempo de execução. 
  const int Interval = 15000; // Intervalo de tempo em mSeg para ativar a leitura do sensor
};

struct Parametros 
{
    int CtrlMenu;
    int BootCount;

    bool DHCP;
    
    struct Local Localizacao;
    struct Hardware Hardware;
    struct Hydro Hydro;
    struct Temperatura Temperatura;
    struct MQTT MQTT;
    struct LED LED;
    struct Level Level;
    struct Clock Clock;
    struct Clock UpTime;
    struct Web Web;
    struct Devices Devices;

    struct OldValue OldValue;
    
    bool ShowIP;
    bool ShowName;
    bool LastShowTimer;
    bool Particao;
};

void ConfigDefault(struct Parametros *Config)
{
    Config->Hardware.HostName = "Hydro";
    
    Config->CtrlMenu = 0;

    Config->DHCP = true;

    Config->Hydro.Auto = 60;
    Config->Hydro.Auto_OLD = 60;
    Config->Hydro.Value = 0;
    Config->Hydro.Min = 100;
    Config->Hydro.Max = 0;
    Config->Hydro.Show = true;
    Config->Hydro.Smoke = false;
    Config->Hydro.Alert.Min = 50;
    Config->Hydro.Alert.Max = 70;
    Config->Hydro.Enabled = true;

    Config->Temperatura.Value = 0;
    Config->Temperatura.Min = 100;
    Config->Temperatura.Max = 0;
    Config->Temperatura.Show = true;
    Config->Temperatura.Alert.Min = 20;
    Config->Temperatura.Alert.Max = 26;
    
    Config->LED.Show = true;
    Config->LED.mSeg = millis();    
    
    Config->Level.Low = false;
    Config->Level.Count = 0;
    Config->Level.Total = 0;
    Config->Level.mSeg = 0;
    Config->Level.Value = 0;
    Config->Level.Alert.Min = 20;
    Config->Level.Enabled = true;

    Config->Clock.Show = true;
    Config->Clock.Full = false;
    Config->UpTime.Show = true;

    Config->ShowIP = true;
    Config->ShowName = true;
    
    Config->LastShowTimer = true;

    Config->Particao = false;

    Config->OldValue.Enabled = false;
}

struct DeviceInfo {
  String DeviceName;
  String IP;
  String Local;
  struct tm DateTime;
  bool Enabled;
};