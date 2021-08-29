#include <iostream>

#ifdef BUILD_NUMBER
constexpr int buildNumber = BUILD_NUMBER;
#else
constexpr int buildNumber = std::numeric_limits<int>::max();
#endif

int main()
{
	std::cout << "build " << buildNumber << std::endl;
	std::cout << "Hello, World!" << std::endl;
	int dummyInput{ 0 };
	std::cin >> dummyInput;

	return 0;
}
