BITS 64
segment .data

GLOBAL SmashWindowsGun_ret
SmashWindowsGun_ret dq 0

GLOBAL SmashWindowsProjectile_ret
SmashWindowsProjectile_ret dq 0

GLOBAL VehicleFlags_offset
VehicleFlags_offset dq 0

segment .text

GLOBAL SmashWindowsGun_patch
SmashWindowsGun_patch:
	push rdx
	push rsi
	mov esi, 1000h
	mov rax, [rbx + 20h]
	mov rcx, [rel VehicleFlags_offset]
	add rcx, 4h
	add rcx, rax
	mov edx, [rcx]
	and edx, esi
	add rcx, 4h
	mov esi, 1000000h
	mov ecx, [rcx]
	and ecx, esi
	or ecx, edx
	pop rsi
	pop rdx
	test ecx, ecx
	jmp [rel SmashWindowsGun_ret]

GLOBAL SmashWindowsProjectile_patch
SmashWindowsProjectile_patch:
	push rdx
	push rsi
	mov esi, 1000h
	mov rax, [rdi + 20h]
	mov rcx, [rel VehicleFlags_offset]
	add rcx, 4h
	add rcx, rax
	mov edx, [rcx]
	and edx, esi
	add rcx, 4h
	mov rsi, 1000000h
	mov ecx, [rcx]
	and ecx, esi
	or ecx, edx
	pop rsi
	pop rdx
	test ecx, ecx
	jmp [rel SmashWindowsProjectile_ret]