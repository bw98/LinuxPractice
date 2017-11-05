/*************************************************************************
	> File Name: BTree.cpp
	> Author: bw98
	> Mail: 786016746@qq.com
	> Created Time: 2017年11月05日 星期日 13时16分41秒
 ************************************************************************/

#include<iostream>
#include<stack>
#include<vector>
#include<queue>
#include"BTree.h"
using namespace std;

BTreeNode* BTree::create()
{
    BTreeNode *current;
    char ch;
    cin.get(ch);
    if(ch == '#')
        return NULL;
    else{
        current = new BTreeNode;
        current->data = ch;
        current->lchild = create();
        current->rchild = create();
        return current;
    }
}

