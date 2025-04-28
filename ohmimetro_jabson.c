/*
 * Por: Wilton Lacerda Silva + Adaptação para mostrar faixas conforme resistência
 *    Ohmímetro utilizando o ADC da BitDogLab
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include "pico/stdlib.h"
 #include "hardware/adc.h"
 #include "hardware/i2c.h"
 #include "lib/ssd1306.h"
 #include "lib/font.h"
 
 #define I2C_PORT i2c1
 #define I2C_SDA 14
 #define I2C_SCL 15
 #define endereco 0x3C
 #define ADC_PIN 28
 #define Botao_A 5
 #define botaoB 6
 
 int R_conhecido = 22000;   
 float ADC_VREF = 2.26;     //Tensão medida para funcionar com a bateria
 int ADC_RESOLUTION = 4095;
 

 //Função para identificar as faixas de cores -- considerando a % de sensibilidade
 void atribuir_faixas(float resistencia, char *faixa1, char *faixa2, char *faixa3) {
     if (resistencia >= 680 && resistencia <= 800) {
         strcpy(faixa1, "Azul");
         strcpy(faixa2, "Cinza");
         strcpy(faixa3, "Marrom");
     } else if (resistencia >= 900 && resistencia <= 1100) {
         strcpy(faixa1, "Marrom");
         strcpy(faixa2, "Preto");
         strcpy(faixa3, "Vermelho");
     } else if (resistencia >= 1000 && resistencia <= 1100) {
         strcpy(faixa1, "Vermelho");
         strcpy(faixa2, "Preto");
         strcpy(faixa3, "Marrom");
        } else if (resistencia >= 9000 && resistencia <= 11000) {
            strcpy(faixa1, "Marrom");
            strcpy(faixa2, "Preto");
            strcpy(faixa3, "Laranja");
        }
      else if (resistencia >= 21000 && resistencia <= 23000) {
         strcpy(faixa1, "Vermelho");
         strcpy(faixa2, "Marrom");
         strcpy(faixa3, "Laranja");
     } else if (resistencia >= 85000 && resistencia <= 110000) {
         strcpy(faixa1, "Marrom");
         strcpy(faixa2, "Preto");
         strcpy(faixa3, "Amarelo");
     } else {
         strcpy(faixa1, "---");
         strcpy(faixa2, "---");
         strcpy(faixa3, "---");
     }
 }
 
 // Trecho para modo BOOTSEL
 #include "pico/bootrom.h"
 void gpio_irq_handler(uint gpio, uint32_t events) {
     reset_usb_boot(0, 0);
 }
 
 int main() {
     gpio_init(botaoB);
     gpio_set_dir(botaoB, GPIO_IN);
     gpio_pull_up(botaoB);
     gpio_set_irq_enabled_with_callback(botaoB, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
 
     gpio_init(Botao_A);
     gpio_set_dir(Botao_A, GPIO_IN);
     gpio_pull_up(Botao_A);
 
     // Inicializa I2C
     i2c_init(I2C_PORT, 400 * 1000);
     gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
     gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
     gpio_pull_up(I2C_SDA);
     gpio_pull_up(I2C_SCL);
 
     ssd1306_t ssd;
     ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);
     ssd1306_config(&ssd);
     ssd1306_send_data(&ssd);
 
     ssd1306_fill(&ssd, false);
     ssd1306_send_data(&ssd);
 
     adc_init();
     adc_gpio_init(ADC_PIN);
 
     float tensao;
     float R_x;
     float resistencia;
     char str_x[10];
     char str_y[10];
     char faixa1[10], faixa2[10], faixa3[10];
     char linha1[20], linha2[20], linha3[20];
 
     bool cor = true;
 
     while (true) {
         adc_select_input(2);
 
         float soma = 0.0f;
         for (int i = 0; i < 500; i++) {
             soma += adc_read();
             sleep_ms(1);
         }
         float media = soma / 500.0f;
 
         // Cálculo da resistência
         R_x = (R_conhecido * media) / (ADC_RESOLUTION - media);
 
         // Agora resistencia é igual a R_x
         resistencia = R_x;
 
         sprintf(str_x, "%1.0f", media);
         sprintf(str_y, "%1.0f", resistencia);
 
         atribuir_faixas(resistencia, faixa1, faixa2, faixa3);
 
         sprintf(linha1, "f1: %s", faixa1);
         sprintf(linha2, "f2: %s", faixa2);
         sprintf(linha3, "f3: %s", faixa3);
 
         ssd1306_fill(&ssd, !cor);
         ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor);
         ssd1306_line(&ssd, 3, 31, 123, 31, cor);
         //ssd1306_line(&ssd, 3, 37, 123, 37, cor);
 
         ssd1306_draw_string(&ssd, linha1, 5, 5);
         ssd1306_draw_string(&ssd, linha2, 5, 14);
         ssd1306_draw_string(&ssd, linha3, 5, 22);
 
         ssd1306_draw_string(&ssd, "ADC", 13, 41);
         ssd1306_draw_string(&ssd, "Resisten.", 50, 41);
 
         ssd1306_line(&ssd, 44, 31, 44, 60, cor);
 
         ssd1306_draw_string(&ssd, str_x, 8, 52);
         ssd1306_draw_string(&ssd, str_y, 59, 52);
 
         ssd1306_send_data(&ssd);
         sleep_ms(700);
     }
 }
 