/*
 * pp.h
 *
 *  Created on: 2012-11-09
 *      Author: pag
 *     Version: $Id$
 */

#ifndef Granary_PP_H_
#define Granary_PP_H_


/// Used to denote entrypoints into Granary.
#define GRANARY_ENTRYPOINT


#if defined(__GNUC__) && defined(__GNUC_MINOR__)
#   if __GNUC__ >= 4 && __GNUC_MINOR__ >= 7
#       define FORCE_INLINE inline
#   else
#       define FORCE_INLINE __attribute__((always_inline))
#   endif
#elif defined(__clang__)
#   define FORCE_INLINE __attribute__((always_inline))
#else
#   define FORCE_INLINE inline
#endif

#define GRANARY

#define ALIGN_TO(lval, const_align) (((lval) % (const_align)) ? ((const_align) - ((lval) % (const_align))) : 0)

#if GRANARY_IN_KERNEL
#   define IF_KERNEL(...) __VA_ARGS__
#   define IF_KERNEL_(...) , __VA_ARGS__
#   define IF_KERNEL_ELSE(if_true, if_false) if_true
#   define IF_KERNEL_ELSE_(if_true, if_false) , if_true
#   define IF_USER(...)
#   define IF_USER_(...)
#else
#   define IF_KERNEL(...)
#   define IF_KERNEL_(...)
#   define IF_KERNEL_ELSE(if_true, if_false) if_false
#   define IF_KERNEL_ELSE_(if_true, if_false) , if_false
#   define IF_USER(...) __VA_ARGS__
#   define IF_USER_(...) , __VA_ARGS__
#endif

#define FAULT (granary_break_on_fault(), granary_fault())
#define BARRIER ASM("" : : : "memory")

#define IF_DEBUG(cond, expr) {if(cond) { expr; }}

/// Use to statically initialize some code.
#define STATIC_INITIALIZE___(id, ...) \
    static void init_func_ ## id(void) throw(); \
    struct init_class_ ## id : public granary::static_init_list { \
    public: \
        init_class_ ## id(void) throw() { \
            this->exec = init_func_ ## id; \
            granary::static_init_list::append(*this); \
        } \
    }; \
    static init_class_ ## id init_val_ ## id; \
    static __attribute__((noinline)) void init_func_ ## id(void) { \
        (void) init_val_ ## id; \
        { __VA_ARGS__ } \
    }

#define STATIC_INITIALIZE__(line, counter, ...) \
    STATIC_INITIALIZE___(line ## _ ## counter, ##__VA_ARGS__)
#define STATIC_INITIALIZE_(line, counter, ...) \
    STATIC_INITIALIZE__(line, counter, ##__VA_ARGS__)
#define STATIC_INITIALIZE(...) \
    STATIC_INITIALIZE_(__LINE__, __COUNTER__, ##__VA_ARGS__)

#if GRANARY_IN_KERNEL
#   define IF_TEST(...)
#   define ADD_TEST(func, desc)
#   define ASSERT(cond)
#else
#   define IF_TEST(...) __VA_ARGS__
#   define ADD_TEST(test_func, test_desc) \
    STATIC_INITIALIZE({ \
        static granary::static_test_list test__; \
        test__.func = test_func; \
        test__.desc = test_desc; \
        granary::static_test_list::append(test__); \
    })
#   define ASSERT(cond) {if(!(cond)) { FAULT; }}
#endif

#define ASM(...) __asm__ __volatile__ ( __VA_ARGS__ )

/// unrolling macros for applying something to all general purpose registers
#define ALL_REGS(R, R_last) \
    R(rdi, R(rsi, R(rdx, R(rbx, R(rcx, R(rax, R(r8, R(r9, R(r10, R(r11, R(r12, R(r13, R(r14, R_last(r15))))))))))))))

/// unrolling macros for applying something to all argument registers
#define ALL_ARG_REGS(R, R_last) \
    R(rdi, R(rsi, R(rdx, R(rcx, R(r8, R_last(r9))))))


#define FOR_EACH_DIRECT_JUMP(macro, ...) \
    macro(jo, 3, ##__VA_ARGS__) \
    macro(jno, 4, ##__VA_ARGS__) \
    macro(jb, 3, ##__VA_ARGS__) \
    macro(jnb, 4, ##__VA_ARGS__) \
    macro(jz, 3, ##__VA_ARGS__) \
    macro(jnz, 4, ##__VA_ARGS__) \
    macro(jbe, 4, ##__VA_ARGS__) \
    macro(jnbe, 5, ##__VA_ARGS__) \
    macro(js, 3, ##__VA_ARGS__) \
    macro(jns, 4, ##__VA_ARGS__) \
    macro(jp, 3, ##__VA_ARGS__) \
    macro(jnp, 4, ##__VA_ARGS__) \
    macro(jl, 3, ##__VA_ARGS__) \
    macro(jnl, 4, ##__VA_ARGS__) \
    macro(jle, 4, ##__VA_ARGS__) \
    macro(jnle, 5, ##__VA_ARGS__) \
    macro(loop, 5, ##__VA_ARGS__) \
    macro(loopne, 7, ##__VA_ARGS__) \
    macro(loope, 6, ##__VA_ARGS__) \
    macro(jmp, 4, ##__VA_ARGS__) \
    macro(jmp_short, 10, ##__VA_ARGS__) \
    macro(jmp_ind, 8, ##__VA_ARGS__) \
    macro(jmp_far, 8, ##__VA_ARGS__) \
    macro(jmp_far_ind, 12, ##__VA_ARGS__) \
    macro(jecxz, 6, ##__VA_ARGS__)

#define TO_STRING_(x) #x
#define TO_STRING(x) TO_STRING_(x)

#endif /* Granary_PP_H_ */
