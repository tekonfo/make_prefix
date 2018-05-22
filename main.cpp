#include <iostream>
#include <vector>
#include <iterator>
#include <fstream>
#include <algorithm>
#include <random>
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
    read_zebra_data(entry_list, argv[1]);
    printf("size = \n");
    std::cout << entry_list.size() << std::endl;
    printf("\n");
    //----------------------書き加えたもの、要素を
    std::random_device seed_gen;
    std::mt19937 engine(seed_gen());
    std::shuffle( entry_list.begin(), entry_list.end(),engine);
    entry_list.erase(entry_list.begin() + std::stoi(argv[2]), entry_list.end());
    //-------------------------------------
    shiftToBit(entry_list);

    //std::string str = makebit_a(argv[2],argv[3],argv[4],argv[5]);
    std::string str = "";
    _node *start = makeStruct(entry_list,str);


    // start tsc measurement

      //----------------もう一回シャッフル
    std::random_device seed_gean;
    std::mt19937 eangine(seed_gen());
    std::shuffle( entry_list.begin(), entry_list.end(),eangine);
      //-------------------------------
    uint64_t all_number = 0;


    for (int i = 0; i < 99; ++i)
    {
      std::string  prefix = make_answer_prefix(entry_list[i].prefix);
      uint64_t number = 0;

      __asm__ volatile ("CPUID\n\t"
                      "RDTSC\n\t"
                      "mov %%edx, %0\n\t"
                      "mov %%eax, %1\n\t": "=r" (cycles_high_s), "=r" (cycles_low_s)::
                      "%rax", "%rbx", "%rcx", "%rdx");

      search(start,prefix);


      __asm__ volatile("RDTSCP\n\t"
               "mov %%edx, %0\n\t"
               "mov %%eax, %1\n\t"
               "CPUID\n\t": "=r" (cycles_high_e), "=r" (cycles_low_e)::
               "%rax", "%rbx", "%rcx", "%rdx");

      uint64_t start_clock = (((uint64_t)cycles_high_s << 32) | cycles_low_s);
      uint64_t end_clock   = (((uint64_t)cycles_high_e << 32) | cycles_low_e);
      number = (end_clock - start_clock);
      std::cout << number << std::endl;

      all_number += number;
    }
    all_number /= 100;
    // end tsc measurement




    std::cout << all_number << std::endl;
    printf("node number = %d\n", count_node(start,0));
    // std::copy(entry_list.begin(), entry_list.end(), std::ostream_iterator<entry_type>(std::cout, "\n"));
    std::for_each(entry_list.begin(), entry_list.end(), [](const entry_type& obj) {
       //     std::cout << obj.prefix << std::endl;
        });

    return 0;
}
