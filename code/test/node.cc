#include <gtest/gtest.h>
#include <containers/tree/node.h>

#include "utils.h"


TEST(SimpleTest, Basic) 
{
	containers::node<int, int> n1(2, 2);
	containers::node<int, int> n2(3, 2);
	PRINTF("Hello world \n");
	ASSERT_TRUE(n1 < n2);
}

TEST(VisitorTest, Basic) 
{
	
	containers::node<int, int> n2(3, 2);
	containers::node<int, int> n3(3, 2);

	containers::node<int, int> n1(2, 2);

	
	ASSERT_TRUE(n1 < n2);

}
