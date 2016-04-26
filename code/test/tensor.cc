#include <gtest/gtest.h>
#include <containers/tensor/tensor.h>

#include "utils.h"


TEST(SimpleTest, Basic) 
{
	containers::ftensor t({4, 4});
	t[2][2] = 100;

	//PRINTF("%f\n", (float) t[2][2]);
}