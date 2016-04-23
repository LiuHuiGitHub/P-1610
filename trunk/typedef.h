#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

#define TRUE                1
#define FALSE               0
#define ON                  1
#define OFF                 0
#define ENABLE					(1u)
#define DISABLE					(0u)

#define BIT0                (UINT8)0x01
#define BIT1                (UINT8)0x02
#define BIT2                (UINT8)0x04
#define BIT3                (UINT8)0x08
#define BIT4                (UINT8)0x10
#define BIT5                (UINT8)0x20
#define BIT6                (UINT8)0x40
#define BIT7                (UINT8)0x80
#define BIT8                (UINT16)0x0100
#define BIT9                (UINT16)0x0200
#define BIT10               (UINT16)0x0400
#define BIT11               (UINT16)0x0800
#define BIT12               (UINT16)0x1000
#define BIT13               (UINT16)0x2000
#define BIT14               (UINT16)0x4000
#define BIT15               (UINT16)0x8000

#ifndef __BOOL
#define __BOOL
typedef bit		BOOL;
#endif

#ifndef __UINT8
#define __UINT8
typedef unsigned char       UINT8;
#endif

#ifndef __SINT8
#define __SINT8
typedef signed char         SINT8;
#endif

#ifndef __UINT16
#define __UINT16
typedef unsigned int        UINT16;
#endif

#ifndef __SINT16
#define __SINT16
typedef signed int          SINT16;
#endif

#ifndef __UINT32
#define __UINT32
typedef unsigned long		UINT32;
#endif

#ifndef __SINT32
#define __SINT32
typedef signed long			SINT32;
#endif

typedef unsigned char		uint8_t;
typedef unsigned long       uint32_t;

#ifdef  USE_FULL_ASSERT
/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports
  *         the name of the source file and the source line number of the call
  *         that failed. If expr is true, it returns no value.
  * @retval None
  */
#define assert_param(expr) if((expr)==0)assert_failed((uint8_t *)__FILE__, __LINE__)
/* Exported functions ------------------------------------------------------- */
void assert_failed(uint8_t *file, uint32_t line);
#else
#define assert_param(expr) //((void)0)
#endif /* USE_FULL_ASSERT */

#endif
