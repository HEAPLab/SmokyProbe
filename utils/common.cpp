
#include <iostream>
#include <string>

#include "common.h"

namespace smokyutil {


std::list<unsigned int> get_channels_from_str(const char * str)
{
	std::string ch_str(str);
	std::list<unsigned int> ch_list;

	size_t end_pos = 0;
	do {
		end_pos = ch_str.find(",");
		std::string token = ch_str.substr(0, end_pos);
		ch_list.push_back(std::stoi(token));
		ch_str = ch_str.substr(end_pos+1);
	} while (end_pos != std::string::npos);

	return ch_list;
}


}

