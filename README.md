# Hydro-IoT
Claro, aqui está um modelo de README.md baseado no seu projeto:

---

# Projeto de Unidade de Controle Ambiental Inteligente

## Introdução

Bem-vindos ao projeto de Unidade de Controle Ambiental Inteligente! Este projeto foi desenvolvido por Ricardo Bochnia dos Anjos
na disciplina de Objetos Inteligentes do curso de Análise e Desenvolvimento de Sistemas da Universidade Presbiteriana Mackenzie,
sob a orientação dos Professores Leandro Carlos Fernandes e André Luiz Oliveira.

## Descrição do Projeto

Este projeto consiste em um sistema inteligente que gerencia a umidade relativa do ar e monitora a temperatura ambiente em
ambientes industriais, comerciais, hospitalares, entre outros. O objetivo é proporcionar um controle ambiental eficiente em 
diversas áreas, com administração centralizada e monitoramento contínuo.

## Tecnologias Utilizadas

- **ESP32**: Utilizado para o gerenciamento do sistema.
- **Servidor Web Interno**: Implementado para configurações iniciais e monitoramento em tempo real.
- **MQTT**: Utilizado para a comunicação entre dispositivos.

## Funcionalidades

- **Configurações Iniciais**: Possibilidade de informar o servidor (broker) MQTT, porta, usuário e senha.
- **Monitoramento Contínuo**: Envio de informações a cada 5 segundos, incluindo umidade atual, mínima e máxima,
- temperatura atual, mínima e máxima e nível do reservatório de água.
- **Ativação do Ultrassom**: Baseada na umidade mínima configurada no (broker) MQTT.
- **Interface Visual**: Disponível no servidor web interno, exibindo o tempo de funcionamento e informações operacionais do Hydro-IoT.
- **Descoberta de Dispositivos**: Botão para descoberta de outros Hydro-IoT na rede local.
- **Reiniciar Valores**: Botão para reiniciar os valores mínimos e máximos.

## Como Usar

1. Configurar o servidor (broker) MQTT, porta, usuário e senha no servidor web interno.
2. Monitorar as informações de umidade e temperatura em tempo real através do servidor web.
3. Utilizar os botões para reiniciar valores e descobrir novos dispositivos na rede local.

## Demonstração

Você pode ver uma demonstração do funcionamento do sistema na interface visual do servidor web interno.
A interface trabalha em tempo real e indica o relógio, tempo de funcionamento e as informações de trabalho do Hydro-IoT.

