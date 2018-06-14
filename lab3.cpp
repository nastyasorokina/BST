# BST
#include <iostream>
#include <iterator>
#include <vector>

using namespace std;

template<typename T>
struct Node {

    Node <T> *parent;
    Node <T> *leftChild;
    Node <T> *rightChild;
    T value;

    Node() : value(T()), leftChild(nullptr), rightChild(nullptr), parent(nullptr) {};

    ~Node() {
        delete leftChild;
        delete rightChild;
    }

    Node(const Node<T> &node) : value(node.value) {
        if (node.leftChild != nullptr) {
            leftChild = new Node<T>(*node.leftChild);
            leftChild->parent = this;
        }
        else
            leftChild = nullptr;
        if (node.rightChild != nullptr) {
            rightChild = new Node<T>(*node.rightChild);
            rightChild->parent = this;
        }
        else
            rightChild = nullptr;
    }

    Node& operator = (const Node<T> &node) {
        this->~Node();
        new (this)Node(node);
        return *this;
    }

};


template<typename T, class Compare = std::less<T>>
class BinaryTree {

private:
    Node<T> *root;
    size_t s;

    bool checkExists(int val)
    {
        return search_method(val) != nullptr;
    }

    Node<T>* search_method(const T& val) {
        Node<T> *currentNode = root;
        while (currentNode != nullptr && val != currentNode->value) {
            if (val < currentNode->value) {
                currentNode = currentNode->leftChild;
            }
            else {
                currentNode = currentNode->rightChild;
            }
        }
        return currentNode;
    }

public:
    template<typename T1>
    class Iterator : public std::iterator<std::bidirectional_iterator_tag, Node<T1>> {
        friend class BinaryTree<T1>;

    public:
        Iterator(Node<T> *node) {
            it = node;
        }

        Iterator& operator ++() {
            if (it->rightChild == nullptr) {
                Node<T> *p;
                p = it->parent;
                while (p != nullptr && p->rightChild == it) {
                    it = p;
                    p = p->parent;
                }
                it = p;
            }
            else {
                it = it->rightChild;
                while (it->leftChild != nullptr) {
                    it = it->leftChild;
                }
            }
            return *this;
        }

        Iterator& operator --() {
            if (it->leftChild == nullptr) {
                Node<T> *p;
                p = it->parent;
                while (p != nullptr && p->leftChild == it) {
                    it = p;
                    p = p->parent;
                }
                it = p;
            }
            else {
                it = it->leftChild;
                while (it->rightChild != nullptr) {
                    it = it->rightChild;
                }
            }
            return *this;
        }

        bool operator == (Iterator& iter) {
            if (iter.it == it) {
                return true;
            }
            else {
                return false;
            }
        }

        bool operator != (Iterator& iter) {
            return !(*this == iter);
        }

        typename Iterator::reference operator*() const {
            return *it;
        }

    private:
        Node <T> *it;
    };

    typedef Iterator<T> iterator;
    typedef Iterator<T> const_iterator;

    BinaryTree() : root(new Node<T>), s(0) {
        root->value = T();
    };

    ~BinaryTree() {
        delete root;
    }

    BinaryTree(const BinaryTree &bst) {
        s = bst.s;
        root = new Node<T>(*bst.root);
    };

    BinaryTree& operator = (const BinaryTree &bst) {
        s = bst.s;
        root = new Node<T>(*bst.root);
        return *this;
    };

    template <class InputIt>
    void assign(InputIt first, InputIt last) {
        while (first != last) {
            insert(*first);
            first++;
        }
    }

    Node<T>* beg() {
        Node<T> *n = root;
        while (n->leftChild != nullptr) {
            n = n->leftChild;
        }
        return n;
    }

    Node<T>* en() {
        Node<T> *n = root;
        while (n->rightChild != nullptr) {
            n = n->rightChild;
        }
        return n;
    }

    iterator begin() {
        return iterator(beg());
    }

    const_iterator cbegin() {
        return const_iterator(beg());
    }

    iterator end() {
        return iterator(en());
    }

    const_iterator cend() {
        return const_iterator(en());
    }

    iterator insert(const T& val) {
        if (checkExists(val)) {
            return find(val);
        }
        s++;
        Node<T> *buffNode = new Node<T>();
        buffNode->parent = buffNode->leftChild = buffNode->rightChild = nullptr;
        buffNode->value = val;
        Node<T> *currentPosition = root;
        while (currentPosition != nullptr)
        {
            buffNode->parent = currentPosition;
            if (!(buffNode->value < currentPosition->value)) {
                currentPosition = currentPosition->rightChild;
            }
            else {
                currentPosition = currentPosition->leftChild;
            }
        }
        if (buffNode->parent == nullptr) {
            root = buffNode;
        }
        else if (buffNode->value < buffNode->parent->value) {
            buffNode->parent->leftChild = buffNode;
        }
        else {
            buffNode->parent->rightChild = buffNode;
        }
        return iterator(buffNode);
    }

    void remove(const T& val) {
        Node<T>* nodeForDelete = &*find(val);

        if (nodeForDelete == nullptr)
            return;

        Node<T> *buff1, *buff2;

        if (nodeForDelete->leftChild == nullptr || nodeForDelete->rightChild == nullptr)
            buff1 = nodeForDelete;
        else
            buff1 = &*(++find(val));

        if (buff1->leftChild != nullptr)
            buff2 = buff1->leftChild;
        else
            buff2 = buff1->rightChild;

        if (buff2 != nullptr)
            buff2->parent = buff1->parent;
        if (buff1->parent == nullptr)
            root = buff2;

        else if (buff1 == buff1->parent->leftChild)
            buff1->parent->leftChild = buff2;
        else
            buff1->parent->rightChild = buff2;

        if (buff1 != nodeForDelete)
            nodeForDelete->value = buff1->value;
        s--;
        delete buff1;
    }

    iterator find(const T& val) {
        return iterator(search_method(val));
    }

    const_iterator cfind(const T& val) {
        return const_iterator(search_method(val));
    }

    bool empty() const {
        return s == 0;
    };

    size_t size() const {
        return s;
    };
};

template<typename T>
void get_tree(BinaryTree<T> bst_test) {
    BinaryTree<int>::iterator s1 = bst_test.begin();
    BinaryTree<int>::iterator f1 = bst_test.end();
    ++s1;
    while (s1 != f1) {
        cout << (*s1).value << " ";
        ++s1;
    }
    cout << (*s1).value << " ";
    cout << endl;
}

template<typename T>
void previous_and_next(BinaryTree<T> Sample_BST1, T val) {
    BinaryTree<int>::iterator f = Sample_BST1.find(val);
    ++f;
    cout << "++f = " << (*f).value << endl;
    --(--f);
    cout << "--f = " << (*f).value << endl;
}

template<typename T>
void check(BinaryTree<T> bst_test, T arg0) {
    get_tree(bst_test);
    previous_and_next(bst_test, arg0);
    cout << "size = " << bst_test.size() << endl;
    bst_test.remove(arg0);
    get_tree(bst_test);
    cout << "size = " << bst_test.size() << endl;
    if (bst_test.empty())
        cout << "empty" << endl;
    else
        cout << "not empty" << endl;
}

int main() {
    BinaryTree<int> Sample_BST1;
    BinaryTree<int> Sample_BST2;
    vector<int> vec = {49, 24, 76, 62, 90, 55, 26, 10, 74};

    Sample_BST1.insert(49);
    Sample_BST1.insert(24);
    Sample_BST1.insert(76);
    Sample_BST1.insert(62);
    Sample_BST1.insert(90);
    Sample_BST1.insert(55);
    Sample_BST1.insert(26);
    Sample_BST1.insert(10);
    Sample_BST1.insert(74);

    Sample_BST2.assign(vec.begin(), vec.end());

    check(Sample_BST1, 49);
    cout << endl << "--------------" << endl;
    check(Sample_BST2, 49);

    return 0;
}
