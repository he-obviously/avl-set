#pragma once
#include <initializer_list>
#include <cassert>

template<class T>
class Node {
public:
    T key;

    Node() = default;

    explicit Node(T k) {
        this->key = k;
    }

    Node(const Node& v) {
        this = Copy(v);
    }

    static Node* Copy(Node* v) {
        if (v == nullptr) {
            return nullptr;
        } else {
            Node *u = new Node(v->key);
            u->parent = v->parent;
            u->left = Copy(v->left);
            u->right = Copy(v->right);
        }
    }

    ~Node() {
        Delete(this);
    }

    static void Delete(Node* v) {
        if (v == nullptr) {
            return;
        } else {
            Delete(v->left);
            Delete(v->right);
            delete v;
        }
    }

    static Node* Search(Node* v, T val) {
        if (v == nullptr) {
            return v;
        } else if (val < v->key) {
            return Search(v->left, val);
        } else if (v->key < val) {
            return Search(v->right, val);
        } else {
            return v;
        }
    }

    static Node* LowerBound(Node* v, T val) {
        Node* ans = nullptr;
        while (v != nullptr) {
            if (!(v->key < val)) {
                ans = v;
                v = v->left;
            } else {
                v = v->right;
            }
        }
        return ans;
    }

    static Node* Insert(Node* v, T val) {
        if (v == nullptr) {
            return new Node(val);
        } else if (val < v->key) {
            v->left = Insert(v->left, val);
            v->left->parent = v;
            return v->Balance();
        } else {
            v->right = Insert(v->right, val);
            v->right->parent = v;
            return v->Balance();
        }
    }

    static Node* Erase(Node* v, T val) {
        if (v->left == nullptr && v->right == nullptr) {
            return nullptr;
        }
        while (v->key < val || val < v->key) {
            if (val < v->key) {
                v = v->left;
            } else {
                v = v->right;
            }
        }
        if (v->left == nullptr && v->right == nullptr) {
            if (v->parent->left == v) {
                v->parent->left = nullptr;
            } else {
                v->parent->right = nullptr;
            }
            v = v->parent;
        } else if (v->right == nullptr) {
            if (v->parent != nullptr) {
                if (v->parent->left == v) {
                    v->parent->left = v->left;
                } else {
                    v->parent->right = v->left;
                }
            }
            if (v->left != nullptr) {
                v->left->parent = v->parent;
            }
            v = v->left;
        } else {
            Node* min = FindMin(v->right);
            if (v->parent != nullptr) {
                if (v->parent->left == v) {
                    v->parent->left = min;
                } else {
                    v->parent->right = min;
                }
            }
            v->right->parent = nullptr;
            min->right = EraseMin(v->right);
            min->parent = v->parent;
            min->left = v->left;
            if (min->left != nullptr) {
                min->left->parent = min;
            }
            if (min->right != nullptr) {
                min->right->parent = min;
            }
            v = min;
        }
        while (v->parent != nullptr) {
            v = v->Balance();
            v = v->parent;
        }
        return v->Balance();
    }

    static Node* Next(Node* v) {
        if (v->right != nullptr) {
            return FindMin(v->right);
        } else {
            Node* u = v->parent;
            while (u != nullptr && v == u->right) {
                v = u;
                u = u->parent;
            }
            return u;
        }
    }

    static Node* Prev(Node* v) {
        if (v == nullptr) {
            return v;
        } else if (v->left != nullptr) {
            return FindMax(v->left);
        } else {
            Node* u = v->parent;
            while (u != nullptr && v == u->left) {
                v = u;
                u = u->parent;
            }
            return u;
        }
    }

    static Node* FindMin(Node* v) {
        if (v->left == nullptr) {
            return v;
        } else {
            return FindMin(v->left);
        }
    }

    static Node* FindMax(Node* v) {
        if (v->right == nullptr) {
            return v;
        } else {
            return FindMax(v->right);
        }
    }

    /*static void Print(Node* v) {
        if (v == nullptr) return;
        Print(v->left);
        std::cout << v->key << " (parent = ";
        std::cout << (v->parent ? v->parent->key : -1);
        std::cout << ")\n";
        Print(v->right);
    }*/

private:
    int height = 1;
    Node* parent = nullptr;
    Node* left = nullptr;
    Node* right = nullptr;

    int Diff() const {
        int left_height = (left == nullptr ? 0 : left->height),
                right_height = (right == nullptr ? 0 : right->height);
        return left_height - right_height;
    }

    void RecalcHeight() {
        int left_height = (left == nullptr ? 0 : left->height),
                right_height = (right == nullptr ? 0 : right->height);
        height = (left_height < right_height ? right_height : left_height) + 1;
    }

    Node* RotateLeft() {
        Node<T>* u = right;
        u->parent = parent;
        parent = u;
        if (u->left != nullptr) {
            u->left->parent = this;
        }
        right = u->left;
        u->left = this;
        RecalcHeight();
        u->RecalcHeight();
        return u;
    }

    Node* RotateRight() {
        Node<T>* u = left;
        u->parent = parent;
        parent = u;
        if (u->right != nullptr) {
            u->right->parent = this;
        }
        left = u->right;
        u->right = this;
        RecalcHeight();
        u->RecalcHeight();
        return u;
    }

    Node* BigRotateLeft() {
        right = right->RotateRight();
        return RotateLeft();
    }

    Node* BigRotateRight() {
        left = left->RotateLeft();
        return RotateRight();
    }

    Node* Balance() {
        RecalcHeight();
        if (Diff() == -2) {
            if (right->Diff() <= 0) {
                return RotateLeft();
            } else {
                return BigRotateLeft();
            }
        }
        if (Diff() == 2) {
            if (left->Diff() >= 0) {
                return RotateRight();
            } else {
                return BigRotateRight();
            }
        }
        return this;
    }

    static Node* EraseMin(Node* v) {
        if (v == nullptr) {
            return v;
        }
        while (v->left != nullptr) {
            v = v->left;
        }
        if (v->parent == nullptr) {
            if (v->right != nullptr) {
                v->right->parent = nullptr;
            }
            return v->right;
        } else {
            v->parent->left = v->right;
            if (v->right != nullptr) {
                v->right->parent = v->parent;
            }
            v = v->parent;
            while (v->parent != nullptr) {
                v = v->Balance();
                v = v->parent;
            }
            return v->Balance();
        }
    }

};

template<class T>
class Set {
public:
    class iterator {
    public:
        Node<T>* ptr = nullptr;

        iterator() = default;

        explicit iterator(Node<T>* v) {
            ptr = v;
        }

        iterator(const iterator& v) {
            ptr = v.ptr;
        }

        friend bool operator==(iterator a, iterator b) {
            return a.ptr == b.ptr;
        }

        friend bool operator!=(iterator a, iterator b) {
            return a.ptr != b.ptr;
        }

        iterator& operator++() {
            ptr = Node<T>::Next(ptr);
            return *this;
        }

        iterator& operator--() {
            ptr = Node<T>::Prev(ptr);
            return *this;
        }

        iterator operator++(int) {
            iterator temp(ptr);
            ++(*this);
            return temp;
        }

        iterator operator--(int) {
            iterator temp(ptr);
            --(*this);
            return temp;
        }

        T operator*() {
            return ptr->key;
        }

        const T* operator->() {
            return &(ptr->key);
        }
    };

    Set() = default;

    template<typename Iterator>
    Set(Iterator a, Iterator b) {
        Iterator c = a;
        while (c != b) {
            Insert(*c);
            ++c;
        }
    }

    Set(const std::initializer_list<T>& a) {
        for (auto& elem : a) {
            Insert(elem);
        }
    }

    ~Set() {
        delete root_;
        begin_ = iterator();
    }

    Set& operator=(const Set& a) {
        if (this == &a) {
            return *this;
        }
        root_ = a.root_;
        begin_ = a.begin_;
    }

    iterator begin() const {
        return begin_;
    }

    iterator end() const {
        return end_;
    }

    int size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }

    iterator find(const T& key) const {
        Node<T>* v = Search(key);
        return iterator(v);
    }

    void insert(const T& key) {
        Insert(key);
    }

    void erase(const T& key) {
        Erase(key);
    }

    iterator lower_bound(const T& key) const {
        Node<T>* v = LowerBound(key);
        return iterator(v);
    }

private:
    Node<T>* root_ = nullptr;
    iterator begin_, end_;
    int size_ = 0;

    Node<T>* Search(T key) const {
        return Node<T>::Search(root_, key);
    }

    void Insert(T key) {
        if (Node<T>::Search(root_, key) == nullptr) {
            ++size_;
            root_ = Node<T>::Insert(root_, key);
            Node<T>* min = Node<T>::FindMin(root_);
            begin_.ptr = min;
        }
    }

    void Erase(T key) {
        if (Node<T>::Search(root_, key) != nullptr) {
            --size_;
            root_ = Node<T>::Erase(root_, key);
            if (size_ == 0) {
                begin_.ptr = nullptr;
            } else {
                Node<T>* min = Node<T>::FindMin(root_);
                begin_.ptr = min;
            }
        }
    }

    Node<T>* LowerBound(T key) const {
        return Node<T>::LowerBound(root_, key);
    }

};
