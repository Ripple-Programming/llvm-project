// REQUIRES: hexagon-registered-target && (target=hexagon{{.*}} || target-x86_64 || target-aarch64)
// RUN: %clang -O2 -fenable-ripple %s -c -o - 2>&1 | FileCheck %s --implicit-check-not="error:" --implicit-check-not="UNREACHABLE"
// RUN: %clang -O2 --target=hexagon-unknown-elf -mv73 -mhvx -fenable-ripple %s -c -o - 2>&1 | FileCheck %s --implicit-check-not="error:" --implicit-check-not="UNREACHABLE"

// Test that ripple_parallel loops compile without crashing when static null
// pointers cause the UB optimizer to eliminate memory accesses before the
// Ripple pass runs.

#include "../ripple_test.h"

static int *in_int;
static int *out_int;
void test_copy_int() {
  ripple_block_t BS = ripple_set_block_shape(0, 32);
  ripple_parallel(BS, 0);
  for (int i = 0; i < 512; i++)
    out_int[i] = in_int[i];
}

static float *in_float;
static float *out_float;
void test_copy_float() {
  ripple_block_t BS = ripple_set_block_shape(0, 32);
  ripple_parallel(BS, 0);
  for (int i = 0; i < 512; i++)
    out_float[i] = in_float[i];
}

// CHECK-NOT: A vectorized instruction has a non-vectorized user
