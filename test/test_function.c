#include "mtest.h"

// These are currently just dummy tests with dummy names.
// Feel free to remove them as tests get added.
TEST_CASE(quotient_of_function, {
  CHECK_TRUE(1);
})

TEST_CASE(modulus_of_function, {
  CHECK_TRUE(1);
})

TEST_CASE(calculate_fibonacci, {
  CHECK_TRUE(1);
})

TEST_CASE(calculate_gcd, {
  CHECK_TRUE(1);
})

MAIN_RUN_TESTS(quotient_of_function,
               modulus_of_function,
               calculate_fibonacci,
               calculate_gcd)
