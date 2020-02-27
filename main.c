//*****************************************************************
// A custom device for transmitting/recieving CAN traffic
// 
// You are free to use this code at your own discretion
// Alex Svetlichnyy 2020
//*****************************************************************
#include "stm32f10x.h"
#include "main.h"

volatile uint32_t TimingDelay;    														 // The value for Systick delay
																															 // The loop buffer used to store recieved CAN-traffic

/* Setting up a systick interrupt. It counts from a delay value to zero*/
void SysTick_Handler(void) {   
	if (TimingDelay){
		TimingDelay--;
  }      
}
/* Setting up a delay value*/
void Delay_mS(uint32_t nTime){
	TimingDelay = nTime;
	while (TimingDelay);
 }



 /******************INTERRUPTS BEGINNING**************************/
 /******************INTERRUPTS BEGINNING**************************/
 /******************INTERRUPTS BEGINNING**************************/


/* Setting up a CAN RX interrupt. Check for FIFO0 not empty event*/	
void USB_LP_CAN1_RX0_IRQHandler(void) {
}

/* Setting up an USART1 RX interrupt*/		
void USART1_IRQHandler(void){	
}

	
 /******************INTERRUPTS END**************************/
 /******************INTERRUPTS END**************************/
 /******************INTERRUPTS END**************************/





int main(void){
	Init_IWDG(500);
	ClockInit();   							// Start HSE, PLL, Flash latency, all the RCC configuration
	EnablePeripherals(); 				// Enable all the Peripherial clocks
	SysTick_Config(72000-1); 		// interrupt in 10 states
	

	Usart1PinConfig(REMAP_USART1_RX_TX_PORTS, USART1_CTS_RTS_DISABLED); // Configure USART pins
	Usart1Config(USART1_IRQ_OFF, USART1_DMA_ON, BAUDRATE_APB1_36_115200);			// Configure USART peripherals
	Usart1Start(USART1_IRQ_OFF);																					// Start USART with its interrupts enabled
	


//	NVIC_EnableIRQ (USART1_IRQn);						// Enable Interrupts 
//	NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
	
	__enable_irq ();		


	Can1_Initializate(NO_REMAP_CAN1, CAN_BAUDRATE_500KB);  
	Can1_InitializateFilters();	
//	Can1_InitializateFilter_1DC();
	


// ***************************************** STB PIN CONTROL **********************************
GPIOB->CRL	&= ~GPIO_CRL_CNF3;						// 
GPIOB->CRL 	|= GPIO_CRL_MODE3_0;					//
GPIOB->CRL 	|= GPIO_CRL_MODE3_1;
GPIOB->BSRR |= GPIO_BSRR_BR3;						// 

GPIOB->CRL	&= ~GPIO_CRL_CNF4;						// 
GPIOB->CRL 	|= GPIO_CRL_MODE4_0;					//
GPIOB->CRL 	|= GPIO_CRL_MODE4_1;
GPIOB->BSRR |= GPIO_BSRR_BR4;						// 


// ***************************************** STB PIN CONTROL **********************************


	static Std_Can_Message_Type _1DC_Message;
	
	_1DC_Message.id_highbyte=0x1;
	_1DC_Message.id_lowbyte=0xDC;
	_1DC_Message.dlc=4;
	_1DC_Message.data[3]=0x33;
	_1DC_Message.data[2]=0xA8;
	_1DC_Message.data[1]=0x04;
	_1DC_Message.data[0]=0x02;
	_1DC_Message.data[4]=0x0;
	_1DC_Message.data[5]=0x0;
	_1DC_Message.data[6]=0x0;
	_1DC_Message.data[7]=0x0;
			
	uint32_t injection_delay = 12; //injection delay - 12ms

	while(1){
		IWDG_reset();	
		
		Delay_mS(injection_delay);
		Can1_Transmit_StdMessage(_1DC_Message);	
								
	}
}
