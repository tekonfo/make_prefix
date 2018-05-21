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
#include "search.c"
/* ノードの構造体 */
typedef struct _n {
//  int id;              /* 添字 */
  std::string str;
  int prefix;             /* prefix(32ビット) */
  int asc;                //asc
  struct _n *left;     /* 左 -- 自己参照 */
  struct _n *b_left;
  struct _n *right;    /* 右 -- 自己参照 */
  struct _n *b_right;
  int skipnum;
  std::string skipval;
  bool blackOrWhite;
} _node;


int search(_node *p,std::string gavePrefix)
{
  //std::cout << "\n gavePrefix = " + gavePrefix;
  std::string latest_long_prefix;
  uint ac_number = 0;
  std::string answer;
  while (p != NULL) {
    if (p->str.length() > 0)
    {
      answer = p->str.substr(0,p->str.size() -1) + p->skipval + p->str.back();
    }else{
      answer = p->skipval;
    }
      if (answer.size() > 0 && gavePrefix.substr(0,answer.size()) != answer)
      {
        // printf("route not match\n");
        // std::cout << "\nlatest_long_prefix = " + latest_long_prefix;
        // printf("ac番号は %uです\n",ac_number);
        return -1;
      }
    if (p->blackOrWhite == true)
    {
      latest_long_prefix = answer;
      ac_number = p->asc;
    }
    if (gavePrefix[answer.length()] == '1') {
      p = p->right;
    } else {
      p = p->left;
    }
  }
  // printf("最長経路は\n");
  // std::cout << latest_long_prefix;
  // printf("です\n");
  // printf("ac番号は %uです\n",ac_number);
  return -1;
}




int toTwo(uint decimal)
{
  int i;
  int binary[33];
  for(i=0;decimal>0;i++){
    binary[i] = decimal % 2;
    decimal = decimal / 2;
  }
  int count = i;
  // printf(" 2進数 = ");
  // while( i>0 ){
  //   printf("%d", binary[--i]);
  // }
  // printf("\n");
  return count;
}


int maskPrefix(int deep,int prefix){
  prefix = prefix & (1 << deep);
  return prefix << (32 - deep);
}

void set_node(_node *p2,std::string str,
  std::string skipval,int skipnum,bool blackOrWhite
  ,struct _n *left,struct _n *right,struct _n *b_left,struct _n *b_right){
    p2->left = left;
    p2->right = right;
    p2->b_left = b_left;
    p2->b_right = b_right;
    p2->skipnum = skipnum;
    p2->skipval = skipval;
    p2->str = str;
    p2->blackOrWhite = blackOrWhite;
}

std::string get_answer(_node *p1,std::string last_skipval){
  std::string answer = "";

  if (p1->str.size() > 0)
  {
    answer = p1->str.substr(0,p1->str.size() -1) + p1->skipval + p1->str.back();
  }else{
    answer = p1->skipval;
  }
  //std::cout << "\nanswer = " + answer;
  return answer;
}

int rollback_node(_node *p1,_node *p2){
  if (p1->b_left == NULL)
  {
    //printf("parent is b_right\n");
    _node *p0 = p1->b_right;
    p0->left = p2;
    return 1;
  }else{
    //printf("parent is b_left\n");
    _node *p0 = p1->b_left;
    p0->right = p2;
    return 0;
  }
}



int search_not_match(std::string answer,std::string givenPrefix){
  int count = 0;
  while(1){
    if (givenPrefix[count] == answer[count])
    {
      count++;
    }else{
      return count;
    }
  }
}

int child_is_left(int not_match,std::string answer){
  if (answer[not_match] == '0')
  {
    return 1;
  }else{
    return 0;
  }
}

/* 二分探索木の生成 -- 再帰を利用*/
void maketree(_node *p1, int deep,entry_type entry,std::string gavePrefix)
{
  //関数移動すると引数の内容忘れる可能性あるので、変数に保存
  std::string givenPrefix = gavePrefix;
  //std::cout << "\n givenPrefix = " + givenPrefix;
  _node *p2 = new _node;
  //std::cout << "\np1->str = " + p1->str + " str = " + givenPrefix;
  std::string last_skipval  = "";
  std::string answer = "";
  std::string str_size = p1->str;

  if (str_size.size() > 0)
  {
    answer = p1->str.substr(0,str_size.size() -1) + p1->skipval + p1->str.back();
  }else{
    answer = p1->skipval;
  }

  //std::cout << "\nanswer = " + answer;

  if (answer.size() > givenPrefix.size() || answer != givenPrefix.substr(0, answer.size()))
  {
    //printf("start rollback\n");
    //どの位置から一致していないのかを検索する
    int match_correct = search_not_match(answer,givenPrefix);
    rollback_node(p1,p2);
    std::string match_answer = answer.substr(0,match_correct);
    //std::cout << "\nmatch answer = " + match_answer;
    if(child_is_left(match_correct,answer)){
      set_node(p2,match_answer,"",0,false,p1,NULL,p1->b_left,p1->b_right);
      set_node(p1,answer,"",0,p1->blackOrWhite,p1->left,p1->right,NULL,p2);
      maketree(p2, deep, entry,givenPrefix);
    }else{
      set_node(p2,match_answer,"",0,false,NULL,p1,p1->b_left,p1->b_right);
      set_node(p1,answer,"",0,p1->blackOrWhite,p1->left,p1->right,p2,NULL);
      maketree(p2, deep, entry,givenPrefix);
    }
    return;
    // if (p1->skipval.size() > 0)
    // {
    //   last_skipval = p1->skipval.back(); // last_skipvalはskipvalの最後
    // }else{
    //   last_skipval = answer[answer.size()-2];
    //   p1->str.pop_back();
    //   p1->str.pop_back();
    // }



    // printf("start rollback\n");
    // std::string  nextstr;
    // nextstr = p1->str;
    // nextstr.pop_back();
    // nextstr = nextstr + last_skipval;
    // p1->skipval.pop_back();
    // rollback_node(p1,p2);



    // if (answer[answer.back()-1] == '1')
    // {
    //   if (p1->right != NULL && p1->left == NULL)
    //   {
    //     printf("lollback P is 1 and child = 1\n");
    //     set_node(p2,nextstr,p1->skipval,p1->skipnum - 1,false,NULL,p1->right,p1->b_left,p1->b_right);
    //     _node *p3 = p1->right;
    //     p3->b_left = p2;
    //     maketree(p2, deep, entry,givenPrefix);
    //   }else if(p1->right == NULL && p1->left != NULL){
    //     printf("lollback P is 1 and child = 0\n");
    //     set_node(p2,nextstr,p1->skipval,p1->skipnum - 1,false,p1->left,NULL,p1->b_left,p1->b_right);
    //     _node *p3 = p1->left;
    //     p3->b_right = p2;
    //     maketree(p2, deep, entry,givenPrefix);
    //   }else{
    //     printf("lollback P is 1 and child are both\n");
    //     set_node(p2,nextstr,p1->skipval,p1->skipnum - 1,false,NULL,p1,p1->b_left,p1->b_right);
    //     set_node(p1,answer,"",0,p1->blackOrWhite,p1->left,p1->right,p2,NULL);
    //     maketree(p2, deep, entry,givenPrefix);
    //   }
    // }else{
    //   if (p1->right != NULL && p1->left == NULL)
    //   {
    //     printf("lollback P is 0 and child = 1\n");
    //     if (p1->str.size()>0)
    //     {
    //       p1->str.pop_back();
    //     }
    //     set_node(p2,nextstr,p1->skipval,p1->skipnum - 1,false,NULL,p1->right,p1->b_left,p1->b_right);
    //     _node *p3 = p1->right;
    //     p3->b_left = p2;
    //     maketree(p2, deep, entry,givenPrefix);
    //   }else if(p1->right == NULL && p1->left != NULL){
    //     printf("lollback P is 0 and child = 0\n");
    //     if (p1->str.size()>0)
    //     {
    //       p1->str.pop_back();
    //     }
    //     set_node(p2,nextstr,p1->skipval,p1->skipnum,false,p1->left,NULL,p1->b_left,p1->b_right);
    //     _node *p3 = p1->left;
    //     p3->b_right = p2;
    //     maketree(p2, deep, entry,givenPrefix);
    //   }else{
    //     printf("lollback P is 0 and child are both or empty\n");
    //     set_node(p2,nextstr,p1->skipval,0,false,p1,NULL,p1->b_left,p1->b_right);
    //     set_node(p1,answer,"",0,p1->blackOrWhite,p1->left,p1->right,NULL,p2);
    //     maketree(p2, deep, entry,givenPrefix);
    //   }
    // }
    // return;


  }


  if (answer == givenPrefix){
    p1->asc = entry.number;
    p1->blackOrWhite = true;
    // printf("\n");
    // std::cout << "success! answer = " + answer;
    // printf("\n");
     return;
  }else{
    if (givenPrefix[answer.size()] == '1') {
      if (p1->blackOrWhite==false && p1->left == NULL){
        //printf("skip active case 1\n");
        std::string last_str = "";
        last_str =  p1->str.back();
        p1->str.pop_back();
        if (rollback_node(p1,p2))
        {
        set_node(p2,p1->str + "1",p1->skipval + last_str,p1->skipnum + 1,false,NULL,NULL,NULL,p1->b_right);
        }else{
        set_node(p2,p1->str + "1",p1->skipval + last_str,p1->skipnum + 1,false,NULL,NULL,p1->b_left,NULL);
        }
        //std::cout << "skipval = " + p2->skipval +" skipnum = ";
        maketree(p2, deep, entry,givenPrefix);
      }else{
        if (p1->right == NULL)
        {
          //printf("right node null active\n");
          set_node(p2,answer + "1","",0,false,NULL,NULL,p1,NULL);
          p1->right = p2;
          maketree(p2, deep, entry,givenPrefix);
        }else{
          //printf("right node is already exist!\n");
          delete p2;
          maketree(p1->right, deep, entry,givenPrefix);
        }
      }
    } else {
      if (p1->blackOrWhite==false && p1->right == NULL){
        //printf("skip active case 0\n");
        std::string last_str = "";
        last_str =  p1->str.back();
        p1->str.pop_back();
        if (rollback_node(p1,p2))
        {
        set_node(p2,p1->str + "0",p1->skipval + last_str,p1->skipnum + 1,false,NULL,NULL,NULL,p1->b_right);
        }else{
        set_node(p2,p1->str + "0",p1->skipval + last_str,p1->skipnum + 1,false,NULL,NULL,p1->b_left,NULL);
        }
        maketree(p2, deep, entry,givenPrefix);
      }else{
        if (p1->left == NULL)
        {
          //printf("left node null active\n");
          set_node(p2,answer + "0","",0 ,false,NULL,NULL,NULL,p1);
          p1->left = p2;
          maketree(p1->left, deep, entry,givenPrefix);
        }else{
          //printf("left node is already exist!\n");
          delete p2;
          maketree(p1->left, deep, entry,givenPrefix);
        }
      }
    }
  }
}





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
  uint decimal = entry.prefix >> (32 - entry.length);
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
    count--;
  }
  //std::cout << "str = " + str;
  return str;
}

/* 木構造をたどって探索 */



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
_node* makeStruct(std::vector<entry_type> entry_list,std::string makep)
{
printf("start\n");
  _node *start, *p1;
  /* 先頭のノードを初期化 */
  start = static_cast<_node *>(malloc(sizeof(_node)));
  start->prefix = 0;
  start->asc = 0;
  start->left = NULL;
  start->right = NULL;
  start->b_left = NULL;
  start->b_right = NULL;
  start->blackOrWhite = true;
  start->str = "";
  start->skipval = "";
  start->skipnum = 0;
  p1 = start;
  /* 残る値から木構造を生成 */
  for (int i = 0;entry_list.size() != i; ++i)
  {
    // char array[entry_list[i].length+1];
    // strinit(array,entry_list[i].length+1);
    // makearray(entry_list[i],array);
    //printf(" entry_list[i],str = %u\n", entry_list[i].prefix);
    std::string str = "";
    str = makearray(entry_list[i],str);

    //32-acs文削る
    maketree(start, 0, entry_list[i],str);
    /* code */
  }
  return start;
}



std::string makebit_a(char* ar2,char* ar3,char* ar4,char* ar5){
  std::string str = "";
  int    a = std::stoi(ar2); // 3
  int    b = std::stoi(ar3); // 3
  int    c = std::stoi(ar4); // 3
  int    d = std::stoi(ar5); // 3
  uint e = a << 24 | b << 16 |c << 8 | d;
  int count = 0;
  int i;
  int binary[32];
  int nonExistLength = 32 - toTwo(e);//25
  int k = 0;
  for (k = 0; k < nonExistLength; ++k)
  {
    str+= "0";
  }
  uint decimal = e;

  for(i=0;decimal>0;i++){
    binary[i] = decimal % 2;
    decimal = decimal / 2;
    count = i;
  }

  while(count >= 0){
    if (binary[count] == 1)
      {
        str+= "1";
      }else{
        str+= "0";
      }
    count--;
  }
  //std::cout << "str = " + str;
  return str;
}

std::string make_answer_prefix(uint prefix){
  std::string str = "";
  int count = 0;
  int i;
  int binary[32];
  int nonExistLength = 32 - toTwo(prefix);//25
  int k = 0;
  for (k = 0; k < nonExistLength; ++k)
  {
    str+= "0";
  }
  uint decimal = prefix;

  for(i=0;decimal>0;i++){
    binary[i] = decimal % 2;
    decimal = decimal / 2;
    count = i;
  }

  while(count >= 0){
    if (binary[count] == 1)
      {
        str+= "1";
      }else{
        str+= "0";
      }
    count--;
  }

  //std::cout << "str = " + str;
  return str;
}

//https://www.grapecity.com/tools/support/powernews/column/clang/052/page03.htmを
//をコピペした
