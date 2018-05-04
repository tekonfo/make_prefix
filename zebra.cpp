/**
 * @brief
 * @author Yuki Koizumi
 */
#include "zebra.hpp"

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/lexical_cast.hpp>
#include <cstdint>
#include <string>
#include <vector>
#include <iterator>
#include <fstream>
#include <algorithm>
#include <iostream>

std::ostream& operator<<(std::ostream &os, const entry_type &v)
{
    os << v.prefix
       << " (0x" << std::hex << v.prefix << std::dec << "), "
       << v.length << ", "
       << v.number;
    return os;
}

void read_zebra_data(std::vector<entry_type> &container, const char *filename)
{
    std::ifstream infile(filename);
    if(infile.fail()) {
        std::cerr << "cannot open file: " << filename << std::endl;
        exit(1);
    }

    std::string line;
    try {
        while(!infile.eof()) {
            std::getline(infile, line);

            boost::algorithm::trim(line);
            if(line.empty()) continue;

            std::vector<std::string> str_element;
            boost::algorithm::split(str_element, line, boost::algorithm::is_space(), boost::algorithm::token_compress_on);

            std::vector<std::string> prefix_element;
            boost::algorithm::split(prefix_element, str_element.at(0), boost::is_any_of("/"), boost::algorithm::token_compress_on);

            std::vector<std::string> ip_element;
            boost::algorithm::split(ip_element, prefix_element.at(0), boost::is_any_of("."), boost::algorithm::token_compress_on);

            std::vector<uint32_t> ip_val_list;
            std::transform(ip_element.begin(), ip_element.end(),
                           std::back_inserter(ip_val_list),
                           [](const std::string &x) { return boost::lexical_cast<uint32_t>(x); });

            uint32_t number = boost::lexical_cast<uint32_t>(str_element.at(1));
            uint32_t length = boost::lexical_cast<uint32_t>(prefix_element.at(1));
            uint32_t prefix = (ip_val_list.at(0) << 24) | (ip_val_list.at(1) << 16) | (ip_val_list.at(2) << 8) | ip_val_list.at(3);

            container.push_back(entry_type(prefix, length, number));
        }
    }
    catch(boost::bad_lexical_cast &cast_error) {
        infile.close();
        std::cerr << cast_error.what() << std::endl;
        exit(1);
    }
    catch(std::exception &e) {
        infile.close();
        std::cerr << e.what() << std::endl;
        exit(1);
    }

    return;
}
