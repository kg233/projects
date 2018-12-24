//HashMap.cpp
#include "HashMap.hpp"


namespace {
    unsigned int hasherF(const std::string& s){
        unsigned int result= 0;
        for (int i = 0; i < s.size(); i++){
            result += s[i];
        }
        return result;
    }
}

HashMap::HashMap()
    :hasher{hasherF}, array_capacity{initialBucketCount}, table{new Node*[initialBucketCount]}
{
    for (int i = 0; i < array_capacity; i++){
        table[i] = nullptr;
    }
}

HashMap::HashMap(HashFunction hasher)
    :hasher{hasher}, array_capacity{initialBucketCount}, table{new Node*[initialBucketCount]}
{
    for (int i = 0; i < array_capacity; i++){
        table[i] = nullptr;
    }
}

unsigned int HashMap::get_hashID(const std::string& key) const{
    unsigned int result;
    result = hasher(key) % bucketCount();
    return result;
}

unsigned int HashMap::size() const{

    unsigned int result = 0;

    for (int i = 0; i < bucketCount(); ++i){
        Node* head = table[i];
        while (head != nullptr){
            result++;
            head = head->next;
        }

    }
    return result;
}

void HashMap::delete_one_bucket(Node* head){
    Node* keeper;
    while (head != nullptr){
        keeper = head;

        head = head->next;

        delete keeper;
    }
}


HashMap::~HashMap(){
    for (int i = 0; i < bucketCount(); ++i){
        delete_one_bucket(table[i]);
    }

    delete[] table;

}

bool HashMap::contains(const std::string& key) const{
    unsigned int hash_id = get_hashID(key);
    Node* head = table[hash_id];

    while (head != nullptr){
        if (head->key == key){
            return true;
        }
        head = head->next;
    }
    return false;
}

void HashMap::empty_all_buckets(){
    for (int cap = 0; cap < bucketCount(); ++cap){
        delete_one_bucket(table[cap]);
        table[cap] = nullptr;
    }
}

unsigned int HashMap::bucketCount() const{
    return array_capacity;
}

double HashMap::loadFactor() const{
    return (size() * 1.0)/ (bucketCount() * 1.0);
}

void HashMap::add(const std::string& key, const std::string& value){
    if (not contains(key)){
        Node* new_node = new Node{key, value, nullptr};
        unsigned int hash_id = get_hashID(key);
        Node* head = table[hash_id];
        if (head == nullptr){
            table[hash_id] = new_node;
        }
        else{
            while (head->next != nullptr){
            head = head->next;
            }
            head->next = new_node;
        }
        if (loadFactor() > 0.8){
            rehasher(bucketCount()*2 + 1);
        }
    }
}

void HashMap::visualize(){
    for (int cap = 0; cap < bucketCount(); ++cap){
        Node* head = table[cap];
        std::cout << "Bucket "<< cap << " ";
        if (head == nullptr){
            std::cout << "nullptr" << std::endl;
        }
        else{
            while (head != nullptr){
                std::cout << head->key << ": " << head->value << " -> ";
                head = head->next;
            }
            std::cout << "nullptr" << std::endl;
        }


    }
    std::cout <<"array_capacity: " << array_capacity << "; Load Factor: " << loadFactor() << std::endl;
}

void HashMap::remove(const std::string& key){
    if (contains(key)){
        unsigned int hash_id = get_hashID(key);

        Node* head = table[hash_id];

        if (head->key == key){
            table[hash_id] = head->next;
            delete head;
        }
        else{
            while (head->next != nullptr){
                if (head->next->key == key){
                    Node* temp = head->next;
                    head->next = temp->next;
                    delete temp;
                    break;
                }
                else{
                    head = head->next;
                }
            }
        }
    }
}

std::string HashMap::value(const std::string& key) const{
    std::string result;
    if (contains(key)){
        unsigned int hash_id = get_hashID(key);
        Node* head = table[hash_id];
        while (head->key != key){
            head = head->next;
        }
        result = head->value;
    }
    return result;
}


void HashMap::rehasher(unsigned int new_cap){
    Node** new_table = new Node*[new_cap];
    for (int i = 0; i < new_cap; ++i){
        new_table[i] = nullptr;
    }
    for (int c = 0; c < bucketCount(); c++){
        Node* head = table[c];
        Node* old;
        while (head != nullptr){
            unsigned int new_hash = hasher(head->key) % new_cap;
            Node* temp = new_table[new_hash];
            if (temp == nullptr){
                new_table[new_hash] = new Node{head->key, value(head->key), nullptr};
            }
            else{
                while (temp->next != nullptr){
                    temp = temp->next;
                }
                temp->next = new Node{head->key, value(head->key), nullptr};
            }

            old = head;
            head = head->next;
            remove(old->key);
        }
    }
    delete[] table;
    table = new_table;
    array_capacity = new_cap;
}


unsigned int HashMap::maxBucketSize() const{
    unsigned int max = 0;
    for (int i = 0; i < bucketCount(); ++i){
        Node* head = table[i];
        unsigned int temp = 0;
        while (head != nullptr){
            temp ++;
            head = head->next;
        }
        if (temp > max){
            max = temp;
        }
    }
    return max;
}

void HashMap::copy_table_from(Node** const orig_table , unsigned int cap){
    Node** new_table = new Node*[cap];
    for (int c = 0; c < cap; c++){
        Node* head = orig_table[c];
        if (head == nullptr){
            new_table[c] = nullptr;
        }
        else{
            new_table[c] = new Node{head->key, head->value, nullptr};
            Node* temp = new_table[c];
            while (head->next != nullptr){
                head = head->next;

                temp->next = new Node{head->key, head->value, nullptr};
                temp = temp->next;
            }
        }
    }
    for (int i = 0; i < bucketCount(); ++i){
        delete_one_bucket(table[i]);
    }
    delete[] table;
    table = new_table;
}

HashMap::HashMap(const HashMap& hm)
    :hasher{hm.hasher}, array_capacity{initialBucketCount}, table{new Node*[initialBucketCount]}
{
    for (int i = 0; i < array_capacity; i++){
        table[i] = nullptr;
    }

    copy_table_from(hm.table, hm.bucketCount());
}

HashMap& HashMap::operator=(const HashMap& hm){
    if (this != &hm){
        copy_table_from(hm.table, hm.bucketCount());
        array_capacity = hm.bucketCount();
        hasher = hm.hasher;
    }
    return *this;
}