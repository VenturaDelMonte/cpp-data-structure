#include <gtest/gtest.h>
#include <containers/tree/rb_tree.h>

#include "utils.h"


TEST(SimpleTest, Basic) 
{
	containers::red_black_tree<char, int> tree;

	int a = 0;
	for (auto c : "puorkbound")
	{
		tree.add(c, a++);
	}

	int val = (int) tree['k'];
	PRINTF("%d\n", val);

	ASSERT_TRUE(4 == val);
}
