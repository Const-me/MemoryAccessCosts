; Random number generation
loc_1400016F0:
mov     eax, r10d
mov     edx, r11d
shl     eax, 10h
xor     eax, r10d
mov     r10d, r11d
mov     ecx, eax
mov     r11d, r9d
shr     ecx, 5
xor     ecx, eax
lea     eax, [rcx+rcx]
xor     eax, ecx
xor     eax, edx
xor     r9d, eax
mov     ecx, r9d
and     ecx, 0FFFFFFFh
shl     rcx, 5
add     rcx, rdi
xor     eax, eax
db      66h, 66h
nop     word ptr [rax+rax+00000000h]

; The inner loop. It adds 16 integer values using 2 iterations, each processing 2 __m128i values, each value containing 4 integers.
loc_140001730:
movdqu  xmm0, xmmword ptr [rcx+rax*4]	; _mm_loadu_si128
paddd   xmm0, xmm1	; _mm_add_epi32
movdqa  xmm1, xmm0
movdqu  xmm0, xmmword ptr [rcx+rax*4+10h]	; _mm_loadu_si128
add     rax, 8
paddd   xmm0, xmm2	; _mm_add_epi32
movdqa  xmm2, xmm0
cmp     rax, 10h
jb      short loc_140001730