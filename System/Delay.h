#ifndef __DELAY_H
#define __DELAY_H

extern volatile uint32_t g_tick;
void Delay_Init(void);
void Delay_Tick(void);
uint32_t Delay_GetTick(void);
void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);
void Delay_s(uint32_t s);

#endif
