/*
 * registers.h
 *
 *   Copyright: Copyright 2012 Peter Goodman, REG_all rights reserved.
 *      Author: Peter Goodman
 */

MAKE_REG(rax, REG_RAX)
MAKE_REG(rcx, REG_RCX)
MAKE_REG(rdx, REG_RDX)
MAKE_REG(rbx, REG_RBX)
MAKE_REG(rsp, REG_RSP)
MAKE_REG(rbp, REG_RBP)
MAKE_REG(rsi, REG_RSI)
MAKE_REG(rdi, REG_RDI)
MAKE_REG(r8, REG_R8)
MAKE_REG(r9, REG_R9)
MAKE_REG(r10, REG_R10)
MAKE_REG(r11, REG_R11)
MAKE_REG(r12, REG_R12)
MAKE_REG(r13, REG_R13)
MAKE_REG(r14, REG_R14)
MAKE_REG(r15, REG_R15)
MAKE_REG(eax, REG_EAX)
MAKE_REG(ecx, REG_ECX)
MAKE_REG(edx, REG_EDX)
MAKE_REG(ebx, REG_EBX)
MAKE_REG(esp, REG_ESP)
MAKE_REG(ebp, REG_EBP)
MAKE_REG(esi, REG_ESI)
MAKE_REG(edi, REG_EDI)
MAKE_REG(r8d, REG_R8D)
MAKE_REG(r9d, REG_R9D)
MAKE_REG(r10d, REG_R10D)
MAKE_REG(r11d, REG_R11D)
MAKE_REG(r12d, REG_R12D)
MAKE_REG(r13d, REG_R13D)
MAKE_REG(r14d, REG_R14D)
MAKE_REG(r15d, REG_R15D)
MAKE_REG(ax, REG_AX)
MAKE_REG(cx, REG_CX)
MAKE_REG(dx, REG_DX)
MAKE_REG(bx, REG_BX)
MAKE_REG(sp, REG_SP)
MAKE_REG(bp, REG_BP)
MAKE_REG(si, REG_SI)
MAKE_REG(di, REG_DI)
MAKE_REG(r8w, REG_R8W)
MAKE_REG(r9w, REG_R9W)
MAKE_REG(r10w, REG_R10W)
MAKE_REG(r11w, REG_R11W)
MAKE_REG(r12w, REG_R12W)
MAKE_REG(r13w, REG_R13W)
MAKE_REG(r14w, REG_R14W)
MAKE_REG(r15w, REG_R15W)
MAKE_REG(al, REG_AL)
MAKE_REG(cl, REG_CL)
MAKE_REG(dl, REG_DL)
MAKE_REG(bl, REG_BL)
MAKE_REG(ah, REG_AH)
MAKE_REG(ch, REG_CH)
MAKE_REG(dh, REG_DH)
MAKE_REG(bh, REG_BH)
MAKE_REG(r8l, REG_R8L)
MAKE_REG(r9l, REG_R9L)
MAKE_REG(r10l, REG_R10L)
MAKE_REG(r11l, REG_R11L)
MAKE_REG(r12l, REG_R12L)
MAKE_REG(r13l, REG_R13L)
MAKE_REG(r14l, REG_R14L)
MAKE_REG(r15l, REG_R15L)
MAKE_REG(spl, REG_SPL)
MAKE_REG(bpl, REG_BPL)
MAKE_REG(sil, REG_SIL)
MAKE_REG(dil, REG_DIL)
MAKE_REG(mm0, REG_MM0)
MAKE_REG(mm1, REG_MM1)
MAKE_REG(mm2, REG_MM2)
MAKE_REG(mm3, REG_MM3)
MAKE_REG(mm4, REG_MM4)
MAKE_REG(mm5, REG_MM5)
MAKE_REG(mm6, REG_MM6)
MAKE_REG(mm7, REG_MM7)
MAKE_REG(xmm0, REG_XMM0)
MAKE_REG(xmm1, REG_XMM1)
MAKE_REG(xmm2, REG_XMM2)
MAKE_REG(xmm3, REG_XMM3)
MAKE_REG(xmm4, REG_XMM4)
MAKE_REG(xmm5, REG_XMM5)
MAKE_REG(xmm6, REG_XMM6)
MAKE_REG(xmm7, REG_XMM7)
MAKE_REG(xmm8, REG_XMM8)
MAKE_REG(xmm9, REG_XMM9)
MAKE_REG(xmm10, REG_XMM10)
MAKE_REG(xmm11, REG_XMM11)
MAKE_REG(xmm12, REG_XMM12)
MAKE_REG(xmm13, REG_XMM13)
MAKE_REG(xmm14, REG_XMM14)
MAKE_REG(xmm15, REG_XMM15)
MAKE_REG(st0, REG_ST0)
MAKE_REG(st1, REG_ST1)
MAKE_REG(st2, REG_ST2)
MAKE_REG(st3, REG_ST3)
MAKE_REG(st4, REG_ST4)
MAKE_REG(st5, REG_ST5)
MAKE_REG(st6, REG_ST6)
MAKE_REG(st7, REG_ST7)
MAKE_SEG(es, SEG_ES)
MAKE_SEG(cs, SEG_CS)
MAKE_SEG(ss, SEG_SS)
MAKE_SEG(ds, SEG_DS)
MAKE_SEG(fs, SEG_FS)
MAKE_SEG(gs, SEG_GS)
MAKE_REG(es, SEG_ES)
MAKE_REG(cs, SEG_CS)
MAKE_REG(ss, SEG_SS)
MAKE_REG(ds, SEG_DS)
MAKE_REG(fs, SEG_FS)
MAKE_REG(gs, SEG_GS)
MAKE_REG(dr0, REG_DR0)
MAKE_REG(dr1, REG_DR1)
MAKE_REG(dr2, REG_DR2)
MAKE_REG(dr3, REG_DR3)
MAKE_REG(dr4, REG_DR4)
MAKE_REG(dr5, REG_DR5)
MAKE_REG(dr6, REG_DR6)
MAKE_REG(dr7, REG_DR7)
MAKE_REG(dr8, REG_DR8)
MAKE_REG(dr9, REG_DR9)
MAKE_REG(dr10, REG_DR10)
MAKE_REG(dr11, REG_DR11)
MAKE_REG(dr12, REG_DR12)
MAKE_REG(dr13, REG_DR13)
MAKE_REG(dr14, REG_DR14)
MAKE_REG(dr15, REG_DR15)
MAKE_REG(cr0, REG_CR0)
MAKE_REG(cr1, REG_CR1)
MAKE_REG(cr2, REG_CR2)
MAKE_REG(cr3, REG_CR3)
MAKE_REG(cr4, REG_CR4)
MAKE_REG(cr5, REG_CR5)
MAKE_REG(cr6, REG_CR6)
MAKE_REG(cr7, REG_CR7)
MAKE_REG(cr8, REG_CR8)
MAKE_REG(cr9, REG_CR9)
MAKE_REG(cr10, REG_CR10)
MAKE_REG(cr11, REG_CR11)
MAKE_REG(cr12, REG_CR12)
MAKE_REG(cr13, REG_CR13)
MAKE_REG(cr14, REG_CR14)
MAKE_REG(cr15, REG_CR15)
MAKE_REG(ymm0, REG_YMM0)
MAKE_REG(ymm1, REG_YMM1)
MAKE_REG(ymm2, REG_YMM2)
MAKE_REG(ymm3, REG_YMM3)
MAKE_REG(ymm4, REG_YMM4)
MAKE_REG(ymm5, REG_YMM5)
MAKE_REG(ymm6, REG_YMM6)
MAKE_REG(ymm7, REG_YMM7)
MAKE_REG(ymm8, REG_YMM8)
MAKE_REG(ymm9, REG_YMM9)
MAKE_REG(ymm10, REG_YMM10)
MAKE_REG(ymm11, REG_YMM11)
MAKE_REG(ymm12, REG_YMM12)
MAKE_REG(ymm13, REG_YMM13)
MAKE_REG(ymm14, REG_YMM14)
MAKE_REG(ymm15, REG_YMM15)

MAKE_REG(arg1, REG_RDI)
MAKE_REG(arg2, REG_RSI)
MAKE_REG(arg3, REG_RDX)

#if GRANARY_IN_KERNEL
    MAKE_REG(arg4, REG_R10)
    MAKE_REG(arg5, REG_R8)
    MAKE_REG(arg6, REG_R9)

    MAKE_REG(arg4_32, REG_R10D)
    MAKE_REG(arg4_16, REG_R10W)
#else
    MAKE_REG(arg4, REG_RCX)
    MAKE_REG(arg5, REG_R8)
    MAKE_REG(arg6, REG_R9)

    MAKE_REG(arg4_32, REG_ECX)
    MAKE_REG(arg4_16, REG_CX)
#endif

MAKE_REG(arg1_32, REG_EDI)
MAKE_REG(arg2_32, REG_ESI)
MAKE_REG(arg3_32, REG_EDX)


MAKE_REG(arg1_16, REG_DI)
MAKE_REG(arg2_16, REG_SI)
MAKE_REG(arg3_16, REG_DX)


MAKE_REG(ret, REG_RAX)
MAKE_REG(ret_32, REG_EAX)
MAKE_REG(ret_16, REG_AX)
MAKE_REG(ret_8, REG_AL)

