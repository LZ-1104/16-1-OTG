#include "stm32f10x.h"                  // Device header


void MySD_Init(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_SDIO,ENABLE);
    SDIO_InitTypeDef SDIO_InitStructure;

    SDIO_InitStructure.SDIO_ClockEdge = SDIO_ClockEdge_Rising;
    SDIO_InitStructure.SDIO_ClockBypass =SDIO_ClockBypass_Enable ;
    SDIO_InitStructure.SDIO_ClockPowerSave = SDIO_ClockPowerSave_Enable;
    SDIO_InitStructure.SDIO_ClockDiv = 2;//0x00--0xFF
    SDIO_InitStructure.SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Enable;
    SDIO_InitStructure.SDIO_BusWide =SDIO_BusWide_4b;

    SDIO_Init(&SDIO_InitStructure);

    SDIO_ClockCmd(ENABLE);

}

void MySD_Write(void)
{
    SDIO_CmdInitTypeDef SDIO_CmdInitStructure;
    SDIO_CmdStructInit(&SDIO_CmdInitStructure);
    SDIO_SendCommand(&SDIO_CmdInitStructure);

}

void MySD_Read(void)
{

}