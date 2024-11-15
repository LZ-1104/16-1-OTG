#include "stm32f10x.h"                  // Device header
#include "HCSR04.h"
#include "Delay.h"
#include "OLED.h"
uint16_t Time,count;
extern uint16_t Distance;

void Timer3_Init(void)
{
	Time = 0;
	count=0;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ѡ��APB1�����µĶ�ʱ��Timer2
	
	TIM_InternalClockConfig(TIM3);		//TIM2ʹ���ڲ�ʱ��
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;		//����ģʽ���˴�Ϊ���ϼ���
	TIM_TimeBaseInitStructure.TIM_Period = 7199;		//ARR 1 = 0.0001S
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;		//�߼���ʱ�����У��ظ�����
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);		//ʹ���ж�
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;		//�ж�ͨ��ѡ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//���ȼ���ͬ��
	
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM3, ENABLE);		//�򿪶�ʱ��
}

void TIM3_IRQHandler(void)		//��ʱ��2���жϺ���������ֱ������
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{

		if (GPIO_ReadInputDataBit(Echo_Port, Echo_Pin) == 1)
		{
			Time ++;
		}
		count++;
		if(count==3000)
		{
			count=0;
			OLED_ShowNum(3,1,Distance,3);
		}
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);		//��ձ�־λ
	}
}

void HCSR04_Init(void)
{
	RCC_APB2PeriphClockCmd(Trig_RCC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = Trig_Pin;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Trig_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStruct.GPIO_Pin = Echo_Pin;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Echo_Port, &GPIO_InitStruct);
	
	GPIO_ResetBits(Trig_Port, Trig_Pin);
	
}

void HCSR04_Start()
{
	GPIO_SetBits(Trig_Port, Trig_Pin);
	Delay_us(35);
	GPIO_ResetBits(Trig_Port, Trig_Pin);
	Timer3_Init();
}

uint16_t HCSR04_GetValue()
{
	HCSR04_Start();
	Delay_ms(100);
	return ((Time * 0.0001) * 34000) / 2;
//	return Time;
}
