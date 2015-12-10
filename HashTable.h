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
//*/




// We implement here an ordered dictionary. Some rationale :
// - I did not found something I like (unordered_map, map, etc)
// - since I'm asked to implement such a structure for an homework assignment, this is the occasion.
//
// The keys are ordered in the order of declaration.
// One get the value associated with 'x' by my_hash_table[x].
// We add at the end and an iterator begins at the beginning.
template <class K,class V>
class HashTable
{
    class node
    {
        K key;
        V value;
        node* next;
    };

    private :
        node* first;
        node* lase;
    public:
        HashTable();
        setValue(K ,V );
};

template <class K,class V>
HashTable<K,V>::HashTable(): first(0)  {}
HashTable<K,V>::setValue(K jey,V value)
{
}
