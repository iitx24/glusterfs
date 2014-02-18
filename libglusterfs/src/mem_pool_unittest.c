/*
  Copyright (c) 2008-2014 Red Hat, Inc. <http://www.redhat.com>
  This file is part of GlusterFS.

  This file is licensed to you under your choice of the GNU Lesser
  General Public License, version 3 or any later version (LGPLv3 or
  later), or the GNU General Public License, version 2 (GPLv2), in all
  cases as published by the Free Software Foundation.
*/

#include "mem-pool.h"
#include "logging.h"
#include "xlator.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <inttypes.h>
#include <cmockery/pbc.h>
#include <cmockery/cmockery.h>

/*
 * From mem-pool.c
 */
#define GF_MEM_HEADER_SIZE  (4 + sizeof (size_t) + sizeof (xlator_t *) + 4 + 8)
#define GF_MEM_TRAILER_SIZE 8

#define GF_MEM_HEADER_MAGIC  0xCAFEBABE
#define GF_MEM_TRAILER_MAGIC 0xBAADF00D

/*
 * Prototypes to private functions
 */
int
gf_mem_set_acct_info (xlator_t *xl, char **alloc_ptr,
                      size_t size, uint32_t type);

/*
 * Helper functions
 */
static xlator_t *
helper_xlator_init(uint32_t num_types)
{
    xlator_t *xl;
    int i, ret;

    REQUIRE(num_types > 0);

    xl = test_calloc(1, sizeof(xlator_t));
    assert_non_null(xl);
    xl->mem_acct.num_types = num_types;
    xl->mem_acct.rec = test_calloc(num_types, sizeof(struct mem_acct_rec));
    assert_non_null(xl->mem_acct.rec);

    for (i = 0; i < num_types; i++) {
            ret = LOCK_INIT(&(xl->mem_acct.rec[i].lock));
            assert_false(ret);
    }

    ENSURE(num_types == xl->mem_acct.num_types);
    ENSURE(NULL != xl);

    return xl;
}

static int
helper_xlator_destroy(xlator_t *xl)
{
    free(xl->mem_acct.rec);
    free(xl);
    return 0;
}

/*
 * Tests
 */
static void
test_gf_mem_acct_enable_set(void **state) 
{
    (void) state;
    glusterfs_ctx_t test_ctx;

    expect_assert_failure(gf_mem_acct_enable_set(NULL));

    memset(&test_ctx, 0, sizeof(test_ctx));
    assert_true(NULL == test_ctx.process_uuid);
    gf_mem_acct_enable_set((void *)&test_ctx);
    assert_true(1 == test_ctx.mem_acct_enable);
    assert_true(NULL == test_ctx.process_uuid);
}

static void 
test_gf_mem_set_acct_info_asserts(void **state)
{
    xlator_t *xl;
    char *alloc_ptr;
    size_t size;
    uint32_t type;

    xl = (xlator_t *)0xBADD;
    alloc_ptr = (char *)0xBADD;
    size = 8196;
    type = 0;

    // Check xl is NULL
    expect_assert_failure(gf_mem_set_acct_info(NULL, &alloc_ptr, size, type));
    // Check size is 0
    expect_assert_failure(gf_mem_set_acct_info(xl, &alloc_ptr, 0, 0)); 
    // Check alloc is NULL
    assert_int_equal(-1, gf_mem_set_acct_info(xl, NULL, size, type));

    // Initialize xl
    xl = helper_xlator_init(10);

    // Test number of types
    type = 100;
    assert_true(NULL != xl->mem_acct.rec);
    assert_true(type > xl->mem_acct.num_types);
    expect_assert_failure(gf_mem_set_acct_info(xl, &alloc_ptr, size, type)); 

    helper_xlator_destroy(xl);
}

static void 
test_gf_mem_set_acct_info_memory(void **state)
{
    xlator_t *xl;
    char *alloc_ptr;
    char *temp_ptr;
    size_t size;
    uint32_t type;

    typedef struct __attribute__((packed)) {
        uint32_t type;
        size_t size;
        xlator_t *xl;
        uint32_t header_magic;
        uint8_t pad[8];
    } mem_header_t;

    size = 8196;
    type = 9;

    // Initialize xl
    xl = helper_xlator_init(10);

    // Test allocation
    temp_ptr = test_calloc(1, size + GF_MEM_HEADER_SIZE + GF_MEM_TRAILER_SIZE);
    assert_non_null(temp_ptr);
    alloc_ptr = temp_ptr;
    gf_mem_set_acct_info(xl, &alloc_ptr, size, type);

    //Check values
    assert_int_equal(xl->mem_acct.rec[type].size, size);
    assert_int_equal(xl->mem_acct.rec[type].num_allocs, 1);
    assert_int_equal(xl->mem_acct.rec[type].total_allocs, 1);
    assert_int_equal(xl->mem_acct.rec[type].max_size, size);
    assert_int_equal(xl->mem_acct.rec[type].max_num_allocs, 1);

    // Check memory
    {
        mem_header_t *p;

        p = (mem_header_t *)temp_ptr;
        assert_int_equal(p->type, type);
        assert_int_equal(p->size, size);
        assert_true(p->xl == xl);
        assert_int_equal(p->header_magic, GF_MEM_HEADER_MAGIC); 
        assert_true(*(uint32_t *)(temp_ptr+sizeof(mem_header_t)+size) == GF_MEM_TRAILER_MAGIC);

        // Check that alloc_ptr has been moved correctly
        // by gf_mem_set_acct_info
        p++;
        p->type = 1234;
        assert_int_equal(*(uint32_t *)alloc_ptr, p->type);
    }


    free(temp_ptr);
    helper_xlator_destroy(xl);
}

int main(void) {
    const UnitTest tests[] = {
        unit_test(test_gf_mem_acct_enable_set),
        unit_test(test_gf_mem_set_acct_info_asserts),
        unit_test(test_gf_mem_set_acct_info_memory),
    };

    return run_tests(tests, "libglusterfs_mem_pool");
}
