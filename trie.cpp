#include<unordered_map>
#include<vector>
using namespace std;

#include <iostream>

using namespace std;

struct Node{
    string value;
    Node* next[256];
};

class Trie{
private:
    Node* root;
    Node* Put(Node* x, string key, string val, int d){
        // cambia valor asociado con llave si en subtrie rooted at x
        if(x== nullptr) x= new Node();
        if(d==key.length()) {x->value=val;}

        else{
            char c = key[d];
            x->next[c] = Put(x->next[c], key, val, d+1);

        }
        return x;
    }
    Node* Delete(Node* x, string key, int d){
        if(x==nullptr) return nullptr;
        if(d==key.length())
            x->value = nullptr;
        else{
            char c = key[d];
            x-> next[c] = Delete(x->next[c], key, d+1);
        }
        if(!x->value.empty()) return x;
        for(char c=0; c<256; c++){
            if(x->next[c] != nullptr) return x;
        }
        return nullptr;
    }
public:
    Trie(){
        root = new Node();
    }
    void Insert(string val, string key){
        root = Put(root,val,key,0);
    }
    void Delete(string key){
        root = Delete(root,key,0);
    }
    bool Search(string key){
        Node* temp = root;
        for(char c:key){
            if(temp->next[c] == nullptr){
                return false;
            }
            temp=temp->next[c];
        }
        return true;
    }
    int size(){
        return size(root);
    }
    int size(Node* x){
        if(x== nullptr) return 0;
        int cnt = 0;
        if(x->value.empty()) cnt++;
        for(int c = 0;c < 256;c++)
            cnt+= size(x->next[c]);
        return cnt;
    }

};

int main() {
    Trie trie;
    trie.Insert("manzana","fruta");
    trie.Insert("banana","fruta");

    cout<<"Buscar 'manzana': "<<trie.Search("manzana")<<endl;
    cout<<"Buscar 'naranja': "<<trie.Search("naranja")<<endl;

    cout<<"Tamaño de Trie: "<<trie.size()<<endl;

    return 0;
}
