#include "stm32l4xx.h"
#define ESC 0x1b
void UART_init(void){
// enable clock to GPIO PINS
RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
// Alternative Function Mode
GPIOA->MODER &= ~(GPIO_MODER_MODE2|
GPIO_MODER_MODE3);
GPIOA->MODER |= (GPIO_MODER_MODE2_1|
GPIO_MODER_MODE3_1);
// Alternative Function 7
GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL2|
GPIO_AFRL_AFSEL3);
GPIOA->AFR[0] |= (7 << GPIO_AFRL_AFSEL2_Pos|
7 << GPIO_AFRL_AFSEL3_Pos);
// enable clk to UART
RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;
// enable receive interrupts
USART2->CR1 |= USART_CR1_RXNEIE; // enable RX interrupt
// enabling baud rate
uint16_t USART_DIV = 4000000 / 115200; // 8MHz/9600
USART2->BRR = USART_DIV;
// Configure the UART
USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;
USART2->CR1 |= USART_CR1_UE; //enable UART
// enable interrupt vector table
NVIC->ISER[1] = (1 << (USART2_IRQn & 0x1F));
} //34.72
void UART_print(char *str) {
int i = 0;
// iterate over the characters of the string
while (str[i] != '\0') {
// wait until the transmit buffer is empty
while (!(USART2->ISR & USART_ISR_TXE));
// send the character over USART
USART2->TDR = str[i];
// move to the next character in the string
i++;
}
while (!(USART2->ISR & USART_ISR_TC));
}
void UART_ESC_Code(){
while (!(USART2->ISR & USART_ISR_TXE));
USART2->TDR = ESC;
}
