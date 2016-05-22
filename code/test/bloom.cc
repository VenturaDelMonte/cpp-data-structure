#include <gtest/gtest.h>
#include <stdint.h>
#include <containers/bloom/bloom_filter.h>
#include <cstring>

#include "utils.h"

TEST(SimpleTest, Basic) 
{
	containers::bloom_filter<std::string> bf(1000, 0.01);
	
	bf.add("asd");

	bool res = bf.test("asd");

	PRINTF("%d\n", res);


	res = bf.test("asd2");

	PRINTF("%d\n", res);

	res = bf.test("pigi merda");

	PRINTF("%d\n", res);

}


TEST(TestChar, Basic) 
{
	containers::bloom_filter<const char*> bf(1000, 0.01);
	
	bf.add("asd");

	bool res = bf.test("asd");

	PRINTF("%d\n", res);


	res = bf.test("asd2");

	PRINTF("%d\n", res);

	res = bf.test("pigi merda");

	PRINTF("%d\n", res);

}