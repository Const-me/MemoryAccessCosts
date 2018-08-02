loc_402758:
; Random number generation
mov     edx, eax
shl     edx, 10h
xor     eax, edx
mov     edx, eax
shr     edx, 5
xor     eax, edx
mov     edx, eax
add     eax, eax
xor     edx, esi
xor     edx, ebx
xor     edx, eax
mov     r12d, edx
and     r12d, 0FFFFFFFh
shl     r12, 5

; The inner loop adds 16 integer values. As you see, gcc unrolled it completely.
mov     eax, [rcx+r12+8]
add     eax, [rcx+r12+4]
add     eax, [rcx+r12]
add     eax, [rcx+r12+0Ch]
add     eax, [rcx+r12+10h]
add     eax, [rcx+r12+14h]
add     eax, [rcx+r12+18h]
add     eax, [rcx+r12+1Ch]
add     eax, [rcx+r12+20h]
add     eax, [rcx+r12+24h]
add     eax, [rcx+r12+28h]
add     eax, [rcx+r12+2Ch]
add     eax, [rcx+r12+30h]
add     eax, [rcx+r12+34h]
add     eax, [rcx+r12+38h]
add     eax, [rcx+r12+3Ch]
add     edi, eax
mov     eax, esi
mov     esi, ebx
mov     ebx, edx
sub     rbp, 1
jnz     loc_402758