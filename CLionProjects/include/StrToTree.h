//
// Created by 梦 on 2019-03-07.
//

#ifndef CLIONPROJECTS_STRTOTREE_H
#define CLIONPROJECTS_STRTOTREE_H

namespace BinaryTree {

    struct TreeNode {
        int val;
        TreeNode *left;
        TreeNode *right;

        TreeNode(int x) : val(x), left(NULL), right(NULL) {}
    };

    void trimLeftTrailingSpaces(std::string & input);

    void trimRightTrailingSpaces(std::string & input);

    TreeNode* stringToTreeNode(std::string input);
}

#endif //CLIONPROJECTS_STRTOTREE_H
