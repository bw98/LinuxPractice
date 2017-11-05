/*************************************************************************
	> File Name: main.cpp
	> Author: bw98 
	> Mail: 786016746@qq.com
	> Created Time: 2017年11月05日 星期日 16时21分40秒
 ************************************************************************/

#include<iostream>
#include"BTree.h"

using namespace std;

int main(void)
{
    BTree *t = new BTree();
    t->setRoot(t->create());
    BTreeNode *node1 = t->getRoot();
    cout << node1->data << endl;
    return 0;
}

