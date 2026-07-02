# SGA Library Code Writing Specification V1.1

To ensure unified code style for the SGA library (specifically the Driver layer, OCD layer, Dev layer, and Algo layer), the following specifications are established. This specification is explained in 5 parts: **Files and Directories**, **Formatting**, **Comments**, **Variables|Structures|Constants|Macros**, and **Functions**. These specifications are common in C language and don't need to be memorized by heart. You can refer to already written driver files for guidance.

# I. Files and Directories

1. All source files have lowercase extensions .c and .h

2. When creating new drivers in the Driver layer, both files need to include `#include "drv_hal_conf.h"`, and corresponding conditional compilation should be added to the driver files, while also adding conditional compilation in `drv_hal_conf.h`

    ```c
    #ifdef DRV_HAL_UART_ENABLE
    #include "drv_hal_uart.h"
    #endif
    ```

    For OCD layer, Dev layer, and Algo layer new files, you only need to include the header file with the same name in the .c file

3. Header file inclusion rules

    - When including header files, use <> to include predefined or specific directory header files, generally used for C language standard libraries

    - Use " " to include header files in the current directory or paths relative to the current directory, generally used for self-written header files

4. To prevent header files from being repeatedly included, use ifndef/define/endif structure to generate preprocessing blocks

    ```c
    #ifndef __DRV_UART_H_
    #define __DRV_UART_H_
    /* Header file body */
    #endif
    ```

5. Header files should only contain **structures**, **macro definitions**, **variable declarations**, and **function declarations**, and should not contain function bodies

    Header file order is: *conditional compilation (preprocessing blocks) -> header file inclusion -> macro definitions -> variable declarations -> structure definitions -> function declarations*

# II. Formatting

1. Functions or procedures, structure definitions, and code in loops, conditional statements, etc. should all use **indentation** style. Case statements should also follow indentation requirements. **Indentation is 4 spaces**, that is, use the TAB key

2. Empty lines **must** be added between independent program blocks and after local variable declarations

    ```c
    void DemoFun(void)
    {
    	uint8_t i;
    	<------empty line----->
    	/* Function block 1 */
    	if()
    	{
    		
    	}
    	<------empty line----->
    	/* Function block 2 */
    	{
    		
    	}
    }
    ```

3. <font color=red>Multiple short statements are not allowed</font> on one line, especially when naming multiple variables

    ```c
    rect.length = 0; rect.width = 0; <---- Incorrect writing
    
    rect.length = 0; <---- Correct writing
    rect.width = 0;
    ```

4. Program block delimiters (such as braces '{' and '}') should each occupy a separate line and be in the same column, while being left-aligned with the statements that reference them.

    ```c
    for (...) { <---- Non-standard writing
    	 /* program code */
    }
    
    for (...)
    { <---- Standard writing
    	/* program code1 */
        if(...)
    	{
        	/* program code2 */
    	}
    }
    ```

5. Add spaces before and after comparison operators, assignment operators "=", "+=", arithmetic operators "+", "%", logical operators "&&", "&", bit field operators "<<", "^", etc.

    ```c
    if (current_time >= MAX_TIME_VALUE)
    a = b + c;
    a *= 2;
    a = b ^ 2;
    ```

# III. Comments

1. The principle of comments is to help understand the program. Add comments where needed, not too many or too few. Comment language must be accurate, understandable, and concise. Source code effective comment volume must be **above 20%**.
   
2. **Must** write comments for variable definitions and branch statements (conditional branches, loop statements, etc.). For maintainers, good comments help better understand the program, sometimes even better than reading design documents.
   
3. Comment format should be as uniform as possible. Strongly recommend using **/* …… */**, because C++ comments **//** are not supported by all C compilers. Also, there should be a space between the two asterisks and the comment, for example: /* comment */
   
4. At the beginning of the file, you need to add descriptions about file copyright, content introduction, modification history, etc., and write modification content here after making modifications (<u>version number can be left blank</u>)
   
    ```c
    /****************************************************************************
    
    * Sigma Team
    
    * Filename: ocd_jy901.c
    
    * Content Description: jy901 module file
    
    * File History:
    
    * Version	       Date  	  Author		Description
    * 1.1.8		2022-10-22	Bao Chenglu		Integrated structure, provided IT mode and DMA mode
    								  Provided jy901 data processing, conversion, and print functions
    
    * 1.0.0a 	2020-03-14	Li Huanyu		Created this file
    
    * 1.1.0a 	2020-03-29	Li Huanyu		Modified some text
    
    ****************************************************************************/
    ```
    
5. **Must** add sufficient and concise comment information related to function implementation before each function. Content includes function introduction, variable and constant descriptions, and parameter descriptions.
   
    <font color=red>Tips:</font> Global variables needed by the function body should be placed above the function comment to avoid comments not being effective for the function.
    
    Example as follows
    
    ```c
    const uint8_t s_ucResetBuf[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
    /**
     * @brief PWM initialization function
     * @param _tPWM-PWM structure pointer
     * @param _ucNum-PWM quantity
     * @note Initialize multiple PWMs		<---------- note written as needed
     * @retval Null
    */
    void Drv_PWM_Init(tagPWM_T *_tPWM, uint8_t _ucNum)
    {
    	uint8_t index;
    	
    	for(index = 0;index < _ucNum;index++)
    	{
    		S_PWM_CLKEnable(&_tPWM[index]);
    		S_PWM_GPIOConfig(&_tPWM[index]);
    		S_PWM_PramConfig(&_tPWM[index]);
    	}
    }
    ```
    
6. Comments should be close to the code they describe. Comments for code should be placed above or to the right (for single statement comments). Comments should have the same indentation as the content they describe.
    - Comments **placed above** should be separated from the code above with an empty line.
    
    - Comments **placed to the right** should be written after one tab. If there are multiple single-line comments, it's best to align all right-side comments on one straight line.
    
        
    
    ```c
    /* code one comments */
    program code one
    
    /* code two comments */
    program code two
    
    /* code two comments */
    for()
    {
        /* code two comments */
        code
    }
    
    program code three	/* code two comments */
    program code four	/* code two comments */    
    ```

# IV. Variables, Structures, Constants, Macros

## Variables

1. **Variable naming** should be clear, understandable, with clear meaning, and use complete words or abbreviations that everyone can basically understand, avoiding misunderstandings.
   
    - temp can be abbreviated as tmp;
    - flag can be abbreviated as flg;
    - statistic can be abbreviated as stat;
    - message can be abbreviated as msg;
    
2. <font color=red>Prohibit using</font> single character variable names like i, j, k, etc. But i, j, k are allowed as local loop variables. Try to use index as loop variable.

3. Naming conventions must maintain similar style with other variables in the library, with appropriate case. Driver layer **must** follow the following naming! When used as **function parameters**, add prefix "_".

    Of course, there are also special variables that can be used, such as x, y when representing coordinates.

    

    <font color=red>Tips:</font> For types not in the table, you can combine them according to the pattern. Currently int types can be used without adding prefix.

    | Variable Type | Prefix | Example | Variable Type | Prefix | Example |
    | :-----------: | :--: | :-------: | :----------: | :--: | :------: |
    | uint8_t | uc | ucSum | int8_t | c | cSum |
    | uint16_t | us | usSum | int16_t | s | sSum |
    | uint32_t | ul | ulSum | int32_t | l | lSum |
    | uint8_t * | ucp | ucpWrite | int8_t * | cp | cpWrite |
    | uint16_t * | usp | uspWrite | int16_t * | sp | spWrite |
    | uint32_t * | ulp | ulpWrite | int32_t * | lp | lpWrite |
    | uint8_t array | uca | ucaNum[5] | int8_t array | ca | caNum[5] |
    | uint16_t array | usa | usaNum[5] | int16_t array | sa | saNum[5] |
    | uint32_t array | ula | ulaNum[5] | int32_t array | la | laNum[5] |
    | double | d | dNum | float | f | fNum |
    | bool | b | bTxEnable | structure pointer | t | tGPIO |

    <center><p>Variable Prefix Naming Definition Table</p></center>

    Specific examples:

    `uint8_t ucDMARxPriority;     /* DMA receive interrupt priority, 0-15 */`

    `uint16_t usRxLenth;      	  /* Receive data length */`

    `void Drv_PMW_FreqSet(tagPWM_T *_tPWM, uint32_t _ulFreq);`

4. Unless necessary, don't use numbers or strange characters to define identifiers, which can cause confusion.

    `uint8_t dat01;`

    `void Set00(uint_8 c);`

5. **Variable scope prefix**

    To clearly identify variable scope and reduce naming conflicts, you should add a prefix representing variable scope before the variable type prefix, and separate the variable type prefix and variable scope prefix with an underscore '_'.

    Specific rules are as follows:

    - For global variables, add "**g**" and variable type symbol prefix before the name.

    `uint32_t g_ulParaWord;`

    `uint8_t g_ucByte;`

    - For static variables, add "**s**" and variable type symbol prefix before the name.

    `static uint32_t s_ulParaWord;`

## Structures

For **structure naming** types, representing type names, all names start with lowercase "tag", followed by the first letter of each English word capitalized (including the first letter of the first word), other letters lowercase, ending with _T identifier.

<font color=red>Tips:</font> Don't use underscores to separate words.

> When calling structure handles later, structure variables should try to start with t. Not mandatory, but naming should be related to purpose.

```c
/* Structure naming */
typedef struct
{
...
}tagDMAUart_T;

/* Structure handle call */
tagDMAUart_T tUSART1;
```

## Constants, Macros

1. For **enum definitions**, use all uppercase, ending with **_E** identifier.

    ```c
    typedef enum
    {
        KB_F1 = 0, /* F1 key code */
        KB_F2, 	   /* F2 key code */
        KB_F3      /* F3 key code */
    }KEY_CODE_E;
    ```

2. **Constants and macros** should be all uppercase. If these names consist of multiple words, separate words with underscores.

    `#define LOG_BUF_SIZE 8000`

# V. Functions

## Function naming rules

Function names should accurately describe the function's purpose. Avoid using meaningless or unclear verbs for function naming.

Specifications are as follows:

- For driver functions that need to be open interface globally called, use underscores for function segmentation, capitalize first letter, capitalize first letter of each segment. Add **Drv_** before function name

    Examples:

    - `void Drv_Delay_Init(void);`
    - `void Drv_GPIO_Write(tagGPIO_T *_tGPIO, GPIO_PinState _pin);`

- For third-party peripheral OCD functions that need to be open interface globally called, use underscores for function segmentation, capitalize first letter, capitalize first letter of each segment. Add **OCD_** before function name

    Examples:

    - `void OCD_JY901_DataProcess(tagJY901_T *_tJY901);`
- For device Dev functions that need to be open interface globally called, use underscores for function segmentation, capitalize first letter, capitalize first letter of each segment. Add **Dev_** before function name

    Examples:

    - `uint8_t Dev_PS2_DataKey(tagPS2_T *_tPS2);`
- For algorithm Algo functions that need to be open interface globally called, use underscores for function segmentation, capitalize first letter, capitalize first letter of each segment. Add **Algo_** before function name

    Examples:

    - `float Algo_PID_Calculate(tagPID_T *_tPid,float _fCurrValue,float _fExpValue);`

- For local functions or static functions called by local files, use underscores for function segmentation, capitalize first letter, capitalize first letter of each segment.

    Add **S_** before function name, can also add **static** modifier to increase library function encapsulation.

    Examples:

    - `static void S_GPIO_CLKEnable(tagGPIO_T *_tGPIO);`

## Function parameter naming rules

**All parameters start with underscore _** to distinguish from ordinary variables. For functions with no parameters (void), parentheses follow immediately after the function.

Parameter prefixes are defined according to the **Variable Prefix Naming Definition Table** in Chapter IV.

`uint32_t UartConvUartBaud(uint32_t _ulBaud);`

## Source code function body arrangement order

*(Internal) local functions or static functions -> externally callable function functions -> initialization functions -> interrupt call operation integration functions*

The following is a UART DMA example

```c
/* (Internal) local functions or static functions */
static void S_UART_DMA_CLKEnable(tagUART_T *_tUART)
{
	...
}

/* Externally callable function functions */
void Drv_Uart_Transmit_DMA(tagUART_T *_tUART, uint8_t *_ucpTxData, uint16_t _uspSize)
{
	...
}

/* Initialization functions */
void Drv_Uart_DMAInit(tagUART_T *_tUART)
{
	...
}

/* Interrupt call operation integration functions */
void Drv_Uart_DMA_RxHandler(tagUART_T *_tUART)
{
	...
}
```

# VI. Pre-submission Preparation

Please read [How to submit PR (Pull Request) on github - Tencent Cloud Developer Community - Tencent Cloud](https://cloud.tencent.com/developer/article/1999727) (PR tutorial) first

1. Need to test on your own IM board or development board, and write into the library after **testing without errors**. The test project can be sent to me privately as an example.

2. In the Doc folder's **handle resource example.txt**, put the handle example of the written driver (optional if it's a bug fix).

3. In the Doc folder's **project maintenance record.txt**, write the date, author, and description according to previous examples, and also need to write specific modification records below. Version number is not written, unified naming by the moderator.

4. Check if the latest library version on GitHub is consistent with the current one, if not consistent, need to pull the latest code first.

5. Perform Git operations, follow PR specifications when committing (no period at the end)

    - Commit specifications are as follows:
    `<type>(<scope>): <subject>` (there's a space after the colon)
    - subject options:

        - feat: new feature
        - fix: fix bug
        - docs: documentation
        - style: format (changes that don't affect code running)
        - refactor: refactor (code changes that are neither new features nor bug fixes)
        - perf: performance optimization
        - test: add tests
        - chore: build process or auxiliary tool changes, such as requirement changes can be put here
    - Commit examples:

        fix: fix UART rate too slow issue

6. Submit through GitHub **Pull Request** method.
