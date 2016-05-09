//
//  main.cpp
//  RedBlackExample
//
//  Created by LeeSangyun on 2016. 5. 3..
//  Copyright © 2016년 LeeSangyun. All rights reserved.
//
#include "RedBlackTree.h"
#include <iostream>
using namespace std;

struct elem
{
    string name;
    string phone;
    int x;
    int y;
    int grade;
    int sumPrice;
    
    elem() : name(), phone(), x(), y(), grade(), sumPrice() {}
    elem(string _name, string _phone, int _x, int _y, int _grade, int _sumPrice):
    name(_name), phone(_phone), x(_x), y(_y), grade(_grade), sumPrice(_sumPrice){}
};

int main()
{
    RedBlackTree < int, int > rbt;
//    rbt.insert(0, 0);
    for(int i = 0; i < 3; i++)
        rbt.insert(i, i);
    
    
    
    return 0;
}
