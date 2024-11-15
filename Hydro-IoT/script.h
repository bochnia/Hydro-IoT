const char script_js[] PROGMEM = R"rawliteral(
  var Socket;
  var intervalClockId = 0;
  var intervalActivityId = 0;
  var timeDifference = 0;
  let startMillis = 0;
  let initialMillis = 0;      
  
    // Certifique-se de que o código é executado após o carregamento do DOM
    document.addEventListener('DOMContentLoaded', () => {
      // Seleciona todos os elementos input
      const inputs = document.querySelectorAll('input[type="text"]');
      
      // Adiciona o atributo writingsuggestions a cada input
      inputs.forEach(input => {
        input.setAttribute('writingsuggestions', 'true');
      });
    });
  
  function ShowRealTime()
  {
    const now = new Date();
    now.setTime(now.getTime() + timeDifference);
    const hours = String(now.getHours()).padStart(2, '0');
    const minutes = String(now.getMinutes()).padStart(2, '0');
    const seconds = String(now.getSeconds()).padStart(2, '0');
    const timeString = `${hours}:${minutes}:${seconds}`;
    document.getElementById('real-time').textContent = timeString;
  }
  
  function ShowActivityTime()
  {
    const nowMillis = Date.now();
    const elapsedMillis = nowMillis - initialMillis + startMillis;
    const elapsedTime = Math.floor(elapsedMillis / 1000);
  
    const days = Math.floor(elapsedTime / 86400);
    const hours = String(Math.floor((elapsedTime % 86400) / 3600)).padStart(2, '0');
    const minutes = String(Math.floor((elapsedTime % 3600) / 60)).padStart(2, '0');
    const seconds = String(elapsedTime % 60).padStart(2, '0');
    let timeString;
    if (days>0)
      { // Formata o número com ponto de separação de milhares
        const formattedDays = days.toLocaleString('pt-BR'); 
        timeString = `${formattedDays} Dias ${hours}:${minutes}:${seconds}`;
      }
    else
      {
        timeString = `${hours}:${minutes}:${seconds}`;
      }
    document.getElementById('activity-time').textContent = timeString;
  }      
  
  function validateNumber(input) 
  {
    input.value = input.value.replace(/[^0-9]/g, '');
  }
  
  function init() 
  {
    Socket = new WebSocket('ws://' + window.location.hostname + ':81/'); 
    var statusElement = document.getElementById("WebSocket");
    Socket.onmessage = function(event) 
    {
      var message = event.data;
  
      try
      {
        var obj = JSON.parse(message);
        if (obj.hasOwnProperty('Dados')) 
        {
            processCommand(event);
        }
        if (obj.hasOwnProperty('Devices')) 
        {
          processDevices(event);
        }
        if (obj.hasOwnProperty('Config')) 
        {
          processConfig(event);
        }
      } catch (e) 
        {
          console.error("Erro ao analisar JSON:", e);
        }
    }; 
  
    Socket.onopen = function(event) 
    {
      statusElement.textContent = "Conectado";
      statusElement.className = "online";
    };
  
    Socket.onclose = function(event) 
    {
      statusElement.textContent = "Off-Line";
      statusElement.className = "offline";
      clearInterval(intervalClockId);
      clearInterval(intervalActivityId);
      document.getElementById('real-time').textContent = "--:--:--";
      document.getElementById('activity-time').textContent = "--:--:--";
    }
  } 
  
  function enviarDados() 
  {
    const H_Auto     = document.getElementById('H_Auto_Input').value;
    const H_AlertMin = document.getElementById('H_AlertMin_Input').value;
    const H_AlertMax = document.getElementById('H_AlertMax_Input').value;
    const T_AlertMin = document.getElementById('T_AlertMin_Input').value;
    const T_AlertMax = document.getElementById('T_AlertMax_Input').value;
    const L_AlertMin = document.getElementById('L_AlertMin_Input').value;
    const ClockFormat = document.getElementById('ClockType').value;
    const ClockShow =  document.getElementById('ClockShow').checked;
    const DeviceName = document.getElementById('DeviceName').value;
  
    const serverMQTT = document.getElementById('serverMQTT').value;
    const passMQTT = document.getElementById('passMQTT').value;
    const userMQTT = document.getElementById('userMQTT').value;
    const portMQTT = document.getElementById('portMQTT').value;
    const enabledMQTT = document.getElementById('enabledMQTT').checked;
  
    var msg = { 
        Tipo : "Update",
        H_Auto: H_Auto,
        H_AlertMin: H_AlertMin,
        H_AlertMax: H_AlertMax,
        T_AlertMin: T_AlertMin,
        T_AlertMax: T_AlertMax,
        L_AlertMin: L_AlertMin,
        ClockFormat: ClockFormat,
        ClockShow: ClockShow,
        DeviceName: DeviceName,
        ServerMQTT: serverMQTT,
        PassMQTT: passMQTT,
        UserMQTT: userMQTT,
        PortMQTT: portMQTT,
        EnabledMQTT: enabledMQTT
      };        
    Socket.send(JSON.stringify(msg)); 
  }
  
  
  function enviarReset() 
  {
    var msg = { 
        Tipo : "Reset"
    };        
    Socket.send(JSON.stringify(msg)); 
  }  
  
  function enviarScan() 
  {
    var msg = { 
        Tipo : "Scan"
    };        
    Socket.send(JSON.stringify(msg)); 
  }  
  
  function enviarDadosCheckbox(elemento) 
  {
    var estado = elemento.checked;
    var msg = { tipo: 'check', id: elemento.id, check: estado };
    Socket.send(JSON.stringify(msg)); 
  }
  
  function enviarDadosRadio(elemento) 
  {
    var estado = elemento.value;
    var msg = { tipo: 'radio', id: elemento.id, value: estado };
    Socket.send(JSON.stringify(msg)); 
  }      
  
  
  function processDevices(event)
  {
    var obj = JSON.parse(event.data);
    var matriz = obj.Devices;
    createDeviceTable(matriz);
  }
  
  function processCommand(event)
  {
    var obj = JSON.parse(event.data);
  
    const epochTime = parseInt(obj.Dados.ESP_Clock, 10);
    startMillis  = parseInt(obj.Dados.ESP_Uptime, 10);
  
    // Converte o timestamp Unix em uma data legível
    const DataTime = new Date(epochTime * 1000);
  
    // Obtenha o horário atual do cliente
    const clientDate = new Date();
  
    // Calcule a diferença em milissegundos
    timeDifference = DataTime - clientDate; 
  
    initialMillis = Date.now();
    
    if (intervalClockId==0)
      intervalClockId = setInterval(() => ShowRealTime(), 1000);
    if (intervalActivityId==0) 
      intervalActivityId = setInterval(() => ShowActivityTime(), 1000);
    
    // Umidade _Input
    document.getElementById('H_Min').innerText = obj.Dados.H_Min;
    document.getElementById('H_Max').innerText = obj.Dados.H_Max;
    document.getElementById('H_Auto').innerText = obj.Dados.H_Auto;
    document.getElementById('H_AlertMin').innerText = obj.Dados.H_AlertMin;
    document.getElementById('H_AlertMax').innerText = obj.Dados.H_AlertMax; 
    document.getElementById('H_Value').innerText = obj.Dados.H_Value; 
    
    // Temperatura
    document.getElementById('T_Min').innerText = obj.Dados.T_Min;
    document.getElementById('T_Max').innerText = obj.Dados.T_Max;
    document.getElementById('T_Value').innerText = obj.Dados.T_Value;
    document.getElementById('T_AlertMin').innerText = obj.Dados.T_AlertMin;
    document.getElementById('T_AlertMax').innerText = obj.Dados.T_AlertMax;
    
    // Nivel de agua        
    document.getElementById('L_Min').innerText = obj.Dados.L_Min;
    document.getElementById('L_Max').innerText = obj.Dados.L_Max;
    document.getElementById('L_Value').innerText = obj.Dados.L_Value;
    document.getElementById('L_AlertMin').innerText = obj.Dados.L_AlertMin;
    
    document.getElementById('H_Smoke').innerText = obj.Dados.H_Smoke; 
  
    document.getElementById('BootCount').innerText = obj.Dados.BootCount; 
  
    //document.getElementById('serverMQTT').value = obj.Dados.ServerMQTT; 
    document.getElementById('passMQTT').value = obj.Dados.PassMQTT; 
    document.getElementById('userMQTT').value = obj.Dados.UserMQTT; 
    document.getElementById('portMQTT').value = obj.Dados.PortMQTT; 
    document.getElementById('enabledMQTT').check = obj.Dados.EnabledMQTT; 
  
  /*
    document.getElementById('DeviceName').value = obj.Dados.HostName; 
    document.getElementById('ssid').value = obj.Dados.SSID; 
    document.getElementById('password').value = obj.Dados.Pwd; 
    document.getElementById('email_Input').value = obj.Dados.Email; 
    
    document.getElementById('uf').value = obj.Dados.UF; 
    document.getElementById('municipio').value = obj.Dados.Municipio; 
    document.getElementById('bairro').value = obj.Dados.Bairro; 
    document.getElementById('logradouro').value = obj.Dados.Logradouro;
    document.getElementById('numero').value = obj.Dados.Numero;
    document.getElementById('complemento').value = obj.Dados.Complemento;
    document.getElementById('andar').value = obj.Dados.Andar;
    document.getElementById('conjunto').value = obj.Dados.Conjunto;
    document.getElementById('local').value = obj.Dados.Local;
    document.getElementById('cep').value = obj.Dados.CEP; 
    document.getElementById('fone').value = obj.Dados.Fone; 
    document.getElementById('ramal').value = obj.Dados.Ramal; 
  */
    // Prepara o preenchimento do campo Endereço
    //var Endereco = formatarEndereco(obj.Dados);
  
    document.getElementById('endereco').value = formatarEndereco(obj.Dados);
    document.getElementById('ClockShow').checked = obj.Dados.ClockShow; // ClockFormat
    if (obj.Dados.ClockFormat)
      document.getElementById('ClockType').value = 1;
    else
      document.getElementById('ClockType').value = 0;
  } 
    
  function formatarEndereco(Dados)
  { // Formata o Endereço para exibisão no Memobox
    let Endereco = Dados.Logradouro;
    if (Dados.Numero != "") {Endereco +=  ", " + Dados.Numero;}
    if (Dados.Complemento != "") {Endereco +=  " - " + Dados.Complemento;}
    Endereco +=  "\n" + Dados.Bairro;
    if (Dados.Municipio != "") {Endereco +=  ", " + Dados.Municipio;}
    if (Dados.UF != "") {Endereco +=  ", " + Dados.UF;}
    Endereco +=  "\n" + Dados.CEP;
    Endereco +=  "\n" + Dados.Andar;
    if (Dados.Conjunto != "") {Endereco +=  "\n" + Dados.Conjunto;}
    if (Dados.Local != "") {Endereco +=  "\n" + Dados.Local;}
    Endereco +=  "\n" + Dados.Fone;
    if (Dados.Ramal != "") {Endereco +=  " - Ramal: " + Dados.Ramal;}
    Endereco +=  "\n" + Dados.Email;
    return Endereco;
  }
  
  function processConfig(event) {
    var obj = JSON.parse(event.data);

    document.getElementById('H_Auto_Input').value = obj.Config.H_Auto;
    document.getElementById('H_AlertMin_Input').value = obj.Config.H_AlertMin;
    document.getElementById('H_AlertMax_Input').value = obj.Config.H_AlertMax; 
    document.getElementById('T_AlertMin_Input').value = obj.Config.T_AlertMin;
    document.getElementById('T_AlertMax_Input').value = obj.Config.T_AlertMax; 
    document.getElementById('L_AlertMin_Input').value = obj.Config.L_AlertMin;

    document.getElementById('DeviceName').value = obj.Config.HostName; 
    document.getElementById('ssid').value = obj.Config.SSID; 
    document.getElementById('password').value = obj.Config.Pwd; 
    document.getElementById('email_Input').value = obj.Config.Email; 

    document.getElementById('serverMQTT').value = obj.Config.ServerMQTT; 
    document.getElementById('passMQTT').value = obj.Config.PassMQTT; 
    document.getElementById('userMQTT').value = obj.Config.UserMQTT; 
    document.getElementById('portMQTT').value =obj.Config.PortMQTT; 
    document.getElementById('enabledMQTT').checked = obj.Config.EnabledMQTT; 

    document.getElementById('uf').value = obj.Config.UF; 
    document.getElementById('municipio').value = obj.Config.Municipio; 
    document.getElementById('bairro').value = obj.Config.Bairro; 
    document.getElementById('logradouro').value = obj.Config.Logradouro;
    document.getElementById('numero').value = obj.Config.Numero;
    document.getElementById('complemento').value = obj.Config.Complemento;
    document.getElementById('andar').value = obj.Config.Andar;
    document.getElementById('conjunto').value = obj.Config.Conjunto;
    document.getElementById('local').value = obj.Config.Local;
    document.getElementById('cep').value = obj.Config.CEP; 
    document.getElementById('fone').value = obj.Config.Fone; 
    document.getElementById('ramal').value = obj.Config.Ramal; 

    document.getElementById('endereco').value = formatarEndereco(obj.Config);
    document.getElementById('ClockShow').checked = obj.Config.ClockShow;
    if (obj.Config.ClockFormat) {
        document.getElementById('ClockType').value = 1;
    } else {
        document.getElementById('ClockType').value = 0;
    }
} 
  
  window.onload = function(event) { init(); }
  
  // Chama a função para criar a tabela de dispositivos
  function createDeviceTable(devices) 
  {
    // Verifica se o container já existe
    let container = document.getElementById('ScanDevices');
    if (!container) 
    {    
      const container = document.querySelector('.container');
      container.id = 'ScanDevices';
      const block = document.createElement('div');
      block.className = 'bloco bloco2';
      block.innerHTML = `
        <h3>Lista de Dispositivos na Rede</h3>
        <table>
          <thead>
            <tr>
              <th>Nome do dispositivo</th>
              <th>Ultimo contato</th>
              <th>Local</th>
              <th>Status</th>
              <th>IP</th>
            </tr>
          </thead>
          <tbody id="deviceTableBody">
            <!-- A tabela de dispositivos será gerada aqui -->
          </tbody>
        </table>`;
      container.appendChild(block);
    } 
    const tableBody = document.getElementById('deviceTableBody');
  
    devices.forEach(device => 
    {
      // Verifica se o dispositivo já está na tabela
      if (!Array.from(tableBody.rows).some(row => row.cells[4].textContent === device.IP)) 
      {
        const tr = document.createElement('tr');
        tr.innerHTML = `
          <td title="${device.Local}" class="tooltip">${device.DeviceName}<span class="tooltiptext">${device.Local}</span></td>
          <td class="DataTime">${device.DateTime}</td>
          <td class="LocalDevice">${device.Local}</td>
          <td class="${device.Status}">${device.Status}</td>
          <td><a href="http://${device.IP}" target="_blank">${device.IP}</a></td>`;
        tableBody.appendChild(tr);
      }
    });
  }
  )rawliteral";