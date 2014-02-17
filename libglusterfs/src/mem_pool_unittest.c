
#include "mem-pool.h"
#include "logging.h"
#include "xlator.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <inttypes.h>
#include <cmockery/cmockery.h>

// A test case that does nothing and succeeds.
static void test_gf_mem_acct_enable_set(void **state) {
    (void) state;
    glusterfs_ctx_t test_ctx;

    expect_assert_failure(gf_mem_acct_enable_set(NULL));

    memset(&test_ctx, 0, sizeof(test_ctx));
    assert_true(NULL == test_ctx.process_uuid);
    gf_mem_acct_enable_set((void *)&test_ctx);
    assert_true(1 == test_ctx.mem_acct_enable);
    assert_true(NULL == test_ctx.process_uuid);


}

int main(void) {
    const UnitTest tests[] = {
        unit_test(test_gf_mem_acct_enable_set),
    };

    return run_tests(tests, "libglusterfs_mem_pool");
}
