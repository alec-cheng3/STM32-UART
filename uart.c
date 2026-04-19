#include "stm32l4xx.h"
#include "UART.h"

#define ESC 0x1b

void UART_init(void)
{
    // Enable clock to GPIOA
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    // Alternate function mode for PA2 (TX) and PA3 (RX)
    GPIOA->MODER &= ~(GPIO_MODER_MODE2 | GPIO_MODER_MODE3);
    GPIOA->MODER |= (GPIO_MODER_MODE2_1 | GPIO_MODER_MODE3_1);

    // Alternate function AF7 for USART2
    GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL2 | GPIO_AFRL_AFSEL3);
    GPIOA->AFR[0] |= (7 << GPIO_AFRL_AFSEL2_Pos) |
                     (7 << GPIO_AFRL_AFSEL3_Pos);

    // Enable clock to USART2
    RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN;

    // Enable receive interrupt
    USART2->CR1 |= USART_CR1_RXNEIE;

    // Baud rate
    uint16_t USART_DIV = 4000000 / 115200;
    USART2->BRR = USART_DIV;

    // Enable transmitter and receiver
    USART2->CR1 |= USART_CR1_TE | USART_CR1_RE;

    // Enable USART
    USART2->CR1 |= USART_CR1_UE;

    // Enable USART2 interrupt in NVIC
    NVIC->ISER[1] = (1 << (USART2_IRQn & 0x1F));
}

void UART_print(char *str)
{
    int i = 0;

    while (str[i] != '\0')
    {
        while (!(USART2->ISR & USART_ISR_TXE));
        USART2->TDR = str[i];
        i++;
    }

    while (!(USART2->ISR & USART_ISR_TC));
}

void UART_ESC_Code(void)
{
    while (!(USART2->ISR & USART_ISR_TXE));
    USART2->TDR = ESC;
}
