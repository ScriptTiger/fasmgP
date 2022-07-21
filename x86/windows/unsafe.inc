;unsafe.Delete(HCX)()
;HCX    Heap allocation to free
cinc    unsafe.Delete
        call    rsto.Stoal
        invoke  HeapFree,\
                [os.Prchp],\
                0,\
                [HCX]
        call    rsto.Clodal
        ret
endcinc

;unsafe.Close(HCX)()
;HCX    Handle to close
cinc    unsafe.Close
        call    rsto.Stoal
        invoke  CloseHandle,\
                [HCX]
        call    rsto.Clodal
        ret
endcinc