#include "stm32f4xx.h"

volatile uint32_t ticks;

void TIM2_IRQhandler(void)   /* should be TIM2_IRQHandler: compiles, links, never called */
{
    TIM2->SR = ~TIM_SR_UIF;
    ++ticks;
}

int main(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->PSC = 16000 - 1;   /* HSI 16 MHz -> 1 kHz */
    TIM2->ARR = 1000 - 1;    /* update event every second */
    TIM2->DIER = TIM_DIER_UIE;
    TIM2->CR1 = TIM_CR1_CEN;
    NVIC_EnableIRQ(TIM2_IRQn);

    while (1) {}
}
