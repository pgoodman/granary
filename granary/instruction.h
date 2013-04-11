/*
 * instruction.h
 *
 *   Copyright: Copyright 2012 Peter Goodman, all rights reserved.
 *      Author: Peter Goodman
 */

#ifndef granary_INSTRUCTION_H_
#define granary_INSTRUCTION_H_

#include <utility>

#include "granary/globals.h"
#include "granary/policy.h"

namespace granary {


    /// Forward declarations.
    struct operand;
    struct operand_base_disp;
    struct operand_ref;
    struct instruction;
    struct instruction_list;


    /// Defines an operand generator for LEA operands
    struct operand_base_disp {
    public:

        typename dynamorio::reg_id_t base;
        typename dynamorio::reg_id_t index;

        unsigned char size;

        int scale;
        int disp;

        operand_base_disp(void) throw();

        /// Add the scale parameter to the LEA operand. Value values are 1, 2,
        /// 4, and 8.
        template <typename T>
        operand_base_disp operator*(T scale_) const throw() {
            static_assert(std::is_integral<T>::value,
                "Scale must have an integral type.");

            operand_base_disp ret(*this);
            ret.scale = static_cast<int>(scale_);
            return ret;
        }

        /// Add in the displacement to the LEA operand.
        template <typename T>
        operand_base_disp operator+(T disp_) const throw() {
            static_assert(std::is_integral<T>::value,
                "Displacement must have an integral type.");

            operand_base_disp ret(*this);
            ret.disp = static_cast<int>(disp_);
            return ret;
        }

        /// Add in the displacement to the LEA operand.
        template <typename T>
        inline operand_base_disp operator-(T disp_) const throw() {
            operand_base_disp ret(this->operator+(disp_));
            ret.disp = -ret.disp;
            return ret;
        }

        /// Implicit conversion from unpacked to packed LEA operand type.
        inline operator typename dynamorio::opnd_t(void) const throw() {
            return dynamorio::opnd_create_base_disp(
                base, index, scale, disp, size);
        }
    };


    /// Defines a generic operand
    struct operand : public dynamorio::opnd_t {
    public:

        inline operand(void) throw() {
            memset(this, 0, sizeof *this);
        }

        inline operand(const dynamorio::opnd_t &&that) throw() {
            memcpy(this, &that, sizeof *this);
        }

        inline operand(const dynamorio::opnd_t &that) throw() {
            memcpy(this, &that, sizeof *this);
        }

        operand(typename dynamorio::reg_id_t reg_) throw();

        inline operand &operator=(dynamorio::opnd_t that) throw() {
        	memcpy(this, &that, sizeof *this);
        	return *this;
        }

        /// De-referencing creates a new operand type
        operand_base_disp operator*(void) const throw();

        /// Accessing some byte offset from the operand (assuming it points to
        /// some memory)
        operand_base_disp operator[](int64_t num_bytes) const throw();

        operand_base_disp operator+(operand index) const throw();

        operand_base_disp operator+(operand_base_disp lea) const throw();

        /// Construct an LEA operand with an index and scale. This is to
        /// respect the order of operations. Note: valid values of scale are
        /// 1, 2, 4, and 8.
        template <typename I>
        operand_base_disp operator*(I scale) const throw() {
            static_assert(std::is_integral<I>::value,
                "Scale must have an integral type.");

            operand_base_disp ret;
            ret.base = dynamorio::DR_REG_NULL;
            ret.index = value.reg;
            ret.scale = static_cast<int>(scale);
            ret.disp = 0;
            return ret;
        }

        /// Construct an LEA operand with a base and displacement.
        template <typename I>
        operand_base_disp operator+(I disp) const throw() {
            static_assert(std::is_integral<I>::value,
                "Displacement must have an integral type.");

            operand_base_disp ret;
            ret.base = dynamorio::DR_REG_NULL;
            ret.index = value.reg;
            ret.scale = 1;
            ret.disp = static_cast<int>(disp);
            return ret;
        }

        inline operator uint64_t(void) const throw() {
            return value.immed_int;
        }

        inline operator app_pc(void) const throw() {
            return value.pc;
        }

        inline operator typename dynamorio::reg_id_t(void) const throw() {
            return value.reg;
        }
    };


    /// Represents a reference to an operand in an instruction. Useful for
    struct operand_ref {
    private:

        friend struct instruction;

        dynamorio::instr_t *instr;
        operand *op;

        operand_ref(void) = delete;

        operand_ref(dynamorio::instr_t *instr_, dynamorio::opnd_t *op_) throw();

    public:

        /// Const accesses of a field of the op are seen as rvalues and need
        /// not invalidate the bits of the instruction.
        inline const operand *operator->(void) const throw() {
            return op;
        }

        /// Assume that a non-const access of a field of the op will be used
        /// as an lvalue in an assignment; invalidate the raw bits.
        operand *operator->(void) throw();

        /// Assign an operand to this operand ref; this will update the operand
        /// referenced by this ref in place, and will invalidate the raw bits
        /// of the instruction.
        operand_ref &operator=(operand that) throw();
        operand_ref &operator=(operand_base_disp that) throw();

        inline operator uint64_t(void) const throw() {
            return op->value.immed_int;
        }

        inline operator app_pc(void) const throw() {
            return op->value.pc;
        }

        inline operator typename dynamorio::reg_id_t(void) const throw() {
            return op->value.reg;
        }

        inline operator typename dynamorio::opnd_t(void) const throw() {
            return *op;
        }
    };


    /// Defines a decoded x86 instruction type. This wraps around DynamoRIO's
    /// Level-3 decoding on x86 instructions.
    struct instruction {
    public:

        template <typename> friend struct list_meta;

        static typename dynamorio::dcontext_t *DCONTEXT;

        enum instruction_flag {
            DONT_MANGLE     = (1 << 0),
            DELAY_BEGIN     = (1 << 1),
            DELAY_END       = (1 << 2)
        };

        typename dynamorio::instr_t *instr;


        /// Constructor
        inline instruction(void) throw()
            : instr(nullptr)
        { }


        /// Construct an instruction by an instr_t pointer.
        inline instruction(dynamorio::instr_t *instr_) throw()
            : instr(instr_)
        { }


        /// Replace one instruction with another.
        void replace_with(instruction) throw();


        /// Return whether or not this instruction is valid.
        inline bool is_valid(void) const throw() {
            return nullptr != instr;
        }


        inline instruction prev(void) throw() {
            return instruction(instr->prev);
        }


        inline instruction next(void) throw() {
            return instruction(instr->next);
        }


        inline operator bool(void) const throw() {
            return nullptr != instr;
        }

        inline bool operator!(void) const throw() {
            return nullptr == instr;
        }


        /// Return the code cache policy to be used for the target of this CTI.
        inline instrumentation_policy policy(void) const throw() {
            return instrumentation_policy::from_id(instr->granary_policy);
        }


        /// Return the opcode of the instruction.
        inline unsigned op_code(void) const throw() {
            return instr->opcode;
        }


        /// Return the number of source operands in this instruction.
        inline unsigned num_sources(void) const throw() {
            return instr->num_srcs;
        }


        /// Return the number of destination operands in this instruction.
        inline unsigned num_destinations(void) const throw() {
            return instr->num_dsts;
        }


        /// Return true iff this instruction is a control-transfer
        /// instruction.
        inline bool is_cti(void) throw() {
            return dynamorio::instr_is_cti(instr);
        }


        /// Return true iff this instruction is a CALL instruction.
        inline bool is_call(void) throw() {
        	return dynamorio::instr_is_call(instr);
        }


        /// Return true iff this instruction is a RET instruction.
        inline bool is_return(void) throw() {
            return dynamorio::instr_is_return(instr);
        }


        /// Return true iff this instruction is a CALL instruction.
        inline bool is_jump(void) throw() {
            return dynamorio::instr_is_jmp(instr);
        }


        /// Return true iff this instruction is a CALL instruction.
        inline bool is_unconditional_cti(void) throw() {
            return is_cti() && !dynamorio::instr_is_cbr(instr);
        }


        /// Return true iff this instruction is a CALL instruction.
		inline bool is_direct_call(void) throw() {
			return dynamorio::instr_is_call_direct(instr);
		}


		/// Return true iff this instruction is a CALL instruction.
		inline bool is_indirect_call(void) throw() {
			return dynamorio::instr_is_call_indirect(instr);
		}


		/// Return true iff this instruction is atomic.
		inline bool is_atomic(void) throw() {
		    return instr->prefixes & PREFIX_LOCK;
		}


        /// Invalidate the raw bits of this instruction.
        inline void invalidate_raw_bits(void) throw() {
            dynamorio::instr_set_raw_bits_valid(instr, false);
            instr->flags &= ~dynamorio::INSTR_RAW_BITS_ALLOCATED;
            instr->flags &= ~dynamorio::INSTR_RAW_BITS_VALID;
            instr->bytes = nullptr;
        }


		/// If this instruction is a CTI, then return the operand
		/// representing the destination of the CTI.
		inline operand cti_target(void) throw() {
		    return dynamorio::instr_get_target(instr);
		}


		/// If this instruction is a CTI, then set the target of the instruction.
        inline void set_cti_target(operand target) throw() {
            invalidate_raw_bits();
            return dynamorio::instr_set_target(instr, target);
        }


        /// Return the original code program counter from the instruction (if
        /// it exists).
        inline app_pc pc(void) const throw() {
            return instr->translation;
        }


        /// Set the program counter of the instruction.
        inline void set_pc(app_pc pc_) throw() {
            invalidate_raw_bits();
            instr->translation = pc_;
        }


        /// Remove a specific granary flag.
        inline void remove_flag(instruction_flag flag) throw() {
            instr->granary_flags &= ~flag;
        }

        /// Remove a specific granary flag.
        inline void add_flag(instruction_flag flag) throw() {
            instr->granary_flags |= flag;
        }


        /// Return true iff this instruction begins a delay region.
        inline bool begins_delay_region(void) const throw() {
            return 0 != (DELAY_BEGIN & instr->granary_flags);
        }


        /// Return true iff this instruction ends a delay region.
        inline bool ends_delay_region(void) const throw() {
            return 0 != (DELAY_END & instr->granary_flags);
        }


        /// Return true iff this instruction is mangled.
        inline bool is_mangled(void) const throw() {
            return 0 != (DONT_MANGLE & instr->granary_flags);
        }


        /// Set the state of the instruction to be mangled.
        inline void set_mangled(void) throw() {
            add_flag(DONT_MANGLE);
        }


        /// Check to see if this instruction can be patched at runtime. If so,
        /// then this instruction needs to be aligned nicely.
        inline bool is_patchable(void) const throw() {
            return 0 != (dynamorio::INSTR_HOT_PATCHABLE & instr->flags);
        }


        /// Set the state of the instruction to be mangled.
        inline void set_patchable(void) throw() {
            instr->flags |= dynamorio::INSTR_HOT_PATCHABLE;
        }


        /// Returns the number of bytes needed to represent this instruction when
        /// it is encoded.
        inline unsigned encoded_size(void) throw() {
            return static_cast<unsigned>(
                dynamorio::instr_length(DCONTEXT, instr));
        }


        /// Widen this instruction if its a CTI.
        void widen_if_cti(void) throw();


        /// Decodes a raw byte, pointed to by *pc, and updated *pc to be the
        /// following byte. The decoded instruction is returned by value. If
        /// the instruction cannot be decoded, then *pc is set to NULL.
        static instruction decode(app_pc *pc) throw();


        /// Encodes an instruction into a sequence of bytes.
        app_pc encode(app_pc pc) throw();


        /// Encodes an instruction into a sequence of bytes, but where the staging
        /// ground is not necessarily the instruction's final location.
        app_pc stage_encode(app_pc staged_pc, app_pc final_pc) throw();


        /// Slightly evil convenience method for implicitly converting instructions
        /// to pointers to their underlying DR type.
        inline operator typename dynamorio::instr_t *(void) throw() {
        	return instr;
        }


        /// Apply a function to all of the destination operands and then all
        /// of the source operands.
        template <typename... Args>
        void for_each_operand(void (*func)(operand_ref, Args&...), Args&... args) throw() {
            if(instr->num_dsts) {
                for(int i(0); i < instr->num_dsts; ++i) {
                    func(operand_ref(instr, &(instr->u.o.dsts[i])), args...);
                }
            }

            if(instr->num_srcs) {
                func(operand_ref(instr, &(instr->u.o.src0)), args...);

                for(int i(0); i < (instr->num_srcs - 1); ++i) {
                    func(operand_ref(instr, &(instr->u.o.srcs[i])), args...);
                }
            }
        }
    };


    /// Mark this instruction as already mangled so that it is not mangled
    /// again.
    inline instruction mangled(instruction in) throw() {
        in.set_mangled();
        return in;
    }


    /// Mark this instruction as hot patchable.
    inline instruction patchable(instruction in) throw() {
        in.set_patchable();
        return in;
    }


    /// represents a generic list of T, where the properties of the list are
    /// configured by specializing list_meta<T>.
    struct instruction_list {
    public:

        typedef instruction_list self_type;

    protected:

        dynamorio::instr_t *first_;
        dynamorio::instr_t *last_;
        unsigned length_;

    public:

        /// Initialise an empty list.
        inline instruction_list(void) throw()
            : first_(nullptr)
            , last_(nullptr)
            , length_(0U)
        { }


        /// Move constructor.
        instruction_list(self_type &&that) throw();


        /// Returns the number of elements in the list.
        inline unsigned length(void) const throw() {
            return length_;
        }


        /// Clear the elements of the list, and release any memory associated
        /// with the elements of the list
        void clear(void) throw();


        /// Return the first element in the list.
        inline instruction first(void) const throw() {
            if(!first_) {
                return instruction(nullptr);
            }

            return instruction(first_);
        }

        /// Return the last element in the list.
        inline instruction last(void) const throw() {
            if(!last_) {
                return instruction(nullptr);
            }

            return instruction(last_);
        }

        /// Adds an element on to the end of the list.
        instruction append(instruction item_) throw();

        /// Adds an element on to the beginning of the list.
        instruction prepend(instruction item_) throw();

        /// Insert an element before another object in the list.
        instruction insert_before(instruction after_item_, instruction item_) throw();

        /// Remove an element from an instruction list.
        void remove(instruction to_remove) throw();

        /// Insert an element after another object in the list
        instruction insert_after(instruction before_item_, instruction item_) throw();

        /// The encoded size of the instruction list.
        unsigned encoded_size(void) throw();

        /// encodes an instruction list into a sequence of bytes
        app_pc encode(app_pc pc) throw();

        /// Performs a staged encoding of an instruction list into a sequence
        /// of bytes.
        ///
        /// Note: This will not do any fancy jump resolution, alignment, etc.
        app_pc stage_encode(app_pc staged_pc, app_pc final_pc) throw();

    protected:

        /// Chain an element into the list.
        instruction chain(
            dynamorio::instr_t *before_item,
            dynamorio::instr_t *item,
            dynamorio::instr_t *after_item
        ) throw();
    };


    /// registers
#define MAKE_REG(name, upper_name) extern operand name;
    namespace reg {
#   include "granary/inc/registers.h"
    }
#undef MAKE_REG
}

#include "granary/gen/instruction.h"

#endif /* granary_INSTRUCTION_H_ */
