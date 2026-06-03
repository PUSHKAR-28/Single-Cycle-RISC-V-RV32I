.section .text.init
.global _start

_start:
    lui  sp, 6          # sp = 0x6000  (top of 16KB DMEM)         
       
    jal  ra, main       
    j    .