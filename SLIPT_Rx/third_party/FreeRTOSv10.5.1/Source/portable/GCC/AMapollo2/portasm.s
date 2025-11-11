/*

FreeRTOS Kernel V10.5.1

Copyright (C) 2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.

THIS IS THE STANDARD CORTEX-M4F PORT ASSEMBLY FOR GCC.

NOTE: This file replaces the IAR version that was causing assembly errors.
*/

.syntax unified
.cpu cortex-m4
.thumb

.section .text
.global vPortSVCHandler
.global xPortPendSVHandler
.global vPortStartFirstTask
.global vPortStart
.global ulCriticalNesting
.global ulPortYield
.extern pxCurrentTCB
.extern vTaskSwitchContext

/*

The FreeRTOS kernel starts the scheduler by setting the MSP to the

start of the stack and calling vPortStartFirstTask.
/
vPortStart:
ldr r0, =0xE000ED08      / Load the address of the NVIC's VTOR register /
ldr r0, [r0]             / Load the address of the vector table itself /
ldr r0, [r0]             / Load the initial stack pointer value /
msr msp, r0              / Set the stack pointer to the initial value /
cpsie i                  / Globally enable interrupts /
cpsie f
dsb
isb
mov r0, #0
bx r0                    / Function returns to the address in LR, but we jump to vPortStartFirstTask */

vPortStartFirstTask:
ldr r0, =0xE000ED08      /* VTOR address /
ldr r0, [r0]
ldr r0, [r0]             / Load the initial stack pointer value /
msr msp, r0              / Set the stack pointer to the initial value */

/* Ensure the Systick handler uses the main stack. */
ldr r0, =0xE000ED14      /* Address of AIRCR register */
ldr r1, [r0]
orr r1, #(1 << 9)        /* Set PRIS bit (0x200) */
str r1, [r0]

/* Call the first task's entry point. */
bl vPortStartFirstTask


/*

Context switch handlers
/
xPortPendSVHandler:
/ Save context of the currently running task. /
mrs r0, psp              / Read PSP into R0 (R0 now holds the stack pointer) /
isb                      / Instruction barrier */

/* Get the location to save the context (R0). /
ldr r3, =pxCurrentTCB    / Load address of pxCurrentTCB /
ldr r2, [r3]             / Load pxCurrentTCB value (R2 holds TCB address) */

/* Only save the context if it's not the first time through. /
tst r14, #0x4            / Check if we're using the FPU /
it eq
vstmiaeq r0!, {s16-s31}  / If FPU is used, save FPU registers (s16-s31) to stack. */

/* Save the remaining context (R4-R11, LR). */
stmia r0!, {r4-r11, r14}

/* Save the new top of stack back to the TCB. /
str r0, [r2]             / Store R0 (new stack top) into TCB (R2) */

/* Call the scheduler function to select the next task. /
mov r0, #0               / Argument for vTaskSwitchContext is 0 */
bl vTaskSwitchContext

/* Restore context of the newly selected task. /
ldr r3, =pxCurrentTCB    / Load address of pxCurrentTCB /
ldr r2, [r3]             / Load pxCurrentTCB value (R2 holds TCB address) /
ldr r0, [r2]             / Load R0 with the stack top from TCB */

/* Restore the remaining context (R4-R11, LR). */
ldmia r0!, {r4-r11, r14}

/* Check if the FPU was used by the next task. /
tst r14, #0x4            / Check LR for FPU usage flag /
it eq
vldmiaeq r0!, {s16-s31}  / If FPU was used, restore FPU registers. */

/* Set PSP to the new task's stack pointer and execute. /
msr psp, r0              / Set PSP to R0 /
isb
bx r14                   / Return from interrupt. */

vPortSVCHandler:
ldr r3, =pxCurrentTCB     /* Load address of pxCurrentTCB /
ldr r1, [r3]              / Load pxCurrentTCB into R1 (TCB address) /
ldr r0, [r1]              / Load R0 with the stack pointer (R0 = stack top) */

ldmia r0!, {r4-r11, r14}  /* Load R4-R11, LR (R0 updated to new stack top) */
msr psp, r0               /* Set PSP to the new stack top */
isb
bx r14                    /* Return to task mode. */


.end
