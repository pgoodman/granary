/*
 * dlmain.cc
 *
 *  Created on: 2013-02-13
 *      Author: pag
 *     Version: $Id$
 */

//#include <cstdio>

#ifndef _GNU_SOURCE
#   define _GNU_SOURCE
#endif
#include <dlfcn.h>

#include "granary/globals.h"
#include "granary/policy.h"
#include "granary/code_cache.h"
#include "granary/basic_block.h"
#include "granary/attach.h"
#include "granary/detach.h"
#include "granary/perf.h"
#include "clients/instrument.h"

#include <unistd.h>
#include <signal.h>

void granary_signal_handler(int) {
    //std::printf("Run `sudo gdb attach %d`\n", getpid());
    for(;;) { /* loop until we manually attach gdb */ }
}

extern "C" {

    __attribute__((noinline, constructor, optimize("O0")))
    static void granary_begin_program(void) {
        auto policy(GRANARY_INIT_POLICY);
        //signal(SIGSEGV, granary_signal_handler);
        //signal(SIGILL, granary_signal_handler);
        granary::init();
        granary::attach(granary::policy_for<decltype(policy)>());
    }


#if CONFIG_ENABLE_PERF_COUNTS
    __attribute__((noinline, destructor, optimize("O0")))
    static void granary_end_program(void) {
        granary::perf::report();
    }

    GRANARY_DETACH_POINT(granary_end_program)
#endif

} /* extern C */

