#include <iostream>
#include <algorithm>
#include <vector>
#include <random>

using namespace std;

vector<int> in_order_traversal_arr;

class Node{
public:
    Node *parent;
    int val;
    int height;
    Node *left;
    Node *right;

    Node(int val, Node *parent):val(val), parent(parent){
        height = 1;
        left = nullptr;
        right = nullptr;
    }
};

class AVLTree{
private:
    Node *root = nullptr;

    Node *find(int val, Node *node){
        if(node == nullptr){
            return nullptr;
        }

        if(node->val == val){
            return node;
        }

        if(val < node->val){
            return find(val, node->left);
        }
        else{
            return find(val, node->right);
        }
    }

    Node *insert(int val, Node *node){
        if(node->val == val){
            return node;
        }

        if(val < node->val){
            if(node->left != nullptr){
                return insert(val, node->left);
            }
            Node *res = new Node(val, node);
            node->left = res;
            return res;
        }
        else{
            if(node->right != nullptr){
                return insert(val, node->right);
            }
            Node *res = new Node(val, node);
            node->right = res;
            return res;
        }
    }

    bool is_avl(Node *node){
        if(node == nullptr){
            return true;
        }

        bool f1 = is_avl(node->left);
        bool f2 = is_avl(node->right);

        if(f1 && f2){
            int left_height;
            int right_height;

            if(node->left == nullptr){
                left_height = 0;
            }
            else{
                left_height = node->left->height;
            }

            if(node->right == nullptr){
                right_height = 0;
            }
            else{
                right_height = node->right->height;
            }

            if(abs(left_height-right_height)<=1){
                return true;
            }
        }
        return false;
    }

    bool is_bst(Node *node){
        in_order_traversal_arr.erase(in_order_traversal_arr.begin(), in_order_traversal_arr.end());
        in_order_traversal_modified(node);

        for(int i = 0; i < in_order_traversal_arr.size() - 1; i++){
            if(in_order_traversal_arr[i] > in_order_traversal_arr[i + 1]){
                return false;
            }
        }

        return true;
    }

    void in_order_traversal_modified(Node *node){
        if(node == nullptr){
            return;
        }
        in_order_traversal_modified(node->left);
        if(node->left != nullptr){
            if(node->left->val == node->val){
                return;
            }
        }
        in_order_traversal_arr.push_back(node->val);
        in_order_traversal_modified(node->right);
    }

    void in_order_traversal(Node *node){
        if(node == nullptr){
            return;
        }
        in_order_traversal(node->left);
        cout << node->val << " ";
        in_order_traversal(node->right);
    }

    void show_tree(Node *node, int shift) const{
        for(int i = 0; i < shift; i++){
            cout << "    ";
        }

        cout << "Data -> [" << node->val << "]" << endl;

        if(node->left != nullptr){
            show_tree(node->left, shift + 1);
        }
        if(node->right != nullptr){
            show_tree(node->right, shift + 1);
        }
    }


    Node *search_min(Node *node){
        if(node->left == nullptr){
            return node;
        }
        return search_min(node->left);
    }

    Node *search_max(Node *node){
        if(node->right == nullptr){
            return node;
        }
        return search_max(node->right);
    }

    void delete_node(Node *node){
        if(node == nullptr){
            return;
        }

        if(node == root){
            delete node;
            root = nullptr;
            return;
        }


        if(node->parent->right == node){
            node->parent->right = nullptr;
        }
        if(node->parent->left == node){
            node->parent->left = nullptr;
        }
        delete node;
    }


public:
    Node *find(int val){
        return find(val, root);
    }

    int height(Node *node){
        if(node == nullptr){
            return 0;
        }
        return node->height;
    }

    void rebalance(Node *node){
        if(node == nullptr){
            return;
        }


        if(height(node->left) > height(node->right) + 1){
            if(height(node->left->right) > height(node->left->left)){
                rotate_left(node->left);
                adjust_height(node->left->left);
                adjust_height(node->left);
            }
            rotate_right(node);

            adjust_height(node);
            adjust_height(node->parent);
            rebalance(node->parent);
        }
        else{
            if(height(node->right) > height(node->left) + 1){
                if(height(node->right->left) > height(node->right->right)){
                    rotate_right(node->right);
                    adjust_height(node->right->right);
                    adjust_height(node->right);
                }

                rotate_left(node);
                adjust_height(node);
                adjust_height(node->parent);
                rebalance(node->parent);
            }
            else{
                adjust_height(node);
                rebalance(node->parent);
            }
        }
    }


    void rotate_right(Node *node){
        Node *left_child = node->left;
        if(node == root){
            root = left_child;
        }

        node->left = left_child->right;

        if(node->left != nullptr){
            node->left->parent = node;
        }

        left_child->parent = node->parent;
        if(node->parent != nullptr){
            if(node->parent->left == node){
                node->parent->left = left_child;
            }
            else{
                node->parent->right = left_child;
            }

        }
        left_child->right = node;
        node->parent = left_child;
    }

    void rotate_left(Node *node){
        Node *right_child = node->right;
        if(node == root){
            root = right_child;
        }

        node->right = right_child->left;
        if(node->right != nullptr){
            node->right->parent = node;
        }


        right_child->parent = node->parent;
        if(node->parent != nullptr){
            if(node->parent->right == node){
                node->parent->right = right_child;
            }
            else{
                node->parent->left = right_child;
            }

        }
        right_child->left = node;
        node->parent = right_child;

    }

    void insert_avl(int val){
        // nodes without repetitions
        if(root == nullptr){
            root = new Node(val, nullptr);
        }

        Node *found_node = insert(val, root);
        rebalance(found_node);
    }

    void remove(int val){
        Node *node = find(val);

        if(node == nullptr){
            return;
        }

        if(node->left == nullptr && node->right == nullptr){
            Node *parent = node->parent;
            delete_node(node);
            rebalance(parent);
            return;
        }

        Node *res;

        if(height(node->left) >= height(node->right)){
            res = search_max(node->left);
        }
        else{
            res = search_min(node->right);
        }

        node->val = res->val;

        if(res->left != nullptr){
            res->val = res->left->val;
            delete_node(res->left);
            rebalance(res);
        }
        else{
            if(res->right != nullptr){
                res->val = res->right->val;
                delete_node(res->right);
                rebalance(res);
            }
            else{
                Node *parent = res->parent;
                delete_node(res);
                rebalance(parent);
            }
        }
    }



    void adjust_height(Node *node){
        node->height = 1 + max(height(node->right), height(node->left));
    }


    bool is_avl(){
        if(root == nullptr){
            return true;
        }

        return is_bst(root) && is_avl(root);
    }


    void in_order_traversal() {
        in_order_traversal(root);
    }

    void show_tree(){
        if(root != nullptr){
            show_tree(root, 1);
            cout << endl;
        }
        else{
            cout << "Empty tree!" << endl;
        }
    }
};


void shuffle(vector<int> arr){
    int tmp;
    int j;
    int size = arr.size();
    for(int i = 0; i < size; i++){
        j = rand() % size;
        tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}


int main(){
    AVLTree avlTree;
    int nb;
    int size = 90000000;
    vector<int> nb_array;

    for(int i = 0; i <= size; i++){
        nb = rand()%200000;
        avlTree.insert_avl(nb);
        nb_array.push_back(nb);
    }

    //avlTree.show_tree();

    shuffle(nb_array);


    for(int i = 0; i <= size; i++){
        avlTree.remove(nb_array[i]);
    }

    avlTree.show_tree();
    cout << avlTree.is_avl() << endl;

    return 0;
}