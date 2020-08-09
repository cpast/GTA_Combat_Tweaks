BITS 64
segment .data

GLOBAL SW_AllocateFromPool
SW_AllocateFromPool dq 0x129a868

GLOBAL SW_ThrowError
SW_ThrowError dq 0x129071c

ERR_MEM_POOLALLOC_ALLOC_2 dq 0xc6f4ec84

CombatFloat dd 0x41a00000

GLOBAL SW_CreateTaskVehicleCombat
SW_CreateTaskVehicleCombat dq 0x939d9c

GLOBAL SW_PatchMakeTaskRet
SW_PatchMakeTaskRet dq 0x9c76b9

GLOBAL SW_TaskPool
SW_TaskPool dq 0x256cb88

GLOBAL SW_PatchAutoSwitchRetFail
SW_PatchAutoSwitchRetFail dq 0x8707fe

GLOBAL SW_PatchAutoSwitchRetSucceed
SW_PatchAutoSwitchRetSucceed dq 0x87075f

GLOBAL SW_VehicleTypeOffset
SW_VehicleTypeOffset dd 0xbc8

segment .text

GLOBAL SW_PatchMakeTask
SW_PatchMakeTask:
	mov rax, rax
	push rax
	sub rsp, 0x28
	mov rax, qword [rel SW_TaskPool]
	mov rcx, qword [rax]
	mov r8, 0x10
	mov edx, 0xe0
	call [rel SW_AllocateFromPool]
	test rax, rax
	jnz .success
	mov rcx, [rel ERR_MEM_POOLALLOC_ALLOC_2]
	call [rel SW_ThrowError]
	xor rax, rax
	jmp .finish
.success:
	mov rcx, rax
	xor r8d, r8d
	lea rdx, [rbx + 0xc0]
	movss xmm3, dword [rel CombatFloat]
	call [rel SW_CreateTaskVehicleCombat]
.finish:
	mov r9, rax
	add rsp, 0x28
	pop rax
	jmp [rel SW_PatchMakeTaskRet]

GLOBAL SW_PatchAutoSwitch
SW_PatchAutoSwitch:
	movsxd rdx, dword [rel SW_VehicleTypeOffset]
	cmp dword [rax+rdx], 0x5
	jz .success
	cmp dword [rax+rdx], 0xf
	jz .success
	jmp [rel SW_PatchAutoSwitchRetFail]
.success:
	jmp [rel SW_PatchAutoSwitchRetSucceed]