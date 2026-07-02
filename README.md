# Deepblue Maker Underwater Robots — SGA Firmware Library

**Embedded C firmware library for STM32-based underwater robot controllers.**  
Built during the Deepblue Maker Underwater Robots camp (China, Summer 2025).

![Language](https://img.shields.io/badge/language-C-blue)
![Platform](https://img.shields.io/badge/platform-STM32%20F1%20%7C%20F4%20%7C%20L4-brightgreen)
![Toolchain](https://img.shields.io/badge/toolchain-Keil%20MDK-orange)
![RTOS](https://img.shields.io/badge/RTOS-FreeRTOS%20%7C%20RT--Thread-lightgrey)
![Library](https://img.shields.io/badge/SGA%20library-V3.4-informational)

---

## Visual

<!-- Add a photo of the robot or a wiring diagram here -->
> _Screenshot / wiring diagram placeholder_

---

## Overview

The **SGA library** is a layered embedded C framework that sits on top of the STM32 HAL and gives underwater robot firmware a consistent, reusable API across the STM32 F1, F4, and L4 families. The active chip family is selected at compile time via a single macro (`STM32F1_SGA_ENABLE`, `STM32F4_SGA_ENABLE`, or `STM32L4_SGA_ENABLE`).

The library is structured into four abstraction layers: a hardware **Driver** layer, a third-party peripheral **OCD** layer, a **Device** layer, and an **Algorithm** layer. Application code sits above all of them in `Apply/`, and only ever touches the public `Drv_`, `OCD_`, `Dev_`, and `Algo_` APIs — never raw HAL calls.

The demo project (`Apply/Logic/Src/usercode.c`) drives four brushless thrusters over PWM and can be extended with the IMU, depth sensor, OLED display, SD-card logging, and PID control that are already wired into the BSP.

---

## Key Features

- **4-channel thruster PWM** — TIM3 CH1–CH4 (PA6, PA7, PB0, PB1), 50 Hz, duty-cycle clamped with safe-mode and emergency-stop helpers (`Dev_Thruster_SetDuty`, `Dev_Thruster_StopAll`, `Dev_Thruster_SafeMode`)
- **9-axis IMU (JY901)** — UART DMA driver; exposes accelerometer, gyroscope, Euler angles (roll/pitch/yaw), magnetometer, and quaternion; configurable output rate up to 200 Hz
- **Depth / pressure sensor (MS5837)** — I²C (software bit-bang), reads depth in cm and temperature in °C with factory calibration compensation; configurable oversampling rate
- **Incremental PID controller** — `Algo_PID_Calculate` / `Algo_PID_Init` / `Algo_PID_Clear`; intended for closed-loop thruster control
- **UART with DMA** — interrupt + DMA TX/DMA RX, configurable buffer sizes; `printf` routed to USART1
- **SPI (hardware + software bit-bang)**, **I²C (software bit-bang)**, **ADC, DAC, CAN, Flash, Watchdog, Power**
- **PS2 gamepad driver** — digital and analog modes, vibration motor control
- **OLED display (SSD1306)** — via software I²C
- **SD card + FatFS** — file read/write for data logging
- **Dual RTOS support** — FreeRTOS (CMSIS-RTOS V2) and RT-Thread; both disabled by default, enabled by uncommenting a `#define` in `drv_hal_conf.h`
- **Thread pool** — `threadpool.c` for task scheduling under either RTOS

---

## System Architecture & Data Flow

```
┌──────────────────────────────────────────────┐
│  Apply / Logic                               │
│  main.c → Task_Sys_Init → Task_UserInit      │
│          → UserLogic_Code (user loop)        │
├─────────────┬──────────────────┬─────────────┤
│ Apply/Task  │ Apply/Logic      │             │
│ task_sysinit│ config.c         │             │
│ task_userinit│ usercode.c      │             │
├─────────────┴──────────────────┴─────────────┤
│  BSP                                         │
│  OCD layer   Dev layer    Algo layer         │
│  ocd_jy901   dev_ps2      algo_pid           │
│  ocd_ms5837               algo_func          │
│  ocd_oled                                   │
│  ocd_sdcard + FatFS                          │
│  ocd_rm3100, ocd_at24cxx, ocd_ds3231         │
│  RTOS: FreeRTOS / RT-Thread (optional)       │
├──────────────────────────────────────────────┤
│  Driver layer  (do not modify)               │
│  drv_hal_gpio  drv_hal_pwm  drv_hal_uart     │
│  drv_hal_spi   drv_hal_iic_soft              │
│  drv_hal_adc   drv_hal_dac  drv_hal_can      │
│  drv_hal_timer drv_hal_flash drv_hal_wdg     │
│  drv_hal_pwr   drv_hal_delay                 │
├──────────────────────────────────────────────┤
│  STM32 HAL (vendor, in Hardware/)            │
│  STM32F1xx V1.1.7 / F4xx V1.8.1 / L4xx V1.10│
└──────────────────────────────────────────────┘
```

**Boot sequence:**  
`main()` → `Task_Sys_Init()` calls `Drv_HAL_Init()` (clock, system tick) → `Task_UserInit()` initializes GPIO and UART → `UserLogic_Code()` enters the main loop.  
If an RTOS is enabled, threads and the kernel start instead of the bare-metal loop.

---

## Project Structure

```
SGA_Demo-main/
├── Apply/
│   ├── Logic/          # User application: config.c, main.c, usercode.c
│   └── Task/           # Initialization tasks, IRQ handlers, thread pool
├── Bsp/
│   ├── Algo/           # Algorithm layer: PID (incremental), math utilities
│   ├── Dev/            # Device layer: PS2 gamepad
│   ├── FatFs/          # FatFS file system (for SD card logging)
│   ├── OCD/            # Third-party peripheral drivers:
│   │   │               #   MS5837 depth sensor, JY901 IMU, RM3100 magnetometer,
│   │   │               #   AT24Cxx EEPROM, DS3231 RTC, ADS1256 ADC,
│   │   │               #   SSD1306 OLED, SD card
│   └── RTOS/
│       ├── FreeRTOS/   # FreeRTOS kernel (Cortex-M3 & M4 ports)
│       └── RT-Thread/  # RT-Thread kernel (Cortex-M3 & M4 ports)
├── Doc/
│   ├── SGA库代码规范V1.1.md          # SGA coding standard (bilingual)
│   ├── Thruster_PWM_Setup_Guide.md   # Thruster PWM wiring & config guide
│   ├── 句柄资源示例.txt               # Handle / struct usage examples
│   ├── 工程目录介绍.txt               # Directory layout explanation
│   └── 工程维护记录.txt               # Changelog (V1.0 → V3.4)
├── Driver/
│   ├── Inc/            # SGA HAL header files (drv_hal_*.h)
│   └── Src/            # SGA HAL source files (drv_hal_*.c) — do not modify
├── Hardware/
│   ├── STM32F1XX/      # STM32F1 CMSIS + HAL library V1.1.7
│   ├── STM32F4XX/      # STM32F4 CMSIS + HAL library V1.8.1
│   └── STM32L4XX/      # STM32L4 CMSIS + HAL library V1.10.0
└── Project/
    ├── STM32.uvprojx   # Keil µVision project file
    ├── STM32.uvoptx    # Keil debug/target options
    ├── DebugConfig/    # J-Link / ST-Link debug configurations per chip
    └── .vscode/        # VSCode + Keil Assistant extension settings
```

---

## Tech Stack

| Component | What it is | Role in this project |
|-----------|-----------|----------------------|
| C11 (bare-metal) | Language standard | All firmware code |
| STM32 HAL | ST's hardware abstraction layer | Clock, DMA, peripherals |
| STM32F103 / F407 / L4 | ARM Cortex-M3/M4 MCUs | Target hardware |
| Keil MDK (µVision) | IDE + ARM compiler | Build and flash |
| FreeRTOS | Real-time OS | Optional task scheduling (disabled by default) |
| RT-Thread | Real-time OS | Alternative RTOS option (disabled by default) |
| FatFS | File system middleware | SD card data logging |
| JY901 | 9-DOF AHRS module | Robot orientation feedback |
| MS5837 | Pressure/depth sensor (I²C) | Depth measurement |
| SSD1306 | OLED display | On-robot status display |

---

## Getting Started

### Prerequisites

- **Keil MDK** (ARM compiler, tested with the project's `.uvprojx`)  
  _or_ VSCode with the **Keil Assistant** extension (`.vscode/` config is included)
- A **J-Link** or **ST-Link** debugger/programmer
- Target board: STM32F103xx, STM32F407xx, or STM32L4xx

### 1. Open the project

Open `SGA_Demo-main/Project/STM32.uvprojx` in Keil µVision.

### 2. Select your chip family

In `SGA_Demo-main/Driver/Inc/drv_hal_conf.h`, the chip macro is set via Keil's preprocessor defines (project options → C/C++ → Define). Choose **one**:

```
STM32F1_SGA_ENABLE    # for STM32F103 series (72 MHz, HAL V1.1.7)
STM32F4_SGA_ENABLE    # for STM32F407 series (168 MHz, HAL V1.8.1)
STM32L4_SGA_ENABLE    # for STM32L4 series   (80 MHz, HAL V1.10.0)
```

### 3. Configure peripherals

Edit `SGA_Demo-main/Apply/Logic/Src/config.c` to match your board's pin assignments.  
The file contains pre-filled examples for GPIO, UART1 (DMA TX+RX), and 4-channel thruster PWM.

### 4. Write application logic

Put your control loop in `SGA_Demo-main/Apply/Logic/Src/usercode.c` inside `UserLogic_Code()`.

### 5. Build and flash

Use Keil's **Build** (F7) and **Download** (F8) buttons, or run the batch file:

```bat
SGA_Demo-main/Project/keilkilll.bat   # kills leftover Keil processes before rebuild
```

### 6. (Optional) Enable an RTOS

Uncomment exactly one line in `drv_hal_conf.h`:

```c
//#define RTT_ENABLE       // RT-Thread
//#define FREERTOS_ENABLE  // FreeRTOS
```

Then create threads in `usercode.c` and write thread kernel functions in `Apply/Logic/Src/threadpool.c`.

---

## Usage Examples

### Thruster control (4 thrusters via PWM)

```c
// Initialize all 4 PWM channels (from usercode.c)
Dev_Thruster_Init();

// Set thruster 0 to 25% duty cycle
Dev_Thruster_SetDuty(0, 25.0f);

// Emergency stop
Dev_Thruster_StopAll();

// Safe test mode (5% duty — minimum ESC startup value)
Dev_Thruster_SafeMode();
```

**Pin mapping** (STM32 TIM3, 50 Hz):

| Thruster | Timer CH | GPIO |
|----------|----------|------|
| 0 | TIM3_CH1 | PA6 |
| 1 | TIM3_CH2 | PA7 |
| 2 | TIM3_CH3 | PB0 |
| 3 | TIM3_CH4 | PB1 |

### PID controller

```c
tagPID_T tDepthPID;
float faPID[3] = {1.0f, 0.1f, 0.05f};  // Kp, Ki, Kd

Algo_PID_Init(&tDepthPID);
Algo_PID_Update(&tDepthPID, faPID);

float fOutput = Algo_PID_Calculate(&tDepthPID, fCurrentDepth, fTargetDepth);
Dev_Thruster_SetDuty(2, fOutput);  // apply to vertical thruster
```

### Depth sensor (MS5837 over I²C)

```c
tagMS5837_T tDepthSensor;
tDepthSensor.setOSR = MS5837_OSR1024;

OCD_MS5837_Init(&tDepthSensor);
OCD_MS5837_GetData(&tDepthSensor);

float fDepthCm    = tDepthSensor.fDepth;
float fTempC      = tDepthSensor.fTemperature;
```

### IMU (JY901 over UART DMA)

```c
tagJY901_T tIMU;
// configure baud, output rate, axes in tIMU.tConfig ...
OCD_JY901_DMAInit(&tIMU);

// In UART DMA receive callback:
if (OCD_JY901_DataProcess(&tIMU))
{
    OCD_JY901_DataConversion(&tIMU);
    float roll  = tIMU.stcAngle.ConRoll;
    float pitch = tIMU.stcAngle.ConPitch;
    float yaw   = tIMU.stcAngle.ConYaw;
}
```

---

## Roadmap

The codebase and maintenance log (`Doc/工程维护记录.txt`) show active development through **V3.4 (2024-08-28)**. Visible incomplete / noted items:

- [ ] RTOS integration in demo `usercode.c` is stubbed — thread creation and `osKernelStart()` are commented out with instructions but no sample tasks
- [ ] `ocd_conf.h` is empty — OCD peripheral enable macros have not yet been added (unlike the equivalent `drv_hal_conf.h` for Driver layer)
- [ ] No CI/CD or automated build system; builds are Keil-only (no CMake or Makefile)

---

## Classification

This project is **embedded firmware**. It does not belong to web, mobile, or AI/ML categories.

---

## License & Contributing

License not specified in the repository.

Contributions follow the SGA library PR process documented in `Doc/SGA库代码规范V1.1.md`:

1. Test on your own hardware before submitting.
2. Add handle examples to `Doc/句柄资源示例.txt`.
3. Record changes in `Doc/工程维护记录.txt`.
4. Pull latest from `main`, then open a Pull Request.

Commit message format: `<type>(<scope>): <subject>` (e.g., `fix(uart): resolve DMA overflow on high baud rates`)

---

## Contact

| | |
|---|---|
| Author | _[your name]_ |
| Email | _[your email]_ |
| Camp | Deepblue Maker Underwater Robots, China, Summer 2025 |

---

## Assumptions to Verify

The following claims are inferences from the code, not confirmed in writing by the project owner:

1. **"SGA" meaning** — the maintenance log header says "Sigma队" (Sigma Team); the full expansion of SGA is not stated anywhere in the codebase.
2. **ESC compatibility** — the PWM guide states 50 Hz is "suitable for most ESCs"; actual compatible ESC models are not documented.
3. **Hardware/HAL versions** — F1 HAL V1.1.7, F4 HAL V1.8.1, L4 HAL V1.10.0 are read from the directory introduction doc, not from the HAL source itself (not present in this repo).
4. **RM3100 magnetometer** — driver exists in OCD layer but no usage example was found in `usercode.c`; not confirmed to be tested in this demo.
