BITS 64
segment .data

GLOBAL AllocateFromPool
AllocateFromPool dq 0x129a868

GLOBAL ThrowError
ThrowError dq 0x129071c

ERR_MEM_POOLALLOC_ALLOC_2 dq 0xc6f4ec84

CombatFloat dd 0x41a00000

GLOBAL CreateTaskVehicleCombat
CreateTaskVehicleCombat dq 0x939d9c

GLOBAL PatchMakeTaskRet
PatchMakeTaskRet dq 0x9c76b9

GLOBAL TaskPool
TaskPool dq 0x256cb88

GLOBAL PatchAutoSwitchRetFail
PatchAutoSwitchRetFail dq 0x8707fe

GLOBAL PatchAutoSwitchRetSucceed
PatchAutoSwitchRetSucceed dq 0x87075f

GLOBAL GetDuskCheck_addr
GetDuskCheck_addr dq 0

GLOBAL GetDuskCheck_ret
GetDuskCheck_ret dq 0

GLOBAL VehicleTypeOffset
VehicleTypeOffset dd 0xbc8

segment .text

GLOBAL PatchMakeTask
PatchMakeTask:
	mov rax, rax
	push rax
	sub rsp, 0x28
	mov rax, qword [rel TaskPool]
	mov rcx, qword [rax]
	mov r8, 0x10
	mov edx, 0xe0
	call [rel AllocateFromPool]
	test rax, rax
	jnz .success
	mov rcx, [rel ERR_MEM_POOLALLOC_ALLOC_2]
	call [rel ThrowError]
	xor rax, rax
	jmp .finish
.success:
	mov rcx, rax
	xor r8d, r8d
	lea rdx, [rbx + 0xc0]
	movss xmm3, dword [rel CombatFloat]
	call [rel CreateTaskVehicleCombat]
.finish:
	mov r9, rax
	add rsp, 0x28
	pop rax
	jmp [rel PatchMakeTaskRet]

GLOBAL PatchAutoSwitch
PatchAutoSwitch:
	movsxd rdx, dword [rel VehicleTypeOffset]
	cmp dword [rax+rdx], 0x5
	jz .success
	cmp dword [rax+rdx], 0xf
	jz .success
	jmp [rel PatchAutoSwitchRetFail]
.success:
	jmp [rel PatchAutoSwitchRetSucceed]

GLOBAL GetDuskCheck_patch
GetDuskCheck_patch:
	mov rcx, [rbx + 0x20]
	mov eax, [rcx + 0x340]
	cmp eax, 0xf
	setz al
	movzx ecx, al
	imul ecx, ecx, 0xffffff
	or ecx, 0xf8003f
	call [rel GetDuskCheck_addr]
	jmp [rel GetDuskCheck_ret]
