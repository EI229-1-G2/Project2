/*
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    CDK66_Ref1.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MK66F18.h"
#include "fsl_gpio.h"
#include "fsl_uart.h"
#include "fsl_pit.h"
#include "fsl_rtc.h"
#include "fsl_i2c.h"
//#include "fsl_edma.h"

/* TODO: insert other include files here. */
#include "CDK66_OLED.h"
#include "CDK66_MT9V034.h"

/* TODO: insert other definitions and declarations here. */
#define USB_RING_BUFFER_SIZE	(128U)
#define cmdDataLenMax			(100U)

#define MEMSx_BASE ADC0
#define MEMSx_CHANNEL 19U
#define MEMSy_BASE ADC0
#define MEMSy_CHANNEL 3U
#define MEMSz_BASE ADC1
#define MEMSz_CHANNEL 19U
#define CCD_BASE ADC0
#define CCD_CHANNEL 14U

#define CCD_CLK_HIGH	GPIO_PinWrite(BOARD_INITPINS_CCDCLK_GPIO, BOARD_INITPINS_CCDCLK_PIN, 1)
#define CCD_CLK_LOW		GPIO_PinWrite(BOARD_INITPINS_CCDCLK_GPIO, BOARD_INITPINS_CCDCLK_PIN, 0)
#define CCD_SI_HIGH		GPIO_PinWrite(BOARD_INITPINS_CCDSI_GPIO, BOARD_INITPINS_CCDSI_PIN, 1)
#define CCD_SI_LOW		GPIO_PinWrite(BOARD_INITPINS_CCDSI_GPIO, BOARD_INITPINS_CCDSI_PIN, 0)

typedef struct _Board_Analog
 {
	 uint32_t	x;
	 uint32_t	y;
	 uint32_t	z;
	 uint32_t	CCD;
	 uint32_t	VSENSE;
	 uint32_t	IRV1;
	 uint32_t	IRV2;
	 uint32_t	TEMP;
	 uint32_t	HALL1;
	 uint32_t	HALL2;
 } Board_Analog_t;

static uint8_t DSPTable[] = { 0x40, 0x79, 0x24, 0x30, 0x19, 0x12, 0x02, 0x78, 0x00, 0x10, 0x08, 0x03, 0x27, 0x21, 0x06, 0x0E };
volatile static uint32_t appTick;		// tick at an interval of 5ms
volatile static uint32_t appTick2;		// tick at an interval of 5ms
volatile static uint32_t QESVar = 100;		// Variable controlled by QES
volatile static float IRread = 0;
volatile static float IR1 = 0;
volatile static float IR2 = 0;

// USB debug port: UART0
uint8_t UsbRingBuffer[USB_RING_BUFFER_SIZE];
uint16_t g_bufferWriteByteCnt;
volatile uint16_t txIndexUSB;	/* Index of the debug port data to send out. */
volatile uint16_t rxIndexUSB;	/* Index of the memory to save new arrived debug port data. */

uint16_t CCDData[128];	// Linear CCD raw data
uint16_t CCDDataToDraw[128], CCDDataToClear[128];	// Linear CCD data to OLED
uint8_t Image_Data[120][188];	// MT9V034 DMA data receiver ROW*COL: 120*188
uint8_t Image_Use[60][94];	// resized CAM data to fit OLED
uint8_t Pixle[60][94];		// 0/1 CAM data
uint16_t CAM_DMA_Counter=0;

uint8_t CmdDataLen;		// PC cmd data length counter
uint8_t CmdDataBuffer[cmdDataLenMax];	// buffer to hold cmd data from PC
bool CmdDataReady;		// new cmd data flag, 1: new cmd received
bool CamDataReady=0;	// MT9V034 new data frame ready in buffer
uint16_t DMALeftSize=0;	// test purpose
uint8_t CamLineCounter=0;

uint16_t MemsX, MemsY, MemsZ;
rtc_datetime_t appDateTime;

void DMA_CAM_Repeat(uint8_t LineNumber)
//void DMA_CAM_Repeat()
{
	/* Channel CH0 status */
	//uint32_t status;

	//status = EDMA_GetChannelStatusFlags(DMA_DMA_BASEADDR, DMA_CH0_DMA_CHANNEL);
	//EDMA_ClearChannelStatusFlags(DMA_DMA_BASEADDR, DMA_CH0_DMA_CHANNEL, status);

	//EDMA_ResetChannel(DMA_DMA_BASEADDR, DMA_CH0_DMA_CHANNEL);
	//EnableIRQ(DMA_DMA_CH_INT_DONE_0_IRQN);
	//EDMA_InstallTCD(DMA_DMA_BASEADDR, DMA_CH0_DMA_CHANNEL, (edma_tcd_t *)&DMA_CH0_TCD0_config);

	//DMA_DMA_BASEADDR->TCD[DMA_CH0_DMA_CHANNEL].SADDR         = (uint32_t) 0x400FF050U;
	//DMA_DMA_BASEADDR->TCD[DMA_CH0_DMA_CHANNEL].DADDR         = (uint32_t) &Image_Data[0];
	DMA_DMA_BASEADDR->TCD[DMA_CH0_DMA_CHANNEL].DADDR         = (uint32_t) &Image_Data[LineNumber][0];
	DMA_DMA_BASEADDR->TCD[DMA_CH0_DMA_CHANNEL].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(188);
    /* Clear DONE bit first, otherwise ESG cannot be set */
	//DMA_DMA_BASEADDR->TCD[DMA_CH0_DMA_CHANNEL].CSR           = 0;
	//DMA_DMA_BASEADDR->TCD[DMA_CH0_DMA_CHANNEL].CSR           = DMA_CSR_START(0U) | DMA_CSR_INTMAJOR(1U) | DMA_CSR_INTHALF(0U) | DMA_CSR_DREQ(1U) | DMA_CSR_ESG(0U) | DMA_CSR_MAJORELINK(0U) | DMA_CSR_MAJORLINKCH(0) | DMA_CSR_BWC(3U);
	DMA_DMA_BASEADDR->TCD[DMA_CH0_DMA_CHANNEL].BITER_ELINKNO = DMA_BITER_ELINKNO_BITER(188);

	//EDMA_EnableChannelRequest(DMA_DMA_BASEADDR, DMA_CH0_DMA_CHANNEL);
	DMA_DMA_BASEADDR->SERQ = DMA_SERQ_SERQ(DMA_CH0_DMA_CHANNEL);

}

/* DMA0_DMA16_IRQn interrupt handler */
void DMA_DMA_CH_INT_DONE_0_IRQHANDLER(void) {

  /* Channel CH0 status */
  uint32_t CH0_status;

  /* Reading all flags of status register */
  CH0_status = EDMA_GetChannelStatusFlags(DMA_DMA_BASEADDR, DMA_CH0_DMA_CHANNEL);
  /* Clearing all flags of the channel status register */
  EDMA_ClearChannelStatusFlags(DMA_DMA_BASEADDR, DMA_CH0_DMA_CHANNEL, CH0_status);

  /* Place your code here */
  //CamDataReady = 1;
  //DMA_CAM_Repeat();

  /*
  CAM_DMA_Counter++;
  if ((CAM_DMA_Counter%50)==0)
	  LED2_TOGGLE();
	*/

  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}

/* PORTB_IRQn interrupt handler */
void DCAM_IRQHANDLER(void) {
  /* Get pin flags */
  uint32_t pin_flags = GPIO_PortGetInterruptFlags(GPIOB);

  /* Place your interrupt code here */
  // HREF on PTB11
  if (pin_flags & BOARD_INITPINS_CAMHREF_GPIO_PIN_MASK)
  {
	  GPIO_PortClearInterruptFlags(GPIOB, BOARD_INITPINS_CAMHREF_GPIO_PIN_MASK);
	  // Start DMA transfer on new line
	  DMA_CAM_Repeat(CamLineCounter);
	  CamLineCounter++;
	  if (CamLineCounter >= 120)
	  {
		  CamLineCounter = 0;
	  }
	  return;
  }

  // VSYN on PTB16
  if (pin_flags & BOARD_INITPINS_CAMVSYN_GPIO_PIN_MASK)
  {
	  GPIO_PortClearInterruptFlags(GPIOB, BOARD_INITPINS_CAMVSYN_GPIO_PIN_MASK);

	  //CAM_DMA_Counter = EDMA_GetRemainingMajorLoopCount(DMA_DMA_BASEADDR, DMA_CH0_DMA_CHANNEL);
	  //DMA_CAM_Repeat();

	  //DMALeftSize = EDMA_GetRemainingMajorLoopCount(DMA_DMA_BASEADDR, DMA_CH0_DMA_CHANNEL);

	  CamDataReady = 1;
	  CamLineCounter = 0;
	  //DMA_CAM_Repeat();

	  CAM_DMA_Counter++;
	  if ((CAM_DMA_Counter%50)==0)
		  LED2_TOGGLE();

	  return;
  }

  // QESa on PTB20
  if (pin_flags & BOARD_INITPINS_QESa_GPIO_PIN_MASK)
  {
	  GPIO_PortClearInterruptFlags(GPIOB, BOARD_INITPINS_QESa_GPIO_PIN_MASK);
	  delay();
	  if (!QESA()) return;
	  if (!QESB() && QESVar<200) QESVar++;
	  else if (QESVar > 0) QESVar--;
	  return;
  }

  /* Clear pin flags */
  GPIO_PortClearInterruptFlags(GPIOB, pin_flags);

  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}

void CCD_delay()
{
	uint16_t delayCount;
	delayCount = 20;
	while(delayCount--) {}
}

/* Drive to and read from TSL1401CL */
void CollectCCD(void)
{
    uint8_t index=0;

    CCD_CLK_HIGH;
    CCD_delay();
    CCD_SI_LOW;
    CCD_delay();

    CCD_SI_HIGH;
    CCD_delay();
    CCD_CLK_LOW;
    CCD_delay();

    CCD_CLK_HIGH;
    CCD_delay();
    CCD_SI_LOW;
    CCD_delay();

    for(index=0; index<128; index++)
    {
        CCD_CLK_LOW;
        CCD_delay();

        ADC16_SetChannelConfig(ADC0_PERIPHERAL, ADC0_CH2_CONTROL_GROUP, &ADC0_channelsConfig[2]);
        while (0U == (kADC16_ChannelConversionDoneFlag &
                      ADC16_GetChannelStatusFlags(ADC0_PERIPHERAL, ADC0_CH2_CONTROL_GROUP))) {}
        CCDData[index] = ADC16_GetChannelConversionValue(ADC0_PERIPHERAL, ADC0_CH2_CONTROL_GROUP) & 0xFFF;

        CCD_CLK_HIGH;
        CCD_delay();
    }
}

// simply generate SI & CLK pulses to flush the previously integrated frame
// while integrating new frame to be read out
void LinearCameraFlush(void)
{
    uint8_t index=0;

    CCD_SI_HIGH;
    CCD_delay();
    CCD_CLK_HIGH;
    CCD_delay();
    CCD_SI_LOW;
    CCD_delay();
    CCD_CLK_LOW;

    for(index=0; index<128; index++)
    {
        CCD_CLK_HIGH;
        CCD_delay();
        CCD_CLK_LOW;
        CCD_delay();
    }
}

void LinearCameraOneShot(void)
{
    uint8_t index=0;

    // flush previously integrated frame before capturing new frame
    LinearCameraFlush();
    // wait for TSL1401 to integrate new frame, exposure time control by delay
    delay1ms(10);

    CCD_SI_HIGH;
    CCD_delay();
    CCD_CLK_HIGH;
    CCD_delay();
    CCD_SI_LOW;
    CCD_delay();
    CCD_CLK_LOW;

    // ADC mux to CCD channel
    ADC16_SetChannelMuxMode(ADC0, kADC16_ChannelMuxA);
    for(index=0; index<128; index++)
    {
    	// read AO value using ADC
        ADC16_SetChannelConfig(ADC0_PERIPHERAL, ADC0_CH2_CONTROL_GROUP, &ADC0_channelsConfig[2]);
        while (0U == (kADC16_ChannelConversionDoneFlag &
                      ADC16_GetChannelStatusFlags(ADC0_PERIPHERAL, ADC0_CH2_CONTROL_GROUP))) {}
        CCDData[index] = ADC16_GetChannelConversionValue(ADC0_PERIPHERAL, ADC0_CH2_CONTROL_GROUP) & 0xFFF;
        // clock pulse to read next pixel output
        CCD_CLK_HIGH;
        CCD_delay();
        CCD_CLK_LOW;
    }

    // 129th pulse to terminate output of 128th pixel
    CCD_CLK_HIGH;
    CCD_delay();
    CCD_CLK_LOW;
}

void Draw_LinearView(void)
{
    uint8_t index=0, tempData;
    uint8_t i=0, j=0, k=0;

    for(index=0; index<128; index++)
	{
    	i = 7 - (CCDData[index]>>6)/8;
    	j = 7- (CCDData[index]>>6)%8;
    	for (k=0; k<8; k++)
    	{
    		OLED_Set_Pos(index, k);
    		if (i==k)
    		{
    			tempData = (1U << j);
    		}
    		else
    		{
    			tempData = 0;
    		}
    		OLED_WrDat(tempData);
    	}
	}
}

/*	failed to use OLED_ClrPixel and OLED_PutPixel
 *  code for reference
	CCDDataToDraw[index] = 63 - (CCDData[index]>>6);
	if (CCDDataToDraw[index] < 0) CCDDataToDraw[index] = 0;
	else if (CCDDataToDraw[index] > 63) CCDDataToDraw[index] = 63;
	//OLED_ClrPixel(index, CCDDataToClear[index]);
	delay();
	delay();
	delay();
	OLED_PutPixel(index, CCDDataToDraw[index]);
	CCDDataToClear[index] = CCDDataToDraw[index];
*/

void PcCmdProcess(void) {
    int year;	/*!< Range from 1970 to 2099.*/
    int month;	/*!< Range from 1 to 12.*/
    int day;	/*!< Range from 1 to 31 (depending on month).*/
    int hour;	/*!< Range from 0 to 23.*/
    int minute;	/*!< Range from 0 to 59.*/
    int second;	/*!< Range from 0 to 59.*/
    rtc_datetime_t PCTIME;


	if (CmdDataLen == 27U)	// "#cmd01data20201014-190000\r\n"
	{
		printf("Command from PC received!\r\n");
		sscanf(CmdDataBuffer, "#cmd01data%4d%2d%2d-%2d%2d%2d",
				&year, &month, &day, &hour, &minute, &second);
		printf("RTC: %04hd-%02hd-%02hd %02hd:%02hd:%02hd\r\n",
				year, month, day, hour, minute, second);

	    /* Set a start date time and start RTC */
		PCTIME.year   = year;
		PCTIME.month  = month;
		PCTIME.day    = day;
		PCTIME.hour   = hour;
		PCTIME.minute = minute;
		PCTIME.second = second;
	    RTC_StopTimer(RTC);
	    RTC_SetDatetime(RTC, &PCTIME);
	    RTC_StartTimer(RTC);
	}
	else
	{
		printf("Invalid command!\r\n");
	}
}

/* UART0_RX_TX_IRQn interrupt handler */
void UART0_SERIAL_RX_TX_IRQHANDLER(void) {
	uint8_t data;
	uint32_t intStatus;

	/* Reading all interrupt flags of status registers */
	intStatus = UART_GetStatusFlags(UART0_PERIPHERAL);

	/* If new data arrived. */
	if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(UART0_PERIPHERAL))
	{
		data = UART_ReadByte(UART0_PERIPHERAL);
		//UART_WriteByte(UART0_PERIPHERAL, data);

		/* PC command data framing */
		/* RTC calibrate example: "#cmd01data20201014-190000\r\n" */
		if (data == '#')		// start of a data frame?
		{
			CmdDataLen = 0;		// reset cmd data counter
			CmdDataBuffer[0] = data;
		}
		else
		{
			CmdDataLen++;
			if (CmdDataLen < cmdDataLenMax)
			{
				CmdDataBuffer[CmdDataLen] = data;
				if (data == '\n')	// end of a data frame?
				{
					CmdDataReady = 1;	// a complete cmd data frame received
					CmdDataLen++;
					PcCmdProcess();		// cmd processing
				}
			}
			else	// no cmd data frame received while data buffer overflow
			{
				CmdDataLen = 0;
			}
		}

		/* If ring buffer is not full, add data to ring buffer. */
		if (((rxIndexUSB + 1) % USB_RING_BUFFER_SIZE) != txIndexUSB)
		{
			UsbRingBuffer[rxIndexUSB] = data;
			rxIndexUSB++;
			rxIndexUSB %= USB_RING_BUFFER_SIZE;
		}
	}

	/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
	 Store immediate overlapping exception return operation might vector to incorrect interrupt. */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
	__DSB();
#endif
}

void InitPITs() {
    pit_config_t pitConfig;

    PIT_GetDefaultConfig(&pitConfig);
    PIT_Init(PIT, &pitConfig);
    /* Set timer period for channel 0: 5000us = 5ms */
    PIT_SetTimerPeriod(PIT, kPIT_Chnl_0, USEC_TO_COUNT(5000U, APPTICK_PIT_SOURCE_CLOCK));
    /* Enable timer interrupts for channel 0 */
    PIT_EnableInterrupts(PIT, kPIT_Chnl_0, kPIT_TimerInterruptEnable);
    /* Enable at the NVIC */
    EnableIRQ(APPTICK_PIT_IRQn);
    /* Start channel 0 */
    PIT_StartTimer(PIT, kPIT_Chnl_0);
    appTick = 0;
    appTick2 = 0;
}

void ShowNumHEX(uint8_t num) {
	uint16_t l;
	l = DSPTable[num & 0x0F];

	// DSP: PD9~PD15
	GPIO_PortSet(BOARD_INITPINS_DSPa_GPIO, 0x7F << BOARD_INITPINS_DSPa_PIN);	// turn off all DSP segments
	GPIO_PortClear(BOARD_INITPINS_DSPa_GPIO, ((~l)&0x7F) << BOARD_INITPINS_DSPa_PIN);	// turn on the code segments
}

void ShowNumDEC(uint8_t num) {
	uint16_t l;
	l = DSPTable[num % 10];

	// DSP: PD9~PD15
	GPIO_PortSet(BOARD_INITPINS_DSPa_GPIO, 0x7F << BOARD_INITPINS_DSPa_PIN);	// turn off all DSP segments
	GPIO_PortClear(BOARD_INITPINS_DSPa_GPIO, ((~l)&0x7F) << BOARD_INITPINS_DSPa_PIN);	// turn on the code segments
}

void ShowNumOFF(void) {
	// DSP: PD9~PD15
	GPIO_PortSet(BOARD_INITPINS_DSPa_GPIO, 0x7F << BOARD_INITPINS_DSPa_PIN);	// turn off all DSP segments
}

// appTicks increasing at an interval of 5ms
void APPTICK_PIT_HANDLER(void)
{
    /* Clear interrupt flag.*/
    PIT_ClearStatusFlags(PIT, kPIT_Chnl_0, kPIT_TimerFlag);
    appTick++;
    appTick2++;

    /* Collect 128 points CCD data */
    //CollectCCD();

#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

void CDK66_Analog_Input(Board_Analog_t *AnalogInput) {
    adc16_channel_config_t adc16ChannelConfigStruct;
    adc16_channel_mux_mode_t ADCMUX0, ADCMUX1;

    //ADCMUX0 = ADC0_muxMode;		// kADC16_ChannelMuxB
    //ADCMUX1 = ADC1_muxMode;		// kADC16_ChannelMuxA
    ADCMUX0 = kADC16_ChannelMuxA;
    ADCMUX1 = kADC16_ChannelMuxA;
    ADC16_SetChannelMuxMode(ADC0, ADCMUX0);
    ADC16_SetChannelMuxMode(ADC1, ADCMUX1);

    // MEMSx: ADC0, se19
    ADC16_SetChannelConfig(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP, &ADC0_channelsConfig[0]);
    while (0U == (kADC16_ChannelConversionDoneFlag &
                  ADC16_GetChannelStatusFlags(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP))) {}
    AnalogInput->x = ADC16_GetChannelConversionValue(ADC0_PERIPHERAL, ADC0_CH0_CONTROL_GROUP) & 0xFFF;
    // MEMSy: ADC0, se3
    ADC16_SetChannelConfig(ADC0_PERIPHERAL, ADC0_CH1_CONTROL_GROUP, &ADC0_channelsConfig[1]);
    while (0U == (kADC16_ChannelConversionDoneFlag &
                  ADC16_GetChannelStatusFlags(ADC0_PERIPHERAL, ADC0_CH1_CONTROL_GROUP))) {}
    AnalogInput->y = ADC16_GetChannelConversionValue(ADC0_PERIPHERAL, ADC0_CH1_CONTROL_GROUP) & 0xFFF;
    // MEMSz: ADC1, se19
    ADC16_SetChannelConfig(ADC1_PERIPHERAL, ADC1_CH0_CONTROL_GROUP, &ADC1_channelsConfig[0]);
    while (0U == (kADC16_ChannelConversionDoneFlag &
                  ADC16_GetChannelStatusFlags(ADC1_PERIPHERAL, ADC1_CH0_CONTROL_GROUP))) {}
    AnalogInput->z = ADC16_GetChannelConversionValue(ADC1_PERIPHERAL, ADC1_CH0_CONTROL_GROUP) & 0xFFF;

    // LinearCCD TSL1401: ADC0, se14
    ADC16_SetChannelConfig(ADC0_PERIPHERAL, ADC0_CH2_CONTROL_GROUP, &ADC0_channelsConfig[2]);
    while (0U == (kADC16_ChannelConversionDoneFlag &
                  ADC16_GetChannelStatusFlags(ADC0_PERIPHERAL, ADC0_CH2_CONTROL_GROUP))) {}
    AnalogInput->CCD = ADC16_GetChannelConversionValue(ADC0_PERIPHERAL, ADC0_CH2_CONTROL_GROUP) & 0xFFF;

    // IRV1: ADC1, se18
    ADC16_SetChannelConfig(ADC1_PERIPHERAL, ADC1_CH1_CONTROL_GROUP, &ADC1_channelsConfig[1]);
    while (0U == (kADC16_ChannelConversionDoneFlag &
                  ADC16_GetChannelStatusFlags(ADC1_PERIPHERAL, ADC1_CH1_CONTROL_GROUP))) {}
    AnalogInput->IRV1 = ADC16_GetChannelConversionValue(ADC1_PERIPHERAL, ADC1_CH1_CONTROL_GROUP) & 0xFFF;

    // IRV2: ADC0, se23
    ADC16_SetChannelConfig(ADC0_PERIPHERAL, ADC0_CH3_CONTROL_GROUP, &ADC0_channelsConfig[3]);
    while (0U == (kADC16_ChannelConversionDoneFlag &
                  ADC16_GetChannelStatusFlags(ADC0_PERIPHERAL, ADC0_CH3_CONTROL_GROUP))) {}
    AnalogInput->IRV2 = ADC16_GetChannelConversionValue(ADC0_PERIPHERAL, ADC0_CH3_CONTROL_GROUP) & 0xFFF;

    // MCU on-chip temperature: ADC1, se26
    ADC16_SetChannelConfig(ADC1_PERIPHERAL, ADC1_CH2_CONTROL_GROUP, &ADC1_channelsConfig[2]);
    while (0U == (kADC16_ChannelConversionDoneFlag &
                  ADC16_GetChannelStatusFlags(ADC1_PERIPHERAL, ADC1_CH2_CONTROL_GROUP))) {}
    AnalogInput->TEMP = ADC16_GetChannelConversionValue(ADC1_PERIPHERAL, ADC1_CH2_CONTROL_GROUP) & 0xFFF;

    // HALL1: ADC1, se23
    ADC16_SetChannelConfig(ADC1_PERIPHERAL, ADC1_CH3_CONTROL_GROUP, &ADC1_channelsConfig[3]);
    while (0U == (kADC16_ChannelConversionDoneFlag &
                  ADC16_GetChannelStatusFlags(ADC1_PERIPHERAL, ADC1_CH3_CONTROL_GROUP))) {}
    AnalogInput->HALL1 = ADC16_GetChannelConversionValue(ADC1_PERIPHERAL, ADC1_CH3_CONTROL_GROUP) & 0xFFF;

    // HALL2: ADC0, se17
    ADC16_SetChannelConfig(ADC0_PERIPHERAL, ADC0_CH5_CONTROL_GROUP, &ADC0_channelsConfig[5]);
    while (0U == (kADC16_ChannelConversionDoneFlag &
                  ADC16_GetChannelStatusFlags(ADC0_PERIPHERAL, ADC0_CH5_CONTROL_GROUP))) {}
    AnalogInput->HALL2 = ADC16_GetChannelConversionValue(ADC0_PERIPHERAL, ADC0_CH5_CONTROL_GROUP) & 0xFFF;


    ADCMUX0 = kADC16_ChannelMuxB;
    ADC16_SetChannelMuxMode(ADC0, ADCMUX0);

    // VSENSE: ADC0, se7b
    ADC16_SetChannelConfig(ADC0_PERIPHERAL, ADC0_CH4_CONTROL_GROUP, &ADC0_channelsConfig[4]);
    while (0U == (kADC16_ChannelConversionDoneFlag &
                  ADC16_GetChannelStatusFlags(ADC0_PERIPHERAL, ADC0_CH4_CONTROL_GROUP))) {}
    AnalogInput->VSENSE = ADC16_GetChannelConversionValue(ADC0_PERIPHERAL, ADC0_CH4_CONTROL_GROUP) & 0xFFF;

    return;
}

void static inline Update_ServoUS(uint8_t ChanNo, uint32_t DutyCycleUS) {
	uint32_t cv, mod;

	if ((ChanNo<2) && (DutyCycleUS<2500) && (DutyCycleUS>500))
	{
		// mod value mapped to 10ms|10000us period (100Hz)
		mod = FTM3_PERIPHERAL->MOD;
		cv = (mod * DutyCycleUS) / 10000U;
		if (cv >= mod)
		{
			cv = mod + 1U;
		}
		FTM3_PERIPHERAL->CONTROLS[ChanNo].CnV = cv;
	    FTM_SetSoftwareTrigger(FTM3_PERIPHERAL, true);
	}
	return;
}

void Get_Use_Image()
{
	uint16_t i=0, j=0, row=0, col=0;

	for(i=0; i<120; i+=2)		// ROW
	{
		for(j=0; j<188; j+=2)	// COL
		{
			Image_Use[row][col] = Image_Data[i][j];
			col++;
		}
		col = 0;
		row++;
	}
}

uint8_t GetOTSU(uint8_t image[60][94])
{
	uint16_t i, j;
	uint32_t Amount = 0;
	uint32_t PixelBack = 0;
	uint32_t PixelIntegralBack = 0;
	uint32_t PixelIntegral = 0;
	uint32_t PixelIntegralFore = 0;
	uint32_t PixelFore = 0;
	float OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // 类间方差;
	uint16_t MinValue, MaxValue;
	uint8_t Threshold = 0;
	uint8_t HistoGram[256];

	for (j=0; j<256; j++)  HistoGram[j] = 0; //初始化灰度直方图

	for (j=0; j<60; j++)
	{
		for (i = 0; i < 94; i++)
		{
			HistoGram[image[j][i]]++; //统计灰度级中每个像素在整幅图像中的个数
		}
	}

	for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++) ;        //获取最小灰度的值
	for (MaxValue = 255; MaxValue > MinValue && HistoGram[MinValue] == 0; MaxValue--) ; //获取最大灰度的值

	if (MaxValue == MinValue)     return MaxValue;         // 图像中只有一个颜色
	if (MinValue + 1 == MaxValue)  return MinValue;        // 图像中只有二个颜色

	for (j = MinValue; j <= MaxValue; j++)    Amount += HistoGram[j];        //  像素总数

	PixelIntegral = 0;
	for (j = MinValue; j <= MaxValue; j++)
	{
		PixelIntegral += HistoGram[j] * j;//灰度值总数
	}
	SigmaB = -1;
	for (j = MinValue; j < MaxValue; j++)
	{
		PixelBack = PixelBack + HistoGram[j];   //前景像素点数
		PixelFore = Amount - PixelBack;         //背景像素点数
		OmegaBack = (float)PixelBack / Amount;//前景像素百分比
		OmegaFore = (float)PixelFore / Amount;//背景像素百分比
		PixelIntegralBack += HistoGram[j] * j;  //前景灰度值
		PixelIntegralFore = PixelIntegral - PixelIntegralBack;//背景灰度值
		MicroBack = (float)PixelIntegralBack / PixelBack;   //前景灰度百分比
		MicroFore = (float)PixelIntegralFore / PixelFore;   //背景灰度百分比
		Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//计算类间方差
		if (Sigma > SigmaB)                    //遍历最大的类间方差g //找出最大类间方差以及对应的阈值
		{
			SigmaB = Sigma;
			Threshold = j;
		}
	}
	return Threshold;                        //返回最佳阈值;

}

// threshold mode: 0 - OTSU; 1 - average
void Get_01_Value(uint8_t mode)
{
	uint16_t i=0, j=0;
	uint8_t Threshold;
	uint32_t tv=0;
	char txt[16];

	if (mode)
	{
		for (i=0; i<60; i++)
		{
			for (j=0; j<94; j++)
				{
					tv += Image_Use[i][j];   //累加
				}
		}
		Threshold = tv/60/94;        	//求平均值,光线越暗越小，全黑约35，对着屏幕约160，一般情况下大约100
		Threshold = Threshold*8/10+10;	//此处阈值设置，根据环境的光线来设定
	}
	else
	{
		Threshold = GetOTSU(Image_Use);	//大津法阈值
		Threshold = (uint8_t)(Threshold*0.5)+70;
	}
	sprintf(txt,"%03d", Threshold);
	OLED_P6x8Str(80, 1, (uint8_t *)txt);

	for (i=0; i<60; i++)
	{
		for(j=0; j<94; j++)
		{
			if(Image_Use[i][j] > Threshold) //数值越大，显示的内容越多，较浅的图像也能显示出来
				Pixle[i][j] = 1;
			else
				Pixle[i][j] = 0;
		}
	}
}

void Draw_CameraView()
{
	uint8_t i=0, j=0, temp=0;

	for(i=8;i<56;i+=8)//6*8=48行
	{
		OLED_Set_Pos(18, i/8+1);	//起始位置
		for(j=0; j<94; j++)  	//列数
		{
			temp = 0;
			if(Pixle[0+i][j]) temp |= 1;
			if(Pixle[1+i][j]) temp |= 2;
			if(Pixle[2+i][j]) temp |= 4;
			if(Pixle[3+i][j]) temp |= 8;
			if(Pixle[4+i][j]) temp |= 0x10;
			if(Pixle[5+i][j]) temp |= 0x20;
			if(Pixle[6+i][j]) temp |= 0x40;
			if(Pixle[7+i][j]) temp |= 0x80;
			OLED_WrDat(temp);
		}
	}
}

void SendImage(void)
{
	uint8_t PcImageHeader[4];

	PcImageHeader[0] = 0x00;
	PcImageHeader[1] = 0xFF;
	PcImageHeader[2] = 0x01;
	PcImageHeader[3] = 0x01;

	//发送帧头标志
	UART_WriteBlocking(UART0_PERIPHERAL, (uint8_t *)PcImageHeader, 4U);
	//发送灰度图像
	UART_WriteBlocking(UART0_PERIPHERAL, (uint8_t *)Image_Data, 22560U);
}

/*
 * @brief   Application entry point.
 */

int main(void) {
	char OLEDLine1[] = "Vbat:0.00V";
	char OLEDLine2[] = "x:1111  IR1:4444";
	char OLEDLine3[] = "y:2222  IR2:5555";
	char OLEDLine4[] = "z:3333  ct:33.44";

	uint32_t tempInt = 0;
	uint16_t testCode = 0;
	uint8_t index = 0;
	uint8_t CCD2PC[260];	// data to be sent to PC
	Board_Analog_t AnalogIn;

	uint8_t TaskID = 0;		// loop switch
	bool CountDir=0;		// 0: count up; 1: count down

	float VBat, ICTemp;

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

    BOARD_InitI2C();
    BOARD_InitRTC();
    delay();

    OLED_Init();
    InitPITs();
	EnableIRQ(BOARD_RESa_IRQ);
	delay();

    //printf("Hello World\n");
    //OLED_P8x16Str(0,0,"## Cyber-K66 ###");
    delay();

	BEEP_ON();
	delay_short();
	BEEP_OFF();
    ShowNumOFF();

    // Header of CCD data frame, seekfree protocol
    CCD2PC[0] = 0x00;
	CCD2PC[1] = 0xFF;
	CCD2PC[2] = 0x01;
	CCD2PC[3] = 0x00;

	TaskID = (~((SW1()<<3) + (SW2()<<2) + (SW3()<<1) + SW4())) & 0x0F;
    //printf("Task ID: %d\r\n", TaskID);

	if (TaskID == 2U)		// using MT9V034?
		MT9V034_Init(50);
	if (TaskID == 3U)		// PWM control
	{
		tempInt = 1500;		// R/C servo central position
		CountDir = 0;
	}
	if (TaskID == 4U)		// Rotary encoder switch
	{
	    OLED_P8x16Str(0,0,"## Cyber-K66 ###");
	    OLED_P8x16Str(0,3,"Operate QES1....");
	}

	/* Enter an infinite loop, just incrementing a counter. */
	CamDataReady = 0;
    while(1) {

    	switch(TaskID)
    	{
    	case 1U:		// using TSL1401
        	//delay1ms(20);
        	//CollectCCD();
        	LinearCameraOneShot();
        	// draw linear CCD 128 pixel on OLED
    		Draw_LinearView();
    		if (!KEY1())		// Key S1 pressed down?
    		{
    	    	//send 128 points CCD data (128*2byte) to UART0, using seekfree protocol
    			for(tempInt=0; tempInt<128; tempInt++)
    			{
    				CCD2PC[tempInt*2+4] = CCDData[tempInt] >> 8;	// Upper byte
    				CCD2PC[tempInt*2+5] = CCDData[tempInt] & 0XFF;	// Lower byte
    			}
    			UART_WriteBlocking(UART0_PERIPHERAL, CCD2PC, 260U);
    		}
    		LED1_TOGGLE();
    		break;
    	case 2U:		// using MT9V034
        	if (CamDataReady)
        	{
        		Get_Use_Image();	// resize data to fit OLED
        		if (KEY1())			// key S1 pressed down? If yes then use OTSU
        			Get_01_Value(0);	// 0 - OTSU; 1 - average
        		else
        			Get_01_Value(1);	// 0 - OTSU; 1 - average
        		Draw_CameraView();		// display on OLED

        		if (!KEY2()) SendImage();	// key S2 pressed down? If yes send ImageData to PC
        		//printf("DMA major counter remained: %d\r\n", DMALeftSize);
        		CamDataReady = 0;
        	}
    		break;
    	case 3U:
            /* Update PWM duty cycle */
        	/* Update to motor control channels */
        	delay1ms(10);	// delay 20ms
        	//if (tempInt<100) tempInt++;
        	//else tempInt = 0;
        	// Add motor drivers enable action here
        	//FTM_UpdatePwmDutycycle(FTM0_PERIPHERAL, kFTM_Chnl_2, kFTM_EdgeAlignedPwm, tempInt);
        	//FTM_UpdatePwmDutycycle(FTM0_PERIPHERAL, kFTM_Chnl_3, kFTM_EdgeAlignedPwm, tempInt);
        	//FTM_UpdatePwmDutycycle(FTM0_PERIPHERAL, kFTM_Chnl_4, kFTM_EdgeAlignedPwm, tempInt);
        	//FTM_UpdatePwmDutycycle(FTM0_PERIPHERAL, kFTM_Chnl_7, kFTM_EdgeAlignedPwm, tempInt);
        	//FTM_SetSoftwareTrigger(FTM0_PERIPHERAL, true);

            /* Update PWM duty cycle */
//        	if (CountDir)	// count up
//        	{
//            	if (tempInt<2300) tempInt++;
//            	//else tempInt = 700;
//            	else CountDir = 0;	// toggle CountDir on upper limit
//        	}
//        	else			// count down
//        	{
//            	if (tempInt>700) tempInt--;
//            	//else tempInt = 2300;
//            	else CountDir = 1;	// toggle CountDir on lower limit
//        	}

        	if (!QESP()) QESVar = 100;
        	tempInt = QESVar*8+550;
        	Update_ServoUS(kFTM_Chnl_0, tempInt);
        	Update_ServoUS(kFTM_Chnl_1, 3000-tempInt);
        	//FTM_SetSoftwareTrigger(FTM3_PERIPHERAL, true);
    		break;
    	case 4U:
            if (KEY1()) BEEP_OFF();
            else BEEP_ON();
            if (KEY2()) DSPdp_OFF();
            else DSPdp_ON();
            ShowNumDEC(QESVar);
    	    OLED_P8x16Str(0,3,"Operate QES1....");
            break;
    	case 5U:
    		CDK66_Analog_Input(&AnalogIn);
    		//if (AnalogIn.IRV1>10&&AnalogIn.IRV2>10){
    			IR1 = 0.998*IR1+0.002*AnalogIn.IRV1;
    			IR2 = 0.998*IR2+0.002*AnalogIn.IRV2;
    		//}

    		IRread = (IR1-IR2)/(IR1+IR2);
    		break;
    	case 8U:
            RTC_GetDatetime(RTC, &appDateTime);
            sprintf (OLEDLine1, "Date: %04hd-%02hd-%02hd", appDateTime.year, appDateTime.month, appDateTime.day);
            sprintf (OLEDLine3, "Time: %02hd:%02hd:%02hd", appDateTime.hour, appDateTime.minute, appDateTime.second);
            OLED_P8x16Str(0,0,(uint8_t *)OLEDLine1);
    	    OLED_P8x16Str(0,4,(uint8_t *)OLEDLine3);
    		break;
    	default:
        	if((appTick2%20) == 0)
            {
            	tempInt++;
                //ShowNumDEC(tempInt);
                testCode = tempInt;
            	BOARD_I2C_GPIO(testCode);
                while ((appTick2%200) == 0) {}

                CDK66_Analog_Input(&AnalogIn);		// Update all analog inputs
                MemsX = AnalogIn.x;
                MemsY = AnalogIn.y;
                MemsZ = AnalogIn.z;
                int absX = MemsX-2020,absY = MemsY-1980,absZ = MemsZ-2000;
                if (sqrt(absX*absX+absY*absY+absZ*absZ)<500){
                	BEEP_ON();
                	ShowNumDEC(1);
                }
                else if (sqrt(absX*absX+absY*absY+absZ*absZ)>1800){
                	BEEP_ON();
                	ShowNumDEC(2);
                }
                else {
                	BEEP_OFF();
                	//ShowNumDEC(0);
                }

            	VBat = ((float)AnalogIn.VSENSE)/4096.0*3.3*5;
                printf("X: %d\t\tY: %d\t\tZ: %d\t\tVBat: %4.2fV\r\n", AnalogIn.x, AnalogIn.y, AnalogIn.z, VBat);

            	ICTemp = (float)AnalogIn.TEMP*3.3/4096.0;
    			ICTemp = 25 - ((ICTemp - .716)/1.62);

                sprintf (OLEDLine1, "Vbat:%4.2fV", VBat);
                if (KEY1())
                {
                    sprintf (OLEDLine2, "x:%04hd  IR1:%04hd", AnalogIn.x, AnalogIn.IRV1);
                    sprintf (OLEDLine3, "y:%04hd  IR2:%04hd", AnalogIn.y, AnalogIn.IRV2);
                }
                else
                {
                    sprintf (OLEDLine2, "x:%04hd  Mg1:%04hd", AnalogIn.x, AnalogIn.HALL1);
                    sprintf (OLEDLine3, "y:%04hd  Mg2:%04hd", AnalogIn.y, AnalogIn.HALL2);
                }
                //sprintf (OLEDLine4, "z:%04hd  ct:%04hd", AnalogIn.z, AnalogIn.TEMP);
                sprintf (OLEDLine4, "z:%04hd  ct:%4.1fC", AnalogIn.z, ICTemp);

                //OLED_P8x16Str(0,0,(uint8_t *)"## Cyber-K66 ###");
                OLED_P8x16Str(0,0,(uint8_t *)OLEDLine1);
        	    OLED_P8x16Str(0,2,(uint8_t *)OLEDLine2);
        	    OLED_P8x16Str(0,4,(uint8_t *)OLEDLine3);
        	    OLED_P8x16Str(0,6,(uint8_t *)OLEDLine4);

        	    while ((appTick2%20) == 0) {}
            }

    		break;
    	}		// end of switch-case
    }			// end of main while loop
    return 0 ;
}
