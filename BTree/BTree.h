/*************************************************************************
	> File Name: BTree.h
	> Author: bw98
	> Mail: 786016746@qq.com
	> Created Time: 2017年11月04日 星期六 13时35分08秒
 ************************************************************************/
#ifndef BTREE_H
#define BTREE_H
typedef char DataType;

struct BTreeNode
{
    DataType data;
    BTreeNode *lchild,*rchild;
};

class BTree{
private:
    BTreeNode* root;
public:
    BTree()
    {
        root = nullptr;
    }
    inline void setRoot(BTreeNode* r){root = r;}
    inline BTreeNode* getRoot(){return root;}
    //create BTree
    BTreeNode* create();
    //visit BTreeNode with recursion
    void proOrder();
    void midOrder();
    void backOrder();
    //vist BTreeNode with non-recursion
    void noReProOrder();
    void noReMidOrder();
    void noReBackOrder();
    void noRelayerOrder();
    //number of BTreeNodes
    int BTreeSize();
    //number of leaves
    int BtreeLeaves();
    //height
    int BTreeHeight();

protected:
    //visit BTreeNode with recursion
    void proOrder(BTreeNode *r);
    void midOrder(BTreeNode *r);
    void backOrder(BTreeNode *r);
    //visit BTreeNode with non-recursion
    void noReProOrder(BTreeNode *r);
    void noReMidOrder(BTreeNode *r);
    void noReBackOrder(BTreeNode *r);
    void noRelayerOrder(BTreeNode *r);
    //number of BTreeNodes
    int BTreeSize(BTreeNode *r);
    //number of leaves
    int BtreeLeaves(BTreeNode *r);
    //height
    int BTreeHeight(BTreeNode *r);


};


#endif
