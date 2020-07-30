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

GLOBAL PatchRet
PatchRet dq 0x9c76b9

GLOBAL TaskPool
TaskPool dq 0x256cb88

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
	jmp [rel PatchRet]