#include "main.h"
#include "usart.h"

void SystemClock_Config(void);

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    UART_init();

    char *str = "[3B";
    char *str2 = "[5C";

    UART_ESC_Code();
    UART_print(str);

    UART_ESC_Code();
    UART_print(str2);

    UART_print("All good students read the");

    UART_ESC_Code();
    UART_print("[1B");

    UART_ESC_Code();
    UART_print("[21D");

    UART_ESC_Code();
    UART_print("[5m");
    UART_print("Reference Manuel");

    UART_ESC_Code();
    UART_print("[H");

    UART_ESC_Code();
    UART_print("[0m");
    UART_print("Input: ");

    while (1)
    {
    }
}

void USART2_IRQHandler(void)
{
    if (USART2->ISR & USART_ISR_RXNE)
    {
        char received_char = USART2->RDR;

        if (received_char == 'R')
        {
            UART_ESC_Code();
            UART_print("[31m"); // red
        }
        else if (received_char == 'B')
        {
            UART_ESC_Code();
            UART_print("[36m"); // blue/cyan in ANSI
        }
        else if (received_char == 'G')
        {
            UART_ESC_Code();
            UART_print("[32m"); // green
        }
        else if (received_char == 'W')
        {
            UART_ESC_Code();
            UART_print("[37m"); // white
        }
        else
        {
            USART2->TDR = received_char;
        }
    }
}
