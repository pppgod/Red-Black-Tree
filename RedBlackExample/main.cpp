//
//  main.cpp
//  RedBlackExample
//
//  Created by LeeSangyun on 2016. 5. 3..
//  Copyright © 2016년 LeeSangyun. All rights reserved.
//
#include "RedBlackTree.h"
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

#define NORMAL 0
#define SILVER 1
#define GOLD 2

struct pay
{
    string name;
    int date;
    int price;
    pay(){};
    pay(string _name, int _date, int _price)
    :name(_name), date(_date), price(_price){}
};

class elem
{
public:
    string name;
    string phone;
    int x;
    int y;
    int grade;
    int limit;
    int sumPrice;
    int maxPrice;
    RedBlackTree< int, pay > payments;
    
    elem(){};
    elem(string _name, string _phone, int _x, int _y, int _grade, int _sumPrice)
    :name(_name), phone(_phone), x(_x), y(_y), grade(_grade), sumPrice(_sumPrice), maxPrice(0)
    {
        if(grade == NORMAL)
        { limit = 3000000; }
        else if(grade == SILVER)
        { limit = 30000000; }
        else if(grade == GOLD)
        { limit = 300000000; }
    }
    bool insert(pay p)
    {
        if(sumPrice+p.price > limit)
        { return false; }
        else
        {
            maxPrice = max(maxPrice, p.price);
            sumPrice += p.price;
            // 최근 날짜순으로 정렬하기 위하여 키 값을 음수로 삽입한다.
            payments.insert(-p.date, p);
            return true;
        }
    }
};

int main()
{
    freopen("query_10k.txt", "r", stdin);
    //freopen("output.txt", "w+", stdout);
    string filename;
    cout << "입력파일의 이름을 입력하세요: ";
    cin >> filename;
    FILE * pFile;
    pFile = fopen (filename.c_str(),"r");
    
    char query[3], name[22], phone[13];
    int id, x, y, grade, sumPrice, date, price, num;
    RedBlackTree < int, elem > rbt;
    
    while(fscanf(pFile, "%d %s %s %d %d %d %d", &id, name, phone, &x, &y, &grade, &sumPrice) != EOF)
    { rbt.insert(id, elem(name, phone, x, y, grade, sumPrice)); }
    
    while(scanf("%s", query) != EOF)
    {
        switch(query[0])
        {
            case 'I':
            {
                scanf("%d %s %s %d %d", &id, name, phone, &x, &y);
                pair < int , bool > rec = rbt.insert(id, elem(string(name), string(phone), x, y, NORMAL, 0));
                printf("%d %d\n", rec.first, rec.second==true? 1:0);
                break;
            }
            case 'P':
            {
                scanf("%d", &id);
                pair < elem, int > rec = rbt.find(id);
                if(rec.second == -1)
                { puts("Not found!"); }
                else
                {
                    printf("%s %s %d %d ", rec.first.name.c_str(), rec.first.phone.c_str(), rec.first.x, rec.first.y);
                    printf("%d %d %d\n", rec.first.grade, rec.first.sumPrice, rec.second);
                }
                break;
            }
            case 'A':
            {
                scanf("%d %d %s %d", &id, &date, name, &price);
                pair < elem, int > rec = rbt.find(id);
                if(rec.second == -1)
                { puts("Not found!"); }
                else
                {
                    if(rec.first.insert(pay(name, date, price)))
                    { printf("%d 1\n", rec.second); }
                    else
                    { printf("%d 0\n", rec.second); }
                }
                break;
            }
            case 'F':
            {
                vector < int > all_users_total_price;
                RedBlackTree< int, elem >::Position p;
                
                for(p = rbt.begin(); p != rbt.end(); p=++p)
                { all_users_total_price.push_back((p.v->elt.sumPrice)); }
                sort(all_users_total_price.begin(), all_users_total_price.end());
                
                for(int i = (int)all_users_total_price.size()-1; i >= max(0, (int)all_users_total_price.size()-3); i--)
                    printf("%d ", all_users_total_price[i]);
                puts("");
                break;
            }
            case 'R':
            {
                scanf("%d %d", &id, &num);
                pair < elem, int > rec = rbt.find(id);
                if(rec.second == -1)
                { puts("Not found!"); }
                else
                {
                    RedBlackTree< int, pay >::Position p;
                    for(p = rec.first.payments.begin(); p != rec.first.payments.end() && num > 0; p = ++p, num--)
                    { printf("%d %s %d\n", p.v->elt.date, p.v->elt.name.c_str(), p.v->elt.price); }
                }
                break;
            }
            case 'Q':
            {
                return 0;
            }
            default:
            { break; }
        }
    }
    puts("END");
    return 0;
}
