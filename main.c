#include "stm32f10x.h"
#include "core_cm3.h"

volatile uint32_t state = 0x00000000;

int main(void)
{



RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; //enable GPIOC clock
RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; //enable timer TIM2 clock

TIM2->PSC = 24000 - 1; //set prescaler for TIM2 as 24000
TIM2->ARR = 1000 - 1; //set auto-reload value for TIM2 as 1000
TIM2->CR1 |= TIM_CR1_CEN; //enable timer TIM2
SCB->AIRCR |= SCB_AIRCR_PRIGROUP0; //set Interrupt priority group 0
NVIC->ISER[0] |= NVIC_ISER_SETENA_28; //enable nested interrupt for TIM2
NVIC->IP[7] |= 0x10; //set priority for TIM2
TIM2->DIER |= TIM_DIER_UIE; //unable update interrupt for TIM2


GPIOC->CRH &= ~GPIO_CRH_CNF8_0; //disable GPIOC8 input mode
GPIOC->CRH |= GPIO_CRH_MODE8_1; //set GPIOC8 to output mode with max speed 2 MHz
GPIOC->CRH &= ~GPIO_CRH_CNF9_0; //disable GPIOC9 input mode
GPIOC->CRH |= GPIO_CRH_MODE9_1; //set GPIOC9 to output mode with max speed 2 MHz

while(1){}	

}

void TIM2_IRQHandler(void){
TIM2->SR ^= TIM_SR_UIF;
	GPIOC->ODR = state;
	GPIOC->BSRR |= GPIO_BSRR_BS8;
	state = 0xFFFFFFFF-state;
}

void _exit(int i)
{
    while (1);
}
