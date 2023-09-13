#ifndef UTILS_HPP
# define UTILS_HPP

#include<iostream>
#include<sstream>
#include<vector>

std::vector<std::string> split(std::string input, char delimiter);
bool contains(std::string &haystack, std::string const &needles);
bool isNum(const std::string &str);
std::string parse_pw(std::string pw);

#endif