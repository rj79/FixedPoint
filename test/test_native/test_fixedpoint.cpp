#include <unity.h>
#include "fixedpoint.h"

void test_int_to_fix()
{
    TEST_ASSERT_EQUAL(0, int2fix(0));
    TEST_ASSERT_EQUAL((int32_t)0x00010000, int2fix(1));
    TEST_ASSERT_EQUAL((int32_t)0xffff0000, int2fix(-1));    
    TEST_ASSERT_EQUAL((int32_t)0x7fff0000, int2fix(32767));
    TEST_ASSERT_EQUAL((int32_t)0x80000000, int2fix(-32768));
}

void test_float_to_fix()
{
    TEST_ASSERT_EQUAL(0, float2fix(0.0));
    TEST_ASSERT_EQUAL((int32_t)0x00010000, float2fix(1.0));
    TEST_ASSERT_EQUAL((int32_t)0xffff0000, float2fix(-1.0));
    TEST_ASSERT_EQUAL((int32_t)0x00008000, float2fix(0.5));
    TEST_ASSERT_EQUAL((int32_t)0xffff8000, float2fix(-0.5));

    // Smallest possible positive fraction
    TEST_ASSERT_EQUAL((int32_t)0x00000001, float2fix(0.0000152587890625));

    // Smallest possible negative fraction
    TEST_ASSERT_EQUAL((int32_t)0xffffffff, float2fix(-0.0000152587890625));
    
    // The precison of float seems to be less than 16.16 fixed roughly here
    TEST_ASSERT_EQUAL((int32_t)0x7fffff80, float2fix(32767.99902));

    // Maximum negative value
    TEST_ASSERT_EQUAL((int32_t)0x80000000, float2fix(-32768));
}

void test_fix_to_float()
{
    TEST_ASSERT_EQUAL(0.0, fix2float(0));
    TEST_ASSERT_EQUAL(1.0, fix2float(0x00010000));
    TEST_ASSERT_EQUAL(-1.0, fix2float(0xffff0000));
    TEST_ASSERT_EQUAL(0.5, fix2float(0x00008000));
    TEST_ASSERT_EQUAL(-0.5, fix2float(0xffff8000));

    // Maximum positive value
    TEST_ASSERT_EQUAL(32767, fix2float(0x7fff0000));
    // Maximum negative value
    TEST_ASSERT_EQUAL(-32768, fix2float(0x80000000));
    
    // The precison of float seems to be less than 16.16 fixed roughly here
    TEST_ASSERT_EQUAL(32767.99902, fix2float(0x7fffff80));
}

void test_fix_to_int()
{
    // 0 is 0
    TEST_ASSERT_EQUAL(0, fix2int(0));

    // Just below 0.5 rounds to 0
    TEST_ASSERT_EQUAL(0, fix2int(32767));
    // 0.5 rounds to 1
    TEST_ASSERT_EQUAL(1, fix2int(0x00008000));

    // 1 is 0
    TEST_ASSERT_EQUAL(1, fix2int(0x00010000));

    // Just below 1.5 rounds to 1
    TEST_ASSERT_EQUAL(1, fix2int(65536 + 32767));
    // Just at 1.5 rounds to 2
    TEST_ASSERT_EQUAL(2, fix2int(65536 + 32768));

    // -1 is -1
    TEST_ASSERT_EQUAL(-1, fix2int(0xffff0000));
    // Just at -0.5 rounds to 0
    TEST_ASSERT_EQUAL(0, fix2int(0xffff8000));
    // Just below -0.5 rounds to -1
    TEST_ASSERT_EQUAL(-1, fix2int(0xffff7fff));

    // Maximum integer value
    TEST_ASSERT_EQUAL(32767, fix2int(0x7fff0000));
    // Just below 32767.5 rounds to 35767
    TEST_ASSERT_EQUAL(32767, fix2int(0x7fff7fff));

    // Maximum integer value
    TEST_ASSERT_EQUAL(32767, fix2int(0x7fff0000));
    // Just below -32768.5 rounds to -35768
    TEST_ASSERT_EQUAL(-32768, fix2int(0x80007fff));
}

void test_shortfix_to_fix()
{
    // 1 (16 bit) == 1 (32 bit)   
    TEST_ASSERT_EQUAL((fixed)0x00010000, shortfix2fix(0x0100));
    // -1 (16 bit) == -1 (32 bit)   
    TEST_ASSERT_EQUAL((fixed)0xffff0000, shortfix2fix(0xff00));

    // 0.5 (16 bit) == 0.5 (32 bit)   
    TEST_ASSERT_EQUAL((fixed)0x00008000, shortfix2fix(0x0080));
    // 0.5 (16 bit) == 0.5 (32 bit)   
    TEST_ASSERT_EQUAL((fixed)0xffff8000, shortfix2fix(0xff80));
}

void test_fix_add()
{
    // 1 + 1 = 2
    TEST_ASSERT_EQUAL((int32_t)0x00020000, fix_add(0x00010000, 0x00010000));

    // 1.5 + 1.5 = 3
    TEST_ASSERT_EQUAL((int32_t)0x00030000, fix_add(0x00018000, 0x00018000));

    // 3 + (-4) = -1
    TEST_ASSERT_EQUAL((int32_t)0xffff0000, fix_add(0x00030000, 0xfffc0000));

    // 32767 + (-32767) = 0
    TEST_ASSERT_EQUAL((int32_t)0x00000000, fix_add(0x7fff0000, 0x80010000));
}

void test_fix_sub()
{
    // 1 - 1 = 0
    TEST_ASSERT_EQUAL((int32_t)0x00000000, fix_sub(0x00010000, 0x00010000));

    // 3 - 2 = 1
    TEST_ASSERT_EQUAL((int32_t)0x00010000, fix_sub(0x00030000, 0x00020000));

    // 3 - 4 = -1
    TEST_ASSERT_EQUAL((int32_t)0xffff0000, fix_sub(0x00030000, 0x00040000));

    // 3 - 2.5 = 0.5
    TEST_ASSERT_EQUAL((int32_t)0x00008000, fix_sub(0x00030000, 0x00028000));
}

void test_fix_mul()
{
    // 1 * 1 = 1
    TEST_ASSERT_EQUAL((int32_t)0x00010000, fix_mul(0x00010000, 0x00010000));

    // 1 * -1 = -1
    TEST_ASSERT_EQUAL((int32_t)0xffff0000, fix_mul(0x00010000, 0xffff0000));

    // 1 * 32767 = 32767
    TEST_ASSERT_EQUAL((int32_t)0x7fff0000, fix_mul(0x00010000, 0x7fff0000));

    // 1 * 0.125 = 0.125
    TEST_ASSERT_EQUAL((int32_t)0x00002000, fix_mul(0x00010000, 0x00002000));
}

void test_fix_div()
{
    // 1 / 1 = 1
    TEST_ASSERT_EQUAL((int32_t)0x00010000, fix_div(0x00010000, 0x00010000));

    // 1 / -1 = -1
    TEST_ASSERT_EQUAL((int32_t)0xffff0000, fix_div(0x00010000, 0xffff0000));

    // 1 / 32767
    TEST_ASSERT_EQUAL((int32_t)0x00000002, fix_div(0x00010000, 0x7fff0000));

    // 1 / -32768
    TEST_ASSERT_EQUAL((int32_t)0xfffffffe, fix_div(0x00010000, 0x80000000));
}

void test_print_fix() {
    char buffer[32] = {0};
    
    snprintfix(buffer, sizeof(buffer), 0x00000000);
    TEST_ASSERT_EQUAL_STRING("0.0000", buffer);

    snprintfix(buffer, sizeof(buffer), 0x00010000);
    TEST_ASSERT_EQUAL_STRING("1.0000", buffer);

    snprintfix(buffer, sizeof(buffer), 0xffff0000);
    TEST_ASSERT_EQUAL_STRING("-1.0000", buffer);

    snprintfix(buffer, sizeof(buffer), 0x7fff8000);
    TEST_ASSERT_EQUAL_STRING("32767.5000", buffer);

    snprintfix(buffer, sizeof(buffer), 0x80000000);
    TEST_ASSERT_EQUAL_STRING("-32768.0000", buffer);

    snprintfix(buffer, sizeof(buffer), 0x000119a0);
    TEST_ASSERT_EQUAL_STRING("1.1000", buffer);
}

int runUnityTests(void) 
{
  UNITY_BEGIN();
  RUN_TEST(test_int_to_fix);
  RUN_TEST(test_float_to_fix);
  RUN_TEST(test_fix_to_float);
  RUN_TEST(test_fix_to_int);
  RUN_TEST(test_shortfix_to_fix);
  RUN_TEST(test_fix_add);
  RUN_TEST(test_fix_sub);
  RUN_TEST(test_fix_mul);
  RUN_TEST(test_fix_div);
  RUN_TEST(test_print_fix);
  return UNITY_END();
}

int main(void) 
{
  return runUnityTests();
}
