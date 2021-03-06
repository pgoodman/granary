/* Copyright 2012-2013 Peter Goodman, all rights reserved. */
/*
 * instrument.h
 *
 *  Created on: 2013-06-28
 *      Author: Peter Goodman
 */

#ifndef CFG_INSTRUMENT_H_
#define CFG_INSTRUMENT_H_

#include "granary/client.h"

#ifndef GRANARY_INIT_POLICY
#   define GRANARY_INIT_POLICY (client::cfg_policy())
#endif


namespace client {


    /// Used to instrument all non-entry basic blocks.
    struct cfg_policy : public granary::instrumentation_policy {
    public:

        enum {
            AUTO_INSTRUMENT_HOST = true
        };

        granary::instrumentation_policy visit_app_instructions(
            granary::cpu_state_handle,
            granary::basic_block_state &,
            granary::instruction_list &
        ) ;

        granary::instrumentation_policy visit_host_instructions(
            granary::cpu_state_handle,
            granary::basic_block_state &,
            granary::instruction_list &
        ) ;

#if CONFIG_FEATURE_CLIENT_HANDLE_INTERRUPT
        granary::interrupt_handled_state handle_interrupt(
            granary::cpu_state_handle,
            granary::thread_state_handle,
            granary::basic_block_state &,
            granary::interrupt_stack_frame &,
            granary::interrupt_vector
        ) ;
#endif /* CONFIG_FEATURE_CLIENT_HANDLE_INTERRUPT */
    };


#if CONFIG_FEATURE_CLIENT_HANDLE_INTERRUPT
    /// Handle an interrupt in kernel code. Returns true iff the client handles
    /// the interrupt.
    granary::interrupt_handled_state handle_kernel_interrupt(
        granary::cpu_state_handle,
        granary::thread_state_handle,
        granary::interrupt_stack_frame &,
        granary::interrupt_vector
    ) ;
#endif
}


#endif /* CFG_INSTRUMENT_H_ */
