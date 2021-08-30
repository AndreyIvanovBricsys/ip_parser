#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <stdint.h>
#include <algorithm>
#include <fstream>
#include <functional>

class IPv4Address
{
public:
	IPv4Address(const std::vector<std::string> & stringValues)
	{
		if (stringValues.size() != 4)
		{
			throw std::invalid_argument{ "" };
		}

		std::transform(stringValues.cbegin(), stringValues.cend(), values.begin(),
			[](const auto & stringValue)
			{
				return static_cast<std::uint8_t>(std::stoi(stringValue));
			});
	}

	uint8_t getValueAt(size_t index) const
	{
		return values.at(index);
	}

	const std::array<uint8_t, 4> & getValues() const
	{
		return values;
	}

	bool operator<(const IPv4Address & other) const
	{
		for (decltype(values)::size_type index{ 0 }; index < values.size(); ++index)
		{
			if (values[index] < other.values[index])
			{
				return true;
			}
			else if (values[index] > other.values[index])
			{
				return false;
			}
		}

		return false;
	}

	bool operator>(const IPv4Address & other) const
	{
		return other < *this;
	}

private:
	std::array<uint8_t, 4> values;

	friend std::ostream & operator<<(std::ostream &, const IPv4Address &);
};

std::ostream & operator<<(std::ostream & out, const IPv4Address & address)
{
	auto & ipv4Values{ address.values };
	return out <<
		static_cast<int>(ipv4Values[0]) << '.' <<
		static_cast<int>(ipv4Values[1]) << '.' <<
		static_cast<int>(ipv4Values[2]) << '.' <<
		static_cast<int>(ipv4Values[3]);
}

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
std::vector<std::string> split(const std::string & str, char delimiter)
{
	std::vector<std::string> result;

	std::string::size_type start = 0;
	std::string::size_type stop = str.find_first_of(delimiter);
	while (stop != std::string::npos)
	{
		result.push_back(str.substr(start, stop - start));

		start = stop + 1;
		stop = str.find_first_of(delimiter, start);
	}

	result.push_back(str.substr(start));

	return result;
}

void processIPs(const std::vector<IPv4Address> & ipList,
	std::function<bool(const IPv4Address &)> accept,
	std::function<void(const IPv4Address &)> process)
{
	for (const auto & ip : ipList)
	{
		if (accept(ip))
		{
			process(ip);
		}
	}
}

int main(int argc, char const * argv[])
{
	try
	{
		std::string inputFilePath;
		std::cout << "Enter path to file with ips" << std::endl;
		std::cin >> inputFilePath;
		std::ifstream inputFile{ inputFilePath };

		std::string outputFilePath;
		std::cout << "Enter path to output file" << std::endl;
		std::cin >> outputFilePath;
		std::ofstream outputFile{ outputFilePath };

		std::vector<IPv4Address> ip_pool;

		for (std::string line; std::getline(inputFile, line);)
		{
			auto tokens{ split(line, '\t') };
			ip_pool.emplace_back(split(tokens.at(0), '.'));
		}

		std::sort(ip_pool.begin(), ip_pool.end(), std::greater<IPv4Address>());

		auto printIP = [&outputFile](const IPv4Address & ip)
		{
			outputFile << ip << std::endl;
		};

		processIPs(ip_pool, [](const IPv4Address &) { return true; }, printIP);

		// 222.173.235.246
		// 222.130.177.64
		// 222.82.198.61
		// ...
		// 1.70.44.170
		// 1.29.168.152
		// 1.1.234.8

		processIPs(ip_pool,
			[](const IPv4Address & ip)
			{
				return ip.getValueAt(0) == 1;
			},
			printIP);

		// 1.231.69.33
		// 1.87.203.225
		// 1.70.44.170
		// 1.29.168.152
		// 1.1.234.8

		processIPs(ip_pool,
			[](const IPv4Address & ip)
			{
				return  (ip.getValueAt(0) == 46) && (ip.getValueAt(1) == 70);
			},
			printIP);

		// 46.70.225.39
		// 46.70.147.26
		// 46.70.113.73
		// 46.70.29.76

		processIPs(ip_pool,
			[](const IPv4Address & ip)
			{
				auto & values{ ip.getValues() };
				return std::any_of(values.cbegin(), values.cend(), [](auto val) { return val == 46; });
			},
			printIP);

		// 186.204.34.46
		// 186.46.222.194
		// 185.46.87.231
		// 185.46.86.132
		// 185.46.86.131
		// 185.46.86.131
		// 185.46.86.22
		// 185.46.85.204
		// 185.46.85.78
		// 68.46.218.208
		// 46.251.197.23
		// 46.223.254.56
		// 46.223.254.56
		// 46.182.19.219
		// 46.161.63.66
		// 46.161.61.51
		// 46.161.60.92
		// 46.161.60.35
		// 46.161.58.202
		// 46.161.56.241
		// 46.161.56.203
		// 46.161.56.174
		// 46.161.56.106
		// 46.161.56.106
		// 46.101.163.119
		// 46.101.127.145
		// 46.70.225.39
		// 46.70.147.26
		// 46.70.113.73
		// 46.70.29.76
		// 46.55.46.98
		// 46.49.43.85
		// 39.46.86.85
		// 5.189.203.46
	}
	catch (const std::exception & e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}