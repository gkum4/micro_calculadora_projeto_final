// serial.c
// Desenvolvido para a placa EK-TM4C1294XL
// Inicializa a porta serial UART0
// Prof. Guilherme Peron

#include <stdint.h>

#include "tm4c1294ncpdt.h"

#define UART_UART0 (0x0001) //bit 0
#define GPIO_PORTA (0x0001) //bits 0

// -------------------------------------------------------------------------------
// Fun��o Uart0_Init
// Inicializa a serial Uart0
// Par�metro de entrada: N�o tem
// Par�metro de sa�da: N�o tem
void Uart0_Init(void)
{
    //1a. Ativar o clock da UART0
    SYSCTL_RCGCUART_R |= UART_UART0;        //ativar o clock da UART0
    //1b. Verificar no PRUART se a UART est� pronta para uso.
    while((SYSCTL_PRUART_R & (UART_UART0) ) != (UART_UART0) ){};
    // 2. Desabilitar a UART
    UART0_CTL_R &= ~0x0001;
    // 3. Baud-rate nos registradores UARTIBRD e UARTFBRD
    // Para um clock de 80 MHz e um baud de 9600 bps
    UART0_IBRD_R = 104;  //Parte inteira
    UART0_FBRD_R = 11;  //Parte fracion�ria    
    // 4. Configurar 8 bits, sem FIFO, sem paridade e 1 stop-bit
    UART0_LCRH_R = 0x0062;
    // 5. Garantir que a fonte de clock seja o clock do sistema
    UART0_CC_R = 0x0000;
    // 6. Habilitar o RXE, TXE e UART
    UART0_CTL_R = 0x0301;        
    
    //GPIO
    //7a. Habilitar o clock no GPIO do port A
    SYSCTL_RCGCGPIO_R |= GPIO_PORTA;        //ativar o clock do PortA
    //7b. Verificar no PRGPIO se a porta est� pronta para uso.
    while((SYSCTL_PRGPIO_R & (GPIO_PORTA) ) != (GPIO_PORTA) ){};
    // 8. Desabilitar a funcionalidade anal�gica 
    GPIO_PORTA_AHB_AMSEL_R &= ~0x03;
    // 9. Escolher a fun��o alternativa para TX e RX 
    // Fun��o 1 nos 2 nibles inferiores    
    GPIO_PORTA_AHB_PCTL_R = (GPIO_PORTA_AHB_PCTL_R & 0xFFFFFF00) | 0x00000011;
    //10. Habilitar os bits de fun��o alternativa nos bits 0 e 1
    GPIO_PORTA_AHB_AFSEL_R |= 0x03;
    //11. Habilitar a fun��o digital
    GPIO_PORTA_AHB_DEN_R |= 0x03;
}

// -------------------------------------------------------------------------------
// Fun��o Uart0_Rcv
// Espera a recep��o de um byte da serial
// Par�metro de entrada: N�o tem
// Par�metro de sa�da: Byte recebido da serial
uint8_t Uart0_Rcv(void)
{
    // Esperar enquanto a FIFO estiver vazia
    while ((UART0_FR_R&0x0010) !=0) {};
    // Verificar se a FIFO n�o est� vazia, ent�o h� caracter para receber
      return ((uint8_t)(UART0_DR_R & 0xFF));
}

// -------------------------------------------------------------------------------
// Fun��o Uart0_Send
// Espera a recep��o de um byte da serial
// Par�metro de entrada: Byte a ser enviado pela serial
// Par�metro de sa�da: N�o tem
void Uart0_Send(uint8_t dado)
{
    // Esperar enquanto a FIFO estiver cheia
    while ((UART0_FR_R&0x0020) !=0) {};
    // Escreve no registrador de dados o dado a ser enviado
    UART0_DR_R = dado;
}
