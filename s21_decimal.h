// "Copyright [year] <Copyright Owner>"
#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define S21_LONG 10
#define SIGN_MASK 0x80000000

typedef struct {
  int bits[4];
} s21_decimal;
typedef struct {
  unsigned int bits[S21_LONG];
} s21_long_decimal;

// арифметические операторы
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
// Функции возвращают код ошибки:

// 0 - OK
// 1 - число слишком велико или равно бесконечности
// 2 - число слишком мало или равно отрицательной бесконечности
// 3 - деление на 0

// операторы сравнения
int s21_is_less(s21_decimal, s21_decimal);
int s21_is_less_or_equal(s21_decimal, s21_decimal);
int s21_is_greater(s21_decimal, s21_decimal);
int s21_is_greater_or_equal(s21_decimal, s21_decimal);
int s21_is_equal(s21_decimal, s21_decimal);
int s21_is_not_equal(s21_decimal, s21_decimal);
// Возвращаемое значение:

// 0 - FALSE
// 1 - TRUE

// преобразователи
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);
// Возвращаемое значение - код ошибки:

// 0 - OK
// 1 - ошибка конвертации

// другие функции
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);
// Возвращаемое значение - код ошибки:

// 0 - OK
// 1 - ошибка вычисления

// ДОПОЛНИТЕЛЬНЫЕ ФУНКЦИИ
void initialization_decimal_number(s21_decimal *src);
int int_sign(int num);
int change_sign_int(int x);
int get_quantity_bit_pos_for_int(int x);
int getbit_pos_for_int(unsigned int x, int *pos);
void long_decimal(s21_decimal src, s21_long_decimal *dst);
s21_long_decimal s21_sub_long(s21_long_decimal value_1,
                              s21_long_decimal value_2);
s21_long_decimal s21_add_long(s21_long_decimal value_1,
                              s21_long_decimal value_2);
int check_if_long_dec_too_big(s21_long_decimal x);
s21_long_decimal s21_mul_long_help(s21_long_decimal value_1,
                                   s21_long_decimal value_2);
s21_long_decimal s21_mul_long(s21_long_decimal value_1,
                              s21_long_decimal value_2);
s21_long_decimal s21_div_long_end(s21_long_decimal value_1,
                                  s21_long_decimal divisor,
                                  s21_long_decimal *reminder);
s21_decimal div_10(s21_decimal src);
s21_long_decimal s21_div_long_drobnoe(s21_long_decimal value_1,
                                      s21_long_decimal divisor, int *scale);
int to_equal_scale(s21_decimal value_1, s21_decimal value_2,
                   s21_long_decimal *long_1, s21_long_decimal *long_2);
int build_dec_from_long(s21_long_decimal res, int scale, int sign,
                        s21_decimal *result, int B_round);
int check_if_long_dec_is_zero(s21_long_decimal x);
int check_if_long_dec_is_one(s21_long_decimal x);
int greater_or_equil_long(s21_long_decimal value_1, s21_long_decimal value_2);
s21_long_decimal creat_equal_long(s21_long_decimal value_1,
                                  s21_long_decimal value_2);
int check_if_dec_is_zero(s21_decimal x);
int isZero(s21_decimal dec);
int s21_bank_round(s21_long_decimal value, int scale, s21_long_decimal *result);
int build_dec_from_float(s21_long_decimal res, int scale, int sign,
                         s21_decimal *result);
int quantity_numbers(s21_long_decimal long_1);

#endif  // SRC_S21_DECIMAL_H_
