GLOBAL  _read_msw,_lidt
GLOBAL  _int_08_hand
GLOBAL  _int80hand
GLOBAL  _mascaraPIC1,_mascaraPIC2,_Cli,_Sti
GLOBAL  _debug
GLOBAL	_SystemCalls

EXTERN	stop
EXTERN  int_08
EXTERN  int80h


SECTION .text



_Cli:
	cli			; limpia flag de interrupciones
	ret

_Sti:

	sti			; habilita interrupciones por flag
	ret

_mascaraPIC1:			; Escribe mascara del PIC 1
	push    ebp
        mov     ebp, esp
        mov     ax, [ss:ebp+8]  ; ax = mascara de 16 bits
        out	21h,al
        pop     ebp
        retn

_mascaraPIC2:			; Escribe mascara del PIC 2
	push    ebp
        mov     ebp, esp
        mov     ax, [ss:ebp+8]  ; ax = mascara de 16 bits
        out	0A1h,al
        pop     ebp
        retn

_read_msw:
        smsw    ax		; Obtiene la Machine Status Word
        retn


_lidt:				; Carga el IDTR
        push    ebp
        mov     ebp, esp
        push    ebx
        mov     ebx, [ss: ebp + 6] ; ds:bx = puntero a IDTR
	rol	ebx,16
	lidt    [ds: ebx]          ; carga IDTR
        pop     ebx
        pop     ebp
        retn


_int80hand:
push ebp
mov ebp, esp

push edi
push esi
push edx
push ecx
push ebx
push esp ; Pointer to args array
push eax ; System calls number

call int80h

; eax: return value
; returns to old stack
pop eax
pop esp
pop ebx
pop ecx
pop edx
pop esi
pop edi
mov esp, ebp
pop ebp
sti

ret

_SystemCalls:
push ebp
mov ebp, esp

push ebx
push ecx
push edx
push esi
push edi

mov eax, [ebp + 8] ; Define number of system calls
mov ebx, [ebp + 12]; Arg1
mov ecx, [ebp + 16]; Arg2
mov edx, [ebp + 20]; Arg3
mov esi, [ebp + 24]; Arg4
mov edi, [ebp + 28]; Arg5

int 80h

pop edi
pop esi
pop edx
pop ecx
pop ebx

mov esp, ebp
pop ebp
ret




; Debug para el BOCHS, detiene la ejecucion para continuar ; colocar en el BOCHSDBG: set $eax=0
_debug:
        push    bp
        mov     bp, sp
        push	ax
vuelve:	mov     ax, 1
        cmp	ax, 0
	jne	vuelve
	pop	ax
	pop     bp
        retn
