/* Copyright 2012-2013 Peter Goodman, all rights reserved. */
/*
 * bound_wrappers.h
 *
 *  Created on: 2013-05-07
 *      Author: Peter Goodman
 */

#ifndef WRAP_CONTEXT
#   define WRAP_CONTEXT APP
#endif


#ifdef CLIENT_WATCHPOINT_BOUND
#   include "clients/watchpoints/clients/bounds_checker/instrument.h"
#endif

#ifdef CLIENT_WATCHPOINT_LEAK
#   include "clients/watchpoints/instrument.h"
#   include "clients/watchpoints/clients/leak_detector/descriptor.h"
#endif

using namespace client::wp;


#if defined(CAN_WRAP___kmalloc) && CAN_WRAP___kmalloc
#   define APP_WRAPPER_FOR___kmalloc
    FUNCTION_WRAPPER(WRAP_CONTEXT, __kmalloc, (void *), (size_t size, gfp_t gfp), {
        void *ret_address(__builtin_return_address(0));

        WRAP_FUNCTION_ONCE(__kmalloc);
        void *ret(__kmalloc(size, gfp));
        if(is_valid_address(ret)) {
            add_watchpoint(ret, ret, size, ret_address);
        }
        return ret;
    })
#endif



#if defined(CAN_WRAP___kmalloc_track_caller) && CAN_WRAP___kmalloc_track_caller
#   define APP_WRAPPER_FOR___kmalloc_track_caller
    FUNCTION_WRAPPER(WRAP_CONTEXT, __kmalloc_track_caller, (void *), (size_t size, gfp_t gfp, unsigned long caller), {
        void *ret_address(__builtin_return_address(0));

        WRAP_FUNCTION_ONCE(__kmalloc_track_caller);
        void *ret(__kmalloc_track_caller(size, gfp, caller));
        if(is_valid_address(ret)) {
            add_watchpoint(ret, ret, size, ret_address);
        }
        return ret;
    })
#endif


#if defined(CAN_WRAP___kmalloc_node) && CAN_WRAP___kmalloc_node
#   define APP_WRAPPER_FOR___kmalloc_node
    FUNCTION_WRAPPER(WRAP_CONTEXT, __kmalloc_node, (void *), (size_t size, gfp_t gfp, int node), {
        void *ret_address(__builtin_return_address(0));

        WRAP_FUNCTION_ONCE(__kmalloc_node);
        void *ret(__kmalloc_node(size, gfp, node));
        if(is_valid_address(ret)) {
            add_watchpoint(ret, ret, size, ret_address);
        }
        return ret;
    })
#endif


#if defined(CAN_WRAP___kmalloc_node_track_caller) && CAN_WRAP___kmalloc_node_track_caller
#   define APP_WRAPPER_FOR___kmalloc_node_track_caller
    FUNCTION_WRAPPER(WRAP_CONTEXT, __kmalloc_node_track_caller, (void *), (size_t size, gfp_t gfp, int node, unsigned long caller), {
        void *ret_address(__builtin_return_address(0));

        WRAP_FUNCTION_ONCE(__kmalloc_node_track_caller);
        void *ret(__kmalloc_node_track_caller(size, gfp, node, caller));
        if(is_valid_address(ret)) {
            add_watchpoint(ret, ret, size, ret_address);
        }
        return ret;
    })
#endif


#if defined(CAN_WRAP___krealloc) && CAN_WRAP___krealloc
#   ifndef APP_WRAPPER_FOR___krealloc
#       define APP_WRAPPER_FOR___krealloc
    FUNCTION_WRAPPER(WRAP_CONTEXT, __krealloc, (void *), (const void * ptr, size_t size, gfp_t gfp), {
        void *ret_address(__builtin_return_address(0));

        if(is_watched_address(ptr)) {
            free_descriptor_of(ptr);
            ptr = unwatched_address(ptr);
        }

        WRAP_FUNCTION_ONCE(__krealloc);
        void *ret(__krealloc(ptr, size, gfp));
        add_watchpoint(ret, ret, size, ret_address);
        return ret;
    })
#   endif
#endif


#if defined(CAN_WRAP_krealloc) && CAN_WRAP_krealloc
#   ifndef APP_WRAPPER_FOR_krealloc
#       define APP_WRAPPER_FOR_krealloc
    FUNCTION_WRAPPER(WRAP_CONTEXT, krealloc, (void *), (const void * ptr, size_t size, gfp_t gfp), {
        void *ret_address(__builtin_return_address(0));

        if(is_watched_address(ptr)) {
            free_descriptor_of(ptr);
            ptr = unwatched_address(ptr);
        }

        WRAP_FUNCTION_ONCE(krealloc);
        void *ret(krealloc(ptr, size, gfp));
        add_watchpoint(ret, ret, size, ret_address);
        return ret;
    })
#   endif
#endif


#if defined(CAN_WRAP_kfree) && CAN_WRAP_kfree
#   define APP_WRAPPER_FOR_kfree
    FUNCTION_WRAPPER_VOID(WRAP_CONTEXT, kfree, (const void *ptr), {
        if(is_watched_address(ptr)) {
            free_descriptor_of(ptr);
            ptr = unwatched_address(ptr);
        }

        WRAP_FUNCTION_ONCE(kfree);
        return kfree(ptr);
    })
#endif


#if defined(CAN_WRAP_kzfree) && CAN_WRAP_kzfree
#   define APP_WRAPPER_FOR_kzfree
    FUNCTION_WRAPPER_VOID(WRAP_CONTEXT, kzfree, (const void *ptr), {
        if(is_watched_address(ptr)) {
            free_descriptor_of(ptr);
            ptr = unwatched_address(ptr);
        }

        WRAP_FUNCTION_ONCE(kzfree);
        return kzfree(ptr);
    })
#endif


#if defined(CAN_WRAP_kmem_cache_alloc) && CAN_WRAP_kmem_cache_alloc
#   define APP_WRAPPER_FOR_kmem_cache_alloc
    FUNCTION_WRAPPER(WRAP_CONTEXT, kmem_cache_alloc, (void *), (struct kmem_cache *cache, gfp_t gfp), {
        void *ret_address(__builtin_return_address(0));

        PRE_OUT_WRAP(cache);
        WRAP_FUNCTION_ONCE(kmem_cache_alloc);
        void *ptr(kmem_cache_alloc(cache, gfp));
        if(!ptr) {
            return ptr;
        }

        struct kmem_cache *unwatched_cache(unwatched_address(cache));

        // Add watchpoint before constructor so that internal pointers
        // maintain their invariants (e.g. list_head structures).
        memset(ptr, 0, unwatched_cache->object_size);
        add_watchpoint(ptr, ptr, unwatched_cache->object_size, ret_address);
        if(is_valid_address(unwatched_cache->ctor)) {
            unwatched_cache->ctor(ptr);
        }

        return ptr;
    })
#endif


#if defined(CAN_WRAP_kmem_cache_alloc_trace) && CAN_WRAP_kmem_cache_alloc_trace
#   define APP_WRAPPER_FOR_kmem_cache_alloc_trace
    FUNCTION_WRAPPER(WRAP_CONTEXT, kmem_cache_alloc_trace, (void *), (struct kmem_cache *cache, gfp_t gfp, size_t size), {
        void *ret_address(__builtin_return_address(0));

        PRE_OUT_WRAP(cache);

        WRAP_FUNCTION_ONCE(kmem_cache_alloc_trace);
        void *ptr(kmem_cache_alloc_trace(cache, gfp, size));
        if(!ptr) {
            return ptr;
        }

        struct kmem_cache *unwatched_cache(unwatched_address(cache));

        // Add watchpoint before constructor so that internal pointers
        // maintain their invariants (e.g. list_head structures).
        memset(ptr, 0, size, ret_address);
        add_watchpoint(ptr, ptr, size, ret_address);
        if(is_valid_address(unwatched_cache->ctor)) {
            unwatched_cache->ctor(ptr);
        }

        return ptr;
    })
#endif


#if defined(CAN_WRAP_kmem_cache_alloc_node) && CAN_WRAP_kmem_cache_alloc_node
#   define APP_WRAPPER_FOR_kmem_cache_alloc_node
    FUNCTION_WRAPPER(WRAP_CONTEXT, kmem_cache_alloc_node, (void *), (struct kmem_cache *cache, gfp_t gfp, int node), {
        void *ret_address(__builtin_return_address(0));

        PRE_OUT_WRAP(cache);

        WRAP_FUNCTION_ONCE(kmem_cache_alloc_node);
        void *ptr(kmem_cache_alloc_node(cache, gfp, node));
        if(!ptr) {
            return ptr;
        }

        struct kmem_cache *unwatched_cache(unwatched_address(cache));

        // Add watchpoint before constructor so that internal pointers
        // maintain their invariants (e.g. list_head structures).
        memset(ptr, 0, unwatched_cache->object_size);
        add_watchpoint(ptr, ptr, unwatched_cache->object_size, ret_address);
        if(is_valid_address(unwatched_cache->ctor)) {
            unwatched_cache->ctor(ptr);
        }

        return ptr;
    })
#endif


#if defined(CAN_WRAP_kmem_cache_free) && CAN_WRAP_kmem_cache_free
#   define APP_WRAPPER_FOR_kmem_cache_free
    FUNCTION_WRAPPER(WRAP_CONTEXT, kmem_cache_free, (void), (struct kmem_cache *cache, void *ptr), {
        if(ptr && is_watched_address(ptr)) {
            free_descriptor_of(ptr);
            ptr = unwatched_address(ptr);
        }

        PRE_OUT_WRAP(cache);
        WRAP_FUNCTION_ONCE(kmem_cache_free);
        kmem_cache_free(cache, ptr);
    })
#endif


#if defined(CAN_WRAP___get_free_pages) && CAN_WRAP___get_free_pages
#   ifndef APP_WRAPPER_FOR___get_free_pages
#       define APP_WRAPPER_FOR___get_free_pages
    FUNCTION_WRAPPER(WRAP_CONTEXT, __get_free_pages, (unsigned long), ( gfp_t gfp_mask , unsigned int order ), {
        void *ret_address(__builtin_return_address(0));

        WRAP_FUNCTION_ONCE(__get_free_pages);
        unsigned long ret(__get_free_pages(gfp_mask, order));
        if(is_valid_address(ret)) {
            add_watchpoint(
                ret, reinterpret_cast<void *>(ret),
                (1 << order) * PAGE_SIZE, ret_address);
        }
        return ret;
    })
#   endif
#endif


#if defined(CAN_WRAP_get_zeroed_page) && CAN_WRAP_get_zeroed_page
#   ifndef APP_WRAPPER_FOR_get_zeroed_page
#       define APP_WRAPPER_FOR_get_zeroed_page
    FUNCTION_WRAPPER(WRAP_CONTEXT, get_zeroed_page, (unsigned long), ( gfp_t gfp_mask ), {
        void *ret_address(__builtin_return_address(0));

        WRAP_FUNCTION_ONCE(get_zeroed_page);
        unsigned long ret(get_zeroed_page(gfp_mask));
        if(is_valid_address(ret)) {
            add_watchpoint(
                ret, reinterpret_cast<void *>(ret), PAGE_SIZE, ret_address);
        }
        return ret;
    })
#   endif
#endif


#if defined(CAN_WRAP_alloc_pages_exact) && CAN_WRAP_alloc_pages_exact
#   ifndef APP_WRAPPER_FOR_alloc_pages_exact
#       define APP_WRAPPER_FOR_alloc_pages_exact
    FUNCTION_WRAPPER(WRAP_CONTEXT, alloc_pages_exact, (void *), ( size_t size, gfp_t gfp_mask ), {
        void *ret_address(__builtin_return_address(0));

        WRAP_FUNCTION_ONCE(alloc_pages_exact);
        void *ret(alloc_pages_exact(size, gfp_mask));
        if(is_valid_address(ret)) {
            add_watchpoint(ret, ret, size, ret_address);
        }
        return ret;
    })
#   endif
#endif


#if defined(CAN_WRAP_free_pages_exact) && CAN_WRAP_free_pages_exact
#   ifndef APP_WRAPPER_FOR_free_pages_exact
#       define APP_WRAPPER_FOR_free_pages_exact
    FUNCTION_WRAPPER_VOID(WRAP_CONTEXT, free_pages_exact, ( void *virt, size_t size ), {
        if(is_watched_address(virt)) {
            free_descriptor_of(virt);
            virt = unwatched_address(virt);
        }

        WRAP_FUNCTION_ONCE(free_pages_exact);
        free_pages_exact(virt, size);
    })
#   endif
#endif


#if defined(CAN_WRAP_alloc_pages_exact_nid) && CAN_WRAP_alloc_pages_exact_nid
#   ifndef APP_WRAPPER_FOR_alloc_pages_exact_nid
#       define APP_WRAPPER_FOR_alloc_pages_exact_nid
    FUNCTION_WRAPPER(WRAP_CONTEXT, alloc_pages_exact_nid, (void *), ( int node, size_t size, gfp_t gfp_mask ), {
        void *ret_address(__builtin_return_address(0));

        WRAP_FUNCTION_ONCE(alloc_pages_exact_nid);
        void *ret(alloc_pages_exact_nid(node, size, gfp_mask));
        if(is_valid_address(ret)) {
            add_watchpoint(ret, ret, size, ret_address);
        }
        return ret;
    })
#   endif
#endif


#if defined(CAN_WRAP_free_pages) && CAN_WRAP_free_pages
#   ifndef APP_WRAPPER_FOR_free_pages
#       define APP_WRAPPER_FOR_free_pages
    FUNCTION_WRAPPER_VOID(WRAP_CONTEXT, free_pages, ( unsigned long addr, unsigned int order ), {
        if(is_watched_address(addr)) {
            free_descriptor_of(addr);
            addr = unwatched_address(addr);
        }

        WRAP_FUNCTION_ONCE(free_pages);
        free_pages(addr, order);
    })
#   endif
#endif


#if defined(CAN_WRAP_free_memcg_kmem_pages) && CAN_WRAP_free_memcg_kmem_pages
#   ifndef APP_WRAPPER_FOR_free_memcg_kmem_pages
#       define APP_WRAPPER_FOR_free_memcg_kmem_pages
    FUNCTION_WRAPPER_VOID(WRAP_CONTEXT, free_memcg_kmem_pages, ( unsigned long addr, unsigned int order ), {
        if(is_watched_address(addr)) {
            free_descriptor_of(addr);
            addr = unwatched_address(addr);
        }

        WRAP_FUNCTION_ONCE(free_memcg_kmem_pages);
        free_memcg_kmem_pages(addr, order);
    })
#   endif
#endif

