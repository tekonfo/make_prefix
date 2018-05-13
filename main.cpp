#include <iostream>
#include <vector>
#include <iterator>
#include <fstream>
#include <algorithm>

#include "zebra.hpp"
#include "city.hpp"
#include "shift.c"
//#include "struct.c"
#include "newStract.c"

int main(int argc, char *argv[])
{
    if(argc < 2) {
        std::cerr << "Usage: " << argv[0] << " input" << std::endl;
        return -1;
    }

    // how to use cityhash lib
    // uint32_t input = 1233;
    // uint32_t val = CityHash32((char *)&input, 4);
    // std::cout << val << std::endl;
    // return 0;

    std::vector<entry_type> entry_list;

    // 初期設定
    uint32_t cycles_high_s, cycles_low_s, cycles_high_e, cycles_low_e;
    __asm__ volatile ("CPUID\n\t"
                      "RDTSC\n\t"
                      "mov %%edx, %0\n\t"
                      "mov %%eax, %1\n\t": "=r" (cycles_high_s), "=r" (cycles_low_s)::
                      "%rax", "%rbx", "%rcx", "%rdx");
    __asm__ volatile ("RDTSC\n\t"
                      "mov %%edx, %0\n\t"
                      "mov %%eax, %1\n\t"
                      "CPUID\n\t": "=r" (cycles_high_e), "=r" (cycles_low_e)::
                      "%rax", "%rbx", "%rcx", "%rdx");
    __asm__ volatile ("CPUID\n\t"
                      "RDTSC\n\t"
                      "mov %%edx, %0\n\t"
                      "mov %%eax, %1\n\t": "=r" (cycles_high_s), "=r" (cycles_low_s)::
                      "%rax", "%rbx", "%rcx", "%rdx");
    __asm__ volatile ("RDTSC\n\t"
                      "mov %%edx, %0\n\t"
                      "mov %%eax, %1\n\t"
                      "CPUID\n\t": "=r" (cycles_high_e), "=r" (cycles_low_e)::
                      "%rax", "%rbx", "%rcx", "%rdx");

    // start tsc measurement
    __asm__ volatile ("CPUID\n\t"
                      "RDTSC\n\t"
                      "mov %%edx, %0\n\t"
                      "mov %%eax, %1\n\t": "=r" (cycles_high_s), "=r" (cycles_low_s)::
                      "%rax", "%rbx", "%rcx", "%rdx");

    read_zebra_data(entry_list, argv[1]);
    shiftToBit(entry_list);
    std::string str = makebit_a(argv[2],argv[3],argv[4],argv[5]);
    makeStruct(entry_list,str);


    // end tsc measurement
    __asm__ volatile("RDTSCP\n\t"
                     "mov %%edx, %0\n\t"
                     "mov %%eax, %1\n\t"
                     "CPUID\n\t": "=r" (cycles_high_e), "=r" (cycles_low_e)::
                     "%rax", "%rbx", "%rcx", "%rdx");

    uint64_t start_clock = (((uint64_t)cycles_high_s << 32) | cycles_low_s);
    uint64_t end_clock   = (((uint64_t)cycles_high_e << 32) | cycles_low_e);

    std::cout << end_clock - start_clock << std::endl;

    // std::copy(entry_list.begin(), entry_list.end(), std::ostream_iterator<entry_type>(std::cout, "\n"));
    std::for_each(entry_list.begin(), entry_list.end(), [](const entry_type& obj) {
            std::cout << obj.prefix << std::endl;
        });

    return 0;
}
