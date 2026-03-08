_TEXT SEGMENT

PUBLIC rdseed_support
PUBLIC get_seed

rdseed_support PROC
    mov     eax, 1      ; check for RDSEED instruction support
    cpuid
    shr     ecx, 30
    and     ecx, 1
    mov     eax, ecx
    ret
rdseed_support ENDP

get_seed PROC
  _empty_the_entropy_buffer: 
    rdseed  rax
    jc      _empty_the_entropy_buffer

  _get_a_freshly_minted_seed:  
    rdseed  rax
    jnc     _get_a_freshly_minted_seed
    and     rax, 03FFFFh
    ret
get_seed ENDP

_TEXT ENDS
END