# Thruster PWM Setup Guide for STM32F1

This guide explains how to set up PWM control for thrusters using the SGA Library on STM32F1 microcontrollers.

## Overview

The thruster PWM system provides 4 independent PWM channels for controlling thrusters or motors. Each channel can be individually controlled with precise duty cycle settings.

## Hardware Configuration

### Pin Mapping

The thruster PWM system uses the following pin configuration:

| Thruster | Timer | Channel | Pin | Function |
|----------|-------|---------|-----|----------|
| Thruster 1 | TIM3 | CH1 | PA6 | PWM Output |
| Thruster 2 | TIM3 | CH2 | PA7 | PWM Output |
| Thruster 3 | TIM3 | CH3 | PB0 | PWM Output |
| Thruster 4 | TIM3 | CH4 | PB1 | PWM Output |

### Timer Configuration

- **TIM3**: 4 channels (Thrusters 1-4)
- **Frequency**: 50Hz (suitable for most ESC controllers)
- **Resolution**: High precision duty cycle control

## Software Configuration

### 1. PWM Configuration Structure

The PWM configuration is defined in `config.c`:

```c
tagPWM_T thrusterPWM[] =
{
    /* Thruster 1 - TIM3_CH1 (PA6) */
    [0] =
    {
        .tPWMHandle.Instance    = TIM3,         /* Timer 3 */
        .fDuty                  = 0.0,          /* Initial duty cycle (%) */
        .ulFreq                 = 50,           /* Frequency (Hz) */
        .ucChannel              = TIM_CHANNEL_1, /* Channel 1 */
        .tGPIO.tGPIOInit.Pin    = GPIO_PIN_6,   /* PA6 pin */
        .tGPIO.tGPIOPort        = GPIOA,        /* GPIOA port */
#ifdef STM32F1_SGA_ENABLE
        .tGPIO.ucAFMode         = NO_REMAP,     /* No remap mode */
#endif
    },
    // ... additional thrusters
};
```

### 2. Control Functions

The following functions are available for thruster control (following SGA Library naming conventions):

#### `Dev_Thruster_Init()`
Initializes all PWM channels for thrusters.

#### `Dev_Thruster_SetDuty(uint8_t _ucThrusterId, float _fDuty)`
Sets the duty cycle for a specific thruster.
- `_ucThrusterId`: Thruster number (0-3)
- `_fDuty`: Duty cycle percentage (0.0-100.0)

#### `Dev_Thruster_Stop(uint8_t _ucThrusterId)`
Stops a specific thruster (sets duty cycle to 0%).

#### `Dev_Thruster_StopAll()`
Stops all thrusters.

#### `Dev_Thruster_SafeMode()`
Sets all thrusters to safe mode (5% duty cycle).

#### `Dev_Thruster_Demo()`
Runs a comprehensive demonstration sequence.

## Usage Examples

### Basic Initialization

```c
void UserLogic_Code(void)
{
    /* Initialize thruster PWM */
    Dev_Thruster_Init();
    
    while(1)
    {
        /* Your control logic here */
    }
}
```

### Individual Thruster Control

```c
/* Set thruster 0 to 25% duty cycle */
Dev_Thruster_SetDuty(0, 25.0f);

/* Set thruster 1 to 50% duty cycle */
Dev_Thruster_SetDuty(1, 50.0f);

/* Stop thruster 2 */
Dev_Thruster_Stop(2);
```

### Synchronous Control

```c
/* Set all thrusters to same duty cycle */
for(int i = 0; i < THRUSTER_COUNT; i++)
{
    Dev_Thruster_SetDuty(i, 30.0f);
}

/* Stop all thrusters */
Dev_Thruster_StopAll();
```

### Safety Features

```c
/* Emergency stop */
Dev_Thruster_StopAll();

/* Safe mode for testing */
Dev_Thruster_SafeMode();
```

## SGA Library Compliance

### Function Naming Convention
- All thruster control functions use the `Dev_` prefix
- Function names follow the pattern: `Dev_Thruster_<Action>`
- Parameters use underscore prefix (`_ucThrusterId`, `_fDuty`)

### Variable Naming Convention
- Local variables use proper type prefixes (`ucI`, `fDuty`)
- Global variables use `g_` prefix if needed
- Static variables use `s_` prefix

### Code Structure
- Functions follow SGA Library documentation standards
- Error checking and validation included
- Comprehensive comments in Chinese and English

## ESC Compatibility

This PWM configuration is compatible with most Electronic Speed Controllers (ESC):

- **Frequency**: 50Hz (standard for most ESCs)
- **Duty Cycle Range**: 0-100%
- **Pulse Width**: 1-2ms (5-10% duty cycle at 50Hz)
- **Signal Type**: Standard PWM signal

### ESC Calibration

For proper ESC operation:

1. **Power up sequence**: Always start with 0% duty cycle
2. **Calibration**: Some ESCs require calibration for full range
3. **Safety**: Use `Dev_Thruster_SafeMode()` for initial testing

## Safety Considerations

### 1. Initialization Safety
- All thrusters start at 0% duty cycle
- Use `Dev_Thruster_SafeMode()` for testing
- Always verify connections before powering

### 2. Operating Limits
- Maximum duty cycle: 100%
- Minimum duty cycle: 0%
- Safe testing duty cycle: 5%

### 3. Emergency Procedures
- `Dev_Thruster_StopAll()` for emergency stop
- Always have a failsafe mechanism
- Test in safe environment first

## Troubleshooting

### Common Issues

1. **No PWM Output**
   - Check GPIO pin configuration
   - Verify timer initialization
   - Ensure PWM is enabled in `drv_hal_conf.h`

2. **Incorrect Frequency**
   - Verify `ulFreq` setting in configuration
   - Check system clock configuration

3. **ESC Not Responding**
   - Verify duty cycle range (5-10% for most ESCs)
   - Check ESC calibration
   - Ensure proper power supply

### Debug Information

The system provides debug output via UART:

```
Thruster PWM channels initialized
Thruster 0 set to 25.0% duty
All thrusters stopped
```

## Advanced Configuration

### Custom Frequency

To change PWM frequency, modify the `ulFreq` parameter:

```c
.ulFreq = 100,  /* 100Hz for high-speed applications */
```

### Custom Pin Mapping

To use different pins, modify the GPIO configuration:

```c
.tGPIO.tGPIOInit.Pin = GPIO_PIN_X,  /* Change pin number */
.tGPIO.tGPIOPort = GPIOX,           /* Change port */
```

### Timer Selection

You can use different timers by changing the instance:

```c
.tPWMHandle.Instance = TIM1,  /* Use Timer 1 instead */
```

## Performance Characteristics

- **Update Rate**: Real-time PWM updates
- **Resolution**: High precision duty cycle control
- **Latency**: Minimal processing delay
- **Reliability**: Robust error checking and safety features

## Integration with Flight Controllers

This PWM system can be integrated with flight control algorithms:

```c
/* Example: PID control integration */
void Thruster_PID_Control(float thrust_command)
{
    float duty_cycle = thrust_command * 100.0f;  /* Convert to percentage */
    Dev_Thruster_SetDuty(0, duty_cycle);
}
```

## Handle Examples

The thruster PWM configuration is documented in `Doc/句柄资源示例.txt`:

```c
/* Thruster PWM configuration example (4 thrusters) */
tagPWM_T thrusterPWM[] =
{
    /* Thruster 1 - TIM3_CH1 (PA6) */
    [0] =
    {
        .tPWMHandle.Instance    = TIM3,         /* Timer 3 */
        .fDuty                  = 0.0,          /* Initial duty cycle (%) */
        .ulFreq                 = 50,           /* Frequency (Hz) */
        .ucChannel              = TIM_CHANNEL_1, /* Channel 1 */
        .tGPIO.tGPIOInit.Pin    = GPIO_PIN_6,   /* PA6 pin */
        .tGPIO.tGPIOPort        = GPIOA,        /* GPIOA port */
#ifdef STM32F1_SGA_ENABLE
        .tGPIO.ucAFMode         = NO_REMAP,     /* No remap mode */
#endif
    },
    // ... additional thrusters
};
```

## Conclusion

This thruster PWM system provides a robust, safe, and flexible solution for controlling thrusters or motors using the SGA Library on STM32F1 microcontrollers. The system includes comprehensive safety features, follows SGA Library coding standards, and is compatible with most ESC controllers. 