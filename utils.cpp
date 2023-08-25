#include"utils.hpp"

std::vector<std::string> split(std::string input, char delimiter)
{
    std::vector<std::string> answer;
    std::stringstream ss(input);
    std::string temp;

    while(getline(ss, temp, delimiter))
        answer.push_back(temp);

    return answer;
}

bool contains(std::string &haystack, std::string const &needles)
{
	for (u_int32_t i = 0; i < needles.length(); i++)
	{
		if (haystack.find(needles[i]) != std::string::npos)
			return (true);
	}
	return (false);
}

std::string parse_pw(std::string pw)
{
    for(unsigned int i = 0; i < pw.size(); i++)
    {
        if(!std::isprint(pw[i]))
            return pw.substr(0, i);
    }
    return pw;
}
