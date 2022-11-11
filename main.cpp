#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <set>

// class ServerBlockParser : public ConfigParser
// {
// 	private:
// 		std::vector<std::string> parseIndex()
// 		{
			
// 		}
// };

class ConfigParser
{
	public:
		std::vector<std::string> splitStr(std::string str, char delimiter)
		{
			std::istringstream iss(str);
			std::string buf;
			std::vector<std::string> res;

			while (getline(iss, buf, delimiter))
			{
				if (buf.size() > 0)
				{
					res.push_back(buf);
				}
			}
			return res;
		}

	public:
		void parseFile(std::string filename)
		{
			std::ifstream file(filename);
			std::string line;
			std::vector<std::string> words;

			if (file.is_open())
			{
				while (getline(file, line))
				{
					std::replace_if(line.begin(), line.end(), isspace, ' ');
					words = splitStr(line, ' ');
					for (int i = 0; i < words.size(); ++i)
						std::cout << words[i] << " ";
					std::cout << std::endl;
				}
			}
			else
			{
				std::cout << "fail to open file" << std::endl;
			}
		}
};

int main(int ac, char **av)
{
	std::string line;
	ConfigParser cf;

	if (ac != 2)
		return 1;
	cf.parseFile(av[1]);
	return 0;
}
