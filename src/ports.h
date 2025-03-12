////////////////////////////////////////////////////////////////////////////////
// Portdefinitioner
////////////////////////////////////////////////////////////////////////////////
// GPIO Port D
#define GPIO_D_BASE 0x40020C00
#define GPIO_D_MODER ((volatile unsigned int *)(GPIO_D_BASE))  
#define GPIO_D_OTYPER ((volatile unsigned short *)(GPIO_D_BASE+0x4))  
#define GPIO_D_OSPEEDR ((volatile unsigned int *)(GPIO_D_BASE+0x8))  
#define GPIO_D_PUPDR ((volatile unsigned int *)(GPIO_D_BASE+0xC))  
#define GPIO_D_IDRLOW ((volatile unsigned char *)(GPIO_D_BASE+0x10))  
#define GPIO_D_IDRHIGH ((volatile unsigned char *)(GPIO_D_BASE+0x11))  
#define GPIO_D_ODRLOW ((volatile unsigned char *)(GPIO_D_BASE+0x14))  
#define GPIO_D_ODRHIGH ((volatile unsigned char *)(GPIO_D_BASE+0x15))  
// GPIO Port E
#define GPIO_E_BASE 0x40021000
#define GPIO_E_MODER ((volatile unsigned int *)(GPIO_E_BASE))  
#define GPIO_E_OTYPER ((volatile unsigned short *)(GPIO_E_BASE+0x4))  
#define GPIO_E_OSPEEDR ((volatile unsigned int *)(GPIO_E_BASE+0x8))  
#define GPIO_E_PUPDR ((volatile unsigned int *)(GPIO_E_BASE+0xC))  
#define  GPIO_E_IDRHIGH ((volatile unsigned char *)(GPIO_E_BASE+0x10+1))  
#define GPIO_E_ODRLOW ((volatile unsigned char *)(GPIO_E_BASE+0x14))  
#define GPIO_E_ODRHIGH ((volatile unsigned char *)(GPIO_E_BASE+0x14+1))  
// SysTick
#define STK_CTRL ((volatile unsigned int *)(0xE000E010))  
#define STK_LOAD ((volatile unsigned int *)(0xE000E014))  
#define STK_VAL ((volatile unsigned int *)(0xE000E018))  
// SYSCFG
#define SYSCFG_EXTICR1 ((volatile unsigned short *)0x40013808)
#define SYSCFG_EXTICR2 ((volatile unsigned short *)0x4001380C)
#define SYSCFG_EXTICR3 ((volatile unsigned short *)0x40013810)
// EXTI
#define EXTI_IMR ((volatile unsigned int *)0x40013c00)
#define EXTI_RTSR ((volatile unsigned int *)0x40013c08)
#define EXTI_FTSR ((volatile unsigned int *)0x40013c0c)
#define EXTI_PR ((volatile unsigned int *)0x40013c14)
// NVIC
#define NVIC_ISER0 ((volatile unsigned int *)0xE000E100)
#define NVIC_ISER1 ((volatile unsigned int *)0xE000E104)
// System Control Block
#define STB_VTOR ((volatile unsigned int *)0xE000ED08)