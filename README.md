# README - Ohmímetro com Raspberry Pi Pico e Display SSD1306

## Sobre o projeto

Este projeto implementa um **ohmímetro digital** utilizando o **Raspberry Pi Pico**, um **display OLED SSD1306** e a leitura analógica de um divisor de tensão via **ADC**. Com base na leitura da resistência medida, o sistema identifica as **faixas de cores** correspondentes de um resistor e as exibe no display.

## Componentes utilizados
- Raspberry Pi Pico
- Display OLED SSD1306 (I2C)
- Resistores (de valor conhecido e desconhecido)
- Botões para controle (opcional, inclui Botão A e Botão B)

## Funcionamento

1. O **Raspberry Pi Pico** realiza a leitura da tensão analógica do divisor de tensão via pino **ADC2 (GPIO 28)**.
2. Com a tensão lida, é calculada a **resistência desconhecida** (`R_x`) usando a fórmula:

   ```
   R_x = (R_conhecido * media) / (ADC_RESOLUTION - media)
   ```
   onde `media` é a média das leituras ADC.

3. A variável `resistencia` recebe o valor de `R_x`.

4. De acordo com o valor de `resistencia`, o programa determina as **três faixas de cor** do resistor:
   - Ex: Se `resistencia` estiver entre 680 e 800 ohms, as faixas são "Azul", "Cinza" e "Marrom".

5. As faixas e as medições são exibidas no **display SSD1306**, organizadas graficamente.

6. O **Botão B** permite reiniciar o Raspberry Pi Pico em **modo BOOTSEL** para atualização de firmware.

## Tela do Display
- Mostra três linhas com as cores das faixas:
  - Faixa 1: (Cor 1)
  - Faixa 2: (Cor 2)
  - Faixa 3: (Cor 3)
- Exibe também o valor bruto da leitura ADC e o valor da resistência calculada.

## Mapeamento de cores das faixas

| Faixa de Resistência | Faixa 1 | Faixa 2 | Faixa 3 |
| :------------------- | :------ | :------ | :------ |
| 680 ô 800 Ω          | Azul    | Cinza   | Marrom |
| 900 ô 1100 Ω         | Marrom  | Preto   | Vermelho |
| 1000 ô 1100 Ω        | Vermelho| Preto   | Marrom |
| 21000 ô 23000 Ω      | Vermelho| Marrom  | Laranja |
| 85000 ô 110000 Ω     | Preto   | Marrom  | Amarelo |

Se a resistência não se enquadrar em nenhuma faixa, são exibidos traços (`---`).

Esses valores foram considerados de acordo com uma faixa maior de variação de 5%, devido a sensibilidade do resistor conhecido que foi utilizado, no vídeo explico afundo sobre

## Observações importantes
- **Ajuste do R_conhecido:** Certifique-se que o resistor conhecido utilizado é de 22kΩ ou ajuste o valor de `R_conhecido` no código.
- **Fonte de alimentação estável:** Flutuações na tensão de referência (`ADC_VREF`) podem afetar a precisão.
- **Display:** O SSD1306 é inicializado usando I2C1 nos pinos **GPIO 14 (SDA)** e **GPIO 15 (SCL)**.

## Estrutura principal do código
- **Configura GPIOs** para botoes, I2C e ADC.
- **Leitura de ADC** com média de 500 amostras.
- **Cálculo de resistência**.
- **Identifica as faixas de cores**.
- **Atualiza o display** com as informações.
- **Controle de modo BOOTSEL** através do Botão B.

