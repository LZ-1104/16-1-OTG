#include "stm32f10x.h"

uint8_t Addr1[]={'1','2','3','4'};
uint8_t Addr2[]={'a','b','c','d'};

void MyDMA_Init(uint16_t Size,uint32_t AddrP,uint32_t AddrM)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

    DMA_InitTypeDef DMA_InitStruct;
    DMA_InitStruct.DMA_BufferSize = Size;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStruct.DMA_M2M = DMA_M2M_Enable;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;

    DMA_InitStruct.DMA_PeripheralBaseAddr = AddrP;
    DMA_InitStruct.DMA_PeripheralDataSize =DMA_PeripheralDataSize_Byte ;
    DMA_InitStruct.DMA_PeripheralInc =DMA_PeripheralInc_Enable ;
   
    DMA_InitStruct.DMA_MemoryBaseAddr = AddrM;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStruct.DMA_MemoryInc =DMA_MemoryInc_Enable ;

    DMA_InitStruct.DMA_Priority = DMA_Priority_High;
    DMA_Init(DMA1_Channel1,&DMA_InitStruct);

    DMA_Cmd(DMA1_Channel1,ENABLE);

    DMA_ClearFlag(DMA1_FLAG_TC1);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE);
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_InitStruct);
}


