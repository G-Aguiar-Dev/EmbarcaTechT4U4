// Inclusão de Bibliotecas
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "T4U4.pio.h"

// Definição de variáveis globais
uint valor_led;
PIO pio;
uint sm;
int static volatile id = 0;
uint volatile cont = 0;
uint static volatile last_time = 0;

// LED RGB
#define LED_PIN_GREEN 11
#define LED_PIN_BLUE 12
#define LED_PIN_RED 13

// FUNÇÃO PARA PISCAR OS LEDs
void piscar_leds(int led, int tempo)
{
  gpio_put(led, 1);
  sleep_ms(tempo);
  gpio_put(led, 0);
  sleep_ms(tempo);
}

// MATRIZ DE LEDs
// Definição de variáveis
#define LED_PIN 7
#define NUM_PIXELS 25

// FUNÇÃO PARA DEFINIÇÃO DE INTENSIDADE DE CORES NOS LEDs
uint matrix_rgb(float r, float g, float b)
{
  unsigned char R, G, B;
  R = r * 255;
  G = g * 255;
  B = b * 255;
  return (G << 24) | (R << 16) | (B << 8);
}

// Função para converter a posição do matriz para uma posição do vetor.
int getid(int x, int y)
{
  // Se a linha for par (0, 2, 4), percorremos da esquerda para a direita.
  // Se a linha for ímpar (1, 3), percorremos da direita para a esquerda.
  if (y % 2 == 0)
  {
    return 24 - (y * 5 + x); // Linha par (esquerda para direita).
  }
  else
  {
    return 24 - (y * 5 + (4 - x)); // Linha ímpar (direita para esquerda).
  }
}

// FUNÇÃO DESENHO P/ MATRIZ DE LEDS
void desenho_pio(double *desenho, uint32_t valor_led, PIO pio, uint sm, double r, double g, double b)
{

  for (int16_t i = 0; i < NUM_PIXELS; i++)
  {
    valor_led = matrix_rgb(desenho[i] * r, desenho[i] * g, desenho[i] * b);
    pio_sm_put_blocking(pio, sm, valor_led);
  };
}

// SPRITES
double apagar_leds[25] = {0.0, 0.0, 0.0, 0.0, 0.0, // Apagar LEDs
                          0.0, 0.0, 0.0, 0.0, 0.0,
                          0.0, 0.0, 0.0, 0.0, 0.0,
                          0.0, 0.0, 0.0, 0.0, 0.0,
                          0.0, 0.0, 0.0, 0.0, 0.0};

double numero_1[25] =    {0.0, 0.0, 1.0, 0.0, 0.0, // Algarismo 1
                          0.0, 0.0, 1.0, 0.0, 0.0,
                          0.0, 0.0, 1.0, 0.0, 0.0,
                          0.0, 1.0, 1.0, 0.0, 0.0,
                          0.0, 0.0, 1.0, 0.0, 0.0};

double numero_2[25] =    {1.0, 1.0, 1.0, 1.0, 1.0, // Algarismo 2
                          1.0, 0.0, 0.0, 0.0, 0.0,
                          1.0, 1.0, 1.0, 1.0, 1.0,
                          0.0, 0.0, 0.0, 0.0, 1.0,
                          1.0, 1.0, 1.0, 1.0, 1.0};

double numero_3[25] =    {1.0, 1.0, 1.0, 1.0, 1.0, // Algarismo 3
                          0.0, 0.0, 0.0, 0.0, 1.0,
                          1.0, 1.0, 1.0, 0.0, 0.0,
                          0.0, 0.0, 0.0, 0.0, 1.0,
                          1.0, 1.0, 1.0, 1.0, 1.0};

double numero_4[25] =    {1.0, 0.0, 0.0, 0.0, 0.0, // Algarismo 4
                          0.0, 0.0, 0.0, 0.0, 1.0,
                          1.0, 1.0, 1.0, 1.0, 1.0,
                          1.0, 0.0, 0.0, 0.0, 1.0,
                          1.0, 0.0, 0.0, 0.0, 1.0};

double numero_5[25] =    {1.0, 1.0, 1.0, 1.0, 1.0, // Algarismo 5
                          0.0, 0.0, 0.0, 0.0, 1.0,
                          1.0, 1.0, 1.0, 1.0, 1.0,
                          1.0, 0.0, 0.0, 0.0, 0.0,
                          1.0, 1.0, 1.0, 1.0, 1.0};

double numero_6[25] =    {1.0, 1.0, 1.0, 1.0, 1.0, // Algarismo 6
                          1.0, 0.0, 0.0, 0.0, 1.0,
                          1.0, 1.0, 1.0, 1.0, 1.0,
                          1.0, 0.0, 0.0, 0.0, 0.0,
                          1.0, 1.0, 1.0, 1.0, 1.0};

double numero_7[25] =    {0.0, 0.0, 0.0, 0.0, 1.0, // Algarismo 7
                          0.0, 1.0, 0.0, 0.0, 0.0,
                          0.0, 0.0, 1.0, 0.0, 0.0,
                          0.0, 0.0, 0.0, 1.0, 0.0,
                          1.0, 1.0, 1.0, 1.0, 1.0};

double numero_8[25] =    {1.0, 1.0, 1.0, 1.0, 1.0, // Algarismo 8
                          1.0, 0.0, 0.0, 0.0, 1.0,
                          1.0, 1.0, 1.0, 1.0, 1.0,
                          1.0, 0.0, 0.0, 0.0, 1.0,
                          1.0, 1.0, 1.0, 1.0, 1.0};

double numero_9[25] =    {1.0, 1.0, 1.0, 1.0, 1.0, // Algarismo 9
                          0.0, 0.0, 0.0, 0.0, 1.0,
                          1.0, 1.0, 1.0, 1.0, 1.0,
                          1.0, 0.0, 0.0, 0.0, 1.0,
                          1.0, 1.0, 1.0, 1.0, 1.0};

double numero_0[25] =    {1.0, 1.0, 1.0, 1.0, 1.0, // Algarismo 0
                          1.0, 0.0, 0.0, 0.0, 1.0,
                          1.0, 0.0, 0.0, 0.0, 1.0,
                          1.0, 0.0, 0.0, 0.0, 1.0,
                          1.0, 1.0, 1.0, 1.0, 1.0};

double *sprites[10] = {numero_0, numero_1, numero_2, numero_3, numero_4, numero_5, numero_6, numero_7, numero_8, numero_9};

// BOTÕES
// Definição de variáveis
#define BOTAO_A 5
#define BOTAO_B 6

// FUNÇÃO PARA CORREÇÃO DO ID
void correcao_id() {
  if (id < 0){
    id = 9;
  }
  else if (id > 9){
    id = 0;
  }
}

// FUNÇÃO PARA CALLBACK DOS BOTÕES
void callback_button(uint botao, uint32_t events)
{
  uint time = to_ms_since_boot(get_absolute_time()); // Tempo em milissegundos desde o boot
  if (time - last_time > 250) //Condição para evitar múltiplos pressionamentos (debounce)
  {
    last_time = time; // Atualiza o último tempo de pressionamento
    printf("Contador: %d\n", cont); //Conferência do contador para demonstrar o debounce
    
    if (!gpio_get(botao)){ // Condição para verificar se o botão foi pressionado
       if (botao == BOTAO_A){ // Condição para verificar qual botão foi pressionado
        id++; // Incrementa o id
        correcao_id();
        printf("Valor do id: %d\n", id); // Conferência do id
        desenho_pio(sprites[id], valor_led, pio, sm, 0.1, 0.1, 0.1); // Desenha o sprite correspondente ao id
        cont++; // Incrementa o contador
        printf("Valor do contador: %d\n", cont); // Conferência do contador dentro da função para demonstrar o debounce
       }
       else {
        id--; // Decrementa o id
        correcao_id();
        printf("Valor do id: %d\n", id); // Conferência do id
        desenho_pio(sprites[id], valor_led, pio, sm, 0.1, 0.1, 0.1); // Desenha o sprite correspondente ao id
        cont++; // Incrementa o contador
        printf("Valor do contador: %d\n", cont); // Conferência do contador dentro da função para demonstrar o debounce
       }
    }
  }
}

// FUNÇÃO SETUP
void setup(){
    // Inicializa a matriz de LED
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    // Inicializa os botões
    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);
    gpio_init(BOTAO_B);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_pull_up(BOTAO_B);
    // Inicializa o LED RGB
    gpio_init(LED_PIN_RED);
    gpio_set_dir(LED_PIN_RED, GPIO_OUT);
    gpio_init(LED_PIN_GREEN);
    gpio_set_dir(LED_PIN_GREEN, GPIO_OUT);
    gpio_init(LED_PIN_BLUE);
    gpio_set_dir(LED_PIN_BLUE, GPIO_OUT);
}

// FUNÇÃO PRINCIPAL
int main()
{
  // Definição de variáveis
  bool frequenciaClock;
  uint16_t i;
  float r = 0.0, b = 0.0, g = 0.0;
  pio = pio0;

  frequenciaClock = set_sys_clock_khz(128000, false); // frequência de clock
  stdio_init_all(); // inicialização da comunicação serial
  setup(); // inicialização dos pinos

  // configurações da PIO
  printf("iniciando a transmissão PIO");
  if (frequenciaClock)
    printf("clock set to %ld\n", clock_get_hz(clk_sys));
  uint offset = pio_add_program(pio, &pio_matrix_program);
  sm = pio_claim_unused_sm(pio, true);
  pio_matrix_program_init(pio, sm, offset, LED_PIN);

  desenho_pio(apagar_leds, valor_led, pio, sm, r, g, b); // Apaga os LEDs ao iniciar o programa

  // Interrupção dos botões
  gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &callback_button);
  gpio_set_irq_enabled_with_callback(BOTAO_B, GPIO_IRQ_EDGE_FALL, true, &callback_button);

  // Loop principal
  while (true)
  {
    piscar_leds(LED_PIN_RED, 200); // Piscar o LED vermelho 5x por segundo
  }

  return 0;
}
