/**
 * @brief
 * @author Yuki Koizumi
 */
#ifndef ZEBRA_HPP_INC
#define ZEBRA_HPP_INC

#include <cstdint>
#include <vector>

class entry_type
{
public:
    entry_type(uint32_t p, uint32_t l, uint32_t n) : prefix(p), length(l), number(n) {}
    entry_type(const entry_type &obj) : prefix(obj.prefix), length(obj.length), number(obj.number) {}
    uint32_t prefix;
    uint32_t length;
    uint32_t number;
};

std::ostream& operator<<(std::ostream &os, const entry_type &v);
void read_zebra_data(std::vector<entry_type> &container, const char *filename);

#endif
