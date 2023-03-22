#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node{
    char name[50];
    int price;
    int id;
    int height;
    Node* left;
    Node* right;
};

Node* root = NULL;

Node* createNode(char name[], int price, int id){
    Node* temp = (Node*)malloc(sizeof(Node));
    strcpy(temp->name, name);
    temp->price = price;
    temp->id = id;
    temp->height = 1;
    temp->left = temp->right = NULL;

    return temp;
}

int max(int a, int b){
    if(a > b){
        return a;
    }else{
        return b;
    }
}

int getHeight(Node* curr){
    if(!curr){
        return 0;
    }

    return curr->height;
}

int updateHeight(Node* curr){
    return max(getHeight(curr->left), getHeight(curr->right)) + 1;
}

int balanceFactor(Node* curr){
    if(!curr){
        return 0;
    }

    return getHeight(curr->left) - getHeight(curr->right);
}

Node* rightRotate(Node* t){
    Node* s = t->left;
    Node* b = s->right;

    s->right = t;
    t->left = b;

    t->height = updateHeight(t);
    s->height = updateHeight(s);

    return s;
}

Node* leftRotate(Node* t){
    Node* s = t->right;
    Node* b = s->left;

    s->left = t;
    t->right = b;

    t->height = updateHeight(t);
    s->height = updateHeight(s);

    return s;
}

Node* insertFood(Node* curr, int id, int price, char name[]){
    if(!curr){
        return createNode(name, price, id);
    }else if(id < curr->id){
        curr->left = insertFood(curr->left, id, price, name);
    }else if(id > curr->id){
        curr->right = insertFood(curr->right, id, price, name);
    }

    curr->height = updateHeight(curr);
    int BF = balanceFactor(curr);

    if(BF > 1){
        if(id > curr->left->id){
            curr->left = leftRotate(curr->left);
            return rightRotate(curr);
        }

        return rightRotate(curr);
    }else if(BF < -1){
        if(id < curr->right->id){
            curr->right = rightRotate(curr->right);
            return leftRotate(curr);
        }

        return leftRotate(curr);
    }

    return curr;
}

Node* deleteFood(Node* curr, int id){
    if(!curr){
        return NULL;
    }else if(id < curr->id){
        curr->left = deleteFood(curr->left, id);
    }else if(id > curr->id){
        curr->right = deleteFood(curr->right, id);
    }else{
        if(!curr->left && !curr->right){
            free(curr);
            curr = NULL;
        }else if(!curr->left){
            Node* temp = curr;
            curr = curr->right;
            free(temp);
        }else if(!curr->right){
            Node* temp = curr;
            curr = curr->left;
            free(temp);
        }else{
            Node* temp = curr->left;
            while(temp->right){
                temp = temp->right;
            }

            curr->id = temp->id;
            curr->price = temp->price;
            strcpy(curr->name, temp->name);
            curr->left = deleteFood(curr->left, temp->id);
        }
    }

    if(!curr){
        return curr;
    }

    curr->height = updateHeight(curr);
    int BF = balanceFactor(curr);

    if(BF > 1){
        if(balanceFactor(curr->left) < 0){
            curr->left = leftRotate(curr->left);
            return rightRotate(curr);
        }
        return rightRotate(curr);
    }else if(BF < -1){
        if(balanceFactor(curr->right) > 0){
            curr->right = rightRotate(curr->right);
            return leftRotate(curr);
        }
        return leftRotate(curr);
    }

    return curr;
}

Node* searchFood(Node* curr, int id){
    if(!curr){
        return NULL;
    }else if(id < curr->id){
        return searchFood(curr->left, id);
    }else if(id > curr->id){
        return searchFood(curr->right, id);
    }else{
        return curr;
    }
}

Node* popAll(Node* curr){
    while(curr){
        curr = deleteFood(curr, curr->id);
    }

    return curr;
}

void inOrder(Node* curr){
    if(!curr){
        return;
    }

    inOrder(curr->left);
    printf("%-3d %-50s %-6d (%d)\n", curr->id, curr->name, curr->price, curr->height);
    inOrder(curr->right);
}

int main(){
    root = insertFood(root, 10, 20000, "Bakso");
    root = insertFood(root, 5, 18000, "Mie Ayam");
    root = insertFood(root, 20, 10000, "Martabak");
    root = insertFood(root, 3, 8000, "Nasi Padang");
    inOrder(root);
    printf("\n");

    root = deleteFood(root, 5);
    root = deleteFood(root, 10);
    root = deleteFood(root, 3);
    inOrder(root);

    root = popAll(root);
    inOrder(root);
    return 0;
}