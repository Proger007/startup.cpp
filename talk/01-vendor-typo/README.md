# 01 · the handler that never runs (§2)

Slide 6 (S-18, limitation 1: weak symbols).

Vendor baseline for STM32F411 with TIM2 firing once a second, and the handler spelled `TIM2_IRQhandler`.
The build is clean (A-05). On the Nucleo, the first update event lands in `Default_Handler`'s loop (A-06: debugger halted there, IPSR = 44 = TIM2 IRQ 28 + 16).
