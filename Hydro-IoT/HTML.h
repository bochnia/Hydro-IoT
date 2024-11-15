const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <link rel='icon' href='favicon.ico' type='image/x-icon'>
  <link rel="stylesheet" type="text/css" href="/style.css">
  <title>Hydro-IoT</title>
  <style></style>
</head>

<body>
  <div class="container ">

    <div class="bloco bloco1">
      <div class="form-container">
        <h2>Dispositivo</h2>
        <form>
          <div class="form-group">
            <div class="icon">
              <img src='/favicon.ico' alt='Favicon' class='favicon'>
              <br><br>
              <p id="hydroapp" class="hydroapp">Hydro-IoT</p>
              <p id="WebSocket" class="offline">Off-Line</p>
            </div>
          </div>
        </form>
      </div>
      <div class="form-container">
        <h2>Localização</h2>
        <div class="form-group">
            <label for="endereco">Endereço:</label>
            <textarea id="endereco" name="endereco" rows="8" cols="60" readonly></textarea>
          </div>
        </div>
    </div> <!-- Final Bloco 1-->

    <div class="bloco bloco2">
      <h2>Indicadores</h2>
      <table class="Tabela Indicadores">
        <tr>
          <th class="col1">Hydro-IoT</th>
          <th class="idade">Mínimo</th>
          <th class="idade">Máximo</th>
          <th class="idade">Aviso</th>
          <th class="idade">Auto</th>
          <th class="idade">Atual</th>
        </tr>
        <tr>
          <td class="col1">Umidade</td>
          <td class="percent" id="H_Min" title="Valor mínimo umidade identificado no tempo de Atividade"></td>
          <td class="percent" id="H_Max" title="Valor máximo umidade identificado no tempo de Atividade"></td>
          <td class="idade" title="Valor programado para a Hydro-IoT enviar Altera de Umidade baixa e alta"><span id='H_AlertMin'></span>% / <span id='H_AlertMax'></span>%</td>
          <td class="percent" id="H_Auto" title="Valor programado para a Hydro-IoT ser ativado"></td>
          <td class="percent" id="H_Value" title="Valor atual que o Hydro-IoT está registrando de umidade"></td>
        </tr>
        <tr>
          <td class="col1">Temperatura</td>
          <td class="celsius" id='T_Min' title="Valor mínimo Temperatura identificado no tempo de Atividade"></td>
          <td class="celsius" id='T_Max'title="Valor máximo umidade identificado no tempo de Atividade"></td>
          <td class="idade" title="Valor programado para a Hydro-IoT enviar Altera de Temperatura baixa e alta"><span id='T_AlertMin'></span>ºc / <span id='T_AlertMax'></span>ºc</td>
          <td></td>
          <td class="celsius" id='T_Value' title="Valor atual que o Hydro-IoT está registrando de Temperatura"></td>
        </tr>
        <tr>
          <td class="col1">Reservatório</td>
          <td class="percent" id='L_Min' title="Valor mínimo de água no reservatório registrado no tempo de Atividade"></td>
          <td class="percent" id='L_Max' title="Valor máxima de água no reservatório registrado no tempo de Atividade"></td>
          <td class="percent" id='L_AlertMin' title="Valor mínimo de água no reservatório para o Hydro-IoT enviar Altera de nível baixo"></td>
          <td></td>
          <td class="percent" id='L_Value' title="Valor atual de água no reservatório que o Hydro-IoT está registrando"></td>
        </tr>
      </table>
      <br>
      <table>
        <tr>
          <th class="col1">Hydro-IoT</th>
          <th class="H_Smoke">Status de trabalho</th>
        </tr>
        <tr>
          <td class="col1">Evaporador</td>
          <td class="H_Smoke" title="Quando o Evaporador está ativo é mostrado a informação de ativdade nesse quadro"><span id='H_Smoke'></span></td>
        </tr>
      </table>    
      <br>
    </div> <!-- Final Bloco 2-->
    <div class="bloco bloco4">
      <h2>Dispositivo</h2>
      <p>Este bloco pode conter qualquer outro tipo de conteúdo que você desejar.</p>
      <p>
        <span class="descricao">Quantidade de Boot: </span>
        <span class="valor" id="BootCount">0</span>
      </p>
      <button onclick="enviarReset()">Reiniciar Mínimos e Máximos</button><br>
      <button onclick="enviarScan()">Localizar outros dispositivos</button><br><br>

      <div class="clock-container">
        <div class="clock">
            <h2>Tempo Real</h2>
            <div id="real-time" title="Horario do dispositivo."></div>
        </div>
        <div class="clock">
            <h2>Tempo de Atividade</h2>
            <div id="activity-time" title="Tempo de atividade do dispositivo."></div></div>
        </div>
      </div>
    </div>  <!-- Final Bloco -->   
    <div class="bloco bloco3">
      <h2>Configuração</h2>
      <p>Configurações do dispositivo</p>

      <div class="tree">
        <ul>
          <li>
            <label>Configuração de Rede</label>
            <ul>
              <li>
                <label for="ssid">Rede Wi-Fi</label>
                <input type="text" id="ssid" name="ssid" writingsuggestions="true" autocomplete="off">
              </li>
              <li>
                <label for="password">Senha de Acesso</label>
                <input type="password" id="password" name="password" minlength="8" maxlength="16">
              </li>
              <li>
                <label for="DeviceName">Dispositivo</label>
                <input type="text" id="DeviceName" name="DeviceName" writingsuggestions="true" autocomplete="off">
              </li>
              <li>
                <label for="enableLogging">Habilitar Log:</label>
                <input type="checkbox" id="enableLogging" name="enableLogging">
              </li>
            </ul>
            <label>MQTT</label>
            <ul>
              <li>
                <label for="serverMQTT">Server</label>
                <input type="text" id="serverMQTT" name="serverMQTT" writingsuggestions="true" autocomplete="off">
              </li>
              <li>
                <label for="passMQTT">Senha</label>
                <input type="password" id="passMQTT" name="passMQTT" minlength="8" maxlength="20">
              </li>
              <li>
                <label for="userMQTT">Usuário</label>
                <input type="text" id="userMQTT" name="userMQTT" writingsuggestions="true" autocomplete="off">
              </li>
              <li>
                <label for='portMQTT'>Porta :</label>
                <input type='number' id='portMQTT' name='portMQTT' min='0' max='65000' step='1' value='1883'/>
              </li>
              <li>
                <label for="enabledMQTT">Ativo:</label>
                <input type="checkbox" id="enabledMQTT" name="enabledMQTT">
              </li>
            </ul>
          </li>
          <li>
            <label>Configuração de Tela</label>
            <ul>
              <li>
                <label for="ClockShow">Exibir Relógio:</label>
                <input type="checkbox" id="ClockShow" name="ClockShow">
              </li>
              <li>
                <label for="ClockType">Formato do Relógio:</label>
                <select id="ClockType" name="ClockType">
                  <option value="0">HH:MM</option>
                  <option value="1">HH:MM:SS</option>
                </select>
              </li>
            </ul>
          </li>
          <li>
            <label>Configuração</label>
            <ul>

              <li>
                <label>Umidade</label>
                <ul>
                  <li>
                    <label for='H_Auto_Input'>Limite de trabalho :</label>
                    <input type='number' id='H_Auto_Input' name='H_Auto' min='30' max='80' step='1' value=''/>
                  </li>
                  <li>
                    <label for='H_AlertMax_Input'>Alerta de Umidade Alta:</label>
                    <input type='number' id='H_AlertMax_Input' name='H_AlertMax' min='10' max='80' step='1' value=''/>
                  </li>
                  <li>
                    <label for='H_AlertMin_Input'>Alerta de Umidade Baixa:</label>
                    <input type='number' id='H_AlertMin_Input' name='H_AlertMin' min='10' max='80' step='1' value=''/>
                  </li>
                </ul>
              </li>


              <li>
                <label>Temperatura</label>
                <ul>
                  <li>
                    <label for='T_AlertMax_Input'>Alerta de Temperatura Alta:</label>
                    <input type='number' id='T_AlertMax_Input' name='T_AlertMax' min='10' max='40' step='1' value=''/>
                  </li>
                  <li>
                    <label for='T_AlertMin_Input'>Alerta de Temperatura Baixa:</label>
                    <input type='number' id='T_AlertMin_Input' name='T_AlertMin' min='10' max='40' step='1' value=''/>
                  </li>
                </ul>
              </li>

              <li>
                <label>Nível de Água</label>
                <ul>
                  <li>
                    <label for='L_AlertMin_Input'>Alerta de Nível Baixo:</label>
                    <input type='number' id='L_AlertMin_Input' name='L_AlertMin' min='10' max='60' step='1' value=''/>
                  </li>
                </ul>
              </li>
              <li>
                <label for="email_Input">Email para envio de alerta:</label>
                <input type="email" id="email_Input" name="email" maxlength="80" placeholder="Digite seu email">
              </li>
              
            </ul>
          </li>

          <li>
            <label>Localização</label>
            <ul>
              <li>
                <label for="uf">UF</label>
                <select id="uf" name="uf">
                  <option value="" selected>Selecione um estado</option>
                  <option value="AC">Acre</option>
                  <option value="AL">Alagoas</option>
                  <option value="AP">Amapá</option>
                  <option value="AM">Amazonas</option>
                  <option value="BA">Bahia</option>
                  <option value="CE">Ceará</option>
                  <option value="DF">Distrito Federal</option>
                  <option value="ES">Espírito Santo</option>
                  <option value="GO">Goiás</option>
                  <option value="MA">Maranhão</option>
                  <option value="MT">Mato Grosso</option>
                  <option value="MS">Mato Grosso do Sul</option>
                  <option value="MG">Minas Gerais</option>
                  <option value="PA">Pará</option>
                  <option value="PB">Paraíba</option>
                  <option value="PR">Paraná</option>
                  <option value="PE">Pernambuco</option>
                  <option value="PI">Piauí</option>
                  <option value="RJ">Rio de Janeiro</option>
                  <option value="RN">Rio Grande do Norte</option>
                  <option value="RS">Rio Grande do Sul</option>
                  <option value="RO">Rondônia</option>
                  <option value="RR">Roraima</option>
                  <option value="SC">Santa Catarina</option>
                  <option value="SP">São Paulo</option>
                  <option value="SE">Sergipe</option>
                  <option value="TO">Tocantins</option>
                </select>              
                </li>
              <li>
                <label for="municipio">Município</label>
                <input type="text" id="municipio" name="municipio">
              </li>
              <li>
                <label for="bairro">Bairro</label>
                <input type="text" id="bairro" name="bairro">
              </li>
              <li>
                <label for="logradouro">Logradouro</label>
                <input type="text" id="logradouro" name="logradouro">
              </li>
              <li>
                <label for="numero">Número</label>
                <input type="text" id="numero" name="numero">
              </li>
              <li>
                <label for="complemento">Complemento</label>
                <input type="text" id="complemento" name="complemento">
              </li>
              <li>
                <label for="andar">Andar</label>
                <input type="text" id="andar" name="andar">
              </li>
              <li>
                <label for="conjunto">Conjunto</label>
                <input type="text" id="conjunto" name="conjunto">
              </li>
              <li>
                <label for="local">Local</label>
                <input type="text" id="local" name="local">
              </li>
              <li>
                <label for="cep">CEP</label>
                <input type="text" id="cep" name="cep">
              </li>
              <li>
                <label for="fone">Fone</label>
                <input type="text" id="fone" name="fone" placeholder="(11)99999-9999">
              </li>
              <li>
                <label for="ramal">Ramal</label>
                <input type="text" id="ramal" name="ramal" placeholder="9999">
              </li>
            </li>
          </li>

        </ul>
        <button onclick="enviarDados()">Salvar as configurações</button>
      </div> <!--Final Bloco 3-->
    </div>
  </div>       
</div>       
</body>
<script src="/script.js"></script>
</html>
)rawliteral";