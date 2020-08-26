#include <iostream>

unsigned int getLimit() {
	unsigned int ret = 0;
	std::cout << "Upper limit: ";
	std::cin >> ret;

	return ret;
}

int main(int, char**) {
	unsigned long long sum = 0;
	auto limit = getLimit();
	for (auto i = 3u; i < limit; i++) {
		if (i % 3 == 0 && i % 5 == 0)
			sum += i;
	}

	std::cout << "sum: " << sum << std::endl;
	return 0;
}
