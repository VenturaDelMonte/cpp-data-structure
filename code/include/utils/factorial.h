#pragma once

template <typename T> T factorial(T n, T acc = T(1))
{
	if (n == 1)
		return acc;
	else
		return factorial(n - 1, n * acc);
}

template <typename T> int trailingZero(T n)
{
	int count = 0;
	for (int i = 5; (n / i) >= 1; i *= 5) 
	{
		count += (n / i);
	}
	return count;
}