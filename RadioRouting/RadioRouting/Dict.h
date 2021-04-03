#pragma once

//super light Dict library, for small dictionaries.

template <class Key, class Value> class Dict{
private:
				struct Node{
								// main values
								Key key;
								Value value;
								
								//references
								Node *previous;
								Node *next;
								
								//size of array
								static int count;
								
								Node(Key _key, Value _value){
												this->key = _key;
												this->value = _value;
								}
				};
				
				Node first;
				
public:
				Dict(){};
				~Dict(){};
				
				Value get(Key _key){
								int size = d.size();
								
								for (int i=0; i<size; i++){
												if (d[i].key == _key){
																return d[i].value;
												}
								}
								
								return NULL;
				}
				
				Value operator [](Key _key){
								return this->get(_key);
				}
				
				bool append(Key _key, Value _value){
								if (this->get(_key) != NULL){
												return false;
								}
								else{
												//TODO: add
								}
				}
};

