	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 13, 0
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:                                ; %.label0
	sub	sp, sp, #64
	stp	x22, x21, [sp, #16]             ; 16-byte Folded Spill
	stp	x20, x19, [sp, #32]             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #48]             ; 16-byte Folded Spill
	.cfi_def_cfa_offset 64
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	.cfi_offset w21, -40
	.cfi_offset w22, -48
	ldr	x0, [x1, #8]
	mov	x19, x1
	bl	_atoi
	ldr	x8, [x19, #16]
	str	w0, [sp, #12]
	mov	x0, x8
	bl	_atoi
Lloh0:
	adrp	x19, l_.fmtstr_s@PAGE
Lloh1:
	adrp	x8, l_.str1@PAGE
Lloh2:
	add	x19, x19, l_.fmtstr_s@PAGEOFF
Lloh3:
	add	x8, x8, l_.str1@PAGEOFF
	str	w0, [sp, #8]
	mov	x0, x19
	str	x8, [sp]
	bl	_printf
Lloh4:
	adrp	x21, l_.fmtstr_true@PAGE
Lloh5:
	adrp	x20, l_.fmtstr_false@PAGE
Lloh6:
	add	x21, x21, l_.fmtstr_true@PAGEOFF
Lloh7:
	add	x20, x20, l_.fmtstr_false@PAGEOFF
	mov	x0, x21
	bl	_printf
Lloh8:
	adrp	x8, l_.str2@PAGE
	mov	x0, x19
Lloh9:
	add	x8, x8, l_.str2@PAGEOFF
	str	x8, [sp]
	bl	_printf
	mov	x0, x21
	bl	_printf
Lloh10:
	adrp	x8, l_.str3@PAGE
	mov	x0, x19
Lloh11:
	add	x8, x8, l_.str3@PAGEOFF
	str	x8, [sp]
	bl	_printf
	mov	x0, x20
	bl	_printf
Lloh12:
	adrp	x8, l_.str4@PAGE
	mov	x0, x19
Lloh13:
	add	x8, x8, l_.str4@PAGEOFF
	str	x8, [sp]
	bl	_printf
	ldp	w8, w9, [sp, #8]
	cmp	w8, w9
	csel	x0, x20, x21, eq
	bl	_printf
	ldp	x29, x30, [sp, #48]             ; 16-byte Folded Reload
	mov	w0, wzr
	ldp	x20, x19, [sp, #32]             ; 16-byte Folded Reload
	ldp	x22, x21, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #64
	ret
	.loh AdrpAdd	Lloh12, Lloh13
	.loh AdrpAdd	Lloh10, Lloh11
	.loh AdrpAdd	Lloh8, Lloh9
	.loh AdrpAdd	Lloh5, Lloh7
	.loh AdrpAdd	Lloh4, Lloh6
	.loh AdrpAdd	Lloh1, Lloh3
	.loh AdrpAdd	Lloh0, Lloh2
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__cstring,cstring_literals
l_.fmtstr_true:                         ; @.fmtstr_true
	.asciz	"true\n"

l_.fmtstr_false:                        ; @.fmtstr_false
	.asciz	"false\n"

l_.fmtstr_d:                            ; @.fmtstr_d
	.asciz	"%d\n"

l_.fmtstr_g:                            ; @.fmtstr_g
	.asciz	"%.08F\n"

l_.fmtstr_s:                            ; @.fmtstr_s
	.asciz	"%s\n"

l_.str4:                                ; @.str4
	.asciz	"!(b==a) result:"

l_.str3:                                ; @.str3
	.asciz	"!(a==a) result:"

	.p2align	4                               ; @.str2
l_.str2:
	.asciz	"a==a||b!=b result:"

	.p2align	4                               ; @.str1
l_.str1:
	.asciz	"a==a&&b==b result:"

.subsections_via_symbols
