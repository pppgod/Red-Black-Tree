//
//  RedBlackTree.h
//  RedBlackExample
//
//  Created by LeeSangyun on 2016. 5. 3..
//  Copyright © 2016년 LeeSangyun. All rights reserved.
//

#ifndef RedBlackTree_h
#define RedBlackTree_h

#include <iostream>
#include <algorithm>
//#include <string>
using namespace std;

#define RED true
#define BLACK false;

template < typename Key, typename Value >
class RedBlackTree
{
protected:
    class node
    {
    public:
        node* par;
        node* left;
        node* right;
        Key identifier;
        Value elt;
        bool color;
        
        // external node
        node() : par(NULL), left(NULL), right(NULL), identifier(NULL)
        { color = BLACK; }
        
        node(Key _identifier, Value _elt) : identifier(_identifier), elt(_elt)
        {
            left = NULL;
            right = NULL;
            par = NULL;
            color = RED;
        }
        bool operator < (const node & N) const
        { return identifier < N.identifier; }
    };
    
public:
    class Position {					// position in the tree
    public:
        node* v;						// pointer to the node
    public:
        Position(node* _v = NULL) : v(_v){} // constructor
        Value& operator*()					// get element
        { return v->elt; }
        Position operator++()
        {
            //puts("!!");
            if(!(this->right()).isExternal())
            {
                Position p = this->right();
                while(!(p.left().isExternal()))
                { p = p.left(); }
                return p;
            }
            
            Position par = this->parent();
            while(*(par.v) < *(this->v))
            {
                if(par.isroot())
                { break; }
                par = par.parent();
            }
            if(*(par.v) < *(this->v))
            { return this->right(); }
            else
            { return par; }
        }
        bool operator != (const Position& p)
        {
            if(this->v == NULL) return false;
            if(this->v != p.v)
            { return true; }
            else
            { return false; }
        }
        Position left() const				// get left child
        { return Position(v->left); }
        Position right() const				// get right child
        { return Position(v->right); }
        Position parent() const				// get parent
        { return Position(v->par); }
        bool isroot() const				// root of the tree?
        { return v->par == NULL; }
        bool isExternal() const				// an external node?
        { return v->left == NULL && v->right == NULL; }
        friend class RedBlackTree;			// give tree access
            
    };
    
// protected 변수
protected:
    node* _root;
    int nodeSize;
    
// pulbic 멤버함수
public:
    RedBlackTree();
    Position root() const;
    Position begin() const;
    Position end() const;
    inline bool empty() const;
    inline int size() const;
    /*
     const Key K 삽입할 키값 K
     Value V 삽입할 value값
     return : 노드의 깊이, 삽입 성공 여부
     */
    pair < int, bool > insert(const Key K, Value V);
    pair < Value, int > find(const Key K) const;
  //  friend class Position;
// protected 멤버함수
protected:
    void expandExternal(const Position& p);
    Position insert_recursive(const Key K, const Position& p) const;
    inline Position sibling(const Key child_key, const Position& p) const;
    void restructuring(Position& p);
    void recoloring(Position& p);
    pair < Position, int > find_recursive(const Key K, const Position& p, int depth) const;
    void inorder(const Position& p) const;
};

template < typename Key, typename Value >
RedBlackTree< Key, Value >::RedBlackTree() : _root(NULL), nodeSize(0){}

template < typename Key, typename Value >
typename RedBlackTree< Key, Value >::Position RedBlackTree< Key, Value >::root() const
{ return Position(_root); }

template < typename Key, typename Value >
typename RedBlackTree< Key, Value >::Position RedBlackTree< Key, Value >::begin() const
{
    if(empty()) return NULL;
    Position p = root();
    while(!(p.left().isExternal()))
    {   p = p.left(); }
    return p;
}

template < typename Key, typename Value >
typename RedBlackTree< Key, Value >::Position RedBlackTree< Key, Value >::end() const
{
    if(empty()) return NULL;
    
    Position p = root();
    while(!p.isExternal())
    {   p = p.right(); }
    return p;
}

template < typename Key, typename Value >
inline bool RedBlackTree< Key, Value >::empty() const
{ return nodeSize == 0; }

template < typename Key, typename Value >
inline int RedBlackTree< Key, Value >::size() const
{ return nodeSize; }

template < typename Key, typename Value >
void RedBlackTree< Key, Value >::expandExternal(const Position& p)
{
    node* v = p.v;					// p's node
    v->left = new node;					// add a new left child
    v->left->par = v;					// v is its parent
    v->right = new node;				// and a new right child
    v->right->par = v;					// v is its parent
    nodeSize += 2;						// two more nodes
}

template < typename Key, typename Value >
pair < int, bool > RedBlackTree< Key, Value >::insert(const Key K, Value V)
{
   // printf("K: %d\n", K);
    // tree가 비어있는 경우
    if(empty())
    {
        _root = new node;
        _root->identifier = K;
        _root->elt = V;
        _root->color = BLACK; // root property
        nodeSize++;
        expandExternal(root()); // 루트에 external 노드를 추가한다.
        return pair< int, bool >(1, true);
    }
    //printf("%d\n", _root->identifier);
    //printf("root color : ");
    
//    if(!_root->color)
//        cout << "black" << endl;
//    else
//        cout << "red" << endl;
    
//    puts("finding");
    // 새로운 노드가 들어갈 위치를 찾는다.
    pair < Position, int > rec = find_recursive(K, root(), 0);
  //  puts("finding end");
    Position p = rec.first;
    // 이미 키 값이 존재하는 경우
   // puts("externaling");
    if(!p.isExternal())
    { return pair < int, bool >(rec.second, false); }
    else
    {
     //   puts("!!");
        p.v->identifier = K;
        p.v->elt = V;
        p.v->color = RED; // 추가된 노드는 처음에는 레드다
        nodeSize++;
        expandExternal(p);
        Position p_parent = p.parent();
        
        // double red 발생시
        while(p_parent.v->color == RED)
        {
//            cout << "p : ";
//            cout << p.v->identifier << endl;
//            cout << "sibling : ";
//            cout << sibling(p.v->identifier, p_parent.parent()
//                            ).v->identifier << endl;
            if(sibling(p.v->identifier, p_parent.parent()).v->color == RED)
            {
                recoloring(p_parent);
                
            }
            else
            {
                //cout << "restruct : " << p
                restructuring(p);
                break;
            }
            p_parent = p_parent.parent().parent();
            p = p.parent().parent();
            if(p.isroot())
            { break; }
        }
//        cout << "left : ";
//        if(root().left().isExternal())
//        {
//            cout << "NULL" << endl;
//        }
//        else
//        {
//            cout << _root->left->identifier;
//            cout << " color : ";
//            if(_root->left->color == RED)
//                cout << "RED" << endl;
//            else
//                cout << "BLACK" << endl;
//        }
//        
//        cout << "right : ";
//        if(root().right().isExternal())
//        {
//            cout << "NULL" << endl;
//        }
//        else
//        {
//            cout << _root->right->identifier;
//            cout << " color : ";
//            if(_root->right->color == RED)
//                cout << "RED" << endl;
//            else
//                cout << "BLACK" << endl;
//        }
        return pair < int, bool >(find_recursive(K, root(), 0).second, true);
    }
}

//template < typename Key, typename Value >
//typename RedBlackTree< Key, Value >::Position RedBlackTree< Key, Value >::insert_recursive(const Key K, const Position &p) const
//{
//    // external node인 경우 재귀 종료
//    if(p.isExternal())
//    {
//        nodeSize++;
//        return p;
//    }
//    // 재귀적으로 함수 호출
//    if(p.v->identifier == K)
//    { return p; }
//    else if(p.v->identifier > K)
//    { return insert_recursive(K, p.left()); }
//    else
//    { return insert_recursive(K, p.right()); }
//}

template < typename Key, typename Value >
inline typename RedBlackTree< Key, Value >::Position RedBlackTree< Key, Value >::sibling(const Key childe_key, const Position &p) const
{
    if(childe_key < p.v->identifier)
    { return p.right(); }
    else
    { return p.left(); }
}

template < typename Key, typename Value >
void RedBlackTree< Key, Value >::restructuring(Position& p)
{
    //puts("Restructuring!");
    Position p_parent = p.parent();
    Position p_grand_parent = p_parent.parent();
    Position p_grand_grand_parent;
    Position t1, t2, t3, t4;
    Position par, left_child, right_child;
    bool grand_is_root = p_grand_parent.isroot();
    bool is_right = true;
    if(!grand_is_root)
    {
        p_grand_grand_parent = p_grand_parent.parent();
        if(p_grand_parent.parent().v->identifier > p_grand_parent.v->identifier)
        { is_right = false; }
        else
        { is_right = true; }
    }
    
    if(*(p_grand_parent.v) < *(p.v))
    {
        left_child = p_grand_parent;
        /*
         o
          \
           o
            \
             o
         */
        if(*(p_parent.v) < *(p.v))
        {
            //puts("1");
            par = p_parent;
            right_child = p;
            
            //t2 달기
//            t2 = p_parent.left();
//            left_child.v->right = t2.v;
//            t2.v->par = left_child.v;
            
            t1 = p_grand_parent.left();
            t2 = p_parent.left();
            t3 = p.left();
            t4 = p.right();
            
        }
        /*
         o
           \
            o
           /
          o
         */
        else
        {
            //puts("2");
            par = p;
            right_child = p_parent;
//            // t2 달기
//            t2 = p.left();
//            left_child.v->right = t2.v;
//            t2.v->par = left_child.v;
//            // t3 달기
//            t3 = p.right();
//            right_child.v->left = t3.v;
//            t3.v->par = right_child.v;
            
            t1 = p_grand_parent.left();
            t2 = p.left();
            t3 = p.right();
            t4 = p_parent.right();
        }
    }
    else
    {
        right_child = p_grand_parent;
        /*
            o
           /
         o
          \
           o
         */
        if(*(p_parent.v) < *(p.v))
        {
           // puts("3");
            par = p;
            left_child = p_parent;
//            // t2 달기
//            t2 = p.left();
//            left_child.v->right = t2.v;
//            t2.v->par = left_child.v;
//            // t3 달기
//            t3 = p.right();
//            right_child.v->left = t3.v;
//            t3.v->par = right_child.v;
            
            t1 = p_parent.left();
            t2 = p.left();
            t3 = p.right();
            t4 = p_grand_parent.right();
        }
        /*
            o
           /
          o
         /
        o
         */
        else
        {
          //  puts("4");
            par = p_parent;
            left_child = p;
//            // t2 달기
//            t3 = p_parent.right();
//            right_child.v->left = t3.v;
//            t3.v->par = right_child.v;
            
            t1 = p.left();
            t2 = p.right();
            t3 = p_parent.right();
            t4 = p_grand_parent.right();
        }
    }
    
    t1.v->par = left_child.v;
    t2.v->par = left_child.v;
    t3.v->par = right_child.v;
    t4.v->par = right_child.v;
    left_child.v->left = t1.v;
    left_child.v->right = t2.v;
    right_child.v->left = t3.v;
    right_child.v->right = t4.v;
    
    
    
    par.v->par = p_grand_parent.parent().v;
    par.v->left = left_child.v;
    par.v->right = right_child.v;
    left_child.v->par = par.v;
    right_child.v->par = par.v;
    //cout << "left : " << left_child.v->identifier << ' ' << "right : " << right_child.v->identifier << endl;
    par.v->color = BLACK;
    left_child.v->color = RED;
    right_child.v->color = RED;
    if(grand_is_root)
    {
        _root = par.v;
        par.v->par = NULL;
    }
    else
    {
       if(is_right)
       { p_grand_grand_parent.v->right = par.v; }
       else
       { p_grand_grand_parent.v->left = par.v; }
    }
}

template < typename Key, typename Value >
void RedBlackTree< Key, Value >::recoloring(Position& p)
{
    p.v->color = BLACK;
    //cout << "recolor sibling : " << sibling(p.v->identifier, p.parent()).v->identifier << endl;
    sibling(p.v->identifier, p.parent()).v->color = BLACK;
    if(!(p.parent()).isroot())
    { p.parent().v->color = RED; }
}

template < typename Key, typename Value >
pair < Value, int > RedBlackTree< Key, Value >::find(const Key K) const
{
    pair < Position, int > rec = find_recursive(K, root(), 1);
    if(rec.first.isExternal())
    { return pair < Value, int >(Value(), -1); }
    else
    { return pair < Value, int >(rec.first.v->elt, rec.second); }
}

template < typename Key, typename Value >
pair < typename RedBlackTree< Key, Value >::Position, int > RedBlackTree< Key, Value >::find_recursive(const Key K, const Position& p, int depth) const
{
    //cout << K << endl;
    int p_id = 0;
    if(!p.isExternal())
    {
        p_id = p.v->identifier;
        //if(K == 1000799) cout << p_id << endl;
    }
    //if(p.isroot()) printf("%d\n", p.v->identifier);
    if(p.isExternal()) return pair < Position, int >(p, depth);;
    if(K == p.v->identifier)
    { return pair < Position, int >(p, depth); }
    else if(K < p.v->identifier)
    { return find_recursive(K, p.left(), depth + 1); }
    else
    { return find_recursive(K, p.right(), depth + 1); }
}

//template < typename Key, typename Value >
//void RedBlackTree< Key, Value >::inorder(const Position& p) const
//{
//    if(p.isExternal())
//    {   return; }
//    inorder(p.left());
//    
//    inorder(p.right());
//}

#endif /* RedBlackTree_h */
