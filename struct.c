#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <iterator>
#include <fstream>
#include <algorithm>

#include "zebra.hpp"
#include "city.hpp"

/* ノードの構造体 */
typedef struct _n {
//  int id;              /* 添字 */
  std::string str;
  int prefix;             /* prefix(32ビット) */
  int asc;                //asc
  struct _n *left;     /* 左 -- 自己参照 */
  struct _n *right;    /* 右 -- 自己参照 */
  bool blackOrWhite;
} _node;

int toTwo(uint decimal)
{
  /* 変数の宣言 */
  int i;
  int binary[32];

  for(i=0;decimal>0;i++){
    binary[i] = decimal % 2;
    decimal = decimal / 2;
  }

  int count = i;

  /* 2進数の出力 */
  printf(" 2進数 = ");
  while( i>0 ){
    printf("%d", binary[--i]);
  }
  printf("\n");

  return count;
}


int maskPrefix(int deep,int prefix){
  prefix = prefix & (1 << deep);
  return prefix << (32 - deep);
}

/* 二分探索木の生成 -- 再帰を利用*/
void maketree(_node *p1, int deep,entry_type entry,std::string str)
{
  _node *p2;
  //該当のprefixの０１を入手する。
  if (deep > entry.length)
  {
    printf("break\n");
    return;
  }
  std::cout << "\np1->str = " + p1->str + "str = " + str;

  if (p1->str == str){
    //一致している場合これが、黒ノード
    p1->asc = entry.number;
    p1->blackOrWhite = true;
    std::cout << "\nsuccess! prefix = " + p1->str;
  }else{
    //prefixを10101であれば最後に0を足して
    /* 値の大小によって左右に振り分ける */
    if (str[deep++] == '1') {    /* 主ノードより大きいとき */
      /* 右がNULLならそこに新たなノードをぶら下げる */
      if (p1->right == NULL) {
        //0加えたprefixを右なので1に変える
        p2 = static_cast<_node *>(malloc(sizeof(_node)));
        p2->left = NULL;
        p2->right = NULL;
        p2->str += p1-> str + "1";
        //std::cout << "\nnext prefix = " + p2->str;
        p1->right = p2;
        maketree(p1->right, deep, entry,str);
      } else {  /* NULLでなければ右側のノードに移動 */
        printf("right node is already exist!\n");
        maketree(p1->right, deep, entry,str);
      }
    } else {
      /* 左がNULLならそこに新たなノードをぶら下げる */
      if (p1->left == NULL) {
        p2 = static_cast<_node *>(malloc(sizeof(_node)));
        p2->blackOrWhite = false;
        p2->left = NULL;
        p2->right = NULL;
        p2->str += p1-> str +  "0";
        //std::cout << "\nnext prefix = " + p2->str;
        p1->left = p2;
        maketree(p1->left, deep, entry,str);
      } else {  /* NULLでなければ左側のノードに移動 */
        printf("left node is already exist!\n");
        maketree(p1->left, deep, entry,str);
      }
    }
  }
}

// void makearray(entry_type entry,char array[]){
//   int nonExistLength = 32 - toTwo(entry.prefix);//25
//   int k = 0;
//   for (k = 0; k < nonExistLength; ++k)
//   {
//     if (k < entry.length)
//     {
//       array[k] = '0';
//     }
//   }
//   int i;
//   int binary[32];
//   int decimal = entry.prefix >> (32 - entry.length);
//   int count =0;

//   for(i=0;decimal>0;i++){
//     binary[i] = decimal % 2;
//     decimal = decimal / 2;
//     count = i;
//   }

//   while(count >= 0){
//     if (k < entry.length){
//       if (binary[count] == 1)
//       {
//         array[k] = '1';
//       }else{
//         array[k] = '0';
//       }
//     }else{
//       break;
//     }
//     k++;
//     /* code */
//     count--;
//   }

//   toTwo(entry.prefix);
//   printf("array = %s\n", array);
// }


std::string makearray(entry_type entry,std::string str){
  int nonExistLength = 32 - toTwo(entry.prefix);//25
  int k = 0;
  for (k = 0; k < nonExistLength; ++k)
  {
    if (k < entry.length)
    {
      str+= "0";
    }
  }
  int i;
  int binary[32];
  int decimal = entry.prefix >> (32 - entry.length);
  int count =0;

  for(i=0;decimal>0;i++){
    binary[i] = decimal % 2;
    decimal = decimal / 2;
    count = i;
  }

  while(count >= 0){
    if (k < entry.length){
      if (binary[count] == 1)
      {
        str+= "1";
      }else{
        str+= "0";
      }
    }else{
      break;
    }
    k++;
    /* code */
    count--;
  }
  std::cout << "str = " + str;
  return str;
}

// int judgeBinary(){

// }

/* 木構造をたどって探索 */
int search(_node *p, int n)
{
  while (p != NULL) {
    if (n == p->prefix) {
      return(0);  /* 値が見付かれば終了 */
    } else if (n <= p->prefix) {
      p = p->left;
    } else {
      p = p->right;
    }
  }
  /* ループを抜け出たということは見付からなかったということ */
  printf("%d は見付かりませんでした。¥n", n);
  return(-1);
}

void strinit(char s[], int num)
{
  int i;

  for (i = 0; i < num; i++) {
    s[i] = '\0';
  }
}

/* ---------------------------------------------
   main
   ---------------------------------------------- */
_node* makeOldStruct(std::vector<entry_type> entry_list)
{
printf("start\n");
  _node *start, *p1;
  /* 先頭のノードを初期化 */
  start = static_cast<_node *>(malloc(sizeof(_node)));
  start->prefix = 0;
  start->asc = 0;
  start->left = NULL;
  start->right = NULL;
  start->str = "";
  p1 = start;


  /* 残る値から木構造を生成 */
  for (int i = 0;entry_list.size() != i; ++i)
  {
    // char array[entry_list[i].length+1];
    // strinit(array,entry_list[i].length+1);
    // makearray(entry_list[i],array);
    std::string str = "";
    str = makearray(entry_list[i],str);
    //32-acs文削る
    maketree(p1, 0, entry_list[i],str);
    /* code */
  }
  /* 木構造をたどって探索 */
  return start;
}


//https://www.grapecity.com/tools/support/powernews/column/clang/052/page03.htmを
//をコピペした
