#include "usercode.h"		/* usercode header file */
#include "drv_hal_conf.h"   /* SGA library header file configuration */
#include "task_conf.h"      /* task header file configuration */
#include "ocd_conf.h"       /* OCD header file configuration */
#include "dev_conf.h"		/* Dev header file configuration */
#include "algo_conf.h"		/* Algo header file configuration */
#include "config.h"			/* I/O configuration header file */

/* Thruster control function declarations */
void Dev_Thruster_Init(void);
void Dev_Thruster_SetDuty(uint8_t _ucThrusterId, float _fDuty);
void Dev_Thruster_Stop(uint8_t _ucThrusterId);
void Dev_Thruster_StopAll(void);
void Dev_Thruster_SafeMode(void);
void Dev_Thruster_Demo(void);

/* User logic code */
void UserLogic_Code(void)
{
	/* Initialize thruster PWM */
	Dev_Thruster_Init();
	
	printf("SGA Thruster PWM Demo Started!\r\n");
	printf("Thruster PWM initialized with 4 channels\r\n");
	
	while(1)
	{
		/* Demonstrate thruster control */
		Dev_Thruster_Demo();
		
		Drv_Delay_Ms(2000);
	}
}

/**
 * @brief Initialize thruster PWM
 * @param None
 * @retval None
 */
void Dev_Thruster_Init(void)
{
	uint8_t ucI;
	
	/* Initialize all thruster PWM channels */
	for(ucI = 0; ucI < THRUSTER_COUNT; ucI++)
	{
		Drv_PWM_Init(&thrusterPWM[ucI], ucI);
		Dev_Thruster_Stop(ucI);  /* Ensure all thrusters start in stopped state */
	}
	
	printf("Thruster PWM channels initialized\r\n");
}

/**
 * @brief Set thruster duty cycle
 * @param _ucThrusterId: Thruster ID (0-3)
 * @param _fDuty: Duty cycle (0.0-100.0)
 * @retval None
 */
void Dev_Thruster_SetDuty(uint8_t _ucThrusterId, float _fDuty)
{
	if(_ucThrusterId >= THRUSTER_COUNT)
	{
		printf("Error: Invalid thruster ID %d\r\n", _ucThrusterId);
		return;
	}
	
	/* Limit duty cycle range */
	if(_fDuty < THRUSTER_MIN_DUTY)
		_fDuty = THRUSTER_MIN_DUTY;
	else if(_fDuty > THRUSTER_MAX_DUTY)
		_fDuty = THRUSTER_MAX_DUTY;
	
	/* Set PWM duty cycle */
	Drv_PWM_DutyFactorSet(&thrusterPWM[_ucThrusterId], _fDuty);
	
	printf("Thruster %d set to %.1f%% duty\r\n", _ucThrusterId, _fDuty);
}

/**
 * @brief Stop specified thruster
 * @param _ucThrusterId: Thruster ID (0-3)
 * @retval None
 */
void Dev_Thruster_Stop(uint8_t _ucThrusterId)
{
	Dev_Thruster_SetDuty(_ucThrusterId, THRUSTER_MIN_DUTY);
}

/**
 * @brief Stop all thrusters
 * @param None
 * @retval None
 */
void Dev_Thruster_StopAll(void)
{
	uint8_t ucI;
	
	for(ucI = 0; ucI < THRUSTER_COUNT; ucI++)
	{
		Dev_Thruster_Stop(ucI);
	}
	
	printf("All thrusters stopped\r\n");
}

/**
 * @brief Safe mode - Set all thrusters to safe duty cycle
 * @param None
 * @retval None
 */
void Dev_Thruster_SafeMode(void)
{
	uint8_t ucI;
	
	for(ucI = 0; ucI < THRUSTER_COUNT; ucI++)
	{
		Dev_Thruster_SetDuty(ucI, THRUSTER_SAFE_DUTY);
	}
	
	printf("All thrusters set to safe mode (%.1f%%)\r\n", THRUSTER_SAFE_DUTY);
}

/**
 * @brief Thruster demonstration function
 * @param None
 * @retval None
 */
void Dev_Thruster_Demo(void)
{
	uint8_t ucI;
	float fDuty;
	
	printf("\r\n=== Thruster Demo Sequence (4 Thrusters) ===\r\n");
	
	/* 1. Stop all thrusters */
	printf("1. Stopping all thrusters...\r\n");
	Dev_Thruster_StopAll();
	Drv_Delay_Ms(1000);
	
	/* 2. Test safe mode */
	printf("2. Testing safe mode...\r\n");
	Dev_Thruster_SafeMode();
	Drv_Delay_Ms(2000);
	
	/* 3. Test individual thrusters */
	printf("3. Testing individual thrusters...\r\n");
	for(ucI = 0; ucI < THRUSTER_COUNT; ucI++)
	{
		printf("   Testing thruster %d\r\n", ucI);
		
		/* Gradually increase duty cycle */
		for(fDuty = THRUSTER_SAFE_DUTY; fDuty <= 30.0f; fDuty += 5.0f)
		{
			Dev_Thruster_SetDuty(ucI, fDuty);
			Drv_Delay_Ms(200);
		}
		
		/* Hold for a period */
		Drv_Delay_Ms(500);
		
		/* Gradually decrease duty cycle */
		for(fDuty = 30.0f; fDuty >= THRUSTER_SAFE_DUTY; fDuty -= 5.0f)
		{
			Dev_Thruster_SetDuty(ucI, fDuty);
			Drv_Delay_Ms(200);
		}
		
		/* Stop current thruster */
		Dev_Thruster_Stop(ucI);
		Drv_Delay_Ms(300);
	}
	
	/* 4. Test all thrusters synchronously */
	printf("4. Testing all thrusters synchronously...\r\n");
	
	/* Synchronous startup */
	for(fDuty = THRUSTER_SAFE_DUTY; fDuty <= 25.0f; fDuty += 2.5f)
	{
		for(ucI = 0; ucI < THRUSTER_COUNT; ucI++)
		{
			Dev_Thruster_SetDuty(ucI, fDuty);
		}
		Drv_Delay_Ms(300);
	}
	
	/* Hold at full speed */
	Drv_Delay_Ms(1000);
	
	/* Synchronous deceleration */
	for(fDuty = 25.0f; fDuty >= THRUSTER_SAFE_DUTY; fDuty -= 2.5f)
	{
		for(ucI = 0; ucI < THRUSTER_COUNT; ucI++)
		{
			Dev_Thruster_SetDuty(ucI, fDuty);
		}
		Drv_Delay_Ms(300);
	}
	
	/* Stop all thrusters */
	Dev_Thruster_StopAll();
	
	printf("Demo sequence completed!\r\n");
}
