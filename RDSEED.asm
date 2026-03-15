_TEXT SEGMENT

PUBLIC rdseed_support
PUBLIC get_seed

; return weether RDSEED instruction is supported
rdseed_support PROC
    mov     eax, 1
    cpuid
    shr     ecx, 30
    and     ecx, 1
    mov     eax, ecx
    ret
rdseed_support ENDP

; get a freshly baked seed
get_seed PROC
  _empty_the_buffer: 
    rdseed  rax
    jc      _empty_the_buffer

  _get_a_freshly_baked_seed:  
    rdseed  rax
    jnc     _get_a_freshly_baked_seed
    and     rax, 03FFFFh
    ret
get_seed ENDP

_TEXT ENDS
END