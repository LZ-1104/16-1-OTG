#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MyDMA.h"
#include "HCSR04.h"

uint16_t Count=0;
uint16_t Distance=0;
int main(void)
{
	/*模块初始化*/
	OLED_Init();				//OLED初始化
	HCSR04_Init();
	
	
	/*显示静态字符串*/
	OLED_ShowString(1, 1, "A");
	OLED_ShowString(2, 1, "B");
	MyDMA_Init(4,(uint32_t)&Addr1,(uint32_t)&Addr2);
	while (1)
	{
		Distance=HCSR04_GetValue();
		Delay_ms(200);
		DMA_SetCurrDataCounter(DMA1_Channel1,4);
		DMA_Cmd(DMA1_Channel1,ENABLE);

	}
}

void DMA1_Channel1_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_FLAG_TC1)==SET)
	{
		Addr1[0]++;
		Count++;
		DMA_Cmd(DMA1_Channel1,DISABLE);
		//DMA_SetCurrDataCounter(DMA1_Channel1,4);
		OLED_ShowNum(4,1,Count,5);
		DMA_ClearFlag(DMA1_FLAG_TC1);
		DMA_ClearITPendingBit(DMA1_IT_TC1);
	}
}
