.extern main, _crt_init, _crt_deinit
.extern md407_runtime_uartinit, md407_runtime_portinit, md407_runtime_clockinit
@.global enableFpu

.section .start_section
startup:
    LDR R0,=__stack_top      @ set stack pointer
    MOV SP,R0
    BL md407_runtime_clockinit
    BL md407_runtime_portinit
    BL md407_runtime_uartinit

    ISB
    BL _crt_init            @ init C-runtime
    BL main                 @ call main 
    BL _crt_deinit          @ deinit C-runtime
.L1: 
    B .L1                   @ never return

@enableFpu:
    @ CPACR is located at address 0xE000ED88
    @LDR R0, =0xE000ED88
    @ Read CPACR
    @LDR R1, [R0]
    @ Set bits 20-23 to enable CP10 and CP11 coprocessors
    @LDR R2, =0xF00000
    @ORR R1, R1, R2
    @ Write back the modified value to the CPACR
    @STR R1, [R0]@ wait for store to complete
    @DSB
    @ reset pipeline now the FPU is enabled
