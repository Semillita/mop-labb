#include "keyb.h"
#include "ports.h"


void InitKeyboard(void (*interrupt_handler)())
{
 // b15-b12 used for output to rows
// b11-b8  used for input from columns
*GPIO_D_MODER &= 0x0000FFFF;
*GPIO_D_MODER |= 0x55000000;
// Pinnarna som läses från tangentbordet är spänningssatta om
// nedtryckta och flytande annars, så behöver Pull Down
*GPIO_D_PUPDR &= 0x0000FFFF;
*GPIO_D_PUPDR |= 0x00AA0000;
// Pinnarna som väljer rad skall vara spänningssatta (Push/Pull)
*GPIO_D_OTYPER= 0x00000000; 
*GPIO_D_OSPEEDR = 0xAAAAAAAA;
// Activate all rows
*GPIO_D_ODRHIGH = 0xF0; 
// Put handler in vector table
*((void (**)(void) ) 0x2001C09C ) = interrupt_handler; // Lines 5-9(IRQ 23)
*((void (**)(void) ) 0x2001C0E0 ) = interrupt_handler; // Lines 10-15 (IRQ 40)
// Enable interrupts on NVIC
*NVIC_ISER0 |= (1 << 23);
*NVIC_ISER1 |= (1 << (40 - 32));
// Configure EXTI
*SYSCFG_EXTICR3 = 0x3333; // Lines 8-11 all on port D
//*SYSCFG_EXTICR3 = 0x4444; // Lines 8-11 all on port E
*EXTI_RTSR = 0xF00; // Trigger on rising
*EXTI_FTSR = 0xF00; // Trigger on falling
*EXTI_IMR = 0xF00; // Enable lines 8-11
}

// Aktivera en rad för läsning
void kbdActivate( unsigned int row )
{
switch( row )
{
case 1: *GPIO_D_ODRHIGH = 0x10; break;
case 2: *GPIO_D_ODRHIGH = 0x20; break;
case 3: *GPIO_D_ODRHIGH = 0x40; break;
case 4: *GPIO_D_ODRHIGH = 0x80; break;
case 0: *GPIO_D_ODRHIGH = 0x00; break;
}
}

// Läs en rad och returnera vilken kolumn som är ett 
// (antar endast en tangent nedtryckt)
int kbdGetCol ( void )
{
unsigned short c;
c = *GPIO_D_IDRHIGH;
 if ( c & 0x8 )return 4;
if ( c & 0x4 ) return 3;
if ( c & 0x2 ) return 2;
if ( c & 0x1 ) return 1;
return 0;
}

// Returnera en byte som motsvarar den knapp som är nedtryckt, 
// eller 0xFF om ingen knapp är nedtryckt
unsigned char keyb(void) 
{
for (int row=1; row <=4 ; row++) {
kbdActivate(row);
int a = 0; 
for(int i=0; i<100; i++){ a = a * a;}
int col = kbdGetCol();
if(col){ 
return 4*(row-1)+(col-1);
}
}
return 0xFF;
}

unsigned int get_row_values() {
    unsigned short c = *GPIO_D_IDRHIGH;
    return c;
}

struct Input get_input() {
    struct Input input;

    kbdActivate(1);
    unsigned int row = get_row_values();
    if (row & 0x1) {
        input.player_1.up = 1;
    }
    if (row & 0x8) {
        input.player_2.up = 1;
    }

    kbdActivate(2);
    row = get_row_values();
    if (row & 0x1) {
        input.player_1.down = 1;
    }
    if (row & 0x8) {
        input.player_2.down = 1;
    }
    
    return input;
}