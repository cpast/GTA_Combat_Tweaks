BITS 64
segment .data

GLOBAL Minimap_Return
Minimap_Return dq 0

GLOBAL Flash_Time
Flash_Time dd 750

segment .text

GLOBAL Minimap_Update
Minimap_Update:
	neg al
	mov ecx, 6
	sbb r8d, r8d
	add r8d, 0x50
	cmp ebx, dword [rel Flash_Time]
	jne .skip
	add r8d, 0x20
.skip:
	jmp qword [rel Minimap_Return]