#include "difference_of_squares.h"

unsigned int sum_of_squares(unsigned int number)
{
   unsigned sum = 0;
   for (unsigned x = 1; x <= number; ++x) {
      sum += x * x;
   }
   return sum;
}

unsigned int square_of_sum(unsigned int number) {
   unsigned sum = 0;
   for (unsigned x = 1; x <= number; ++x) {
      sum += x;
   }
   return sum * sum;
}

unsigned int difference_of_squares(unsigned int number) {
   return square_of_sum(number) - sum_of_squares(number);
}
