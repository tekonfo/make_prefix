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
  struct _n *b_left;
  struct _n *right;    /* 右 -- 自己参照 */
  struct _n *b_right;
  int skipnum;
  std::string skipval;
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

void set_node(){

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
  std::cout << "\nanswer = " + answer;
  return answer;
}


/* 二分探索木の生成 -- 再帰を利用*/
void maketree(_node *p1, int deep,entry_type entry,std::string givenPrefix)
{
  _node *p2;
  if (deep > entry.length)
  {
    printf("break\n");
    return;
  }
  std::cout << "\np1->str = " + p1->str + " str = " + givenPrefix;
  std::string last_skipval  = "";
  std::string  answer;
  answer = get_answer(p1,last_skipval);
  if (p1->skipval.size() > 0)
  {
    last_skipval = p1->skipval.back(); // last_skipvalはskipvalの最後
  }else{
    printf("\n");
    printf("skipval.size = 0\n");
    last_skipval = answer[answer.size()-2];
    std::cout << "lastskipval  = " + last_skipval;
    printf("\n");
  }

  // if (answer.size() >= givenPrefix.size())
  // {
  if (answer.size() > givenPrefix.size() || answer != givenPrefix.substr(0, answer.size()))
  {
    printf("start rollback\n");
    //ロールバック
    p2 = static_cast<_node *>(malloc(sizeof(_node)));
    std::string  nextstr;
    nextstr = p1->str;
    nextstr.pop_back();
    nextstr = nextstr + last_skipval;
    p1->skipval.pop_back();
    // strncmp(str.front(),"0",1)
    if (last_skipval == "1")
    {
      if (p1->right != NULL && p1->left == NULL)
      {
        printf("lollback P is 1 and child = 1\n");
        set_node(p2,nextstr,p1->skipval,p1->skipnum - 1,false,NULL,p1->right,p1->b_left,p1->b_right);
        _node *p0 = p1->b_left;
        p0->right = p2;
        _node *p3 = p1->right;
        p3->b_left = p2;
        p3->b_right = NULL;
        //p1のメモリの解放
        //set_node(p1,answer,"",0,p1->blackOrWhite,p1->left,p1->right,p2,NULL);
        //set_node(p3,p3->str,"1" + p3->skipval,,p3->blackOrWhite,p3->left,p3->right,p2,NULL);
        printf("\n");
        maketree(p2, deep, entry,givenPrefix);
      }else if(p1->right == NULL && p1->left != NULL){
        printf("lollback P is 1 and child = 0\n");
        set_node(p2,nextstr,p1->skipval,p1->skipnum - 1,false,p1->left,NULL,p1->b_left,p1->b_right);
        _node *p0 = p1->b_right;
        p0->left = p2;
        _node *p3 = p1->left;
        p3->b_right = p2;
        //set_node(p1,answer,"",0,p1->blackOrWhite,p1->left,p1->right,p2,NULL);
        //set_node(p3,p3->str,"1" + p3->skipval,p3->skipnum + 1,p3->blackOrWhite,p3->right,p3->right,NULL,p2);
        maketree(p2, deep, entry,givenPrefix);
      }else{
        printf("lollback P is 1 and child are both\n");
        set_node(p2,nextstr,p1->skipval,p1->skipnum - 1,false,NULL,p1,p1->b_left,p1->b_right);
        _node *p0 = p1->b_left;
        p0->right = p2;
        set_node(p1,answer,"",0,p1->blackOrWhite,p1->left,p1->right,p2,NULL);
        maketree(p2, deep, entry,givenPrefix);
      }
    }else{
      if (p1->right != NULL && p1->left == NULL)
      {
        printf("lollback P is 0 and child = 1\n");
        p1->str.pop_back();
        set_node(p2,nextstr,p1->skipval,p1->skipnum - 1,false,NULL,p1->right,p1->b_left,p1->b_right);
        _node *p0 = p1->b_left;
        p0->right = p2;
        _node *p3 = p1->right;
        p3->b_left = p2;
        p3->b_right = NULL;
        //set_node(p3,p3->str,"0" + p3->skipval,p3->skipnum + 1,p3->blackOrWhite,p3->left,p3->right,p2,NULL);
        //p1のメモリの解放
        //set_node(p1,answer,"",0,p1->blackOrWhite,p1->left,p1->right,p2,NULL);
        maketree(p2, deep, entry,givenPrefix);
      }else if(p1->right == NULL && p1->left != NULL){
        printf("lollback P is 0 and child = 0\n");
        set_node(p2,nextstr,p1->skipval,p1->skipnum - 1,false,p1->left,NULL,p1->b_left,p1->b_right);
        _node *p0 = p1->b_right;
        p0->left = p2;
        _node *p3 = p1->left;
        p3->b_right = p2;
        p3->b_left = NULL;
        //set_node(p1,answer,"",0,p1->blackOrWhite,p1->left,p1->right,p2,NULL);
        //set_node(p3,p3->str,"0" + p3->skipval,p3->skipnum + 1,p3->blackOrWhite,p3->right,p3->right,NULL,p2);
        maketree(p2, deep, entry,givenPrefix);
      }else{
        printf("lollback P is 0 and child are both or empty\n");
        set_node(p2,nextstr,p1->skipval,p1->skipnum - 1,false,p1,NULL,p1->b_left,p1->b_right);
        printf("test\n");
        if (p1->b_right == NULL)
        {
          printf("error\n");//ポインタ刺し間違えている
        }
        _node *p0 = p1->b_right;
        printf("test\n");///上のノードが存在していない
        p0->left = p2;
        printf("test\n");
        set_node(p1,answer,"",0,p1->blackOrWhite,p1->left,p1->right,NULL,p2);
        printf("test\n");
        maketree(p2, deep, entry,givenPrefix);

        // printf("lollback P is 1 and child are both\n");
        // set_node(p2,last_skipval,p1->skipval,p1->skipnum - 1,false,NULL,p1,p1->b_left,p1->b_right);
        // _node *p0 = p1->b_left;
        // p0->right = p2;
        // set_node(p1,answer,"",0,p1->blackOrWhite,p1->left,p1->right,p2,NULL);
        // maketree(p2, deep, entry,givenPrefix);
      }

    }
    return;
  }

  if (answer == givenPrefix){
    p1->asc = entry.number;
    p1->blackOrWhite = true;
    std::cout << "\nsuccess! answer = " + answer;
    printf("\n");
  }else{
    printf("deep = %d\n", deep);
    std::cout << "nextprefix = " + givenPrefix[answer.size()];
    printf("\n");
    if (givenPrefix[answer.size()] == '1') {
      /* 右がNULLならそこに新たなノードをぶら下げる */
      if (p1->blackOrWhite==false && p1->left == NULL){
        //スキップ
        printf("skip active case 1\n");
        p2 = static_cast<_node *>(malloc(sizeof(_node)));
        std::string last_str = "";
        last_str =  p1->str.back();
        p1->str.pop_back();
        set_node(p2,p1->str + "1",p1->skipval + last_str,p1->skipnum + 1,false,NULL,NULL,p1->b_left,NULL);
        _node *p0 = p1->b_left;
        p0->right = p2;
        std::cout << "\nskipval = " + p2->skipval +" skipnum = ";
        printf("%d\n", p2->skipnum);
        maketree(p2, deep, entry,givenPrefix);
      }else{
        if (p1->right == NULL)
        {
          printf("right node null \n");
          p2 = static_cast<_node *>(malloc(sizeof(_node)));
          set_node(p2,p1->str + "1",p1->skipval,p1->skipnum,false,NULL,NULL,p1,NULL);
          p1->right = p2;
          maketree(p2, deep, entry,givenPrefix);
        }else{
          printf("right node is already exist!\n");
          maketree(p1->right, deep, entry,givenPrefix);
        }
      }
    } else {
      /* 左がNULLならそこに新たなノードをぶら下げる */
      if (p1->blackOrWhite==false && p1->right == NULL){
        printf("skip active case 0\n");
        p2 = static_cast<_node *>(malloc(sizeof(_node)));
        std::string last_str = "";
        last_str =  p1->str.back();
        p1->str.pop_back();
        set_node(p2,p1->str + "0",p1->skipval + last_str,p1->skipnum + 1,false,NULL,NULL,NULL,p1->b_right);
        _node *p0 = p1->b_right;
        p0->left = p2;
        std::cout << "\nskipval = " + p2->skipval +" skipnum = ";
        maketree(p2, deep, entry,givenPrefix);
      }else{
        if (p1->left == NULL)
        {
          printf("left node null active\n");
          p2 = static_cast<_node *>(malloc(sizeof(_node)));
          set_node(p2,p1->str + "0",p1->skipval,p1->skipnum ,false,NULL,NULL,NULL,p1);
          p1->left = p2;
          maketree(p1->left, deep, entry,givenPrefix);
        }else{
          printf("left node is already exist!\n");
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
    count--;
  }
  std::cout << "str = " + str;
  return str;
}

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
void makeStruct(std::vector<entry_type> entry_list)
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
    maketree(start, 0, entry_list[i],str);
    /* code */
  }
  /* 木構造をたどって探索 */
}


//https://www.grapecity.com/tools/support/powernews/column/clang/052/page03.htmを
//をコピペした
