/*
    ॐ
    Sachin Kumar Gupta
    Roll No. 21075074
    B.Tech CSE
*/

#include <bits/stdc++.h>
using namespace std;
int order_of_tree = 4;
class Node
{
public:
    int order;
    /*
    to store the order of the node
    Order = maximum number of children a node can have
    */
    bool isLeafNode;
    // to check if this is a lead node or not
    vector<int> data;
    // to store the keys that are present in that node
    int sz;
    // to store current size
    vector<Node *> pointers;
    // to store the pointers to the children
    Node *parent;
    Node(int order)
    {
        this->order = order;
        data.assign(order - 1, 0);
        pointers.assign(order, nullptr);
        this->parent = nullptr;
        this->isLeafNode = 0;
        this->sz = 0;
    }
};
class BPlusTree
{
private:
    int order;
    Node *root;
    int min_number_of_children, max_number_of_children;

public:
    BPlusTree(int order)
    {
        root = nullptr;
        this->order = order;
        this->min_number_of_children = (order + 1) / 2;
        this->max_number_of_children = order;
    }
    void insert(int key)
    {
        if (root == nullptr)
        {
            Node *temp = new Node(this->order);
            temp->data[0] = key;
            temp->isLeafNode = 1;
            temp->sz = 1;
            root = temp;
            return;
        }
        Node *curr = root;
        Node *par = new Node(order);
        par = nullptr;
        while (!curr->isLeafNode)
        {
            if (par != nullptr)
                curr->parent = par;
            par = curr;
            for (int id = 0; id < curr->sz; ++id)
            {
                if (key < curr->data[id])
                {
                    // moving to the child node
                    curr = curr->pointers[id];
                    if (par != nullptr)
                        curr->parent = par;
                    break;
                }
                if (id == curr->sz - 1)
                {
                    curr = curr->pointers[id + 1];
                    if (par != nullptr)
                        curr->parent = par;
                    break;
                }
            }
        }
        if (curr->sz < order - 1)
        {
            /*
            We are at the leaf node and we can insert the new value here
            */
            int id = 0;
            while (curr->data[id] < key and id < curr->sz)
                id++;
            for (int j = curr->sz; j > id; j--)
                curr->data[j] = curr->data[j - 1];
            curr->sz = curr->sz + 1; // size increased
            curr->data[id] = key;    // inserted at sorted position
            curr->pointers[curr->sz] = curr->pointers[curr->sz - 1];
            // changing the pointer to child node
            curr->pointers[curr->sz - 1] = nullptr;
            if (curr != nullptr)
                curr->parent = par;
            return;
        }
        int size1 = (order) / 2, size2 = (order)-size1;
        // merge all the keys into new node structure
        vector<int> arb;
        for (auto x : curr->data)
            arb.push_back(x);
        arb.push_back(key);
        sort(arb.begin(), arb.end());
        Node *newLeaf = new Node(order);
        newLeaf->isLeafNode = true;
        curr->sz = size1;
        // Splitting the current node into two
        newLeaf->sz = size2;
        for (int i = 0; i < size1; i++)
            curr->data[i] = arb[i];
        for (int i = 0; i < size2; i++)
            newLeaf->data[i] = arb[i + size1];
        if (curr == root)
        {
            Node *newRoot = new Node(order);
            // New Record pointer either side of the new Root
            newRoot->data[0] = newLeaf->data[0];
            newRoot->pointers[0] = curr, newRoot->pointers[1] = newLeaf;
            newRoot->sz = 1;
            if (curr != nullptr)
                curr->parent = newRoot;
            if (newLeaf != nullptr)
                newLeaf->parent = newRoot;
            root = newRoot;
            newRoot->parent = nullptr;
            return;
        }
        // Recursive merging and splittin if overflow occurs
        if (curr != nullptr)
            curr->parent = par;
        RecursiveInsertion(par, arb[size1], newLeaf);
        return;
    }
    void RecursiveInsertion(Node *curr, int key, Node *record_pointer)
    {
        if (curr->sz < order - 1)
        {
            int id = 0;
            while (curr->data[id] < key and id < curr->sz)
                id += 1;
            for (auto j = curr->sz; j > id; j -= 1)
                curr->data[j] = curr->data[j - 1];
            for (auto j = curr->sz + 1; j > id + 1; j -= 1)
                curr->pointers[j] = curr->pointers[j - 1];
            curr->sz = curr->sz + 1; // size increased
            curr->data[id] = key;    // inserted at sorted position
            curr->pointers[id + 1] = record_pointer;
            record_pointer->parent = curr;
            return;
        }
        int size1 = (order) / 2, size2 = order - 1 - size1;
        // merge all the keys into new node structure
        vector<int> arb_keys(order, 0);
        vector<Node *> RecordPointers(order + 1, nullptr);
        for (int i = 0; i < curr->sz; i++)
            arb_keys[i] = curr->data[i];
        for (int i = 0; i < curr->sz + 1; i++)
            RecordPointers[i] = curr->pointers[i];
        int id = 0;
        while (key > curr->data[id] && id < order - 1)
            id++;
        for (int j = curr->sz + 1; j > id; j--)
            arb_keys[j] = arb_keys[j - 1];
        for (int j = curr->sz + 2; j > id + 1; j--)
            RecordPointers[j] = RecordPointers[j - 1];
        arb_keys[id] = key;
        RecordPointers[id + 1] = record_pointer;
        // Splitting the node into two
        Node *newIntermediate = new Node(order);
        curr->sz = size1;
        newIntermediate->sz = size2;
        for (int i = 0; i < size2; i++)
            newIntermediate->data[i] = arb_keys[i + size1 + 1];
        (id < size1) ? (record_pointer->parent = curr) : (record_pointer->parent = newIntermediate);
        for (int i = 0; i < size2 + 1; i++)
            newIntermediate->pointers[i] = RecordPointers[i + size1 + 1];
        if (curr == root)
        {
            Node *newRoot = new Node(order);
            // New Record pointer either side of the new Root
            newRoot->pointers[0] = curr, newRoot->pointers[1] = newIntermediate;
            newRoot->data[0] = arb_keys[size1];
            newRoot->sz = 1;
            if (curr != nullptr)
                curr->parent = newRoot;
            if (newIntermediate != nullptr)
                newIntermediate->parent = newRoot;
            root = newRoot; // updating parent
            newRoot->parent = nullptr;
            return;
        }
        RecursiveInsertion(curr->parent, arb_keys[size1], newIntermediate);
        return;
    }
    void remove(Node *target, int key)
    {
        Node *curr = root;
        int leftIndex, rightIndex;
        // To find the left and right neighbour of the current node
        while (!curr->isLeafNode)
        {
            for (int id = 0; id < curr->sz; ++id)
            {
                leftIndex = id - 1, rightIndex = id + 1;
                if (key < curr->data[id])
                {
                    curr = curr->pointers[id];
                    break;
                }
                if (id == curr->sz - 1)
                {
                    leftIndex = id;
                    rightIndex = id + 2;
                    curr = curr->pointers[id + 1];
                    break;
                }
            }
        }
        // deletion of that value directly
        int id;
        for (id = 0; id < target->sz; ++id)
            if (target->data[id] == key)
                break;
        for (int j = id; j < target->sz; j++)
            target->data[j] = target->data[j + 1];
        for (int j = id; j < target->sz + 1; j++)
            target->pointers[j] = target->pointers[j + 1];
        target->pointers[target->sz] = nullptr;
        target->sz--;
        if (target == root)
        {
            // if root becomes empty then making it null
            if (target->sz == 0)
            {
                printf("Tree is Empty Now!");
                root = nullptr;
            }
            return;
        }
        if (target->sz >= min_number_of_children - 1)
            return;
        // Borrowing from the left sibling
        if (leftIndex >= 0)
        {
            Node *left = target->parent->pointers[leftIndex];
            if (left->sz >= min_number_of_children)
            {
                int index_to_transfer = left->sz - 1;
                // tranfering the last index in the left node to the target node
                for (auto j = target->sz; j > 0; j--)
                    target->data[j] = target->data[j - 1];
                target->data[0] = left->data[index_to_transfer];
                target->parent->data[leftIndex] = left->data[index_to_transfer];
                for (auto j = target->sz + 1; j > 0; j--)
                    target->pointers[j] = target->pointers[j - 1];
                target->pointers[0] = left->pointers[left->sz];
                left->sz--, target->sz++;
                return;
            }
        }
        // Borrowing from the right sibling
        if (rightIndex <= target->parent->sz)
        {
            Node *right = target->parent->pointers[rightIndex];
            if (right->sz >= min_number_of_children)
            {
                // tranfering the first index in the right node to the target node
                target->data[target->sz] = right->data[0];
                target->pointers[target->sz + 1] = right->pointers[0];
                right->sz -= 1;
                target->sz += 1;
                for (auto j = 0; j < right->sz; j++)
                    right->data[j] = right->data[j + 1];
                for (auto j = 0; j < right->sz + 1; j++)
                    right->pointers[j] = right->pointers[j + 1];
                target->parent->data[rightIndex - 1] = right->data[0];
                return;
            }
        }
        // If the nodes can be merged, merge it!
        // Merging with the left Sibling
        if (leftIndex >= 0)
        {
            Node *left = target->parent->pointers[leftIndex];
            // Transfer the content of the target node into the left Node
            left->pointers[left->sz] = nullptr;
            left->sz += target->sz;
            left->pointers[left->sz] = target->pointers[target->sz];
            for (int j = 0; j < target->sz; j++)
                left->data[left->sz - target->sz + j] = target->data[j];
            removeRecursively(key, target->parent->data[leftIndex], target->parent, target);
            delete target;
            return;
        }
        // Merging with the right Sibling
        if (rightIndex <= target->parent->sz)
        {
            Node *right = target->parent->pointers[rightIndex];
            // Transfer the content of the right node into the current Node
            target->pointers[target->sz] = nullptr;
            target->sz += right->sz;
            target->pointers[target->sz] = right->pointers[right->sz];
            for (int j = 0; j < right->sz; j++)
                target->data[target->sz - right->sz + j] = right->data[j];
            // Second Option
            // right->pointers[right->sz + target->sz] = right->pointers[right->sz];
            // right->pointers[right->sz] = nullptr;
            // right->sz += target->sz;
            // for (int j = right->sz - 1; j >= right->sz - target->sz - 1; j--)
            //     right->data[j] = right->data[j - target->sz];
            // for (int j = target->sz - 1; j >= 0; j--)
            //     right->data[j] = target->data[j];
            // right->pointers[0] = target->pointers[0];
            removeRecursively(key, target->parent->data[rightIndex - 1], target->parent, right);
            delete right;
            return;
        }
    }
    void removeRecursively(int deleted, int key, Node *target, Node *child)
    {
        if (target == root)
        {
            if (target->sz == 1)
            {
                if (target->pointers[0] == child)
                {
                    root = target->pointers[1];
                    root->parent = nullptr;
                    delete child;
                    return;
                }
                root = target->pointers[0];
                root->parent = nullptr;
                delete child;
                return;
            }
        }
        int id;
        for (id = 0; id < target->sz; id++)
            if (deleted < target->data[id])
            {
                break;
            }
        for (int j = 0; j < target->sz; j++)
            cout << target->data[j] << " ";
        cout << endl;
        cout << id << endl;
        for (int j = id; j < target->sz; j++)
            target->data[j] = target->data[j + 1];
        for (int j = 0; j < target->sz; j++)
            cout << target->data[j] << " ";
        cout << endl;
        for (id = 0; id < target->sz + 1; id++)
            if (target->pointers[id] == child)
                break;
        for (int j = id; j < target->sz + 1; j++)
            target->pointers[j] = target->pointers[j + 1];
        if (id != target->sz)
            target->sz--;
        if (target->sz >= min_number_of_children - 1 || target == root)
            return;
        int leftIndex, rightIndex;
        for (int j = 0; j < target->parent->sz + 1; ++j)
        {
            if (target->parent->pointers[j] == target)
            {
                leftIndex = j - 1, rightIndex = j + 1;
                break;
            }
        }
        // Borrowing from the left sibling
        if (leftIndex >= 0)
        {
            Node *left = target->parent->pointers[leftIndex];
            if (left->sz >= min_number_of_children)
            {
                int index_to_transfer = left->sz - 1;
                // tranfering the last index in the left node to the target node
                for (auto j = target->sz; j > 0; j--)
                    target->data[j] = target->data[j - 1];
                target->data[0] = left->data[index_to_transfer];
                target->parent->data[leftIndex] = left->data[index_to_transfer];
                for (auto j = target->sz + 1; j > 0; j--)
                    target->pointers[j] = target->pointers[j - 1];
                target->pointers[0] = left->pointers[left->sz];
                left->sz--, target->sz++;
                return;
            }
        }
        // Borrowing from the right sibling
        if (rightIndex <= target->parent->sz)
        {
            Node *right = target->parent->pointers[rightIndex];
            if (right->sz >= min_number_of_children)
            {
                // tranfering the first index in the right node to the target node
                target->data[target->sz] = right->data[0];
                target->pointers[target->sz + 1] = right->pointers[0];
                right->sz -= 1;
                target->sz += 1;
                for (auto j = 0; j < right->sz; j++)
                    right->data[j] = right->data[j + 1];
                for (auto j = 0; j < right->sz + 1; j++)
                    right->pointers[j] = right->pointers[j + 1];
                target->parent->data[rightIndex - 1] = right->data[0];
                return;
            }
        }
        // If the nodes can be merged, merge it!
        // Merging with the left Sibling
        if (leftIndex >= 0)
        {
            Node *left = target->parent->pointers[leftIndex];
            // Transfer the content of the target node into the left Node
            left->pointers[left->sz] = nullptr;
            left->sz += target->sz;
            left->pointers[left->sz] = target->pointers[target->sz];
            for (int j = 0; j < target->sz; j++)
                left->data[left->sz - target->sz + j] = target->data[j];
            removeRecursively(key, target->parent->data[leftIndex], target->parent, target);
            delete target;
            return;
        }
        // Merging with the right Sibling
        if (rightIndex <= target->parent->sz)
        {
            Node *right = target->parent->pointers[rightIndex];
            // Transfer the content of the right node into the current Node
            target->pointers[target->sz] = nullptr;
            target->sz += right->sz;
            target->pointers[target->sz] = right->pointers[right->sz];
            for (int j = 0; j < right->sz; j++)
                target->data[target->sz - right->sz + j] = right->data[j];
            removeRecursively(key, target->parent->data[rightIndex - 1], target->parent, right);
            delete target;
            return;
        }
    }
    void display()
    {
        if (root == nullptr)
        {
            printf("There are 0 nodes in the tree\n");
            return;
        }
        queue<Node *> q;
        q.push(root);
        int level = 1;
        while (q.size())
        {
            printf("************** Level: %d ", level);
            printf("**************\n");
            level++;
            int sz = q.size();
            while (sz--)
            {
                auto node = q.front();
                q.pop();
                for (int j = 0; j < node->sz; j++)
                {
                    if (j == 0)
                        printf("[%d", node->data[j]);
                    else
                        printf(", %d", node->data[j]);
                    // if (node->parent != nullptr)
                    //     printf(": %d] ", node->parent->data[0]);
                }
                printf("]     ");
                for (int j = 0; j < node->sz + 1; j++)
                    if (node->pointers[j] != nullptr)
                        q.push(node->pointers[j]);
            }
            printf("\n");
        }
        return;
    }
    Node *search(int key)
    {
        if (root == nullptr)
            return nullptr;
        Node *curr = root;
        while (!curr->isLeafNode)
        {
            for (int id = 0; id < curr->sz; ++id)
            {
                if (key < curr->data[id])
                {
                    // moving to the child node
                    curr = curr->pointers[id];
                    break;
                }
                if (id == curr->sz - 1)
                {
                    curr = curr->pointers[id + 1];
                    break;
                }
            }
        }
        for (int id = 0; id < curr->sz; ++id)
        {
            if (key == curr->data[id])
                return curr;
        }
        return nullptr;
    }
    ~BPlusTree()
    {
        delete this->root;
    }
};
void menu() // Menu
{
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf(">>> Enter 1 for Insertion\n");
    printf(">>> Enter 2 for Deletion\n");
    printf(">>> Enter 3 to Display the stored data\n");
    printf(">>> Enter 4 to Search any key\n");
    printf(">>> Enter 5 to Exit\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    return;
}
void run_code()
{
    string val;
    BPlusTree *bPlusTree = new BPlusTree(order_of_tree);
    // order = 4;
    // vector<int> insert = {1, 4, 7, 10, 17, 21, 31, 25, 19, 20, 28, 42};
    // for (auto x : insert)
    //     bPlusTree->insert(x);
    // bPlusTree->display();
    // /*
    //     ************** Level: 1 **************
    //     20
    //     ************** Level: 2 **************
    //     7 17    25 31
    //     ************** Level: 3 **************
    //     1 4     7 10    17 19   20 21   25 28   31 42
    //     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // */
    while (true)
    {
        menu();
        cin >> val;
        if (val == "1") // Insertion
        {
            printf("Enter the key to be inserted\n");
            int ins;
            cin >> ins;
            Node *val = bPlusTree->search(ins);
            if (val == nullptr)
            {
                bPlusTree->insert(ins);
                printf("Key is inserted!\n");
            }
            else
                printf("Key is already present!\n");
        }
        else if (val == "2") // Deletion
        {
            printf("Enter the key to be deleted\n");
            int del;
            cin >> del;
            Node *val = bPlusTree->search(del);
            if (val != nullptr)
            {
                bPlusTree->remove(val, del);
                printf("Key is deleted!\n");
            }
            else
                printf("No such key is present!\n");
        }
        else if (val == "3") // Display the Data
            bPlusTree->display();
        else if (val == "4") // Searching of the key
        {
            printf("Enter the key to search\n");
            int key;
            cin >> key;
            Node *val = bPlusTree->search(key);
            if (val == nullptr)
                printf("Key not found!\n");
            else
                printf("Key found!\n");
        }
        else if (val == "5") // exit
        {
            printf("Bye!\n");
            break;
        }
        else
            printf("Wrong Choice! Please Try Again!\n");
    }
    return;
}
signed main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    printf("Enter the order of B+ Tree(geater than 2):\n");
    cin >> order_of_tree;
    run_code();
    return 0;
}