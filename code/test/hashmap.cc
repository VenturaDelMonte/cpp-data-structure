#include <gtest/gtest.h>
#include <containers/hashmap/hashmap.h>
#include <string>

#include "utils.h"

TEST(SimpleTest, Basic) 
{
	containers::hashmap<int, int> val;
	containers::hashmap<int, int> va1l;
	val = va1l;
	va1l = containers::hashmap<int, int>();

	containers::hashmap<int, int> val3(containers::hashmap<int, int>());
}



TEST(AddGetTest, Basic) 
{
	using namespace std;
	containers::hashmap<string, int> val;
	int value = 23;
	val.add("one", value);

	PRINTF("%d\n", val["one"]);	


	val.add("eno", 12);
	val.add("neo", 112);
	//val.add("none", 1112);

	PRINTF("%d\n", (int)val["eno"]);	
	PRINTF("%d\n", (int)val["neo"]);	
	PRINTF("%d\n", (int)val["none"]);	

	int a = 24;
	val["one"] = 24;
	PRINTF("%d\n", (int)val["one"]);

	ASSERT_TRUE(containers::proxy<int>::empty == val["none"]);
	ASSERT_TRUE(24 == int(val["one"]));
}