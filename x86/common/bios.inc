;bios.ClearScreen(ch)()
cinc	bios.ClearScreen
	mov	ah,2h
	xor	bh,bh
	xor	dh,dh
	xor	dl,dl
	int	10h
	mov	ah,6h
	xor	al,al
	mov	bh,ch
	xor	ch,ch
	mov	dh,24
	mov	dl,79
	int	10h
	ret
endcinc

;bios.Cout(si)()
cinc	bios.Cout
	mov	ah,0eh

	.getchar:
	lodsb
	cmp	al,0
	jne	.prntchar
	ret

	.prntchar:
	int	10h
	jmp	.getchar
endcinc

;bios.GetKey()()
cinc	bios.GetKey
	xor	ah,ah
	int	16h
	ret
endcinc

bios.Reboot	equ	0ffffh:0

;bios.PowerOff()()
cinc	bios.PowerOff
	mov	ax,5307h
	mov	cx,3
	mov	bx,1
	int	15h
endcinc