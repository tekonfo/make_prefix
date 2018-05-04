#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <iterator>
#include <fstream>
#include <algorithm>
#include "zebra.hpp"
#include "city.hpp"
void shiftToBit(std::vector<entry_type> entry_list)
{
  //全ての配列に対してビットシフトを行う。
  for (int i = 0;entry_list.size() == i; ++i)
  {
    entry_list[i].prefix = entry_list[i].prefix << (32 - entry_list[i].length);
    /* code */
    //果たしてやる意味あるのだろうか？
  }
}
