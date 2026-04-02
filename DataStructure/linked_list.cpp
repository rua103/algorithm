#include <iostream>
#include <string>
#include <stdexcept>

template<typename E>
class MyLinkedList {
    struct Node {
        E val;
        Node* next;
        Node* prev;

        Node(E value): val(value), next(nullptr), prev(nullptr) {}
    };

    Node* head;
    Node* tail;
    int size;

    public:
        MyLinkedList() {
            head = new Node(E());
            tail = new Node(E());
            head->next = tail;
            tail->prev = head;
            size = 0;
        }

        ~MyLinkedList() {
            while (size > 0) {
                removeFirst();
            }
            delete head;
            delete tail;
        }

        void addLast(E e) {
            Node* x = new Node(e);
            Node* temp = tail->prev;
            temp->next = x;
            x->prev = temp;

            x->next = tail;
            tail->prev = x;

            size++;
        }

        void addFirst(E e) {
            Node* x = new Node(e);
            Node* temp = head->next;

            temp->prev = x;
            x->next = temp;

            head->next = x;
            x->prev = head;

            size++;
        }

        void add(int index, E e) {
            checkPositionIndex(index);
            if (index == size) {
                addLast(e);
                return;
            }

            Node* p = getNode(index);
            Node* temp = p->prev;

            Node* x = new Node(e);

            temp->next = x;
            x->prev = temp;

            p->prev = x;
            x->next = p;

            size++;
        }

        E removeFirst() {
            if (size < 1) {
                throw out_of_range("No element to remove");
            }

            Node* x = head->next;
            Node* temp = x->next;

            head->next = temp;
            temp->prev = head;

            x->next = nullptr;
            x->prev = nullptr;

            E val = x->val;
            delete x;

            size--;
            return val;
        }

        E removeLast() {
            if (size < 1) {
                throw out_of_range("No element to remove");
            }
            Node* x = tail->prev;
            Node* temp = x->prev;

            temp->next = tail;
            tail->prev = temp;

            x->next = nullptr;
            x->prev = nullptr;

            E val = x->val;
            delete x;

            size--;
            return val;
        }

        E remove(int index) {
            checkElementIndex(index);
            Node* x = getNode(index);
            Node* prev = x->prev;
            Node* next = x->next;

            prev->next = next;
            next->prev = prev;

            x->next = nullptr;
            x->prev = nullptr;

            E val = x->val;
            delete x;

            size--;
            return val;
        }

        E get(int index) {
            checkElementIndex(index);
            return getNode(index)->val;
        }

        E getFirst() {
            if (size < 1) {
                throw out_of_range("No element to get");
            }

            return head->next->val;
        }

        E getLast() {
            if (size < 1) {
                throw out_of_range("No element to get");
            }

            return tail->prev->val;
        }

        E set(int index, E val) {
            checkElementIndex(index);
            Node* x = getNode(index);

            E oldVal = x->val;
            x->val = val;

            return oldVal;
        }

        int getSize() const {
            return size;
        }

        bool isEmpty() const {
            return size == 0;
        }

        void display() {
            cout << "size = " << size << endl;
            for (Node* p = head->next; p != tail; p = p->next) {
                cout << p->val << " <-> ";
            }
            cout << "nullptr" << endl;
        }
    private:
        Node* getNode(int index) {
            checkElementIndex(index);
            Node* p = head->next;
            for (int i = 0; i < index; i++) {
                p = p->next;
            }

            return p;
        }

        bool isElementIndex(int index) {
            return index >= 0 && index < size;
        }

        bool isPositionIndex(int index) {
            return index >= 0 && index <= size;
        }

        void checkElementIndex(int index) const {
            if (!isElementIndex(index)) {
                throw out_of_range("Index: " + to_string(index) + ", Size: " + to_string(size));
            }
        }

        void checkPositionIndex(int index) const {
            if (!isPositionIndex(index)) {
                throw out_of_range("Index: " + to_string(index) + ", Size: " + to_string(size));
            }
        }
};

int main() {
    MyLinkedList<int> list;
    list.addLast(1);
    list.addLast(2);
    list.addLast(3);
    list.addFirst(0);
    list.add(2, 100);

    list.display();
    // size = 5
    // 0 <-> 1 <-> 100 <-> 2 <-> 3 <-> null

    return 0;
}