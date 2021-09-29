BITS 64
segment .data

GLOBAL SpawnPolicePassengers_ret
SpawnPolicePassengers_ret dq 0

GLOBAL GetVehicleOccupant_ret
GetVehicleOccupant_ret dq 0

GLOBAL gtaRand
gtaRand dq 0

GLOBAL ClassOffsets_CVehicleModelInfo_NumPeds
ClassOffsets_CVehicleModelInfo_NumPeds dd 0

segment .text

GLOBAL SpawnPolicePassengers_patch
SpawnPolicePassengers_patch:
	cmp byte [rsp+0x98], 0
	jz .skip
	or eax, 80000000h
.skip:
	jmp [rel SpawnPolicePassengers_ret]

GLOBAL GetVehicleOccupant_patch
GetVehicleOccupant_patch:
	push rax
	push rcx
	push rdx
	push r8
	push r9
	push r10
	push r11
	mov r10d, [rel ClassOffsets_CVehicleModelInfo_NumPeds]
	movzx r11d, word [rax + r10]
	test r11d, r11d
	jz .skip
	push r11
	sub rsp, 20h
	call [rel gtaRand]
	add rsp, 20h
	pop rcx
	xor rdx, rdx
	div ecx
	mov esi, edx
	jmp .noskip
.skip:
	xor esi, esi
.noskip:
	pop r11
	pop r10
	pop r9
	pop r8
	pop rdx
	pop rcx
	pop rax
	xor r14d, r14d
	mov rbx, rax
	jmp [rel GetVehicleOccupant_ret]