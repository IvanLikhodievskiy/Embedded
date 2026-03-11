.global _start

.text
_start:
    mov r0, #3
    mov r1, #2
    mov r2, #3
    bl calculate
    mov r7, #1
    svc #0

calculate:
    push {r4-r6}
    mov r6, r0
    mov r3, #10
    mov r0, #0

    cmp r6, #0 // Check if repeat == 0
    beq .exit

.loop:
    add r4, r1, r2
    mul r5, r1, r2
    add r0, r0, r4
    add r0, r0, r5
    cmp r4, r3
    movlt r1, r4
    movge r1, r3
    subs r6, r6, #1
    bne .loop

.exit:
    pop {r4-r6}
    bx lr
