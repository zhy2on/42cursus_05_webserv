#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

std::vector<std::string> split(std::string str, char delimiter)
{
	std::istringstream iss(str);
	std::string buf;
	std::vector<std::string> res;

	while (getline(iss, buf, delimiter))
	{
		res.push_back(buf);
	}
	return res;
}


int main(int ac, char **av)
{
	std::string line;

	if (ac != 2)
		return 1;

	std::ifstream file(av[1]);

	if (file.is_open())
	{
		while (getline(file, line))
		{
			std::vector<std::string> words;
			std::replace_if(line.begin(), line.end(), isspace, ' ');
			words = split(line, ' ');
			for (int i = 0; i < words.size(); i++)
				std::cout << words[i] << " ";
			std::cout << std::endl;
		}
	}
	else
	{
		std::cout << "fail to open file" << std::endl;
		return 1;
	}
	return 0;
}
