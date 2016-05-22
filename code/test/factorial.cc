#include <gtest/gtest.h>
#include <utils/factorial.h>

#include "utils.h"


TEST(SimpleTest, Basic) 
{
	
	double a = factorial(100.);
	int t = trailingZero(100);

	PRINTF("%f\n", a);

	PRINTF("%d\n", t);
}


TEST(StringInverse, Basic) 
{
	
	char a[] = "hello";
	for (int i = 0, n = strlen(a), l = n / 2; i < l; i++)
	{
		char tmp = *(a + i);
		*(a + i) = a[n - 1 - i];
		a[n - 1 - i] = tmp;
	}

	PRINTF("%s\n", a);
}