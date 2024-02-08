#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MIN_KEYS = 2;
const int MAX_KEYS = 4;

struct BPlusTreeNode {
    bool is_leaf;
    vector<int> keys;
    vector<BPlusTreeNode*> children;
};

BPlusTreeNode* createNode(bool is_leaf) {
    BPlusTreeNode* node = new BPlusTreeNode;
    node->is_leaf = is_leaf;
    return node;
}

void printTree(BPlusTreeNode* root, int depth = 0) {
    if (root != nullptr) {
        for (int i = 0; i < root->keys.size(); ++i) {
            cout << root->keys[i] << " ";
        }
        cout << endl;
        if (!root->is_leaf) {
            for (int i = 0; i < root->children.size(); ++i) {
                printTree(root->children[i], depth + 1);
            }
        }
    }
}



// Función para insertar una clave en un árbol B+
void insert(BPlusTreeNode*& root, int key) {
    if (root == nullptr) {
        root = createNode(true);
        root->keys.push_back(key);
        return;
    }

    if (root->is_leaf) {
        // Insertar la clave en el nodo hoja
        root->keys.push_back(key);
        sort(root->keys.begin(), root->keys.end());
        return;
    }

    // Encontrar el hijo adecuado y llamar recursivamente a insertar
    int i = 0;
    while (i < root->keys.size() && key > root->keys[i]) {
        i++;
    }
    insert(root->children[i], key);
}

// Función para buscar una clave en un árbol B+
bool search(BPlusTreeNode* root, int key) {
    if (root == nullptr) {
        return false;
    }

    // Buscar en los nodos hoja
    if (root->is_leaf) {
        for (int k : root->keys) {
            if (k == key) {
                return true;
            }
        }
        return false;
    }

    // Buscar en los nodos internos
    int i = 0;
    while (i < root->keys.size() && key > root->keys[i]) {
        i++;
    }
    return search(root->children[i], key);
}

// Función para eliminar una clave de un árbol B+
void remove(BPlusTreeNode* root, int key) {
    if (root == nullptr) {
        return;
    }

    // Buscar la posición de la clave en el nodo
    int i = 0;
    while (i < root->keys.size() && key > root->keys[i]) {
        i++;
    }

    if (i < root->keys.size() && root->keys[i] == key && root->is_leaf) {
        // Eliminar la clave si se encuentra en un nodo hoja
        root->keys.erase(root->keys.begin() + i);
        return;
    }

    if (!root->is_leaf) {
        // Encontrar el hijo adecuado y llamar recursivamente a remove
        remove(root->children[i], key);
    }

    // Después de eliminar, verificar si el nodo hijo necesita redistribución o fusión
    if (root->children[i] != nullptr && root->children[i]->keys.size() < MIN_KEYS) {
        if (i > 0 && root->children[i - 1]->keys.size() > MIN_KEYS) {
            // Redistribuir desde el nodo izquierdo
            BPlusTreeNode* left_sibling = root->children[i - 1];
            int borrow_key = left_sibling->keys.back();
            left_sibling->keys.pop_back();

            // Insertar el valor tomado del nodo hermano en el nodo actual
            root->children[i]->keys.insert(root->children[i]->keys.begin(), borrow_key);
            if (!root->children[i]->is_leaf) {
                BPlusTreeNode* borrow_child = left_sibling->children.back();
                left_sibling->children.pop_back();
                root->children[i]->children.insert(root->children[i]->children.begin(), borrow_child);
            }

            root->keys[i - 1] = root->children[i]->keys.front();
        } else if (i < root->keys.size() && root->children[i + 1]->keys.size() > MIN_KEYS) {
            // Redistribuir desde el nodo derecho
            BPlusTreeNode* right_sibling = root->children[i + 1];
            int borrow_key = right_sibling->keys.front();
            right_sibling->keys.erase(right_sibling->keys.begin());

            // Insertar el valor tomado del nodo hermano en el nodo actual
            root->children[i]->keys.push_back(borrow_key);
            if (!root->children[i]->is_leaf) {
                BPlusTreeNode* borrow_child = right_sibling->children.front();
                right_sibling->children.erase(right_sibling->children.begin());
                root->children[i]->children.push_back(borrow_child);
            }

            root->keys[i] = right_sibling->keys.front();
        } else {
            // Fusión con el hermano izquierdo o derecho
            if (i > 0) {
                // Fusionar con el nodo izquierdo
                BPlusTreeNode* left_sibling = root->children[i - 1];
                left_sibling->keys.push_back(root->keys[i - 1]);
                left_sibling->keys.insert(left_sibling->keys.end(), root->children[i]->keys.begin(), root->children[i]->keys.end());

                if (!root->children[i]->is_leaf) {
                    left_sibling->children.insert(left_sibling->children.end(), root->children[i]->children.begin(), root->children[i]->children.end());
                }

                // Eliminar el nodo fusionado y la clave
                delete root->children[i];
                root->children.erase(root->children.begin() + i);
                root->keys.erase(root->keys.begin() + (i - 1));
            } else {
                // Fusionar con el nodo derecho
                BPlusTreeNode* right_sibling = root->children[i + 1];
                root->children[i]->keys.push_back(root->keys[i]);
                root->children[i]->keys.insert(root->children[i]->keys.end(), right_sibling->keys.begin(), right_sibling->keys.end());

                if (!root->children[i]->is_leaf) {
                    root->children[i]->children.insert(root->children[i]->children.end(), right_sibling->children.begin(), right_sibling->children.end());
                }

                // Eliminar el nodo fusionado y la clave
                delete root->children[i + 1];
                root->children.erase(root->children.begin() + (i + 1));
                root->keys.erase(root->keys.begin() + i);
            }
        }
    }
}


int main() {
    BPlusTreeNode* root = nullptr;

    // Insertar claves
    insert(root, 10);
    insert(root, 20);
    insert(root, 5);

    // Visualizar el árbol después de insertar claves
    cout << "arbol despues de insertar claves:" << endl;
    printTree(root);

    // Buscar claves
    cout << "Buscar 10: " << (search(root, 10) ? "Encontrado" : "No encontrado") << endl;
    cout << "Buscar 15: " << (search(root, 15) ? "Encontrado" : "No encontrado") << endl;

    // Eliminar claves
    remove(root, 10);

    // Visualizar el árbol después de eliminar claves
    cout << "arbol despues de eliminar la clave 10:" << endl;
    printTree(root);

    return 0;
}
