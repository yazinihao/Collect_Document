//
// Created by 梦 on 2019-03-07.
//
#include "StrToTree.h"

#include <string>
#include <sstream>
#include <queue>

void trimLeftTrailingSpaces(std::string & input){
    input.erase(input.begin(), find_if(input.begin(), input.end(),[](int ch){
        return !isspace(ch);
    }));
}

void trimRightTrailingSpaces(std::string & input){
    input.erase(find_if(input.rbegin(), input.rend(), [](int ch){
        return !isspace(ch);
    }).base(), input.end());
}

TreeNode* stringToTreeNode(std::string input){
    trimLeftTrailingSpaces(input);
    trimRightTrailingSpaces(input);
    input = input.substr(1, input.length() - 2);
    if(!input.size()){
        return nullptr;
    }

    std::string item;
    std::stringstream ss;
    ss.str(input);
    getline(ss, item, ',');
    TreeNode* root = new TreeNode(stoi(item));
    std::queue<TreeNode*> nodeQueue;
    nodeQueue.push(root);

    while(true) {
        TreeNode *node = nodeQueue.front();
        nodeQueue.pop();

        if (!getline(ss, item, ',')) {
            break;
        }

        trimLeftTrailingSpaces(item);
        if (item != 'null') {
            int leftNumber = stoi(item);
            node->left = new TreeNode(leftNumber);
            nodeQueue.push(node->left);
        }

        if(!getline(ss, item, ',')){
            break;
        }

        trimLeftTrailingSpaces(item);
        if(item != "null"){
            int rightNumber = stoi(item);
            node->rigth = new TreeNode(rightNumber);
            nodeQueue.push(node->right);
        }
    }
    return root;
}




