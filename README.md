<h1 align="center"><i>Super Auto: uma corrida divertida</i></h1>

<h3 align="center">Utilização do processador gráfico CoLenda no desenvolvimento de jogos 2D</h3>

<p align="center">Super Auto é um jogo 2D multiplayer de corrida de obstáculos implementado com a utilização do processador gráfico CoLenda e seu respectivo driver. Além do jogo, 
 módulos kernel e bibliotecas foram implementados para realizar a comunicação botões push e displays de 7 segmentos.</p>

## Sobre o projeto
Os anos 80 foram uma década revolucionária para os videogames, marcada por avanços tecnológicos significativos e o surgimento de jogos icônicos que moldaram a indústria. Jogos como <i>Space Invaders</i> e <i>Super Mario Bros</i>, que continuam a influenciar o design de jogos atuais, utilizavam representações bidimensionais chamadas <i>sprites</i> para elementos visuais do jogo como personagens e objetos. A animações promoveram uma melhor estética e interatividade fundamentais para a evolução da indústia de videogames.

Nesse cenário, Super Auto é um jogo bidimensional que utiliza sprites e outras representações em uma corrida de carros. Para tal, foi utilizado kit de desenvolvimento DE1-SoC juntamente com o processador gráfico CoLenda na HPS do kit. Que a corrida comece!


<details>
<summary> <b>Requisitos</b> </summary>

### Requisitos
O presente projeto deve atender às condições e aos requisitos predeterminados, de modo que:
- O código deve ser escrito em linguagem C
- O kit de desenvolvimento De1-SoC deve ser utilizado para implementação do projeto
- Ao menos um novo sprite deve ser criado e utilizado
- As ações do ator devem ser comandadas pelo mouse, que também deve refletir a velocidade no movimento
- o display de 7-segmentos deve ser utilizado para a exibição das informações do jogo
- O jogo deve permitir ações de pause, retorno, reinício e término por meio dos botões da DE1-SoC
    - O usuário poderá parar e reiniciar o jogo em qualquer momento; 
    - O usuário poderá sair do jogo em qualquer momento.
- Pelo menos um elemento passivo do jogo deverá se mover.


</details>

## Índice
- [Instalação](#instalação)
	-  [Pré-requisitos](#pré-requisitos)
-  [Softwares utilizados](#softwares-utilizados)
	- [Linguagem C](#linguagem-c)
	- [Compilador GNU](#compilador-gnu)
 	- [VS Code](#vs-code)
- [DE1-SoC](#kit-de-desenvolvimento-de1-soc)
	- [Visão geral da DE1-SoC](#visão-geral-da-de1-soc)
  	- [Sistema computacional da placa](#sistema-computacional-de1-soc)
- [Processador gráfico](#processador-gráfico)
    - [Como funciona](#como-funciona)
- [Solução geral](#solução-geral)

## Contribuidores

<a href="https://github.com/brendabo1"><img src="https://avatars.githubusercontent.com/u/69097241?v=4" title="brendabo1" width="50" height="50"></a>
<a href="https://github.com/camilaqPereira"><img src="https://avatars.githubusercontent.com/u/116687830?v=4" title="camilaqPereira" width="50" height="50"></a>
<a href="https://github.com/DestinyWolf"><img src="https://avatars.githubusercontent.com/u/64764136?v=4" title="DestinyWolf" width="50" height="50"></a>


## Instalação

## Softwares utilizados


<details>
<summary> <b>Linguagem C</b> </summary>

### Linguagem C

É uma linguagem de programação de propósito geral que combina abstrações e controles de baixo nível sobre o hardware resultando em ganho de eficiência. O software criado em 1970 por 
Dennis Ritchie é estreitamente associada ao sistema operacional UNIX, uma vez que as versões desse sistema foram escritas em linguagem C. Além disso, a sintaxe simples e a alta 
portabilidade desta linguagem entre dispositivos contribui para seu amplo uso em sistemas embarcados de recursos limitados.

</details>
<details>
<summary> <b>Compilador GNU</b> </summary>

### Compilador GNU

O GNU Compiler Collection GCC (Coleção de Compiladores GNU), ou GCC, é um conjunto de compiladores de código aberto desenvolvido pelo Projeto GNU que oferecem suporte a uma gama de 
linguagens de programação, incluindo C, C++, Fortran, Ada e Go. Esta ferramenta otimiza a compilação, ou seja a produção de código de máquina, nas várias linguagens e arquiteturas de 
processadores suportadas.

</details>
<details>

<summary> <b>VS Code</b> </summary>

### VS Code
O Visual Studio Code, ou VS Code, é um editor de texto gratuito com suporte a várias linguagens de programação, incluindo Python, Java, C, C++ e JavaScript. A ferramenta desenvolvida 
pela Microsoft Corporation dispõe de diversos recursos de depuração, destaque de erros, sugestões, personalização dentre outros para auxiliar a codificação.

</details>
<details>

<summary> <b>Nano</b> </summary>

### Nano
Também, o editor de texto simples Nano, na versão 2.2.6, presente no Linux embarcado do Kit de desenvolvimento DE1-SoC foi utilizado para codificação da solução. O Nano é um software 
leve e que oferece uma interface de linha de comando intuitiva, tornando-o ideal para rápida edição de arquivos, scripts e outros documentos de texto.

</details>


## Kit de desenvolvimento DE1-SoC

<details>
<summary><b>Visão geral da DE1-SoC</b></summary>

### Visão geral da DE1-SoC

Equipado com processador, USB, memória DDR3, Ethernet e uma gama de periféricos, o kit de desenvolvimento DE1-SoC (Figura 1) integra no mesmo Cyclone® V da Intel®, sistema em chip 
(SoC), um *hard processor system* (HPS) a uma FPGA (*Field Programmable Gate Arrays*). Este design permite uma grande flexibilidade da placa nas mais variadas aplicações. Para o acesso 
ao sistema operacional Linux embarcado na placa, o protocolo de rede SSH *(Secure Shell)* foi utilizado, estabelecendo uma conexão criptografada para comunicação entre a placa e 
computador *host*.

<div align="center">
  <figure>  
    <img src="Docs/Imagens/kit_desenvolvimento_DE1-SoC.jpg" width="600px">
    <figcaption>
      <p align="center"> 

[**Figura 1** - Kit de Desenvolvimento DE1-SoC](https://fpgacademy.org/index.html)

</p>
    </figcaption>
  </figure>
</div>

</details>
<details>
<summary><b>Sistema computacional DE1-SoC</b></summary>

### Sistema computacional DE1-SoC

<div align="center">
  <figure>  
    <img src="Docs/Imagens/diagramaDE1SoC_FPGAcademy.png" width="500px">
    <figcaption>
      <p align="center">

[**Figura 2** - Diagrama de Blocos da DE1-SoC](https://fpgacademy.org/index.html)

</p>
    </figcaption>
  </figure>
</div>

O diagrama de blocos do sistema computacional, apresentado na figura 2,  explicita os componentes do Cyclone® V da Intel®, bem como suas conexões. O HPS inclui um processador ARM® 
Cortex-A9 MPCore™ de 2 núcleos com uma distribuição Linux embarcada destinada a processamentos de propósito geral,  além da memória DDR3 e dos dispositivos periféricos. Já a FPGA 
possibilita uma variedade de implementações através da programação dos blocos lógicos.


> A comunicação bidirecional entre a o HPS e a FPGA se dá por meio das *FPGA bridges*. 
> No sentido HPS-FPGA, todos os dispositivos de entrada e saída (E/S) conectados à FPGA são acessíveis ao processador através do mapeamento de memória.
> As informações sobre o endereçamento dos periféricos estão disponíveis na [documentação da placa](https://fpgacademy.org/index.html).

</details>

> [!WARNING]
> O processador gráfico Colenda é um hardware a parte implementado em uma FPGA e carregado no kit de desenvolvimento. Por isso, os endereços base físicos dos periféricos não são os
> mesmos dos originais. Além disso, por decisão de projeto, apenas a saída VGA, os displays de 7 segmentos e os botões do tipo push foram acoplados ao hardware. 

## Processador gráfico Colenda

O processador gráfico CoLenda é um dispositivo gráfico, desenvolvido em uma FPGA, que permite o controle de polígonos convexos (triângulos e quadrados) e *sprites* em um monitor VGA com 
resolução de 640x480 pixels. Além do controle destes elementos, a GPU CoLenda permite alterar a cor de fundo do monitor, alterar blocos de fundo de dimensão 8x8 e criar *sprites* 
próprios. 

> [!NOTE]
> No projeto original, o NIOS II foi utilizado como unidade de processamento principal.
> No presente projeto, o NIOS II foi substituído pelo HPS da placa DE1-SoC.

<details>
<summary><b>Como funciona?</b></summary>

### Como funciona?

O dispositivo gráfico CoLenda disponibiliza um conjunto de instruções para o gerenciamentos dos polígonos, sprites e background (mais informações sobre as instruções [aqui]
(https://drive.google.com/file/d/1MlIlpB9TSnoPGEMkocr36EH9-CFz8psO/view) e [aqui](https://github.com/camilaqPereira/coLenda_driver?tab=readme-ov-file#instru%C3%A7%C3%B5es)). Estes 
comandos são passados da HPS para o hardware na FPGA por meio dos barramentos de dados *dataA*, que recebe os dados de acesso à memória e registradores e dos *opcodes*, e *dataB*, que 
recebe os demais dados customizáveis tais como cores e posição.

Para que os dados sejam escritos nas filas de instruções A e B, respectivas aos barramentos *dataA* e *dataB*, o sinal de controle *wr_reg*(input) deve ser colocado em alto por 1 pulso 
de clock. As FIFOs comportam cada uma um total de 16 palavras de 32 bits. Para controlar a escrita nessas estruturas, é utilizado, além do *wr_reg*, o sinal *wr_full*(output) que indica 
que as FIFOs estão cheias.

A escrita nas FIFOs deve ocorrer durante o período em que o sinal *screen*(output) está em alto, o que indica que a renderização de um frame foi finalizada. **Instruções escritas 
durante o processo de renderização de um frame correm o risco de ser perdidas! **
Além dos sinais descritos acima, a GPU conta com mais um sinal de controle: *reset_pulse_counter*(output). Este sinal é utilizado para resetar o contador externo responsável por contar 
o tempo de renderização de uma tela.


<div align="center">
  <figure>  
    <img src="Docs/Imagens/diagrama_gpu.png" width="500px">
    <figcaption>
      <p align="center">

[**Figura 3** - Representação da arquitetura do processador gráfico](https://drive.google.com/file/d/1MlIlpB9TSnoPGEMkocr36EH9-CFz8psO/view)

</p>
    </figcaption>
  </figure>
</details>

## Periféricos utilizados

Neste projeto, utilizou-se todos os periféricos disponíveis no hardware do processador gráfico CoLenda: a saída VGA, os 4 botões do tipo *push* e os displays de 7 segmentos. Além disso, 
utilizou-se também dois mouses conectados à placa via USB.

### 📖 Background
<details>
<summary><b>Padrão VGA</b></summary>

### Padrão VGA

O padrão VGA é um modelo bastante utilizado, estando presente principalmente nos monitores do tipo CRT.
Este padrão é composto pela transmissão de sinais de geração de vídeo e sinais de cores (RGB - red, green, blue). Segue abaixo a especificação dos dois sinais de geração de vídeo:

- sinal de sincronismo horizontal (*hsync*): indica o tempo necessário para percorrer uma linha do monitor;
- sinal de sincronismo vertical (*vsync*): indica o tempo necessário para percorrer toda a tela (finalização de um frame);

Neste padrão, um frame é definido pelos parâmetros indicados na figura 4:
- área ativa: espaço de exibição dos pixels na tela;
- front e back porches: tempos de espera a serem implementados para delimitar a região ativa

<div align="center">
  <figure>  
    <img src="Docs/Imagens/frame.PNG">
    <figcaption>
      <p align="center">

[**Figura 4** - Representação de um frame por meio do padrão VGA](https://drive.google.com/file/d/1MlIlpB9TSnoPGEMkocr36EH9-CFz8psO/view)

</p>
    </figcaption>
  </figure>
</div>

Estes parâmetros são calculados utilizando a resolução e taxa de atualização dos monitores. Os cálculos para a definição dos valores utilizados no projeto estão disponíveis na
[seção III.C deste arquivo](https://drive.google.com/file/d/1MlIlpB9TSnoPGEMkocr36EH9-CFz8psO/view). As especificações de parâmetros para diferentes resoluções podem ser encontrados no 
[manual da placa](https://fpgacademy.org/index.html)

</details>

<details>
<summary><b>USB</b></summary>

### USB
O Barramento Serial Universal, ou USB, é um padrão de conexão que permite a anexação de até 127 dispositivos sem a necessidade de reiniciar o computador, sendo assim realizadas todas as 
configurações para funcionamento do dispositivo em tempo de execução. Ademais, o padrão USB também dispensa o uso de drivers, fazendo com que toda a configuração do dispositivo seja 
automática. A comunicação entre um dispositivo _host_ (computador) e um periférico pode envolver a troca de ate três pacotes de dados, sendo o primeiro, enviado pelo host que informa 
dados como endereço do dispositivo de destino, tipo de transação, fluxo de dados, entre outros.

</details>

### 🖱️ Periféricos

<details>
<summary><b>Saída VGA e integração com a GPU</b></summary>

### Saída VGA e integração com a GPU

Na placa DE1-SoC, os sinais de sincronização são gerados diretamente pela Cyclone V SoC FPGA, com a conversão digital-analógica sendo feita por meio de Analog Devices ADV7123 para a 
representação das 3 cores RGB. Cada canal de cor RGB é representado por 8 bits e os sinais *hsync* e *vsync* são representados por 1 bit cada.
 
<div align="center">
  <figure>  
    <img src="Docs/Imagens/vga_placa.PNG">
    <figcaption>
      <p align="center">

[**Figura 5** - Conexões entre FPGA e VGA](https://fpgacademy.org/index.html)

</p>
    </figcaption>
  </figure>
</div>

No projeto, a saída VGA foi conectada a um monitor de tubo CRT - *Cathode Ray Tube* -  com resolução de 640×480 pixels (figura 6) e uma taxa de atualização de 60Hz. Os sinais de geração 
de vídeo e de cores são controlados pelo *Controlador VGA* implementado pelo processador CoLenda.

<div align="center">
  <figure>  
    <img src="Docs/Imagens/monitor.jpg" width="500px">
    <figcaption>
      <p align="center">

**Figura 6** - Monitor CRT usado

</p>
    </figcaption>
  </figure>
</div>
</details>

<details>
<summary><b>Mouse USB</b></summary>

### Mouse USB
Na placa DE1-SoC, os conectores USB estão ligados a um HUB controlador que se comunica diretamente com o HPS. Assim, o sistema operacional instalado e em execução na processador ARM da 
placa é quem gerencia as portas. Esta organização facilita o desenvolvimento e utilização das mesmas. Neste projeto, utilizou-se dois mouses idênticos (figura 7) cada um conectado à uma 
porta USB *host* da placa.

<div align="center">
  <figure>  
    <img src="Docs/Imagens/mouse.jpg" height="400px">
    <figcaption>
      <p align="center">

**Figura 6** - Mouse USB usado

</p>
    </figcaption>
  </figure>
</div>
</details>

<details>
<summary><b>Botões do tipo <i>push</i></b></summary>

### Botões do tipo *push*
A placa utilizada disponibiliza para o usuário quatro botões do tipo *push* (integrados à FPGA e à GPU), os quais possuem um registrador de dados. Este registrador possui 32 bits, sendo apenas os 4 menos significativos utilizados, e é usado para identificar quais botões estão pressionados em um determinado instante de tempo. Quando um botão é pressionado, seu bit correspondente no registrador (KEY0 -> bit 0, e assim por diante) é setado para 0. Quando liberado, o bit é setado para 1 novamente.

</details>

## O Jogo

<details>
<summary> <b>Interface do Jogador</b> </summary>

### Interface do Jogador

<!-- Interface Menu
interface partida 
interface pause
interface vitoria
interface derrota
-->
</details>

<details>
<summary> <b>Elementos do Jogo</b> </summary>

### Elementos do Jogo

<div align="center">
  <figure>  
    <img src="Docs/Imagens/sprites.png">
    
<figcaption>

**Figura** - Alguns dos sprites criados
    </figcaption>
  </figure>
</div>

</details>



<details>
<summary> <b>Como Jogar</b> </summary>

### Como Jogar
Dois players competem em uma corrida de obstáculos. O objetivo é destruir o máximo de obstáculos antes do seu oponente.
Vence o jogo quem atingir 1000 pontos primeiro ou quem ficar mais tempo sem zerar sua pontuação.
Cada obstáculo destruído acrescenta uma pontuação ao score do jogador. De maneira análoga, a colisão do carro com os obstáculos reduz uma pontuação do score. 
Jogabilidade: Por meio do mouse, cada jogador pode controlar o seu carro para desviar dos obstáculos. Para destruir um obstáculo, o jogador deve alinhar seu carro com o obstáculo e atirar com o botão esquerdo do mouse.

</details>

## Solução Geral

## Drivers e Dispositivos

## Algoritmos do Jogo

<details>
<summary> <b>Algoritimos do jogo</b> </summary>

<details>
<summary> <b>Movimento e ações do jogador</b> </summary>

### Movimento e ações do jogador

A movimentação do sprite do jogador ocorre apenas no eixo _X_, ela é feita na thread do polling do mouse, onde é feita a leitura dos eventos do mouse e após isso, é atualizada a nova posição do jogador. Caso ocorra alguma movimentação no eixo _X_, esse valor é lido e transformado em um valor valido para o jogo, de -2 a 2, e multiplicado pela velocidade base do carro que é de 2 pixeis por frame, esse valor é incrementado, ou decrementado, na posição do carro e o mesmo é exibido na tela nessa nova localização.
No momento de atualizar a posição do jogador é verificado 2 possiveis ocorrencias:

>- **O carro do jogador já esta localizado em uma das bordas:**
Neste caso nada ocorre e o carro é desenhado na mesma posição
>
>- **O deslocamento total é maior do que o espaço existente entre a borda e o carro**:  Nesse caso, é calculado o espaço de deslocamento que o carro tem antes de chegar a borda e esse valor se torna o novo deslocamento queserá realizado pelo carro.

Caso não haja nenhuma das ocorrencias anteriores a posição do carro é atualizada normalmente, e o carro é desenhado na nova posição.

Outra ação que o jogador pode fazer é clicar com o botão esquerdo do mouse pra realizar disparos, a geração desses disparos ocorre na thread do polling do mouse, porém a atualização de sua posição e checagem de colisão ocorre em outras threads.

No momento que ocorre a captura de um evento de clique, verifica-se se é do botão esquerdo e é verificado também se o jogador pode realizar algum disparo, cada jogador tem 5 disparos, a cada disparo esse valor é decrementado, quando um tiro chega a borda superior da tela ou colide com um obstáculo esse contador é incrementado e assim o jogador pode vir a realizar um novo disparo.

</details>
<details>
<summary> <b>Detecção de colisão</b> </summary>

### Detecção de colisão

A detecção de colisão no jogo funciona analisando se alguma parte da area do jogador esta dentro da area do objeto, a imagem abaixo descreve um caso de colisão.

![Exemplo de Colisão](/Docs/Imagens/Exemplo_colisão.png)

>[!NOTE] A colisão pode ocorrer lateralmente tambem

Quando é detectada a colisão do jogador com um obstáculo é iniciado um timer e o jogador fica invuneravel por 1 segundo, esse tempo de invunerabilidade é indicado pelo piscar do carro e, enquanto estiver nesse estado, a colisão do jogador é desabilitada permitindo que o mesmo perceba e desvie dos próximos obstáculos. Outro evento que ocorre em conjunto é o desaparecer do obstáculo a qual o jogador colidiu e a diminuição da pontuação daquele objeto dos pontos do jogador.

> O mesmo calculo e verificação de colisão é realizado para os disparos, com a diferença que o disparo possui uma area menor que o obstáculo e o jogador.

Uma diferença entre a colisão de um disparo com um obstáculo é que quando colide com um obstáculo o sprite do obstáculo é trocado por uma chama que desaparece após um curto período, apenas pra indicar que naquele local houve uma colisão. No momento que é detectada a colisão do disparo com um obstáculo é acrescida a pontuação do jogador o valor do objeto destruído.

Outros dois pontos de analise de colisão é o disparo chegar ao topo da tela e o obstáculo chegar ao final, onde os mesmos devem desaparecer, nesse caso é analisado se o valor da coordenada _Y_ deles são iguais a 479, pros obstáculos, e 0, para os disparos.

Ao final é verificado se algum jogador perdeu, _sua pontuação ser menor que zero_, ou se algum ganhou, _pontuação ser maior ou igual a 1000_, nesses casos são chamadas as telas de **vitória** ou **derrota** respectivamente.

</details>
<details>
<summary> <b>Movimentação de objetos</b> </summary>

### Movimentação de objetos

A atualização da posição de alguns sprites e o cenário de fundo é realizado em funções distintas e abaixo segue uma breve explicação de como objetos distintos tem suas posições atualizadas.

</details>
<details>
<summary> <b>Movimentação do fundo</b> </summary>

#### Movimentação do fundo

Para passar a sensação de movimento é realizada a alteração da pista do fundo, dando a entender que o carro se move para frente, entretanto o mesmo permanece parado no eixo. Devido a limitações de hardware a unica parte da pista que realmente é alterada a cada 100 ms são as listras brancas, o que diminui significativamente o gasto de recursos de hardware, outra opção seria redesenhar toda a pista a cada frame mas foi notado que seria muito custoso e afetaria de maneira significativa o desempenho do jogo pois introduziria latencia e ghosting no movimento do usuario
>**Ghosting:** Termo utilizado para quando se tem um processador mais rapido que a unidade grafica e assim a imagem que é exibida na tela não esta sincronizada com o que esta sendo processado dando a impressão de _latência_ ou imprecisão nos comandos relizados.
>
>**Latencia:** Termo utilizado para se referir ao tempo que uma instrução leva para ser processada e exibida na tela

</details>
<details>
<summary> <b>Movimentação dos disparos</b> </summary>

#### Movimentação dos disparos

Os disparos realizados pelo jogador tem sua posição atualizada em uma thread diferente daquela onde são gerados, isso ocorre para garantir que os disparos vão ter sua atualização independente da leitura de algum evento do mouse.

A thread de atualização dos disparos fica responsavel por pegar a posição deles e subtrair o valor padrão da aceleração no eixo _Y_, pois como eles estão subindo o valor da sua coordenada _Y_ deve reduzir com o passar do tempo. Quando os disparos chegam a borda superior da tela sem colidir com nenhum obstáculo, ou seja, seu valor pra _Y_ é 0 ou menor que 15, o disparo desaparece e o jogador ganha mais uma munição para disparar.

</details>
<details>
<summary> <b>Moviemntação dos obstáculos</b> </summary>

#### Movimentação dos obstáculos

</details>
</details>

<details>
<summary> <b>Fluxo do jogo</b> </summary>

## Fluxo do jogo

O diagrama abaixo descreve o fluxo de execução do jogo

![Fluxo do Jogo](/Docs/Imagens/jogo.png)

Para garantir que o jogo respondesse aos comandos de maneira eficaz e um bom aproveitamento do hardware, o software foi divido em threads onde cada uma possui suas reponsabilidades de maneira que uma thread não interferisse de maneira direta no funcionamento de outra.
>O unico momento que uma thread vem a interferir no funcionamento das demais é na ocorrencia do acesso a variaveis compartilhadas

A tabela abaixo descreve as threads criadas e suas responsabilidades

Thread              |                  Responsabilidade
:----------------------|:-----------------------------
_Polling do mouse do jogador 1 e 2_| Realiza a leitura dos eventos do mouse e realiza o tratamento dessas entradas
_Atualização do background_ | Faz a atualização da pista para criar impressão de movimento
_Atualização dos disparos_ | Faz a atualização da posição dos disparos de ambos os jogadores
_Timers de invunerabilidade 1 e 2_ | Timer ativado quando um jogador colide com um obstáculo
_Verificação de colisão_ | Responsavel por verificar a colisão e fazer a checagem de condição de vitória ou derrota
Geração de obstáculos | Gera os obstáculos na tela e faz a atualização de suas posições
_Menu_ | Menu do jogo, fica rodando durante o jogo realizando a captua das entradas dos botões da FPGA



<details>
<summary> <b>Controle do fluxo da execulçao das threads</b> </summary>

### Controle do fluxo da execução das threads

O fluxo do jogo é controlado por uma maquina de estados simples que esta na thread principal, a thread do menu. O diagrama abaixo descreve em alto nivel a maquina de estados e suas condições de transição

![Maquina de Estados](/Docs/Imagens/fluxograma_estados_jogo.png)

No caso a thread principal do menu fica responsavel por alterar entre esses estados e assim se torna possivel gerenciar quando algumas threads devem ser pausadas ou não.

>Exemplo: enquanto o usuario está no menu não há motivos para nenhuma outra thread está rodando.

Assim, foram estabelecidas condições de parada das threads, uma das condições de pause das threads é o jogo estar no menu, outra condição de pausa das threads é o jogo estar em estado de pause durante alguma partida. Caso algum jogador ganhe, as threads tambem são pausadas.


<details>
<summary> <b>Casos especificos de pause das threads</b> </summary>

#### Casos especificos de pause das threads

como há um grande compartilhamento de variaveis e vetores, em certos momentos é necessario que algumas threads sejam interrompidas para que o valor lido seja o correto.
>Exemplo: Quando o jogador colide com um obstáculo, tanto a thread de geração dos obstáculos quanto a thread do polling do mouse daquele jogador são pausadas.

A seguinte tabela descreve os cenarios e casos particulares de quando cada thread é interrompida visando a garantia da consistência dos dados.

Caso    | Threads que sao paradas
:------|:-----------------------------
_Jogador colide com obstáculo_ | Obstáculos, Colisão e Polling do mouse
_Disparo colide com obstáculo_ | Obstáculos, Atualização dos disparos, Polling do mouse.
_Partida single player_ | Polling do mouse do segundo jogador

</details>

<details>
<summary> <b>Criação e finalização das threads</b> </summary>

#### Criação e finalização das threads

Todas as threads são criadas na inicialização do jogo e a depender do modo de jogo escolhido apenas a thread do mouse do segundo jogador tem sua execução opcional. Ao invés de encerrar as threads foi optado por apenas pausar suas execuções enquanto não forem utilizadas.

O unico momento em que as threads são de fato finalizadas é o momento em que o jogador opta por encerrar o jogo, só então são finalizadas e o programa é encerrado.

</details>
</details>
</details>



## Testes

## Contribuições

