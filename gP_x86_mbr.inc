include	"gP_equates.inc"
include	"x86/x86_16.inc"
include	"x86/common/macros.inc"

stack	STACKCP

;MBR addressing space
org	7c00h

;MBR code entry point
jmp	main

;BIOS parameter block

;8 bytes for OEM Label
db	"MYOS    "

;Change at your own risk
dw	512
db	1
dw	1
db	2
dw	224
dw	2880
db	0f0h
dw	9
dw	18
dw	2
rd	1
rd	1
rw	1
db	41
dd	0

;11 bytes for volume label
db	"MYOS       "

;Do not change
db	"FAT12   "