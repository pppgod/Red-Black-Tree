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
#include <list>
#include <string>
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
        node() : par(NULL), left(NULL), right(NULL), identifier(NULL), elt(NULL)
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
    private:
        node* v;						// pointer to the node
    public:
        Position(node* _v = NULL) : v(_v) { }		// constructor
        Value& operator*()					// get element
        { return v->elt; }
        Position operator++()
        {
            if(!(this->right()).isExternal())
            {
                Position p = this->right();
                while(!p.left() -> isExternal())
                { p = p.left(); }
                return p;
            }
            
            Position par = this->parent();
            while(*(par.v) < *(this->v))
            {
                par = par.parent();
                if(par.isroot())
                { break; }
            }
            if(*(par.v) < *(this->v))
            { return this->right(); }
            else
            { return par; }
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
    void insert(const Key K, Value V);
    int find(const Key K, Value* v) const;
  //  friend class Position;
// protected 멤버함수
protected:
    void expandExternal(const Position& p);
    Position insert_recursive(const Key K, const Position& p) const;
    inline Position sibling(const Key child_key, const Position& p) const;
    void restructuring(Position& p);
    void recoloring(Position& p);
    Position find_recursive(const Key K, const Position& p) const;
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
    Position p = root();
    while(!(p.left())->isExternal())
    {   p = p.left(); }
    return p;
}

template < typename Key, typename Value >
typename RedBlackTree< Key, Value >::Position RedBlackTree< Key, Value >::end() const
{
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
void RedBlackTree< Key, Value >::insert(const Key K, Value V)
{
    if(empty())
    {
        _root = new node;
        _root->identifier = K;
        _root->elt = V;
        _root->color = BLACK;
        cout << _root->identifier << endl;
        nodeSize++;
        expandExternal(root());
        
        return ;
    }
    Position p = insert_recursive(K, root());
    p.v->identifier = K;
    p.v->elt = V;
    p.v->color = RED;
    expandExternal(p);
    nodeSize++;
    Position p_parent = p.parent();
    while(p_parent.v->color == RED)
    {
        if(sibling(p.v->identifier, p_parent).v->color == RED)
        { recoloring(p_parent); }
        else
        {
            restructuring(p);
            break;
        }
        p_parent = p_parent.parent();
    }
}

template < typename Key, typename Value >
typename RedBlackTree< Key, Value >::Position RedBlackTree< Key, Value >::insert_recursive(const Key K, const Position &p) const
{
    // external node인 경우 재귀 종료
    if(p.isExternal())
    { return p; }
    // 재귀적으로 함수 호출
    if(p.v->identifier > K)
    { return insert_recursive(K, p.left()); }
    else
    { return insert_recursive(K, p.right()); }
}

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
    Position p_parent = p.parent();
    Position p_grand_parent = p_parent.parent();
    Position t1, t2, t3, t4;
    Position par, left_child, right_child;
    bool grand_is_root = p_grand_parent.isroot();
    
    if(*(p_grand_parent.v) < *(p.v))
    {
        left_child = p_grand_parent;
        if(*(p_parent.v) < *(p.v))
        {
            par = p_parent;
            right_child = p;
            //t2 달기
            t2 = p_parent.left();
            left_child.v->right = t2.v;
            t2.v->par = left_child.v;
        }
        else
        {
            par = p;
            right_child = p_parent;
            // t2 달기
            t2 = p.left();
            left_child.v->right = t2.v;
            t2.v->par = left_child.v;
            // t3 달기
            t3 = p.right();
            right_child.v->left = t3.v;
            t3.v->par = right_child.v;
        }
    }
    else
    {
        right_child = p_grand_parent;
        if(*(p_parent.v) < *(p.v))
        {
            par = p;
            left_child = p_parent;
            // t2 달기
            t2 = p.left();
            left_child.v->right = t2.v;
            t2.v->par = left_child.v;
            // t3 달기
            t3 = p.right();
            right_child.v->left = t3.v;
            t3.v->par = right_child.v;
        }
        else
        {
            par = p_parent;
            left_child = p;
            // t2 달기
            t3 = p_parent.right();
            right_child.v->left = t3.v;
            t3.v->par = right_child.v;
        }
    }
    par.v->par = p_grand_parent.parent().v;
    par.v->left = left_child.v;
    par.v->right = right_child.v;
    left_child.v->par = par.v;
    right_child.v->par = par.v;
    par.v->color = BLACK;
    left_child.v->color = RED;
    right_child.v->color = RED;
    if(grand_is_root)
    {
        _root = par.v;
        par.v->par = NULL;
    }
}

template < typename Key, typename Value >
void RedBlackTree< Key, Value >::recoloring(Position& p)
{
    p.v->color = BLACK;
    sibling(p.v->identifier, p.parent()).v->color = BLACK;
    if(!(p.parent()).isroot())
    { p.parent().v->color = RED; }
}

template < typename Key, typename Value >
int RedBlackTree< Key, Value >::find(const Key K, Value* V) const
{
    Position rec = find_recursive(K, root());
    if(rec.isExternal())
        return -1;
    *V = *rec;
    return 1;
    
}

template < typename Key, typename Value >
typename RedBlackTree< Key, Value >::Position RedBlackTree< Key, Value >::find_recursive(const Key K, const Position& p) const
{
    if(p.isExternal()) return p;
    if(K == p.v->identifier)
    { return p; }
    else if(K < p.v->identifier)
    { return find_recursive(K, p.left()); }
    else
    { return find_recursive(K, p.right()); }
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
