#ifndef __CONFIG_H_
#define __CONFIG_H_

#include "drv_hal_conf.h"

/* GPIO configuration example */
extern tagGPIO_T demoGPIO[];

/* UART configuration example */
extern tagUART_T demoUart;

/* PWM configuration - Thruster control */
extern tagPWM_T thrusterPWM[];

/* Thruster count definition */
#define THRUSTER_COUNT 4

/* Thruster control related definitions */
#define THRUSTER_MIN_DUTY    0.0f    /* Minimum duty cycle - Stop */
#define THRUSTER_MAX_DUTY    100.0f  /* Maximum duty cycle - Full speed */
#define THRUSTER_SAFE_DUTY   5.0f    /* Safe duty cycle - Minimum startup value */

/* Thruster state enumeration */
typedef enum
{
    THRUSTER_STOP = 0,       /* Stop */
    THRUSTER_FORWARD,        /* Forward */
    THRUSTER_REVERSE,        /* Reverse */
    THRUSTER_SAFE            /* Safe mode */
} THRUSTER_STATE_E;

/* Thruster control function declarations */
void Dev_Thruster_Init(void);
void Dev_Thruster_SetDuty(uint8_t _ucThrusterId, float _fDuty);
void Dev_Thruster_Stop(uint8_t _ucThrusterId);
void Dev_Thruster_StopAll(void);
void Dev_Thruster_SafeMode(void);
void Dev_Thruster_Demo(void);

#endif
