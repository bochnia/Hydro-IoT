const char style_css[] PROGMEM = R"rawliteral(
html, body
{
  height: 100%;
  margin: 0;
  font-family: Arial, sans-serif;
}
body
{
  display: grid;
  grid-template-columns: 1fr 1fr;
  grid-template-rows: auto;
  gap: 50px; /* Espaçamento de 50px entre os quadros */
  padding: 20px;
  /*background: linear-gradient(to bottom, #8595a0, #011f4b);  Degradê azul */
  background-color: #a7c2d4; /* Fundo branco para os body */
  align-items: start; /* Alinhando os quadros ao topo */
  display: flex;
  flex-wrap: wrap;            
}
.container
{
  display: flex;
  flex-wrap: wrap;
  width: 100%;
  gap: 30px; /* Espaçamento de 50px entre os quadros */
  min-height: 350px; /* Definindo a altura mínima */
}
.bloco
{
  border: 1px solid #ccc;
  padding: 20px;
  border-radius: 8px;
  min-height: 350px; /* Definindo a altura mínima */
  box-shadow: 0 4px 23px rgba(0, 0, 0, 0.4); /* Adicionando sombra */
  background-color: white; /* Fundo branco para os blocos */
  width: calc(49% - 50px); /* Ajustando a largura para evitar sobreposição */
  box-sizing: border-box; /* Inclui padding e border na largura total */
  overflow: hidden; /* Evita que o conteúdo transborde */      
}
.bloco1
{
  background-color: #e7f1fe;
  display: flex; /* Define que os elementos filhos serão dispostos em linha */
  flex-wrap: wrap; /* Permite que os elementos quebrem linha se necessário */
  align-items: flex-start;
  flex-direction: row;
}
.bloco1 .icon
{
  flex: 1;
  text-align: center;
  background-color: #e9f7ef; /* Cor de fundo diferente para a metade esquerda */
  padding: 20px;
}
.bloco1 .form-container
{
  flex: 1;
  background-color: #e7f1fe; /* Cor de fundo diferente para a metade direita */
  padding: 10px;
}
.bloco2
{
  background-color: #e9f7ef;
}
.bloco3
{
  background-color: #fef9e7;
}
.bloco4
{
  background-color: #f5eef8;
}
.bloco5
{
  background-color: #a265bd;
}
#real-time, #activity-time
{
  font-size: 48px;
  font-weight: bold;
  color: blue;
}
@media (max-width: 768px)
{
  body
  {
    grid-template-columns: 1fr;
  }
  .bloco
  {
    width: 100%; /* Largura total em telas menores */
  }
}
.form-container
{
  flex: 1 1 50%; /* Define que cada bloco ocupará 50% da largura disponível */
  max-width: 450px; /* Define a largura máxima de 300px para cada bloco */
  box-sizing: border-box; /* Inclui padding e border na largura total do elemento */
  padding: 10px; /* Adiciona espaçamento interno */
}
table
{
  width: 100%;
  border-collapse: collapse;
  box-shadow: 0 5px 9px rgba(0,0,0,0.3);
  box-sizing: border-box; 
}
th, td
{
  border: 1px solid #ddd;
  padding: 12px;
  text-align: left;
}
th
{
  background: linear-gradient(to right, #1e3c72, #2a5298); /* Degradê azul */
  color: white;
}
tr:nth-child(even)
{
  background-color: #f2f2f2;
}
tr:hover
{
  background-color: #ddd;
}
th, td
{
  border-radius: 4px;
}
.col1
{
  width: 80px; /* Definindo a largura da coluna Idade */
  min-width: 30px; /* Define a largura mínima das colunas */
  text-align: center; /* Centralizando o texto */
}
.idade
{
  width: 50px; /* Definindo a largura da coluna Idade */
  min-width: 30px; /* Define a largura mínima das colunas */
  text-align: center; /* Centralizando o texto */
}
.percent
{
  text-align: center; /* Centralizando o texto */
}        
.percent::after 
{
  content: '%';
}    
.celsius
{
  text-align: center; /* Centralizando o texto */
}        
.celsius::after 
{
  content: 'ºc';
}    
ul
{
  line-height: 1.8; /* Aumenta a distância entre linhas */
}
.H_Smoke
{
  width: 255px; /* Definindo a largura da coluna Idade */
  text-align: center; /* Centralizando o texto */
}
.clock-container
{
  display: flex;
  gap: 20px;
}
.clock
{
  background: rgba(154, 189, 230, 0.5);
  padding: 20px;
  border-radius: 10px;
  box-shadow: 0 12px 16px rgba(0, 0, 0, 0.6);
  text-align: center;
}
.tooltip .tooltiptext
{
  visibility: hidden;
  width: 200px;
  background-color: yellow; /* Fundo amarelo */
  color: darkblue; /* Texto em azul escuro */
  text-align: center;
  border-radius: 10px; /* Cantos arredondados */
  padding: 5px;
  position: absolute;
  z-index: 1;
  bottom: 125%; /* Posiciona o tooltip acima do texto */
  left: 50%;
  margin-left: -100px;
  opacity: 0;
  transition: opacity 0.3s;
  white-space: pre-line; /* Permite quebras de linha */
}
.tooltip:hover .tooltiptext
{
  visibility: visible;
  opacity: 1;
}
.tree ul
{
  padding-left: 10px; /* Ajuste o valor conforme necessário */
  list-style-type: none;
}
.tree ul ul
{
  margin-left: 0px;
}
.tree li::before 
{
  content: "•"; /* Adiciona um marcador antes de cada item */
  margin-right: 10px; /* Espaçamento entre o marcador e o texto */
}    
.online
{
  color: green;
}
.offline
{
  color: red;
}
.numero 
{
  min-width: 10px; 
}
#cep /* o simbolo # indica que é ID do componente*/
{
  width: 70px;
}
#endereco
{
  width: 320px;
}
#uf
{
  width: 320px;
}
#municipio
{
  width: 320px;
}
#bairro
{
  width: 320px;
}
#local
{
  width: 320px;
}
#fone
{
  width: 100px;
}
#WebSocket
{
  font-weight: bold;
}
.icon 
{
  width: 80px; 
}
.icon img.favicon 
{
  width: 100%; /* Define a largura como 20% do tamanho original */
  height: auto; /* Mantém a proporção da altura */
}  
#ip
{
  width: 90px;
}
#porta
{
  width: 45px;
}
.form-row-ip
{
  display: flex;
  gap: 0px;
}
.form-group-ip
{
  display: flex;
  flex-direction: column;
  margin-right: 10px;
}
.form-group-ip label
{
  margin-bottom: 0;
  text-align: left;
}    


/********************************************/
@media (max-width: 600px)
{
  #real-time, #activity-time
{
  font-size: 24px;
  font-weight: bold;
  color: rgb(255, 0, 76);
}
  body
  {
    padding: 0;
    overflow-x: hidden;
  }
  .form-container
  {
    flex: 1 1 100%; /* Define que cada bloco ocupará 100% da largura disponível quando a janela for menor que 600px */
    max-width: 100%; /* Remove a largura máxima de 300px para permitir que o bloco ocupe toda a largura */
  }
  .bloco 
  {
    /*width: calc(100% - 40px);  Ajuste para telas menores */
    width: 100%;
    margin-left: 0px; /* Espaço à esquerda em telas menores */
    display: flex;
    flex-direction: column;
    align-items: left; /* Centraliza o conteúdo para evitar transbordamento */
    padding: 5px;
    padding-right: 5px;
    box-sizing: border-box; 
  }
  .bloco1
  {
    flex-direction: column;
    align-items: flex-start;
  } 
  .bloco1 .form-container
  {
    margin-left: 0px;
    width: 100%;
  }
  .bloco2
  {
    padding-bottom: 20px;
  }
  #endereco
  {
    width: 100%; /* Ajusta a largura para 100% da tela em dispositivos ainda menores */
  }
  .Indicadores
  {
    width: 60%;
    max-width: 200px; /* A largura máxima da tabela será de 600px */
    border-collapse: collapse;
    padding: 50px;
    background-color: #f9f9f9; /* Define a cor de fundo da tabela */
  }
  /* Estilo para o componente real-time */
}
)rawliteral";