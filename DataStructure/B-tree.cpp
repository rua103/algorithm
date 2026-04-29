#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

class BTreeNode {
public:
    bool leaf;
    vector<int> keys;
    vector<BTreeNode*> children;

    explicit BTreeNode(bool isLeaf) : leaf(isLeaf) {}
};

class BTree {
private:
    BTreeNode* root;
    int t;

    int size(const vector<int>& v) const {
        return static_cast<int>(v.size());
    }

    void destroy(BTreeNode* node) {
        if (node == nullptr) {
            return;
        }

        for (BTreeNode* child : node->children) {
            destroy(child);
        }

        delete node;
    }

    int findKey(BTreeNode* node, int key) const {
        int index = 0;
        while (index < size(node->keys) && key > node->keys[index]) {
            ++index;
        }
        return index;
    }

    void traverse(BTreeNode* node) const {
        if (node == nullptr) {
            return;
        }

        int i = 0;
        for (; i < size(node->keys); ++i) {
            if (!node->leaf) {
                traverse(node->children[i]);
            }
            cout << node->keys[i] << ' ';
        }

        if (!node->leaf) {
            traverse(node->children[i]);
        }
    }

    bool search(BTreeNode* node, int key) const {
        if (node == nullptr) {
            return false;
        }

        int index = findKey(node, key);
        if (index < size(node->keys) && key == node->keys[index]) {
            return true;
        }

        if (node->leaf) {
            return false;
        }

        return search(node->children[index], key);
    }

    void splitChild(BTreeNode* parent, int index) {
        BTreeNode* child = parent->children[index];
        BTreeNode* rightChild = new BTreeNode(child->leaf);

        int middleKey = child->keys[t - 1];

        for (int j = 0; j < t - 1; ++j) {
            rightChild->keys.push_back(child->keys[j + t]);
        }

        if (!child->leaf) {
            for (int j = 0; j < t; ++j) {
                rightChild->children.push_back(child->children[j + t]);
            }
        }

        child->keys.resize(t - 1);
        if (!child->leaf) {
            child->children.resize(t);
        }

        parent->keys.insert(parent->keys.begin() + index, middleKey);
        parent->children.insert(parent->children.begin() + index + 1, rightChild);
    }

    void insertNonFull(BTreeNode* node, int key) {
        int i = size(node->keys) - 1;

        if (node->leaf) {
            node->keys.push_back(0);

            while (i >= 0 && key < node->keys[i]) {
                node->keys[i + 1] = node->keys[i];
                --i;
            }

            node->keys[i + 1] = key;
            return;
        }

        while (i >= 0 && key < node->keys[i]) {
            --i;
        }
        ++i;

        if (size(node->children[i]->keys) == 2 * t - 1) {
            splitChild(node, i);

            if (key > node->keys[i]) {
                ++i;
            }
        }

        insertNonFull(node->children[i], key);
    }

    int getPredecessor(BTreeNode* node, int index) const {
        BTreeNode* current = node->children[index];

        while (!current->leaf) {
            current = current->children.back();
        }

        return current->keys.back();
    }

    int getSuccessor(BTreeNode* node, int index) const {
        BTreeNode* current = node->children[index + 1];

        while (!current->leaf) {
            current = current->children.front();
        }

        return current->keys.front();
    }

    void removeFromLeaf(BTreeNode* node, int index) {
        node->keys.erase(node->keys.begin() + index);
    }

    void merge(BTreeNode* node, int index) {
        BTreeNode* child = node->children[index];
        BTreeNode* sibling = node->children[index + 1];

        child->keys.push_back(node->keys[index]);

        for (int key : sibling->keys) {
            child->keys.push_back(key);
        }

        if (!child->leaf) {
            for (BTreeNode* siblingChild : sibling->children) {
                child->children.push_back(siblingChild);
            }
        }

        node->keys.erase(node->keys.begin() + index);
        node->children.erase(node->children.begin() + index + 1);

        delete sibling;
    }

    void removeFromInternal(BTreeNode* node, int index) {
        int key = node->keys[index];

        if (size(node->children[index]->keys) >= t) {
            int predecessor = getPredecessor(node, index);
            node->keys[index] = predecessor;
            remove(node->children[index], predecessor);
            return;
        }

        if (size(node->children[index + 1]->keys) >= t) {
            int successor = getSuccessor(node, index);
            node->keys[index] = successor;
            remove(node->children[index + 1], successor);
            return;
        }

        merge(node, index);
        remove(node->children[index], key);
    }

    void borrowFromPrevious(BTreeNode* node, int index) {
        BTreeNode* child = node->children[index];
        BTreeNode* sibling = node->children[index - 1];

        child->keys.insert(child->keys.begin(), node->keys[index - 1]);

        if (!child->leaf) {
            child->children.insert(child->children.begin(), sibling->children.back());
            sibling->children.pop_back();
        }

        node->keys[index - 1] = sibling->keys.back();
        sibling->keys.pop_back();
    }

    void borrowFromNext(BTreeNode* node, int index) {
        BTreeNode* child = node->children[index];
        BTreeNode* sibling = node->children[index + 1];

        child->keys.push_back(node->keys[index]);

        if (!child->leaf) {
            child->children.push_back(sibling->children.front());
            sibling->children.erase(sibling->children.begin());
        }

        node->keys[index] = sibling->keys.front();
        sibling->keys.erase(sibling->keys.begin());
    }

    void fill(BTreeNode* node, int index) {
        if (index != 0 && size(node->children[index - 1]->keys) >= t) {
            borrowFromPrevious(node, index);
            return;
        }

        if (index != size(node->keys) && size(node->children[index + 1]->keys) >= t) {
            borrowFromNext(node, index);
            return;
        }

        if (index != size(node->keys)) {
            merge(node, index);
        } else {
            merge(node, index - 1);
        }
    }

    void remove(BTreeNode* node, int key) {
        int index = findKey(node, key);

        if (index < size(node->keys) && node->keys[index] == key) {
            if (node->leaf) {
                removeFromLeaf(node, index);
            } else {
                removeFromInternal(node, index);
            }
            return;
        }

        if (node->leaf) {
            return;
        }

        bool keyMayBeInLastChild = index == size(node->keys);

        if (size(node->children[index]->keys) < t) {
            fill(node, index);
        }

        if (keyMayBeInLastChild && index > size(node->keys)) {
            remove(node->children[index - 1], key);
        } else {
            remove(node->children[index], key);
        }
    }

public:
    explicit BTree(int minimumDegree) : root(nullptr), t(minimumDegree) {
        if (t < 2) {
            throw invalid_argument("BTree minimum degree must be at least 2");
        }
    }

    ~BTree() {
        destroy(root);
    }

    void insert(int key) {
        if (root == nullptr) {
            root = new BTreeNode(true);
            root->keys.push_back(key);
            return;
        }

        if (size(root->keys) == 2 * t - 1) {
            BTreeNode* newRoot = new BTreeNode(false);
            newRoot->children.push_back(root);

            splitChild(newRoot, 0);

            int childIndex = 0;
            if (key > newRoot->keys[0]) {
                childIndex = 1;
            }

            insertNonFull(newRoot->children[childIndex], key);
            root = newRoot;
            return;
        }

        insertNonFull(root, key);
    }

    void remove(int key) {
        if (root == nullptr) {
            return;
        }

        remove(root, key);

        if (root->keys.empty()) {
            BTreeNode* oldRoot = root;

            if (root->leaf) {
                root = nullptr;
            } else {
                root = root->children[0];
            }

            oldRoot->children.clear();
            delete oldRoot;
        }
    }

    bool search(int key) const {
        return search(root, key);
    }

    void traverse() const {
        traverse(root);
        cout << '\n';
    }
};

int main() {
    BTree tree(3);

    vector<int> values = {10, 20, 5, 6, 12, 30, 7, 17, 3, 2, 1, 8, 9};

    for (int value : values) {
        tree.insert(value);
    }

    cout << "After insert: ";
    tree.traverse();

    cout << "Search 6: " << (tree.search(6) ? "found" : "not found") << '\n';
    cout << "Search 15: " << (tree.search(15) ? "found" : "not found") << '\n';

    tree.remove(6);
    tree.remove(13);
    tree.remove(7);
    tree.remove(4);
    tree.remove(2);
    tree.remove(16);

    cout << "After remove: ";
    tree.traverse();

    return 0;
}
