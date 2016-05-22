#include <gtest/gtest.h>
#include <stdint.h>
#include <containers/bit/bits_field.h>
#include <string>

#include "utils.h"

TEST(SimpleTest, Basic) 
{
	containers::bits_field<uint64_t> field(64);
	
	
	bool a = field[2].get();
	field[3].set(true);
	bool a1 = field[3].get();
	PRINTF("%d\n", a1);
	PRINTF("%d\n", a);
	field[45].set(true);
	field[45].set(false);
	PRINTF("- %d\n", (bool) field[45]);
	field[45] = true;
	PRINTF("- %d\n", (int) field[45]);
	PRINTF("%s\n", field.dump().c_str());

	uint64_t b = static_cast<uint64_t>(field[12]);
	PRINTF("%lu\n", b);
}
