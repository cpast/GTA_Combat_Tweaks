BITS 64
segment .data

GLOBAL SpawnPolicePassengers_ret
SpawnPolicePassengers_ret dq 0

segment .text

GLOBAL SpawnPolicePassengers_patch
SpawnPolicePassengers_patch:
	cmp byte ptr [rsp+0x98], 0
	jz .skip
	or eax, 80000000h
.skip:
	jmp [rel SpawnPolicePassengers_ret]