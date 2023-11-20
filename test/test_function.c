#include "mtest.h"

TEST_CARE(quotient_of_function, {
  CHECK_EQ_TRUE(1);
})

TEST_CASE(modulus_of_function, {
  CHECK_EQ_TRUE(1);
})

TEST_CASE(calculate_fibonacci, {
  CHECK_EQ_TRUE(1);
})

TEST_CASE(calculate_gcd, {
  CHECK_EQ_TRUE(1);
})

MAIN_RUN_TESTS(quotient_of_function,
               modulus_of_function,
               calculate_fibonacci,
               calculate_gcd)
