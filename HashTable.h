/*
Copyright 2015 Laurent Claessens
contact : moky.math@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.



INTRODUCTION

 We implement here an ordered dictionary. Some rationale :
 - I did not found something I like (unordered_map, map, etc)
 - since I'm asked to implement such a structure for an homework assignment, this is the occasion.

 The keys are ordered in the order of declaration.
 One get the value associated with 'x' by my_hash_table[x].
 We add at the end and an iterator begins at the beginning.
 When iterating, we get the values (not the keys);

//*/

#include <iostream>
#include <string>

template <class K,class V>
class HashTable
{
    private:
        class node
        {
            template <K,V> friend class HashTable<K,V>;
            private:
                node* next;
            public:
                const K key;
                V value;
                node(const K,V);
                ~node();
        };

        node* first;
        node* last;     // this is the last one, not the 'past-the-last'

    public:
        class iterator
        {
            private:
                node* punt;
            public:
                iterator();
                iterator(node*);
                bool operator==(iterator itr) const;
                bool operator!=(iterator itr) const;
                iterator operator++(int);       //i++
                iterator& operator++();         //++i
                node* operator->() const;
        };
        HashTable();
        void setValue(K ,V );
        bool isKey(K) const;     // return true if the given key exists.

        iterator begin() const;
        iterator end() const;
        V& operator[](const K);
        V& operator[](iterator);
};


// NODE

template<class K,class V>
HashTable<K,V>::node::~node() { if (next) { delete next; } }
template<class K,class V>
HashTable<K,V>::node::node(const K k,V v): next(0),key(k),value(v)  { }

// HASH TABLE

template <class K,class V>
HashTable<K,V>::HashTable(): first(0),last(0)  {}
template <class K,class V>
void HashTable<K,V>::setValue(const K key,V value)
{ 
    if (first) 
    {
        for (HashTable<K,V>::iterator itr=begin();itr!=end();++itr)
        {
            if (K k=itr->key==key)
            {
                throw std::string("You cannot assign a new value to a key");
            }
        }
    }
    node* n=new node(key,value);
    if (!first) 
    { 
        first=n;  
        last=n;
    }
    else
    {
        last->next=n;
        last=n;
    }
}

template <class K,class V>
V& HashTable<K,V>::operator[](HashTable<K,V>::iterator itr ) 
{
    return itr->value;
}
template <class K,class V>
V& HashTable<K,V>::operator[](const K key) 
{
    for ( HashTable<K,V>::iterator itr=begin();itr!=end();++itr )
    {
        if (itr->key==key)
        {
            return itr->value;
        }
    }
    throw std::string( "Illegal key value "+key);
}

// ITERATOR

template <class K,class V>
HashTable<K,V>::iterator::iterator():punt(0){}
template <class K,class V>
HashTable<K,V>::iterator::iterator(HashTable<K,V>::node* n):punt(n){}

template <class K,class V>
typename HashTable<K,V>::iterator HashTable<K,V>::begin() const 
{
    return iterator(first);
}
template <class K,class V>
typename HashTable<K,V>::iterator HashTable<K,V>::end() const  
{
    return iterator(last->next);
}

template <class K,class V>
typename HashTable<K,V>::iterator& HashTable<K,V>::iterator::operator++()
{
    if (punt) punt=punt->next;       // if the iterator points to 0, making "itr++" gives again an itarator pointing to 0.
    return *this;
}
template <class K,class V>
typename HashTable<K,V>::iterator HashTable<K,V>::iterator::operator++(int)
{
    iterator aux=*this;
    ++*this;
    return aux;
}

template <class K,class V>
typename HashTable<K,V>::node*  HashTable<K,V>::iterator::operator->() const
{
    return punt;
}
template <class K,class V>
bool HashTable<K,V>::iterator::operator==(const HashTable<K,V>::iterator itr) const
{
    return punt == itr.punt;
}
template <class K,class V>
bool HashTable<K,V>::iterator::operator!=(const HashTable<K,V>::iterator itr) const
{
    return !(*this==itr);
}

