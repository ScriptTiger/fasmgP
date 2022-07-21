;libc.Pause()()
;Standard pause prompt to press any key to continue
cinc libc.Pause
	call	rsto.Stoal
	mov	HBX,HAX
	mov	HSI,.AskKey
	call	os.Cout
	invoke	FlushConsoleInputBuffer,\
		[os.Stdin]
	.sleep:
	invoke Sleep,\
		100
	cinvoke _kbhit
	cmp HAX,0
	je .sleep
	invoke FlushConsoleInputBuffer,\
		[os.Stdin]
	mov HSI,eol
	call os.Cout
	mov HAX,HBX
	call rsto.Clodal
	ret

	.AskKey db "Press any key to continue . . . ",0
endcinc