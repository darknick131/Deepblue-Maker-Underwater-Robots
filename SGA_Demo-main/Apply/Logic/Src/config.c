#include "config.h"


/* GPIO configuration example */
tagGPIO_T demoGPIO[] =
{
	
	[0]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_13,				/* GPIO pin */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIO mode */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO pull-up/pull-down settings, whether external pull-up/pull-down hardware is needed */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO speed */	
		.tGPIOPort 			= GPIOD,					/* GPIO port */
	},
    [1]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_14,				/* GPIO pin */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIO mode */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO pull-up/pull-down settings, whether external pull-up/pull-down hardware is needed */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO speed */	
		.tGPIOPort 			= GPIOD,					/* GPIO port */
	},
    [2]=
	{ 
		.tGPIOInit.Pin 		= GPIO_PIN_15,				/* GPIO pin */
		.tGPIOInit.Mode 	= GPIO_MODE_OUTPUT_PP,		/* GPIO mode */
		.tGPIOInit.Pull 	= GPIO_NOPULL,				/* GPIO pull-up/pull-down settings, whether external pull-up/pull-down hardware is needed */
		.tGPIOInit.Speed 	= GPIO_SPEED_FREQ_HIGH,		/* GPIO speed */	
		.tGPIOPort 			= GPIOD,					/* GPIO port */
	},
    
};

/* UART configuration example */
tagUART_T demoUart = 
{
	// UART working mode configuration
	.tUARTHandle.Instance 						= USART1,					/* STM32 serial device */
	.tUARTHandle.Init.BaudRate   				= 115200,					/* UART baud rate */
	.tUARTHandle.Init.WordLength 				= UART_WORDLENGTH_8B,		/* Data bit length */
	.tUARTHandle.Init.StopBits   				= UART_STOPBITS_1,			/* Stop bit length */
	.tUARTHandle.Init.Parity     				= UART_PARITY_NONE,			/* Parity bit length */
	.tUARTHandle.Init.HwFlowCtl  				= UART_HWCONTROL_NONE,		/* Hardware flow control */
	.tUARTHandle.Init.Mode       				= UART_MODE_TX_RX,			/* UART mode */
	.tUARTHandle.Init.OverSampling 				= UART_OVERSAMPLING_16,		/* Oversampling */

#if defined (STM32L4_SGA_ENABLE)
	.tUARTHandle.Init.OneBitSampling 			= UART_ONE_BIT_SAMPLE_DISABLE,
	.tUARTHandle.AdvancedInit.AdvFeatureInit 	= UART_ADVFEATURE_NO_INIT,
#endif
	
	.ucPriority									= 1,						/* Interrupt priority */
	.ucSubPriority								= 3,						/* Interrupt sub-priority */
	
	// UART DMA receive configuration parameters
	.tUartDMA.bRxEnable							= true,						/* DMA receive enable */
#ifdef STM32F1_SGA_ENABLE
	.tUartDMA.tDMARx.Instance					= DMA1_Channel5,
#endif
#ifdef STM32F4_SGA_ENABLE
	.tUartDMA.tDMARx.Instance					= DMA2_Stream2,
	.tUartDMA.tDMARx.Init.Channel				= DMA_CHANNEL_4,
#endif
	.tUartDMA.tDMARx.Init.Direction				= DMA_PERIPH_TO_MEMORY,
	.tUartDMA.tDMARx.Init.PeriphInc				= DMA_PINC_DISABLE,
	.tUartDMA.tDMARx.Init.MemInc				= DMA_MINC_ENABLE,
	.tUartDMA.tDMARx.Init.PeriphDataAlignment 	= DMA_PDATAALIGN_BYTE,
	.tUartDMA.tDMARx.Init.MemDataAlignment	  	= DMA_MDATAALIGN_BYTE,
	.tUartDMA.tDMARx.Init.Mode					= DMA_NORMAL,
	.tUartDMA.tDMARx.Init.Priority				= DMA_PRIORITY_LOW,

	.tRxInfo.usDMARxMAXSize             		= 100,              		/* DMA receive buffer size - minimum serial protocol maximum byte * 2 or more, ensure receiving one can stably receive one complete data frame */

	.tUartDMA.ucDMARxPriority					= 1,						/* DMA receive interrupt priority */
	.tUartDMA.ucDMARxSubPriority				= 1,						/* DMA receive interrupt sub-priority */
	
	// UART DMA transmit configuration parameters
	.tUartDMA.bTxEnable							= true,						/* DMA transmit enable */
#ifdef STM32F1_SGA_ENABLE
	.tUartDMA.tDMATx.Instance					= DMA1_Channel4,
#endif
#ifdef STM32F4_SGA_ENABLE
	.tUartDMA.tDMATx.Instance					= DMA2_Stream7,
	.tUartDMA.tDMATx.Init.Channel				= DMA_CHANNEL_4,
	.tUartDMA.tDMATx.Init.FIFOMode				= DMA_FIFOMODE_ENABLE,
#endif
	.tUartDMA.tDMATx.Init.Direction				= DMA_MEMORY_TO_PERIPH,
	.tUartDMA.tDMATx.Init.PeriphInc				= DMA_PINC_DISABLE,
	.tUartDMA.tDMATx.Init.MemInc				= DMA_MINC_ENABLE,
	.tUartDMA.tDMATx.Init.PeriphDataAlignment	= DMA_PDATAALIGN_BYTE,
	.tUartDMA.tDMATx.Init.MemDataAlignment		= DMA_MDATAALIGN_BYTE,
	.tUartDMA.tDMATx.Init.Mode					= DMA_NORMAL,
	.tUartDMA.tDMATx.Init.Priority				= DMA_PRIORITY_HIGH,

	.tTxInfo.usDMATxMAXSize						= 50,						/* DMA transmit buffer size */
	
	.tUartDMA.ucDMATxPriority					= 1,						/* DMA transmit interrupt priority */
	.tUartDMA.ucDMATxSubPriority				= 1,						/* DMA transmit interrupt sub-priority */

	// UART GPIO configuration
	.tGPIO[0].tGPIOInit.Pin 					= GPIO_PIN_9,				/* GPIO pin */
	.tGPIO[0].tGPIOInit.Mode 					= GPIO_MODE_AF_PP,			/* GPIO mode */
	.tGPIO[0].tGPIOInit.Pull 					= GPIO_NOPULL,				/* GPIO pull-up/pull-down settings, whether external pull-up/pull-down hardware is needed */
	.tGPIO[0].tGPIOInit.Speed 					= GPIO_SPEED_FREQ_HIGH,		/* GPIO speed */	
	.tGPIO[0].tGPIOPort 						= GPIOA,					/* GPIO port */
#ifdef STM32F1_SGA_ENABLE
	.tGPIO[0].ucAFMode							= NO_REMAP,					/* GPIO remap */
#endif
#ifdef STM32F4_SGA_ENABLE
	.tGPIO[0].tGPIOInit.Alternate				= GPIO_AF7_USART1,
#endif

	.tGPIO[1].tGPIOInit.Pin 					= GPIO_PIN_10,				/* GPIO pin */
	.tGPIO[1].tGPIOInit.Pull 					= GPIO_NOPULL,				/* GPIO pull-up/pull-down settings, whether external pull-up/pull-down hardware is needed */
	.tGPIO[1].tGPIOInit.Speed 					= GPIO_SPEED_FREQ_HIGH,		/* GPIO speed */	
	.tGPIO[1].tGPIOPort 						= GPIOA,					/* GPIO port */
#ifdef STM32F1_SGA_ENABLE	
	.tGPIO[1].tGPIOInit.Mode 					= GPIO_MODE_INPUT,			/* F4 series needs to be set as input mode */
	.tGPIO[1].ucAFMode							= NO_REMAP,					/* GPIO remap */
#endif
#ifdef STM32F4_SGA_ENABLE
	.tGPIO[1].tGPIOInit.Mode 					= GPIO_MODE_AF_PP,			/* F4 series needs to be set as alternate function */
	.tGPIO[1].tGPIOInit.Alternate				= GPIO_AF7_USART1,
#endif
};

/* PWM configuration - Thruster control (4 thrusters) */
tagPWM_T thrusterPWM[] =
{
	/* Thruster 1 - TIM3_CH1 (PA6) */
	[0] =
	{
		.tPWMHandle.Instance	= TIM3,         	/* Timer 3 */
		.fDuty					= 0.0,				/* Initial duty cycle (%) - 0% means stop */
		.ulFreq					= 50,				/* Frequency (Hz) - 50Hz suitable for most ESCs */
		.ucChannel				= TIM_CHANNEL_1,	/* Channel 1 */
		.tGPIO.tGPIOInit.Pin	= GPIO_PIN_6,		/* PA6 pin */
		.tGPIO.tGPIOPort		= GPIOA,			/* GPIOA port */
#ifdef STM32F1_SGA_ENABLE
		.tGPIO.ucAFMode			= NO_REMAP,			/* No remap mode */
#endif
	},
	
	/* Thruster 2 - TIM3_CH2 (PA7) */
	[1] =
	{
		.tPWMHandle.Instance	= TIM3,         	/* Timer 3 */
		.fDuty					= 0.0,				/* Initial duty cycle (%) */
		.ulFreq					= 50,				/* Frequency (Hz) */
		.ucChannel				= TIM_CHANNEL_2,	/* Channel 2 */
		.tGPIO.tGPIOInit.Pin	= GPIO_PIN_7,		/* PA7 pin */
		.tGPIO.tGPIOPort		= GPIOA,			/* GPIOA port */
#ifdef STM32F1_SGA_ENABLE
		.tGPIO.ucAFMode			= NO_REMAP,			/* No remap mode */
#endif
	},
	
	/* Thruster 3 - TIM3_CH3 (PB0) */
	[2] =
	{
		.tPWMHandle.Instance	= TIM3,         	/* Timer 3 */
		.fDuty					= 0.0,				/* Initial duty cycle (%) */
		.ulFreq					= 50,				/* Frequency (Hz) */
		.ucChannel				= TIM_CHANNEL_3,	/* Channel 3 */
		.tGPIO.tGPIOInit.Pin	= GPIO_PIN_0,		/* PB0 pin */
		.tGPIO.tGPIOPort		= GPIOB,			/* GPIOB port */
#ifdef STM32F1_SGA_ENABLE
		.tGPIO.ucAFMode			= NO_REMAP,			/* No remap mode */
#endif
	},
	
	/* Thruster 4 - TIM3_CH4 (PB1) */
	[3] =
	{
		.tPWMHandle.Instance	= TIM3,         	/* Timer 3 */
		.fDuty					= 0.0,				/* Initial duty cycle (%) */
		.ulFreq					= 50,				/* Frequency (Hz) */
		.ucChannel				= TIM_CHANNEL_4,	/* Channel 4 */
		.tGPIO.tGPIOInit.Pin	= GPIO_PIN_1,		/* PB1 pin */
		.tGPIO.tGPIOPort		= GPIOB,			/* GPIOB port */
#ifdef STM32F1_SGA_ENABLE
		.tGPIO.ucAFMode			= NO_REMAP,			/* No remap mode */
#endif
	},
};

