#include <iostream>
#include <vector>
using namespace std;

struct Nodo {
    int key;
    int degree;
    Nodo* parent;
    Nodo* child;
    Nodo* sibling;

    explicit Nodo(int k) {
        key = k;
        degree = 0;
        parent = child = sibling = nullptr;
    }
};

class BinomialHeap {
private:
    Nodo* head;
    std::vector<Nodo*> roots; // roots[i] = raíz del árbol de grado i (o nullptr)
    int nodeCount;

    // Une dos árboles del mismo grado
    static void linkTrees(Nodo* t1, Nodo* t2) {
        // t1 será la raíz (menor valor)
        t2->parent = t1;
        t2->sibling = t1->child;
        t1->child = t2;
        t1->degree++;
    }

public:
    BinomialHeap() : head(nullptr), nodeCount(0) {}

    // INSERT optimizado por representación binaria del conteo de nodos
    void insert(int key) {
        nodeCount++;
        Nodo* carry = new Nodo(key); // nuevo árbol B0
        int i = 0;

        // Asegurar tamaño suficiente del vector de raíces
        while (true) {
            if (i >= (int)roots.size()) roots.resize(i + 1, nullptr);

            if (roots[i] == nullptr) {
                // No hay árbol de grado i, colocamos carry aquí
                roots[i] = carry;
                break;
            } else {
                // Ya hay un árbol de grado i: unir (carry + roots[i]) -> carry para i+1
                Nodo* existing = roots[i];
                roots[i] = nullptr;

                // Determinar cuál será la nueva raíz (menor clave)
                if (existing->key <= carry->key) {
                    linkTrees(existing, carry);
                    carry = existing;
                } else {
                    linkTrees(carry, existing);
                    // carry ya apunta a la nueva raíz (la que quedó)
                }
                i++; // continuar el "carry" al siguiente grado
            }
        }

        // Reconstruir la lista simple `head` en orden creciente de grado
        std::vector<Nodo*> nonNull;
        nonNull.reserve(roots.size());
        for (size_t j = 0; j < roots.size(); ++j) {
            if (roots[j]) nonNull.push_back(roots[j]);
        }

        head = nullptr;
        Nodo* prev = nullptr;
        for (size_t k = 0; k < nonNull.size(); ++k) {
            if (k == 0) {
                head = nonNull[0];
                prev = head;
            } else {
                prev->sibling = nonNull[k];
                prev = prev->sibling;
            }
            prev->sibling = nullptr; // asegurar terminación
        }
    }

    void print() {
        Nodo* curr = head;
        while (curr) {
            cout << "Tree of degree " << curr->degree
                 << " with root " << curr->key << endl;
            curr = curr->sibling;
        }
    }
};

int main() {
    BinomialHeap heap;

    // Insertamos varios valores
    heap.insert(10);
    heap.insert(20);
    heap.insert(5);
    heap.insert(7);
    heap.insert(3);
    heap.insert(17);
    heap.insert(19);


    cout << "Estado del Binomial Heap:\n";
    heap.print();

    return 0;
}