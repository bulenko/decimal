// "Copyright [year] <Copyright Owner>"
#include "s21_decimal.h"

#include <check.h>
#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef TEST_NAME_all
#define TEST_NAME_add
#define TEST_NAME_sub
#define TEST_NAME_mul
#define TEST_NAME_div
#define TEST_NAME_mod
#define TEST_NAME_less
#define TEST_NAME_less_or_equal
#define TEST_NAME_greater
#define TEST_NAME_greater_or_equal
#define TEST_NAME_equal
#define TEST_NAME_not_equal
#define TEST_NAME_int_to_dec
#define TEST_NAME_dec_to_int
#define TEST_NAME_float_to_dec
#define TEST_NAME_dec_to_float
#define TEST_NAME_negate
#define TEST_NAME_truncate
#define TEST_NAME_floor
#define TEST_NAME_round
#endif

s21_decimal decMax = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}},
            decMin = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
            decZero = {{0, 0, 0, 0}};
void equal_bits(s21_decimal dec_res, s21_decimal result);

#ifdef TEST_NAME_int_to_dec
START_TEST(s21_int_to_dec_test_1) {
  s21_decimal dst;
  int src = 1;
  s21_from_int_to_decimal(src, &dst);
  ck_assert_uint_eq(1, dst.bits[0]);
  ck_assert_uint_eq(0, dst.bits[3]);
}
END_TEST

START_TEST(s21_int_to_dec_test_2) {
  s21_decimal dst;
  int src = -1;
  s21_from_int_to_decimal(src, &dst);
  unsigned b3 = dst.bits[3];
  ck_assert_uint_eq(1, dst.bits[0]);
  ck_assert_uint_eq(1, b3 >> 31);
}
END_TEST

START_TEST(s21_int_to_dec_test_3) {
  s21_decimal dst;
  int src = -10;
  s21_from_int_to_decimal(src, &dst);
  unsigned b3 = dst.bits[3];
  ck_assert_uint_eq(10, dst.bits[0]);
  ck_assert_uint_eq(1, b3 >> 31);
}
END_TEST

START_TEST(s21_int_to_dec_test_4) {
  s21_decimal src = {{10, 0, 0, 0}};
  src = div_10(src);
  ck_assert_int_eq(1, src.bits[0]);
}
END_TEST
#endif  // END TESTS FOR int_to_dec

#ifdef TEST_NAME_dec_to_int
START_TEST(s21_dec_to_int_test_1) {
  s21_decimal src = {{10, 0, 0, 0}};
  int dst;

  int s21_res = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(10, dst);
  ck_assert_int_eq(0, s21_res);
}
END_TEST

START_TEST(s21_dec_to_int_test_2) {
  s21_decimal src = {{10, 0, 0, 0x80000000}};
  int dst;

  int s21_res = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(-10, dst);
  ck_assert_int_eq(0, s21_res);
}
END_TEST

START_TEST(s21_dec_to_int_test_3) {
  s21_decimal src = {{10, 0, 0, 0x80010000}};
  int dst;
  int s21_res = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(-1, dst);
  ck_assert_int_eq(0, s21_res);
}
END_TEST

START_TEST(s21_dec_to_int_test_4) {
  s21_decimal src = {{100, 0, 0, 0x80020000}};
  int dst;
  int s21_res = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(-1, dst);
  ck_assert_int_eq(0, s21_res);
}
END_TEST

START_TEST(s21_dec_to_int_test_5) {
  s21_decimal src = decMin;
  int dst;
  int s21_res = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(1, s21_res);
}
END_TEST

START_TEST(s21_dec_to_int_test_6) {
  s21_decimal src = decMax;
  int dst;
  int s21_res = s21_from_decimal_to_int(src, &dst);
  ck_assert_int_eq(1, s21_res);
}
END_TEST
#endif  // END TESTS FOR s21_dec_to_int

#ifdef TEST_NAME_negate
START_TEST(s21_negate_1) {
  s21_decimal value = {{10, 0, 0, 0}};
  s21_decimal result;
  s21_negate(value, &result);
  unsigned b3 = result.bits[3];
  ck_assert_uint_eq(1, b3 >> 31);
}
END_TEST

START_TEST(s21_negate_2) {
  s21_decimal value = {{10, 0, 0, 0x80020000}};
  s21_decimal result;
  s21_negate(value, &result);
  unsigned int b3 = result.bits[3];
  ck_assert_uint_eq(0, b3 >> 31);
  ck_assert_uint_eq(2, b3 >> 16);
}
END_TEST

START_TEST(s21_negate_3) {
  s21_decimal value = {{10, 0, 0, 0x800F0000}};
  s21_decimal result;
  s21_negate(value, &result);
  unsigned int b3 = result.bits[3];
  ck_assert_uint_eq(0, b3 >> 31);
  ck_assert_uint_eq(b3, 0xF0000);
}
END_TEST
#endif  // END TESTS FOR s21_negate

#ifdef TEST_NAME_truncate
START_TEST(s21_truncate_test_1) {
  s21_decimal value = {{10, 0, 0, 0}};
  s21_decimal result;
  s21_truncate(value, &result);
  ck_assert_int_eq(10, result.bits[0]);
}
END_TEST

START_TEST(s21_truncate_test_2) {
  s21_decimal value = {{150, 0, 0, 0x80020000}};
  s21_decimal result;
  s21_truncate(value, &result);
  unsigned b3 = result.bits[3];
  ck_assert_uint_eq(1, result.bits[0]);
  ck_assert_uint_eq(1, b3 >> 31);
  ck_assert_uint_eq(b3, 0x80000000);
}
END_TEST

START_TEST(s21_truncate_test_3) {
  s21_decimal value = {{5150, 0, 0, 0x80020000}};
  s21_decimal result;
  s21_truncate(value, &result);
  unsigned b3 = result.bits[3];
  ck_assert_int_eq(51, result.bits[0]);
  ck_assert_uint_eq(b3, 0x80000000);
}
END_TEST
#endif  // END TESTS FOR s21_truncate

#ifdef TEST_NAME_floor
START_TEST(s21_floor_t1) {
  s21_decimal value = {{0x00450474, 0, 0, 0x00030000}};  // 4523.124
  s21_decimal s21_result;
  s21_decimal res = {{4523, 0, 0, 0}};
  int res_comp = s21_floor(value, &s21_result);
  ck_assert_int_eq(0, res_comp);
  res_comp = s21_is_equal(s21_result, res);
  ck_assert_int_eq(1, res_comp);
}
END_TEST

START_TEST(s21_floor_t2) {
  s21_decimal value = {{0x0000B0B2, 0, 0, 0x00010000}};  // 4523.4
  s21_decimal s21_result;
  s21_decimal res = {{4523, 0, 0, 0}};
  int res_comp = s21_floor(value, &s21_result);
  ck_assert_int_eq(0, res_comp);
  res_comp = s21_is_equal(s21_result, res);
  ck_assert_int_eq(1, res_comp);
}
END_TEST

START_TEST(s21_floor_t3) {
  s21_decimal value = {{0x0006E70A, 0, 0, 0x00020000}};  // 4523.62
  s21_decimal s21_result;
  s21_decimal res = {{4523, 0, 0, 0}};
  int res_comp = s21_floor(value, &s21_result);
  ck_assert_int_eq(0, res_comp);
  res_comp = s21_is_equal(s21_result, res);
  ck_assert_int_eq(1, res_comp);
}
END_TEST

START_TEST(s21_floor_t4) {
  s21_decimal value = {{0x00006420D, 0, 0, 0x80040000}};  // -41.0125
  s21_decimal s21_result;
  s21_decimal res = {{42, 0, 0, 0x80000000}};
  int res_comp = s21_floor(value, &s21_result);
  ck_assert_int_eq(0, res_comp);
  res_comp = s21_is_equal(s21_result, res);
  ck_assert_int_eq(1, res_comp);
}
END_TEST

START_TEST(s21_floor_t5) {
  s21_decimal value = {{0x00001052, 0, 0, 0x80020000}};  // -41.78
  s21_decimal s21_result;
  s21_decimal res = {{42, 0, 0, 0x80000000}};
  int res_comp = s21_floor(value, &s21_result);
  ck_assert_int_eq(0, res_comp);
  res_comp = s21_is_equal(s21_result, res);
  ck_assert_int_eq(1, res_comp);
}
END_TEST

START_TEST(s21_floor_t6) {
  s21_decimal value = {{0xFFFFFC95, 0xFFFFFFFF, 0x00007CF,
                        0x00030000}};  // 36893488147419103231.125
  s21_decimal s21_result;
  s21_decimal res = {{0xFFFFFFFF, 0xFFFFFFFF, 0x00000001, 0x00000000}};
  int res_comp = s21_floor(value, &s21_result);
  ck_assert_int_eq(0, res_comp);
  res_comp = s21_is_equal(s21_result, res);
  ck_assert_int_eq(1, res_comp);
}
END_TEST

START_TEST(s21_floor_t7) {
  s21_decimal value = {{456592, 0, 0, 0}};
  s21_decimal s21_result;
  s21_decimal res = {{456592, 0, 0, 0}};
  int res_comp = s21_floor(value, &s21_result);
  ck_assert_int_eq(0, res_comp);
  res_comp = s21_is_equal(s21_result, res);
  ck_assert_int_eq(1, res_comp);
}
END_TEST

START_TEST(s21_floor_t8) {
  s21_decimal s21_result;
  int res_comp = s21_floor(decZero, &s21_result);
  ck_assert_int_eq(0, res_comp);
  res_comp = s21_is_equal(decZero, s21_result);
  ck_assert_int_eq(1, res_comp);
}
END_TEST

START_TEST(s21_floor_t9) {
  s21_decimal value = {{456592, 0, 0, 0x80000000}};
  s21_decimal s21_result;
  s21_decimal res = {{456592, 0, 0, 0x80000000}};
  int res_comp = s21_floor(value, &s21_result);
  ck_assert_int_eq(0, res_comp);
  res_comp = s21_is_equal(s21_result, res);
  ck_assert_int_eq(1, res_comp);
}
END_TEST
#endif  // END TESTS FOR s21_floor

#ifdef TEST_NAME_round
START_TEST(s21_round_t1) {
  s21_decimal value = {{0x00450474, 0, 0, 0x00030000}};  // 4523.124
  s21_decimal s21_result;
  s21_decimal res = {{4523, 0, 0, 0}};
  s21_round(value, &s21_result);
  int res_comp = s21_is_equal(s21_result, res);
  ck_assert_int_eq(1, res_comp);
}
END_TEST

START_TEST(s21_round_t2) {
  s21_decimal value = {{0x00450474, 0, 0, 0x00030000}};  // 4523.124
  s21_decimal s21_result;
  s21_decimal res = {{4523, 0, 0, 0}};
  s21_round(value, &s21_result);
  int res_comp = s21_is_equal(s21_result, res);
  ck_assert_int_eq(1, res_comp);
}
END_TEST

START_TEST(s21_round_t3) {
  s21_decimal value = {{0x00450474, 0, 0, 0x80030000}};  // -4523.124
  s21_decimal s21_result;
  s21_decimal res = {{4523, 0, 0, 0x80000000}};
  s21_round(value, &s21_result);
  int res_comp = s21_is_equal(s21_result, res);
  ck_assert_int_eq(1, res_comp);
}
END_TEST

START_TEST(s21_round_t4) {
  s21_decimal value = {{0x006E702, 0, 0, 0x80020000}};  // -4523.54
  s21_decimal s21_result;
  s21_decimal res = {{4524, 0, 0, 0x80000000}};
  s21_round(value, &s21_result);
  int res_comp = s21_is_equal(s21_result, res);
  ck_assert_int_eq(1, res_comp);
}
END_TEST

START_TEST(s21_round_t5) {
  s21_decimal value = {{0x840DDDC6, 0, 0, 0x800B0000}};  // -0.02215501254
  s21_decimal s21_result;
  s21_decimal res = {{0, 0, 0, 0}};
  int res_comp = s21_round(value, &s21_result);
  ck_assert_int_eq(0, res_comp);
  res_comp = s21_is_equal(s21_result, res);
  ck_assert_int_eq(1, res_comp);
}
END_TEST

START_TEST(s21_round_t6) {
  s21_decimal value = {{0x1F1F742E, 0, 0, 0x80090000}};  // -0.522155054
  s21_decimal s21_result;
  s21_decimal res = {{1, 0, 0, 0x80000000}};
  int res_comp = s21_round(value, &s21_result);
  equal_bits(res, s21_result);
  ck_assert_int_eq(0, res_comp);
}
END_TEST

START_TEST(s21_round_t7) {
  s21_decimal value = {
      {0x63E41BC6, 0x0000000C, 0, 0x000B0000}};  // 0.53215501254
  s21_decimal s21_result;
  s21_decimal res = {{1, 0, 0, 0}};

  int res_round = s21_round(value, &s21_result);
  ck_assert_int_eq(0, res_round);
  equal_bits(res, s21_result);
}
END_TEST

START_TEST(s21_round_t8) {
  s21_decimal value = {
      {0xCE36A3C6, 0x00000080, 0, 0x000B0000}};  // 5.53215501254
  s21_decimal s21_result;
  s21_decimal res = {{6, 0, 0, 0}};
  int res_round = s21_round(value, &s21_result);
  ck_assert_int_eq(0, res_round);
  int res_comp = s21_is_equal(s21_result, res);
  equal_bits(res, s21_result);
  ck_assert_int_eq(1, res_comp);
}
END_TEST

START_TEST(s21_round_t9) {
  s21_decimal value = {
      {0x224287C6, 0x00000083, 0, 0x000B0000}};  // 5.63215501254
  s21_decimal s21_result;
  s21_decimal res = {{6, 0, 0, 0}};
  int res_round = s21_round(value, &s21_result);

  ck_assert_int_eq(0, res_round);
  int res_comp = s21_is_equal(s21_result, res);
  equal_bits(res, s21_result);
  ck_assert_int_eq(1, res_comp);
}
END_TEST

START_TEST(s21_round_t10) {
  s21_decimal value = {
      {0x63E41BC6, 0x0000000C, 0, 0x000A0000}};  // 5.3215501254
  s21_decimal s21_result;
  s21_decimal res = {{5, 0, 0, 0}};
  int res_round = s21_round(value, &s21_result);

  ck_assert_int_eq(0, res_round);
  equal_bits(res, s21_result);
  int res_comp = s21_is_equal(s21_result, res);
  ck_assert_int_eq(1, res_comp);
}
END_TEST

START_TEST(s21_round_t11) {
  s21_decimal value = {{0x000002D3, 0, 0, 0x80030000}};  // -0.723
  s21_decimal s21_result;
  s21_decimal res = {{1, 0, 0, 0x80000000}};
  int res_comp = s21_round(value, &s21_result);
  equal_bits(res, s21_result);
  ck_assert_int_eq(0, res_comp);
}
END_TEST
#endif  // END TESTS FOR s21_round

#ifdef TEST_NAME_less
START_TEST(s21_less_t1) {
  int result = s21_is_less(decMax, decMin);
  ck_assert_int_eq(0, result);
}
END_TEST

START_TEST(s21_less_t2) {
  int result = s21_is_less(decMax, decMax);
  ck_assert_int_eq(0, result);
}
END_TEST

START_TEST(s21_less_t3) {
  int result = s21_is_less(decZero, decMax);
  ck_assert_int_eq(1, result);
}
END_TEST

START_TEST(s21_less_t4) {
  s21_decimal dec1 = {{0x63E41BC6, 0x0000000C, 0,
                       0x000B0000}},  // 0.53215501254,
      dec2 = {{0x00000006, 0x00000000, 0x00000000, 0x00010000}};  // 0.6
  int result = s21_is_less(dec1, dec2);
  ck_assert_int_eq(1, result);
}
END_TEST
#endif  // END TESTS FOR s21_is_less

#ifdef TEST_NAME_less_or_equal
START_TEST(s21_less_or_equal_t1) {
  int result = s21_is_less_or_equal(decMax, decMin);
  ck_assert_int_eq(0, result);
}
END_TEST

START_TEST(s21_less_or_equal_t2) {
  int result = s21_is_less_or_equal(decMax, decMax);
  ck_assert_int_eq(1, result);
}
END_TEST

START_TEST(s21_less_or_equal_t3) {
  int result = s21_is_less_or_equal(decZero, decMax);
  ck_assert_int_eq(1, result);
}
END_TEST
#endif  // END TESTS FOR s21_is_less_or_equal

#ifdef TEST_NAME_greater
START_TEST(s21_greater_t1) {
  int result = s21_is_greater(decMax, decMin);
  ck_assert_int_eq(1, result);
}
END_TEST

START_TEST(s21_greater_t2) {
  int result = s21_is_greater(decMax, decMax);
  ck_assert_int_eq(0, result);
}
END_TEST

START_TEST(s21_greater_t3) {
  int result = s21_is_greater(decZero, decMax);
  ck_assert_int_eq(0, result);
}
END_TEST
#endif  // END TESTS FOR s21_is_greater

#ifdef TEST_NAME_greater_or_equal
START_TEST(s21_greater_or_equal_t0_1) {
  s21_decimal value = {
      {0x63E41BC6, 0x0000000C, 0, 0x000B0000}};  // 0.53215501254
  s21_decimal half = {{5, 0, 0, 0x10000}};       // 0.5
  s21_long_decimal long_1;
  s21_long_decimal long_2;
  to_equal_scale(value, half, &long_1, &long_2);
  int result = greater_or_equil_long(long_1, long_2);
  ck_assert_int_eq(1, result);
}
END_TEST

START_TEST(s21_greater_or_equal_t0) {
  s21_decimal value = {
      {0x63E41BC6, 0x0000000C, 0, 0x000B0000}};  // 0.53215501254
  s21_decimal half = {{5, 0, 0, 0x10000}};
  int result = s21_is_greater_or_equal(value, half);
  ck_assert_int_eq(1, result);
}
END_TEST

START_TEST(s21_greater_or_equal_t1) {
  int result = s21_is_greater_or_equal(decMax, decMin);
  ck_assert_int_eq(1, result);
}
END_TEST

START_TEST(s21_greater_or_equal_t2) {
  int result = s21_is_greater_or_equal(decMax, decMax);
  ck_assert_int_eq(1, result);
}
END_TEST

START_TEST(s21_greater_or_equal_t3) {
  int result = s21_is_greater_or_equal(decZero, decMax);
  ck_assert_int_eq(0, result);
}
END_TEST

START_TEST(s21_greater_or_equal_t4) {
  s21_decimal dec1 = {{10, 0, 0, 0x80000000}}, dec2 = {{3, 0, 0, 0x80000000}};
  int result = s21_is_greater_or_equal(dec1, dec2);
  ck_assert_int_eq(0, result);
}
END_TEST

START_TEST(s21_greater_or_equal_t5) {
  int result = s21_is_greater_or_equal(decZero, decZero);
  ck_assert_int_eq(1, result);
}
END_TEST

START_TEST(s21_greater_or_equal_t6) {
  int result = s21_is_greater_or_equal(decMax, decZero);
  ck_assert_int_eq(1, result);
}
END_TEST

START_TEST(s21_greater_or_equal_t7) {
  s21_decimal dec1 = {{10, 0, 0, 0x80000000}}, dec2 = {{3, 0, 0, 0}};
  int result = s21_is_greater_or_equal(dec1, dec2);
  ck_assert_int_eq(0, result);
}
END_TEST

START_TEST(s21_greater_or_equal_t8) {
  s21_decimal dec1 = {{10, 0, 0, 0}}, dec2 = {{3, 0, 0, 0x80000000}};
  int result = s21_is_greater_or_equal(dec1, dec2);
  ck_assert_int_eq(1, result);
}
END_TEST

START_TEST(s21_greater_or_equal_t9) {
  s21_decimal dec1 = {{0, 0, 0, 0}}, dec2 = {{3, 0, 0, 0x80000000}};
  int result = s21_is_greater_or_equal(dec1, dec2);
  ck_assert_int_eq(1, result);
}
END_TEST

START_TEST(s21_greater_or_equal_t10) {
  s21_decimal dec1 = {{0, 0, 0, 0}}, dec2 = {{0, 0, 0, 0x80000000}};
  int result = s21_is_greater_or_equal(dec1, dec2);
  ck_assert_int_eq(1, result);
}
END_TEST

#endif  // END TESTS FOR s21_is_greater_or_equal

#ifdef TEST_NAME_equal
START_TEST(s21_equal_t1) {
  int result = s21_is_equal(decMax, decMin);
  ck_assert_int_eq(0, result);
}
END_TEST

START_TEST(s21_equal_t2) {
  int result = s21_is_equal(decMax, decMax);
  ck_assert_int_eq(1, result);
}
END_TEST

START_TEST(s21_equal_t3) {
  int result = s21_is_equal(decZero, decZero);
  ck_assert_int_eq(1, result);
}
END_TEST

START_TEST(s21_equal_t4) {
  s21_decimal dec1 = {{10, 0, 0, 0x00000000}}, dec2 = {{100, 0, 0, 0x00010000}},
              dec3 = {{0x4E72A000, 0x00000918, 0, 0x000C0000}};

  int result = s21_is_equal(dec1, dec2);
  ck_assert_int_eq(1, result);
  result = s21_is_equal(dec1, dec3);
  ck_assert_int_eq(1, result);
}
END_TEST

START_TEST(s21_equal_t5) {
  s21_decimal dec1 = {{10, 0, 0, 0x80000000}}, dec2 = {{3, 0, 0, 0}};
  int result = s21_is_equal(dec1, dec2);
  ck_assert_int_eq(0, result);
}
END_TEST

START_TEST(s21_equal_t6) {
  s21_decimal dec1 = {{10, 0, 0, 0}}, dec2 = {{3, 0, 0, 0x80000000}};
  int result = s21_is_equal(dec1, dec2);
  ck_assert_int_eq(0, result);
}
END_TEST

START_TEST(s21_equal_t7) {
  s21_decimal dec1 = {{0, 0, 0, 0}}, dec2 = {{3, 0, 0, 0x80000000}};
  int result = s21_is_equal(dec1, dec2);
  ck_assert_int_eq(0, result);
}
END_TEST

START_TEST(s21_equal_t8) {
  s21_decimal dec1 = {{0, 0, 0, 0}}, dec2 = {{0, 0, 0, 0x80000000}};
  int result = s21_is_equal(dec1, dec2);
  ck_assert_int_eq(1, result);
}
END_TEST

START_TEST(s21_equal_t9) {
  s21_decimal dec1 = {{123, 0, 0, 0}}, dec2 = {{10223, 0, 0, 0}};
  int result = s21_is_equal(dec1, dec2);
  ck_assert_int_eq(0, result);
}
END_TEST

#endif  // END TESTS FOR s21_is_equal

#ifdef TEST_NAME_not_equal
START_TEST(s21_not_equal_t1) {
  int result = s21_is_not_equal(decMax, decMin);
  ck_assert_int_eq(1, result);
}
END_TEST

START_TEST(s21_not_equal_t2) {
  int result = s21_is_not_equal(decMax, decMax);
  ck_assert_int_eq(0, result);
}
END_TEST

START_TEST(s21_not_equal_t3) {
  int result = s21_is_not_equal(decZero, decZero);
  ck_assert_int_eq(0, result);
}
END_TEST

START_TEST(s21_not_equal_t4) {
  s21_decimal dec1 = {{10, 0, 0, 0x00000000}}, dec2 = {{100, 0, 0, 0x00010000}},
              dec3 = {{0x4E72A000, 0x00000918, 0, 0x000C0000}};

  int result = s21_is_not_equal(dec1, dec2);
  ck_assert_int_eq(0, result);
  result = s21_is_not_equal(dec1, dec3);
  ck_assert_int_eq(0, result);
}
END_TEST

START_TEST(s21_not_equal_t5) {
  s21_decimal dec1 = {{10, 0, 0, 0x80000000}}, dec2 = {{3, 0, 0, 0}};
  int result = s21_is_not_equal(dec1, dec2);
  ck_assert_int_eq(1, result);
}
END_TEST

START_TEST(s21_not_equal_t6) {
  s21_decimal dec1 = {{10, 0, 0, 0}}, dec2 = {{3, 0, 0, 0x80000000}};
  int result = s21_is_not_equal(dec1, dec2);
  ck_assert_int_eq(1, result);
}
END_TEST

START_TEST(s21_not_equal_t7) {
  s21_decimal dec1 = {{0, 0, 0, 0}}, dec2 = {{3, 0, 0, 0x80000000}};
  int result = s21_is_not_equal(dec1, dec2);
  ck_assert_int_eq(1, result);
}
END_TEST

START_TEST(s21_not_equal_t8) {
  s21_decimal dec1 = {{0, 0, 0, 0}}, dec2 = {{0, 0, 0, 0x80000000}};
  int result = s21_is_not_equal(dec1, dec2);
  ck_assert_int_eq(0, result);
}
END_TEST
#endif  // END TESTS FOR s21_is_not_equal

#ifdef TEST_NAME_mod
START_TEST(s21_mod_t1) {
  s21_decimal value_1 = {{10, 0, 0, 0}};
  s21_decimal value_2 = {{3, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int x = s21_mod(value_1, value_2, &result);
  ck_assert_int_eq(1, result.bits[0]);
  ck_assert_int_eq(0, x);
}
END_TEST

START_TEST(s21_mod_t2) {
  s21_decimal value_1 = {{10, 0, 0, 0}};
  s21_decimal value_2 = {{0, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int x = s21_mod(value_1, value_2, &result);
  ck_assert_int_eq(3, x);
}
END_TEST

START_TEST(s21_mod_t3) {
  s21_decimal value_1 = {{10, 0, 0, 0x80000000}};
  s21_decimal value_2 = {{3, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int x = s21_mod(value_1, value_2, &result);
  ck_assert_int_eq(1, result.bits[0]);
  unsigned int b3 = result.bits[3];
  ck_assert_int_eq(0x80000000, b3);
  ck_assert_int_eq(0, x);
}
END_TEST

START_TEST(s21_mod_t4) {
  s21_decimal value_1 = {{14, 0, 0, 0x10000}};
  s21_decimal value_2 = {{3, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int x = s21_mod(value_1, value_2, &result);
  ck_assert_int_eq(14, result.bits[0]);
  ck_assert_int_eq(0x10000, result.bits[3]);
  ck_assert_int_eq(0, x);
}
END_TEST

START_TEST(s21_mod_t5) {
  s21_decimal dec1 = {{10, 0, 0, 0x80000000}}, dec2 = {{3, 0, 0, 0x80000000}},
              dec_res = {{1, 0, 0, 0x80000000}};
  s21_decimal result;
  int f_res = s21_mod(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_mod_t6) {
  s21_decimal dec1 = {{10, 0, 0, 0}}, dec2 = {{3, 0, 0, 0x80000000}},
              dec_res = {{1, 0, 0, 0}};
  s21_decimal result;
  int f_res = s21_mod(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_mod_t7) {
  s21_decimal dec1 = {{10, 0, 0, 0}},
              dec2 = {{0, 0, 0, 0}};
  // dec_res = {{1, 0, 0, 0}};
  s21_decimal result;
  int f_res = s21_mod(dec1, dec2, &result);
  // equal_bits(dec_res, result);
  ck_assert_int_eq(3, f_res);
}
END_TEST

START_TEST(s21_mod_t8) {
  // 7922816.2514264337593543950335
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00160000}};
  // 9872541556.1254784
  s21_decimal value_2 = {{0x0CD4D780, 0x015EBE3E, 0x00000000, 0x00070000}};
  // 7922816.2514264337593543950335
  s21_decimal s21_res = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00160000}},
              result;
  int f_res = s21_mod(value_1, value_2, &result);
  equal_bits(result, s21_res);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_mod_t9) {
  s21_decimal value_1 = {{0x686FF97F, 0x001C25C2, 0, 0}};  // 7922816253950335
  s21_decimal value_2 = {{79228162, 0, 0, 0}};             // 79228162
  // 53950335
  s21_decimal s21_res = {{53950335, 0, 0, 0}}, result;
  int f_res = s21_mod(value_1, value_2, &result);
  equal_bits(result, s21_res);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_mod_t10) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}},
              value_2 = {{0xE7FFFFFF, 0x9FD0803C, 0x033B2E3C, 0x001B0000}},
              dec_res = {{0x6800004E, 0xAEA86D34, 0x00BCBB4A, 0x001B0000}};
  s21_decimal result;
  int f_res = s21_mod(value_1, value_2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_mod_t12) {
  // 79228162514264337593543950335
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}};
  // 79228162514264337593543953
  s21_decimal value_2 = {{0x9DB22D11, 0x4BC6A7EF, 0x00418937, 0x00000000}};
  // 999.9999999999999999999999664
  s21_decimal s21_res = {{0x9DB222A8, 0x4BC6A7EF, 0x00418937, 0x00000000}},
              result;
  int f_res = s21_mod(value_1, value_2, &result);
  equal_bits(result, s21_res);
  ck_assert_int_eq(0, f_res);
}
END_TEST
START_TEST(s21_mod_t13) {
  // 79228162514264337593543950335
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  // 0.0000000000000000000000000001
  s21_decimal value_2 = {{1, 0, 0, 0x001C0000}}, result;

  s21_decimal s21_res = {{0, 0, 0, 0x001C0000}};

  int f_res = s21_mod(value_1, value_2, &result);
  equal_bits(result, s21_res);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_mod_t14) {
  s21_decimal src1, src2, res_mod;
  int a = 3;
  int b = 2;
  int res_origin = a % b;
  int res = 0;
  s21_from_int_to_decimal(a, &src1);
  s21_from_int_to_decimal(b, &src2);
  s21_mod(src1, src2, &res_mod);
  s21_from_decimal_to_int(res_mod, &res);
  ck_assert_int_eq(res_origin, res);
}
END_TEST

START_TEST(s21_mod_t15) {
  s21_decimal src1, src2, res_mod;
  int a = -98765;
  int b = 1234;
  int res_origin = a % b;
  int res = 0;
  s21_from_int_to_decimal(a, &src1);
  s21_from_int_to_decimal(b, &src2);
  s21_mod(src1, src2, &res_mod);
  s21_from_decimal_to_int(res_mod, &res);
  ck_assert_int_eq(res_origin, res);
}
END_TEST

START_TEST(s21_mod_t16) {
  s21_decimal src1, src2, res_mod;
  int a = 30198;
  int b = 20210;
  int res_origin = a % b;
  int res = 0;
  s21_from_int_to_decimal(a, &src1);
  s21_from_int_to_decimal(b, &src2);
  s21_mod(src1, src2, &res_mod);
  s21_from_decimal_to_int(res_mod, &res);
  ck_assert_int_eq(res_origin, res);
}
END_TEST

START_TEST(s21_mod_t17) {
  s21_decimal src1, src2, res_mod;
  int a = -98765;
  int b = -1234;
  int res_origin = a % b;
  int res = 0;
  s21_from_int_to_decimal(a, &src1);
  s21_from_int_to_decimal(b, &src2);
  s21_mod(src1, src2, &res_mod);
  s21_from_decimal_to_int(res_mod, &res);
  ck_assert_int_eq(res_origin, res);
}
END_TEST

START_TEST(s21_mod_t18) {
  s21_decimal src1, src2, res_mod;
  int a = 98765;
  int b = 127234;
  int res_origin = a % b;
  int res = 0;
  s21_from_int_to_decimal(a, &src1);
  s21_from_int_to_decimal(b, &src2);
  s21_mod(src1, src2, &res_mod);
  s21_from_decimal_to_int(res_mod, &res);
  ck_assert_int_eq(res_origin, res);
}
END_TEST

START_TEST(s21_mod_t19) {
  s21_decimal src1, src2, res_mod;
  int a = 342576;
  int b = 1542134;
  int res_origin = a % b;
  int res = 0;
  s21_from_int_to_decimal(a, &src1);
  s21_from_int_to_decimal(b, &src2);
  s21_mod(src1, src2, &res_mod);
  s21_from_decimal_to_int(res_mod, &res);
  ck_assert_int_eq(res_origin, res);
}
END_TEST

START_TEST(s21_mod_t20) {
  s21_decimal src1, src2, res_mod;
  float a = 1.2;
  float b = 0.3;
  float res_origin = fmod(a, b);
  float res = 0.0;
  s21_from_float_to_decimal(a, &src1);
  s21_from_float_to_decimal(b, &src2);
  s21_mod(src1, src2, &res_mod);
  s21_from_decimal_to_float(res_mod, &res);
  ck_assert_float_eq(res_origin, res);
}
END_TEST

START_TEST(s21_mod_t21) {
  s21_decimal src1, src2, res_mod;
  float a = 1.2;
  int b = 3;
  float res_origin = fmod(a, b);
  float res = 0;
  s21_from_float_to_decimal(a, &src1);
  s21_from_int_to_decimal(b, &src2);
  s21_mod(src1, src2, &res_mod);
  s21_from_decimal_to_float(res_mod, &res);
  ck_assert_float_eq(res_origin, res);
}


START_TEST(s21_mod_t22) {
  s21_decimal value_1 = {{0x0000000B, 0x00000000, 0x00000000, 0x00010000}},
              value_2 = {{0x00000007, 0x00000000, 0x00000000, 0x00010000}},
              dec_res = {{0x00000004, 0x00000000, 0x00000000, 0x00010000}};
  s21_decimal result;
  int f_res = s21_mod(value_1, value_2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_mod_t23) {
  // 10000000000000000000000000000
  s21_decimal value_1 = {{0x10000000, 0x3E250261, 0x204FCE5E, 0}},
              // 20000000000000000000000000000
              value_2 = {{0x20000000, 0x7C4A04C2, 0x409F9CBC, 0}},
              // 10000000000000000000000000000
              dec_res = {{0x10000000, 0x3E250261, 0x204FCE5E, 0}};
  s21_decimal result;
  int f_res = s21_mod(value_1, value_2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

#endif

#ifdef TEST_NAME_add
START_TEST(s21_add_t1) {
  s21_decimal dec1 = {{10, 0, 0, 0}}, dec2 = {{3, 0, 0, 0}},
              dec_res = {{0x0000000D, 0, 0, 0}};  // 13
  s21_decimal result;
  int f_res = s21_add(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_add_t2) {
  s21_decimal dec1 = {{10, 0, 0, 0}}, dec2 = {{0, 0, 0, 0}},
              dec_res = {{10, 0, 0, 0}};
  s21_decimal result;
  int f_res = s21_add(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_add_t3) {
  s21_decimal dec1 = {{10, 0, 0, 0x80000000}}, dec2 = {{3, 0, 0, 0x00000000}},
              dec_res = {{7, 0, 0, 0x80000000}};
  s21_decimal result;
  int f_res = s21_add(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_add_t4) {
  s21_decimal dec1 = {{10, 0, 0, 0}}, dec2 = {{3, 0, 0, 0x80000000}},
              dec_res = {{7, 0, 0, 0}};
  s21_decimal result;
  int f_res = s21_add(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_add_t5) {
  s21_decimal dec_res = {{0, 0, 0, 0}};
  s21_decimal result;
  int f_res = s21_add(decMax, decMin, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_add_t6) {
  s21_decimal dec_res = {{0, 0, 0, 0}};
  s21_decimal result;
  int f_res = s21_add(decZero, decZero, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_add_t7) {
  s21_decimal dec_res = decMax;
  s21_decimal result;
  int f_res = s21_add(decMax, decZero, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_add_t8) {
  // переполнение
  s21_decimal dec2 = {{2, 0, 0, 0}};
  s21_decimal result;
  int f_res = s21_add(decMax, dec2, &result);
  ck_assert_int_eq(1, f_res);
}
END_TEST

START_TEST(s21_add_t9) {
  s21_decimal dec2 = {{2, 0, 0, 0x80000000}},
              dec_res = {{0xFFFFFFFD, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal result;
  int f_res = s21_add(decMax, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_add_t10) {
  s21_decimal dec2 = {{2, 0, 0, 0}},
              dec_res = {{0xFFFFFFFD, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal result;
  int f_res = s21_add(decMin, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_add_t11) {
  // переполнение
  s21_decimal dec2 = {{2, 0, 0, 0x80000000}};
  s21_decimal result;
  int f_res = s21_add(decMin, dec2, &result);
  ck_assert_int_eq(2, f_res);
}
END_TEST

START_TEST(s21_add_t12) {
  s21_decimal dec1 = {{0x00450474, 0, 0, 0x00030000}},  // 4523.124
      dec2 = {{0x00450474, 0, 0, 0x00060000}},          // 4.523124
      dec_res = {
          {0x0DDE6994, 0x00000001, 0x00000000, 0x00060000}};  // 4527.647124
  s21_decimal result;
  int f_res = s21_add(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_add_t13) {
  s21_decimal dec1 = {{0x00450474, 0, 0, 0x00030000}},  // 4523.124
      dec2 = {{0x00450474, 0, 0, 0x80060000}},          // 4.523124
      dec_res = {
          {0x0D5460AC, 0x00000001, 0x00000000, 0x00060000}};  // 4518.600876
  s21_decimal result;
  int f_res = s21_add(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_add_t14) {
  s21_decimal dec1 = {{0x00450474, 0, 0, 0x80030000}},  // 4523.124
      dec2 = {{0x00450474, 0, 0, 0x00060000}},          // 4.523124
      dec_res = {
          {0x0D5460AC, 0x00000001, 0x00000000, 0x80060000}};  // -4518.600876
  s21_decimal result;
  int f_res = s21_add(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_add_t15) {
  s21_decimal dec1 = {{13, 0, 0, 0x80000000}},  // -13
      dec2 = {{7, 0, 0, 0x80000000}},           // -7
      dec_res = {{20, 0, 0, 0x80000000}};       // -20
  s21_decimal result;
  int f_res = s21_add(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_add_t16) {
  s21_decimal dec1 = {{10, 0, 0, 0x80000000}}, dec2 = {{3, 0, 0, 0}},
              dec_res = {{7, 0, 0, 0x80000000}};
  s21_decimal result;
  int f_res = s21_add(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_add_t17) {
  s21_decimal dec1 = {{10, 0, 0, 0x80000000}}, dec2 = {{20, 0, 0, 0}},
              dec_res = {{10, 0, 0, 0}};
  s21_decimal result;
  int f_res = s21_add(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_add_t18) {
  s21_decimal dec1 = {{10, 0, 0, 0}}, dec2 = {{25, 0, 0, 0x80000000}},
              dec_res = {{15, 0, 0, 0x80000000}};

  s21_decimal result;
  int f_res = s21_add(dec1, dec2, &result);

  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_add_t19) {
  // -12345677.987654345678987654346
  s21_decimal dec1 = {{0x910AF4CA, 0xC045D5F2, 0x27E41B00, 0x80150000}},
              // 87654323456.9876545678987653
      dec2 = {{0x11CDD185, 0xF5BC6FC0, 0x02D50F3F, 0x00100000}},
              // 87641977779.00000022221977765
      dec_res = {{0b10010010000001100111100010100101,
                  0b10001011011010100100100011011111,
                  0b00011100010100011001001100010001,
                  0b00000000000100010000000000000000}};

  s21_decimal result;
  int f_res = s21_add(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_add_t20) {
  s21_decimal dec1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}},
              dec2 = {{1, 0, 0, 0}}, dec_res = {{0, 0, 0, 0}};
  s21_decimal result;
  int f_res = s21_add(dec1, dec2, &result);
  ck_assert_int_eq(1, f_res);
  equal_bits(dec_res, result);
}
END_TEST

START_TEST(s21_add_t21) {
  s21_decimal dec1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
              dec2 = {{1, 0, 0, 0x80000000}}, dec_res = {{0, 0, 0, 0}};

    s21_decimal result;

    int f_res = s21_add(dec1, dec2, &result);
    equal_bits(dec_res, result);
    ck_assert_int_eq(2, f_res);
}
END_TEST

START_TEST(s21_add_t22) {
  s21_decimal dec1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
              dec2 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
              dec_res = {{0, 0, 0, 0}};
  s21_decimal result;
  int f_res = s21_add(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(2, f_res);
}
START_TEST(s21_add_t23) {
  s21_decimal dec1 = {{0x7f7f7f7f, 0x00007f7f, 0x00000000, 0x00040000}},
              dec2 = {{0x7f7f7f7f, 0x007f7f7f, 0x00000000, 0x00040000}},
              dec_res = {{0xfefefefe, 0x007ffefe, 0x00000000, 0x00040000}};
  s21_decimal result;
  int f_res = s21_add(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

#endif  // END TESTS FOR s21_add

#ifdef TEST_NAME_sub
START_TEST(s21_sub_t0) {
  s21_decimal dec1 = {{10, 0, 0, 0}}, dec2 = {{3, 0, 0, 0}},
              dec_res = {{7, 0, 0, 0}};
  s21_decimal result;
  int f_res = s21_sub(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_sub_t1) {
  s21_decimal dec1 = {{10, 0, 0, 0}}, dec2 = {{23, 0, 0, 0}},
              dec_res = {{0x0000000D, 0, 0, 0x80000000}};  // -23
  s21_decimal result;
  int f_res = s21_sub(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_sub_t2) {
  s21_decimal dec1 = {{10, 0, 0, 0}}, dec2 = {{0, 0, 0, 0}},
              dec_res = {{10, 0, 0, 0}};
  s21_decimal result;
  int f_res = s21_sub(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_sub_t3) {
  s21_decimal dec1 = {{10, 0, 0, 0x80000000}}, dec2 = {{3, 0, 0, 0}},
              dec_res = {{0x0000000D, 0, 0, 0x80000000}};  // -13
  s21_decimal result;
  int f_res = s21_sub(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_sub_t3_3) {
  s21_decimal dec1 = {{10, 0, 0, 0x80000000}}, dec2 = {{3, 0, 0, 0x80000000}},
              dec_res = {{7, 0, 0, 0x80000000}};  // -13
  s21_decimal result;
  int f_res = s21_sub(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_sub_t4) {
  s21_decimal dec1 = {{10, 0, 0, 0}}, dec2 = {{3, 0, 0, 0x80000000}},
              dec_res = {{13, 0, 0, 0x00000000}};  // 13
  s21_decimal result;
  int f_res = s21_sub(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_sub_t5) {
  s21_decimal result;
  int f_res = s21_sub(decMax, decMin, &result);  // переполнение
  ck_assert_int_eq(1, f_res);
}
END_TEST

START_TEST(s21_sub_t6) {
  s21_decimal dec_res = {{0, 0, 0, 0}};
  s21_decimal result;
  int f_res = s21_sub(decZero, decZero, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_sub_t7) {
  s21_decimal dec_res = decMax;
  s21_decimal result;
  int f_res = s21_sub(decMax, decZero, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_sub_t8) {
  s21_decimal dec2 = {{2, 0, 0, 0}},
              dec_res = {{0xFFFFFFFD, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}};
  s21_decimal result;
  int f_res = s21_sub(decMax, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_sub_t9) {
  // переполнение
  s21_decimal dec2 = {{2, 0, 0, 0x80000000}};
  s21_decimal result;
  int f_res = s21_sub(decMax, dec2, &result);
  ck_assert_int_eq(1, f_res);
}
END_TEST

START_TEST(s21_sub_t10) {
  // переполнение
  s21_decimal dec2 = {{2, 0, 0, 0}};
  s21_decimal result;
  int f_res = s21_sub(decMin, dec2, &result);
  ck_assert_int_eq(2, f_res);
}
END_TEST

START_TEST(s21_sub_t11) {
  s21_decimal dec2 = {{2, 0, 0, 0x80000000}},
              dec_res = {{0xFFFFFFFD, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal result;
  int f_res = s21_sub(decMin, dec2, &result);
  int tr = s21_is_equal(dec_res, result);
  ck_assert_int_eq(1, tr);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_sub_t12) {
  s21_decimal dec1 = {{0x00450474, 0, 0, 0x00030000}},  // 4523.124
      dec2 = {{0x00450474, 0, 0, 0x00060000}},          // 4.523124
      dec_res = {
          {0x0D5460AC, 0x00000001, 0x00000000, 0x00060000}};  // 4518.600876
  s21_decimal result;
  int f_res = s21_sub(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_sub_t13) {
  s21_decimal dec1 = {{0x00450474, 0, 0, 0x00030000}},  // 4523.124
      dec2 = {{0x00450474, 0, 0, 0x80060000}},          // 4.523124
      dec_res = {
          {0x0DDE6994, 0x00000001, 0x00000000, 0x00060000}};  // 4527.647124
  s21_decimal result;
  int f_res = s21_sub(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_sub_t14) {
  s21_decimal dec1 = {{0x00450474, 0, 0, 0x80030000}},  // 4523.124
      dec2 = {{0x00450474, 0, 0, 0x00060000}},          // 4.523124
      dec_res = {
          {0x0DDE6994, 0x00000001, 0x00000000, 0x80060000}};  // -4527.647124
  s21_decimal result;
  int f_res = s21_sub(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_sub_t15) {
  s21_decimal dec1 = {{0xFFFFFFFA, 0xFFFFFFFF, 0xFFFFFFFF,
                       00000000}},  // 79228162514264337593543950330
      dec2 = {{0x00000005, 0x00000000, 0x00000000,
               0x00010000}},  // 0.5 0x00010000
      dec_res = {{0xFFFFFFFA, 0xFFFFFFFF, 0xFFFFFFFF,
                  00000000}};  // 79228162514264337593543950330
  s21_decimal result;

  int f_res = s21_sub(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_sub_t16) {
  s21_decimal dec1 = {{0xFFFFFFF8, 0xFFFFFFFF, 0xFFFFFFFF,
                       0}},  // 79,228,162,514,264,337,593,543,950,328
      dec2 = {{0x00000004, 0x00000000, 0x00000000, 0x00010000}},  // 0.4
      dec_res = {{0xFFFFFFF8, 0xFFFFFFFF, 0xFFFFFFFF,
                  0}};  // 79228162514264337593543950328
  s21_decimal result;

  int f_res = s21_sub(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_sub_t17) {
  s21_decimal dec1 = {{0xFFFFFFF8, 0xFFFFFFFF, 0xFFFFFFFF,
                       0}},  // 79,228,162,514,264,337,593,543,950,328
      dec2 = {{0x00000006, 0x00000000, 0x00000000, 0x00010000}},  // 0.6
      dec_res = {{0xFFFFFFF7, 0xFFFFFFFF, 0xFFFFFFFF,
                  0}};  // 79228162514264337593543950327
  s21_decimal result;

  int f_res = s21_sub(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_sub_t18) {
  s21_decimal dec1 = {{0xFFFFFFF8, 0xFFFFFFFF, 0xFFFFFFFF,
                       0}},  // 79,228,162,514,264,337,593,543,950,328
      dec2 = {{0x00000005, 0x00000000, 0x00000000, 0x00010000}},  // 0.5
      dec_res = {{0xFFFFFFF8, 0xFFFFFFFF, 0xFFFFFFFF,
                  0}};  // 79228162514264337593543950328
  s21_decimal result;

  int f_res = s21_sub(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_sub_t19) {
  s21_decimal dec1 = {{0xFFFFFFF8, 0xFFFFFFFF, 0xFFFFFFFF,
                       0}},  // 79,228,162,514,264,337,593,543,950,328
      dec2 = {{0x0000000F, 0x00000000, 0x00000000, 0x00010000}},  // 1.5
      dec_res = {{0xFFFFFFF6, 0xFFFFFFFF, 0xFFFFFFFF,
                  0}};  // 79228162514264337593543950326
  s21_decimal result;

  int f_res = s21_sub(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_sub_t20) {
  s21_decimal dec1 = {{0xFFFFFFF8, 0xFFFFFFFF, 0xFFFFFFFF,
                       0}},  // 79,228,162,514,264,337,593,543,950,328
      dec2 = {{0x0000000F, 0x00000000, 0x00000000, 0x00010000}},  // 1.5
      dec_res = {{0xFFFFFFF6, 0xFFFFFFFF, 0xFFFFFFFF,
                  0}};  // 79228162514264337593543950326
  s21_decimal result;

  int f_res = s21_sub(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_sub_t21) {
  // 79,228,162,514,264,337,593,543,950,334
  s21_decimal dec1 = {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
              dec2 = {{0x00000006, 0x00000000, 0x00000000, 0x00010000}},  // 0.6
      // 79228162514264337593543950335
      dec_res = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}};
  s21_decimal result;

  int f_res = s21_sub(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_sub_t22) {
  s21_decimal dec1 = {{0x48CBD4D9, 0x00000017, 0x00000000,
                       0x00050000}},  // 1000055.65657m
      dec2 = {{0xE0B77B69, 0x00000002, 0x00000000,
               0x00030000}},  // 12360055.657
      dec_res = {
          {0x7EE0602B, 0x00000108, 0x00000000, 0x80050000}};  // -11360000.00043
  s21_decimal result;
  int f_res = s21_sub(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_sub_t23) {
  s21_decimal dec1 = {{0x3CF489AD, 0, 0, 0x80070000}},  // -102.2658989
      dec2 = {{0x6A504D37, 0x00000005, 0x00000000,
               0x80030000}},  // -23258484.023
      dec_res = {{0xA3D3AAC3, 0x0000D388, 0x00000000,
                  0x00070000}};  // 23258381.7571011
  s21_decimal result;
  int f_res = s21_sub(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST
#endif  // END TESTS FOR s21_sub

#ifdef TEST_NAME_dec_to_float

START_TEST(s21_dec_to_float_test_1) {
  s21_decimal dec1 = {{0, 0, 0, 0x80030000}};  // -4527.647124
  float result;
  int f_res = s21_from_decimal_to_float(dec1, &result);
  ck_assert_int_eq(0, f_res);
  ck_assert_ldouble_eq(0, result);
}

START_TEST(s21_dec_to_float_test_2) {
  s21_decimal src;
  int result = 0;
  float number = 0.0;
  src.bits[0] = 18122;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 2147680256;
  result = s21_from_decimal_to_float(src, &number);
  ck_assert_float_eq(number, -18.122);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_float_2) {
  s21_decimal src;
  int result = 0;
  float number = 0.0;
  src.bits[0] = 1812;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 2147483648;
  result = s21_from_decimal_to_float(src, &number);
  ck_assert_float_eq(number, -1812);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_float_3) {
  s21_decimal src;
  int result = 0;
  float number = 0.0;
  src.bits[0] = 16777215;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 0;
  result = s21_from_decimal_to_float(src, &number);
  ck_assert_float_eq(number, 16777215);  // 16777216.000000
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_float_4) {
  s21_decimal src;
  int result = 0;
  float number = 0.0;
  src.bits[0] = 23450987;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 2147745792;
  result = s21_from_decimal_to_float(src, &number);
  ck_assert_float_eq(number, -2345.0987);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_float_5) {
  s21_decimal src;
  int result = 0;
  float number = 0.0;
  src.bits[0] = 4294967295;
  src.bits[1] = 4294967295;
  src.bits[2] = 0;
  src.bits[3] = 0;
  result = s21_from_decimal_to_float(src, &number);
  ck_assert_float_eq_tol(number, 1.84467e+19f, 1e+14);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_float_6) {
  s21_decimal src;
  int result = 0;
  float number = 0.6;
  src.bits[0] = 6;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 0x00010000;
  result = s21_from_decimal_to_float(src, &number);
  ck_assert_float_eq_tol(number, 0.6, 1e-6);
  ck_assert_float_eq(number, 0.6);
  ck_assert_int_eq(result, 0);
}
END_TEST
#endif  // END TESTS FOR s21_from_decimal_to_float

#ifdef TEST_NAME_div
START_TEST(s21_div_t01) {
  s21_decimal dec1 = {{0, 0, 0, 0x80000000}};
  s21_decimal dec2 = {{3, 0, 0, 0}};
  s21_decimal dec_res = {{0, 0, 0, 0x80000000}};
  s21_decimal result;
  int f_res = s21_div(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST
START_TEST(s21_div_t0) {
  s21_decimal value_1 = {{5523, 0, 0, 0x20000}};
  s21_decimal value_2 = {{3, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int x = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(1841, result.bits[0]);
  ck_assert_int_eq(0x20000, result.bits[3]);
  ck_assert_int_eq(0, x);
}
END_TEST
START_TEST(s21_div_t1) {
  s21_decimal dec1 = {{0b00000000000000000000000000000010, 0, 0, 0}},
              dec2 = {{0b00000000000000000000000000000101, 0, 0, 0}},
              dec_res = {{0b00000000000000000000000000000100, 0, 0,
                          0b00000000000000010000000000000000}};
  s21_decimal result;
  int f_res = s21_div(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST
START_TEST(s21_div_t2) {
  s21_decimal dec1 = {{0b00000000000000000000000000000010, 0, 0, 0}},
              dec2 = {{0b00000000000000000000000000000101, 0, 0, 0}},
              dec_res = {{0b00000000000000000000000000000100, 0, 0,
                          0b00000000000000010000000000000000}};
  s21_decimal result;
  int f_res = s21_div(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_div_t3) {
  s21_decimal dec1 = {{0b01001100110100101000000111000101,
                       0b00000000000000000000000000000010, 0,
                       0b10000000000000000000000000000000}},
              dec2 = {{0b00000000000000000000000000000011, 0, 0,
                       0b10000000000000000000000000000000}},
              dec_res = {{0x0282AAAB, 0x18C9BEC4, 0x6A6680B6, 0x00130000}};

  s21_decimal result;
  int f_res = s21_div(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_div_t4) {
  s21_decimal s21_res = {{1, 0, 0, 0x80000000}};
  s21_decimal result;
  int f_res = s21_div(decMin, decMax, &result);
  equal_bits(result, s21_res);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_div_t5) {
  s21_decimal s21_res = {{1, 0, 0, 0x80000000}};
  s21_decimal result;
  int f_res = s21_div(decMax, decMin, &result);
  equal_bits(result, s21_res);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_div_t6) {
  s21_decimal value_1 = {{5523, 0, 0, 0x20000}};
  s21_decimal value_2 = {{3, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  int x = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(1841, result.bits[0]);
  ck_assert_int_eq(0x20000, result.bits[3]);
  ck_assert_int_eq(0, x);
}
END_TEST

START_TEST(s21_div_t7) {
  s21_decimal value_1 = {{5523, 0, 0, 0x20000}};
  s21_decimal value_2 = {{0, 0, 0, 0}}, result;
  int x = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(3, x);
}
END_TEST

START_TEST(s21_div_t7_1) {
  s21_decimal value_1 = {{0, 0, 0, 0}};
  s21_decimal value_2 = {{0, 0, 0, 0}}, result;
  int x = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(3, x);
}
END_TEST

START_TEST(s21_div_t8) {
  s21_decimal value_1 = {{0, 0, 0, 0x8}};
  s21_decimal value_2 = {{5523, 0, 0, 0x20000}};
  s21_decimal s21_res = {{0, 0, 0, 0}}, result;
  int x = s21_div(value_1, value_2, &result);
  equal_bits(result, s21_res);
  ck_assert_int_eq(0, x);
}
END_TEST

START_TEST(s21_div_t9) {
  // 7922816.2514264337593543950335
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00160000}};
  // 9872541556.1254784
  s21_decimal value_2 = {{0x0CD4D780, 0x015EBE3E, 0x00000000, 0x00070000}};
  // 0.0008025102965011754518631374
  s21_decimal s21_res = {{0x67A0D3CE, 0xB4159468, 0x0006A361, 0x001C0000}},
              result;
  int x = s21_div(value_1, value_2, &result);
  equal_bits(result, s21_res);
  ck_assert_int_eq(0, x);
}
END_TEST

START_TEST(s21_div_t10) {
  s21_decimal value_1 = {
      {0x686FF97F, 0x001C25C2, 0x00000000, 0x00000000}};  // 7922816253950335
  s21_decimal value_2 = {{0x04B8ED02, 0, 0, 0}};          // 79228162
  // 100000000.6809489661012204221
  s21_decimal s21_res = {{0b01010111010001101000101101100010,
                          0b11101111001001101010111000010000,
                          0b00100000010011111100111001100001,
                          0b00000000000101000000000000000000}},
              result;
  int x = s21_div(value_1, value_2, &result);
  equal_bits(result, s21_res);
  ck_assert_int_eq(0, x);
}
END_TEST

START_TEST(s21_div_t11) {
  s21_decimal dec1 = {{0b01001100110100101000000111000101,
                       0b00000000000000000000000000000010, 0,
                       0b10000000000000000000000000000000}},
              dec2 = {{0b00000000000000000000000000000011, 0, 0, 0}},
              dec_res = {{0x0282AAAB, 0x18C9BEC4, 0x6A6680B6, 0x80130000}};

  s21_decimal result;
  int f_res = s21_div(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_div_t12) {
  s21_decimal dec1 = {{0b01011111000010000000010001011011,
                       0b01101101111100110111111101100111, 0,
                       0b00000000000100000000000000000000}},
              dec2 = {{0b00000011111010011010110001001110, 0, 0, 0}},
              dec_res = {{0xD18F6296, 0xA759CA7B, 0x0000198E, 0x001C0000}};

  s21_decimal result;
  int f_res = s21_div(dec1, dec2, &result);
  equal_bits(dec_res, result);
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_div_t13) {
  // 79228162514264337593543950335
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000}};
  // 79228162514264337593543953
  s21_decimal value_2 = {{0x9DB22D11, 0x4BC6A7EF, 0x00418937, 0x00000000}};
  // 999.9999999999999999999999664
  s21_decimal s21_res = {{0x0FFFFEB0, 0x3E250261, 0x204FCE5E, 0x00190000}},
              result;
  int x = s21_div(value_1, value_2, &result);
  equal_bits(result, s21_res);
  ck_assert_int_eq(0, x);
}
END_TEST
START_TEST(s21_div_t14) {
  // 79228162514264337593543950335
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  // 0.0000000000000000000000000001
  s21_decimal value_2 = {{1, 0, 0, 0x001C0000}}, result;
  int x = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(1, x);
}
END_TEST
START_TEST(s21_div_t15) {
  s21_decimal value_1 = {{2, 0, 0, 0}};
  s21_decimal value_2 = {{3, 0, 0, 0}};
  s21_decimal s21_res = {{0b00001010101010101010101010101011,
                          0b00101001011011100000000110010110,
                          0b00010101100010101000100110010100,
                          0b00000000000111000000000000000000}}, result;
  int x = s21_div(value_1, value_2, &result);
  equal_bits(result, s21_res);
  ck_assert_int_eq(0, x);
}
END_TEST
#endif

#ifdef TEST_NAME_mul
START_TEST(s21_mul_t1) {
  s21_decimal dec1 = {{10, 0, 0, 0}}, dec2 = {{3, 0, 0, 0}},
              dec_res = {{30, 0, 0, 0}};
  s21_decimal result;
  int f_res = s21_mul(dec1, dec2, &result);
  for (int i = 0; i <= 3; i++) {
    unsigned int dr = dec_res.bits[i];
    unsigned int r = result.bits[i];
    ck_assert_int_eq(dr, r);
  }
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_mul_t2) {
  s21_decimal dec1 = {{0b00000000000000000001100101111001, 0, 0, 0}},  // 6521
      dec2 = {{0b00000000000000010010000110001001, 0, 0, 0}},          // 74121
      dec_res = {{0b00011100110011110011101011000001, 0, 0, 0}};  // 483343041
  s21_decimal result;
  int f_res = s21_mul(dec1, dec2, &result);
  for (int i = 0; i <= 3; i++) {
    unsigned int dr = dec_res.bits[i];
    unsigned int r = result.bits[i];
    ck_assert_int_eq(dr, r);
  }
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_mul_t3) {
  s21_decimal dec1 = {{0b01001100110100101000000111000101,
                       0b00000000000000000000000000000010, 0,
                       0b10000000000000000000000000000000}},   //  -9878798789
      dec2 = {{0b00000000000000000000000000000011, 0, 0, 0}},  // -3
      dec_res = {{0b11100110011101111000010101001111,
                  0b00000000000000000000000000000110, 0,
                  0b10000000000000000000000000000000}};  // -29636396367
  s21_decimal result;
  int f_res = s21_mul(dec1, dec2, &result);
  for (int i = 0; i <= 3; i++) {
    unsigned int dr = dec_res.bits[i];
    unsigned int r = result.bits[i];
    ck_assert_int_eq(dr, r);
  }
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_mul_t4) {
  s21_decimal
      dec1 =
          {{0b10000010111000100101101011101101,
            0b11111001111010000010010110101101,
            0b10110000001111101111000010010100,
            0b10000000000011100000000000000000}},  // -545454512454545.35265454545645
      dec2 =
          {{0b01001000000110110001111110011000,
            0b11111011111111011000100101101101,
            0b00000000000001000110110101110111,
            0b10000000000110000000000000000000}},  // -5.352654545456454545645464
      dec_res = {{0b01110001110011111110010111001111,
                  0b01000100010011101101011110011001,
                  0b01011110010101101001110000011100,
                  0b00000000000011010000000000000000}};
  s21_decimal result;
  int f_res = s21_mul(dec1, dec2, &result);

  for (int i = 0; i <= 3; i++) {
    unsigned int dr = dec_res.bits[i];
    unsigned int r = result.bits[i];
    ck_assert_int_eq(dr, r);
  }
  ck_assert_int_eq(0, f_res);
}
END_TEST

START_TEST(s21_mul_t4_2) {
  s21_decimal dec1 = {{0b01101101111100110111111101100111, 0, 0, 0}},
              dec2 = {{0b10111000110011100000000000000000,
                       0b10010011011101001111101010011100,
                       0b00000000000000001000101100000010,
                       0}};  // -5.352654545456454545645464
  s21_long_decimal long_1;
  s21_long_decimal long_2;
  long_decimal(dec1, &long_1);
  long_decimal(dec2, &long_2);

  int x = greater_or_equil_long(long_1, long_2);

  ck_assert_int_eq(0, x);
}
END_TEST

START_TEST(s21_mul_t5) {
  s21_decimal result;
  int f_res = s21_mul(decMax, decMax, &result);
  ck_assert_int_eq(1, f_res);
}
END_TEST
START_TEST(s21_mul_t6) {
  s21_decimal result;
  int f_res = s21_mul(decMin, decMin, &result);
  ck_assert_int_eq(1, f_res);
}
END_TEST
START_TEST(s21_mul_t7) {
  s21_long_decimal result = {{1, 1, 0, 0, 0, 0, 0, 0, 0, 0}};
  int f_res = check_if_long_dec_is_one(result);
  ck_assert_int_eq(0, f_res);
}
END_TEST
#endif

#ifdef TEST_NAME_float_to_dec
START_TEST(s21_from_float_to_decimal_1) {
  s21_decimal val;
  float x = 0.01f;
  s21_decimal res = {{1, 0, 0, 0x20000}};
  int result;
  result = s21_from_float_to_decimal(x, &val);
  for (int i = 0; i <= 3; i++) {
    unsigned int r = val.bits[i];
    unsigned int dr = res.bits[i];
    ck_assert_uint_eq(dr, r);
  }
  ck_assert_uint_eq(result, 0);
}
END_TEST

START_TEST(s21_from_float_to_decimal_1_1) {
  s21_decimal val;
  float x = -2000000000.0f;
  s21_decimal res = {{0b01110111001101011001010000000000, 0, 0, 0x80000000}};
  int result;
  result = s21_from_float_to_decimal(x, &val);
  for (int i = 0; i <= 3; i++) {
    unsigned int r = val.bits[i];
    unsigned int dr = res.bits[i];
    ck_assert_uint_eq(dr, r);
  }
  ck_assert_uint_eq(result, 0);
}
END_TEST

START_TEST(s21_from_float_to_decimal_2) {
  s21_decimal val;
  int result;
  result = s21_from_float_to_decimal(-128.023, &val);
  s21_decimal res = {{128023,
                      0, 0,
                      0x80030000}};
  for (int i = 0; i <= 3; i++) {
    unsigned int r = val.bits[i];
    unsigned int dr = res.bits[i];
    ck_assert_uint_eq(dr, r);
  }
  ck_assert_uint_eq(result, 0);
}
END_TEST

START_TEST(s21_from_float_to_decimal_3) {
  s21_decimal result = {{0x80000160, 0, 0, 0x80000000}};
  s21_decimal val;
  int f_res = s21_from_float_to_decimal(-2.1474836E+09, &val);
  equal_bits(val, result);
  ck_assert_int_eq(f_res, 0);
}
END_TEST

START_TEST(s21_from_float_to_decimal_4) {
  s21_decimal val;
  int result;
  result = s21_from_float_to_decimal(0.5, &val);
  ck_assert_int_eq(val.bits[0], 5);
  ck_assert_int_eq(val.bits[1], 0);
  ck_assert_int_eq(val.bits[2], 0);
  ck_assert_int_eq(val.bits[3], 0x10000);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(s21_from_float_to_decimal_5) {
  s21_decimal val;
  int result = s21_from_float_to_decimal(INFINITY, &val);
  ck_assert_int_eq(result, 1);
  result = s21_from_float_to_decimal(-INFINITY, &val);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(s21_from_float_to_decimal_6) {
  s21_decimal val;
  int result = s21_from_float_to_decimal(0.0f / 0.0, &val);
  ck_assert_int_eq(result, 1);
  result = s21_from_float_to_decimal(NAN, &val);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(s21_from_float_to_decimal_7) {
  s21_decimal val;
  int result = s21_from_float_to_decimal(1e-29, &val);
  ck_assert_int_eq(result, 1);
  result = s21_from_float_to_decimal(-1e-29, &val);
  ck_assert_int_eq(result, 1);
}
END_TEST

START_TEST(s21_from_float_to_decimal_8) {
  s21_long_decimal dec1 = {{10, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
  int result = quantity_numbers(dec1);
  ck_assert_int_eq(result, 2);
}
END_TEST
START_TEST(s21_from_float_to_decimal_9) {
  s21_decimal val;
  float x = 127.1234F;
  s21_decimal res = {{1271234, 0, 0, 0x00040000}};
  int result;
  result = s21_from_float_to_decimal(x, &val);
  for (int i = 0; i <= 3; i++) {
    unsigned int r = val.bits[i];
    unsigned int dr = res.bits[i];
    ck_assert_uint_eq(dr, r);
  }
  ck_assert_uint_eq(result, 0);
}
END_TEST
#endif

void equal_bits(s21_decimal dec_res, s21_decimal result) {
  for (int i = 0; i <= 3; i++) {
    unsigned int dr = dec_res.bits[i];
    unsigned int r = result.bits[i];
    ck_assert_uint_eq(dr, r);
  }
}

// test suite, test cases
Suite *s21_decimal_suite(void) {
  Suite *s;
  TCase *tc_core;
  s = suite_create("s21_decimal");
  // Core test case
  tc_core = tcase_create("Core");

#ifdef TEST_NAME_int_to_dec
  tcase_add_test(tc_core, s21_int_to_dec_test_1);
  tcase_add_test(tc_core, s21_int_to_dec_test_2);
  tcase_add_test(tc_core, s21_int_to_dec_test_3);
  tcase_add_test(tc_core, s21_int_to_dec_test_4);
#endif
#ifdef TEST_NAME_dec_to_int
  tcase_add_test(tc_core, s21_dec_to_int_test_1);
  tcase_add_test(tc_core, s21_dec_to_int_test_2);
  tcase_add_test(tc_core, s21_dec_to_int_test_3);
  tcase_add_test(tc_core, s21_dec_to_int_test_4);
  tcase_add_test(tc_core, s21_dec_to_int_test_5);
  tcase_add_test(tc_core, s21_dec_to_int_test_6);
#endif
#ifdef TEST_NAME_negate
  tcase_add_test(tc_core, s21_negate_1);
  tcase_add_test(tc_core, s21_negate_2);
  tcase_add_test(tc_core, s21_negate_3);
#endif
#ifdef TEST_NAME_truncate
  tcase_add_test(tc_core, s21_truncate_test_1);
  tcase_add_test(tc_core, s21_truncate_test_2);
  tcase_add_test(tc_core, s21_truncate_test_3);
#endif

#ifdef TEST_NAME_floor
  tcase_add_test(tc_core, s21_floor_t1);
  tcase_add_test(tc_core, s21_floor_t2);
  tcase_add_test(tc_core, s21_floor_t3);
  tcase_add_test(tc_core, s21_floor_t4);
  tcase_add_test(tc_core, s21_floor_t5);
  tcase_add_test(tc_core, s21_floor_t6);
  tcase_add_test(tc_core, s21_floor_t7);
  tcase_add_test(tc_core, s21_floor_t8);
  tcase_add_test(tc_core, s21_floor_t9);
#endif

#ifdef TEST_NAME_round
  tcase_add_test(tc_core, s21_round_t1);
  tcase_add_test(tc_core, s21_round_t2);
  tcase_add_test(tc_core, s21_round_t3);
  tcase_add_test(tc_core, s21_round_t4);
  tcase_add_test(tc_core, s21_round_t5);
  tcase_add_test(tc_core, s21_round_t6);
  tcase_add_test(tc_core, s21_round_t7);
  tcase_add_test(tc_core, s21_round_t8);
  tcase_add_test(tc_core, s21_round_t9);
  tcase_add_test(tc_core, s21_round_t10);
  tcase_add_test(tc_core, s21_round_t11);
#endif

#ifdef TEST_NAME_less
  tcase_add_test(tc_core, s21_less_t1);
  tcase_add_test(tc_core, s21_less_t2);
  tcase_add_test(tc_core, s21_less_t3);
  tcase_add_test(tc_core, s21_less_t4);
#endif

#ifdef TEST_NAME_less_or_equal
  tcase_add_test(tc_core, s21_less_or_equal_t1);
  tcase_add_test(tc_core, s21_less_or_equal_t2);
  tcase_add_test(tc_core, s21_less_or_equal_t3);
#endif

#ifdef TEST_NAME_greater
  tcase_add_test(tc_core, s21_greater_t1);
  tcase_add_test(tc_core, s21_greater_t2);
  tcase_add_test(tc_core, s21_greater_t3);
#endif

#ifdef TEST_NAME_greater_or_equal
  tcase_add_test(tc_core, s21_greater_or_equal_t0_1);
  tcase_add_test(tc_core, s21_greater_or_equal_t0);
  tcase_add_test(tc_core, s21_greater_or_equal_t1);
  tcase_add_test(tc_core, s21_greater_or_equal_t2);
  tcase_add_test(tc_core, s21_greater_or_equal_t3);
  tcase_add_test(tc_core, s21_greater_or_equal_t4);
  tcase_add_test(tc_core, s21_greater_or_equal_t5);
  tcase_add_test(tc_core, s21_greater_or_equal_t6);
  tcase_add_test(tc_core, s21_greater_or_equal_t7);
  tcase_add_test(tc_core, s21_greater_or_equal_t8);
  tcase_add_test(tc_core, s21_greater_or_equal_t9);
  tcase_add_test(tc_core, s21_greater_or_equal_t10);
#endif

#ifdef TEST_NAME_equal
  tcase_add_test(tc_core, s21_equal_t1);
  tcase_add_test(tc_core, s21_equal_t2);
  tcase_add_test(tc_core, s21_equal_t3);
  tcase_add_test(tc_core, s21_equal_t4);
  tcase_add_test(tc_core, s21_equal_t5);
  tcase_add_test(tc_core, s21_equal_t6);
  tcase_add_test(tc_core, s21_equal_t7);
  tcase_add_test(tc_core, s21_equal_t8);
  tcase_add_test(tc_core, s21_equal_t9);
#endif

#ifdef TEST_NAME_not_equal
  tcase_add_test(tc_core, s21_not_equal_t1);
  tcase_add_test(tc_core, s21_not_equal_t2);
  tcase_add_test(tc_core, s21_not_equal_t3);
  tcase_add_test(tc_core, s21_not_equal_t4);
  tcase_add_test(tc_core, s21_not_equal_t5);
  tcase_add_test(tc_core, s21_not_equal_t6);
  tcase_add_test(tc_core, s21_not_equal_t7);
  tcase_add_test(tc_core, s21_not_equal_t8);
#endif

#ifdef TEST_NAME_mod
  tcase_add_test(tc_core, s21_mod_t1);
  tcase_add_test(tc_core, s21_mod_t2);
  tcase_add_test(tc_core, s21_mod_t3);
  tcase_add_test(tc_core, s21_mod_t4);
  tcase_add_test(tc_core, s21_mod_t5);
  tcase_add_test(tc_core, s21_mod_t6);
  tcase_add_test(tc_core, s21_mod_t7);
  tcase_add_test(tc_core, s21_mod_t8);
  tcase_add_test(tc_core, s21_mod_t9);
  tcase_add_test(tc_core, s21_mod_t10);
  tcase_add_test(tc_core, s21_mod_t12);
  tcase_add_test(tc_core, s21_mod_t13);
  tcase_add_test(tc_core, s21_mod_t14);
  tcase_add_test(tc_core, s21_mod_t15);
  tcase_add_test(tc_core, s21_mod_t16);
  tcase_add_test(tc_core, s21_mod_t17);
  tcase_add_test(tc_core, s21_mod_t18);
  tcase_add_test(tc_core, s21_mod_t19);
  tcase_add_test(tc_core, s21_mod_t20);
  tcase_add_test(tc_core, s21_mod_t21);
  tcase_add_test(tc_core, s21_mod_t22);
  tcase_add_test(tc_core, s21_mod_t23);
#endif

#ifdef TEST_NAME_add
  tcase_add_test(tc_core, s21_add_t1);
  tcase_add_test(tc_core, s21_add_t2);
  tcase_add_test(tc_core, s21_add_t3);
  tcase_add_test(tc_core, s21_add_t4);
  tcase_add_test(tc_core, s21_add_t5);
  tcase_add_test(tc_core, s21_add_t6);
  tcase_add_test(tc_core, s21_add_t7);
  tcase_add_test(tc_core, s21_add_t8);
  tcase_add_test(tc_core, s21_add_t9);
  tcase_add_test(tc_core, s21_add_t10);
  tcase_add_test(tc_core, s21_add_t11);
  tcase_add_test(tc_core, s21_add_t12);
  tcase_add_test(tc_core, s21_add_t13);
  tcase_add_test(tc_core, s21_add_t14);
  tcase_add_test(tc_core, s21_add_t15);
  tcase_add_test(tc_core, s21_add_t16);
  tcase_add_test(tc_core, s21_add_t17);
  tcase_add_test(tc_core, s21_add_t18);
  tcase_add_test(tc_core, s21_add_t19);
  tcase_add_test(tc_core, s21_add_t20);
  tcase_add_test(tc_core, s21_add_t21);
  tcase_add_test(tc_core, s21_add_t22);
  tcase_add_test(tc_core, s21_add_t23);
#endif

#ifdef TEST_NAME_sub
  tcase_add_test(tc_core, s21_sub_t0);
  tcase_add_test(tc_core, s21_sub_t1);
  tcase_add_test(tc_core, s21_sub_t2);
  tcase_add_test(tc_core, s21_sub_t3);
  tcase_add_test(tc_core, s21_sub_t3_3);
  tcase_add_test(tc_core, s21_sub_t4);
  tcase_add_test(tc_core, s21_sub_t5);
  tcase_add_test(tc_core, s21_sub_t6);
  tcase_add_test(tc_core, s21_sub_t7);
  tcase_add_test(tc_core, s21_sub_t8);
  tcase_add_test(tc_core, s21_sub_t9);
  tcase_add_test(tc_core, s21_sub_t10);
  tcase_add_test(tc_core, s21_sub_t11);
  tcase_add_test(tc_core, s21_sub_t12);
  tcase_add_test(tc_core, s21_sub_t13);
  tcase_add_test(tc_core, s21_sub_t14);
  tcase_add_test(tc_core, s21_sub_t15);
  tcase_add_test(tc_core, s21_sub_t16);
  tcase_add_test(tc_core, s21_sub_t17);
  tcase_add_test(tc_core, s21_sub_t18);
  tcase_add_test(tc_core, s21_sub_t19);
  tcase_add_test(tc_core, s21_sub_t20);
  tcase_add_test(tc_core, s21_sub_t21);
  tcase_add_test(tc_core, s21_sub_t22);
  tcase_add_test(tc_core, s21_sub_t23);
#endif

#ifdef TEST_NAME_div
  tcase_add_test(tc_core, s21_div_t01);
  tcase_add_test(tc_core, s21_div_t0);
  tcase_add_test(tc_core, s21_div_t1);
  tcase_add_test(tc_core, s21_div_t2);
  tcase_add_test(tc_core, s21_div_t3);
  tcase_add_test(tc_core, s21_div_t4);
  tcase_add_test(tc_core, s21_div_t5);
  tcase_add_test(tc_core, s21_div_t6);
  tcase_add_test(tc_core, s21_div_t7);
  tcase_add_test(tc_core, s21_div_t7_1);
  tcase_add_test(tc_core, s21_div_t8);
  tcase_add_test(tc_core, s21_div_t9);
  tcase_add_test(tc_core, s21_div_t10);
  tcase_add_test(tc_core, s21_div_t11);
  tcase_add_test(tc_core, s21_div_t12);
  tcase_add_test(tc_core, s21_div_t13);
  tcase_add_test(tc_core, s21_div_t14);
  tcase_add_test(tc_core, s21_div_t15);
#endif

#ifdef TEST_NAME_mul
  tcase_add_test(tc_core, s21_mul_t1);
  tcase_add_test(tc_core, s21_mul_t2);
  tcase_add_test(tc_core, s21_mul_t3);
  tcase_add_test(tc_core, s21_mul_t4);
  tcase_add_test(tc_core, s21_mul_t4_2);
  tcase_add_test(tc_core, s21_mul_t5);
  tcase_add_test(tc_core, s21_mul_t6);
  tcase_add_test(tc_core, s21_mul_t7);
#endif

#ifdef TEST_NAME_float_to_dec
  tcase_add_test(tc_core, s21_from_float_to_decimal_1);
  tcase_add_test(tc_core, s21_from_float_to_decimal_1_1);
  tcase_add_test(tc_core, s21_from_float_to_decimal_2);
  tcase_add_test(tc_core, s21_from_float_to_decimal_3);
  tcase_add_test(tc_core, s21_from_float_to_decimal_4);
  tcase_add_test(tc_core, s21_from_float_to_decimal_5);
  tcase_add_test(tc_core, s21_from_float_to_decimal_6);
  tcase_add_test(tc_core, s21_from_float_to_decimal_7);
  tcase_add_test(tc_core, s21_from_float_to_decimal_8);
  tcase_add_test(tc_core, s21_from_float_to_decimal_9);
#endif

#ifdef TEST_NAME_dec_to_float
  tcase_add_test(tc_core, s21_dec_to_float_test_1);
  tcase_add_test(tc_core, s21_dec_to_float_test_2);
  tcase_add_test(tc_core, s21_from_decimal_to_float_2);
  tcase_add_test(tc_core, s21_from_decimal_to_float_3);
  tcase_add_test(tc_core, s21_from_decimal_to_float_4);
  tcase_add_test(tc_core, s21_from_decimal_to_float_5);
  tcase_add_test(tc_core, s21_from_decimal_to_float_6);

#endif

  suite_add_tcase(s, tc_core);
  return s;
}

int main() {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = s21_decimal_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
