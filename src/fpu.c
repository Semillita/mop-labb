#define CPACR (unsigned int *) 0xE000ED88

__attribute( (naked) ) unsigned int getControlReg( void )
{
    __asm volatile ("MRS R0,CONTROL\n");
    __asm volatile ("BX  LR\n");
}

__attribute( (naked) ) void setControlReg( unsigned int x )
{
    __asm volatile ("MSR CONTROL,R0\n");
    __asm volatile ("BX  LR\n");
}

void enable_fpu() {
    __asm volatile(".SYNTAX unified\n"
                   ".CPU cortex-m4\n"
                   ".FPU fpv4-sp-d16\n"
                   ".THUMB\n"
                   "LDR R0,=25200\n");

    *CPACR |= 0x00F00000; // Sätt igång FPU, gör ingen skillnad
    unsigned int controlReg = getControlReg() | 4;
    setControlReg(controlReg);
}