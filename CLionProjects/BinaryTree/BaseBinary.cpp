//
// Created by 梦 on 2019-03-05.
//

#include "BinaryTree.h"

#include <vector>
#include <list>
#include <stdio.h>
#include "gtest/gtest.h"

class Base{
public:

    Base(){printf("Base class Value %d",value);}
    static void addValue(int size){

        printf("test test a");
    }
    virtual void Display() = 0;
private:
    int value;
};

class SubClassA: public Base{
public:
    void Display(){
        printf("subValue :%d\n",subValue);
    }
    void addValue(int x){

        printf("test test b");
    }
protected:
    int subValue;
};

struct mixData{
    int x;
    static int y;
    int z;
}a = {1,2};

TEST(testclass,test1){
    SubClassA* pa = new SubClassA();
    Base::addValue(5);
    pa->addValue(6);
}

TEST(teststru,test1){
    mixData data = {1,2};
    printf("data:%d, %d ",data.x,data.z);
}

TEST(testattr,test1){
   int num;
   scanf("%d",&num);
   int a[num];
   EXPECT_EQ(sizeof(a),sizeof(int)*num);
   printf("a size:%lu\n",sizeof(a));
}

int main(void)
{

    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}