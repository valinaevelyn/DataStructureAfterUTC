#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Food{
    int id;
    int price;
    char name[50];
    int height;
    Food* left;
    Food* right;
};

Food* root = NULL;

Food* createFood(int id, char name[], int price){
    Food* temp = (Food*)malloc(sizeof(Food));
    temp->id = id;
    strcpy(temp->name, name);
    temp->price = price;
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

int getHeight(Food* curr){
    if(curr == NULL){
        return 0;
    }

    return curr->height;
}

int updateHeight(Food* curr){
    return max(getHeight(curr->left), getHeight(curr->right)) + 1;
}

int balanceFactor(Food* curr){
    if(curr == NULL){
        return 0;
    }

    return getHeight(curr->left) - getHeight(curr->right);
}

Food* rightRotate(Food* t){
    Food* s = t->left;
    Food* b = s->right;

    t = s->right;
    b = t->left;

    t->height = updateHeight(t);
    s->height = updateHeight(s);

    return s;
}

Food* leftRotate(Food* t){
    Food* s = t->right;
    Food* b = s->left;

    t = s->left;
    b = t->right;

    t->height = updateHeight(t);
    s->height = updateHeight(s);

    return s;
}

Food* insertFood(Food* curr, int id, char name[], int price){
    if(!curr){
        return createFood(id, name, price);
    }else if(id < curr->id){
        curr->left = insertFood(curr->left, id, name, price);
    }else if(id > curr->id){
        curr->right = insertFood(curr->right, id, name, price);
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

Food* deleteFood(Food* curr, int id){
    if(!curr){
        return NULL;
    }else if(id < curr->id){
        curr->left = deleteFood(curr->left, id);
    }else if(id > curr->id){
        curr->right = deleteFood(curr->right, id);
    }else{
        if(!curr->left && !curr->right){
            free(curr);
            return NULL;
        }else if(!curr->left){
            Food* temp = curr;
            curr = curr->right;
            free(temp);
        }else if(!curr->right){
            Food* temp = curr;
            curr = curr->left;
            free(temp);
        }else{
            Food* temp = curr->left;
            while(temp->right){
                temp = temp->right;
            }

            curr->id = temp->id;
            curr->left = deleteFood(curr->left, temp->id);
        }
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
            curr->right =  rightRotate(curr->right);
            return leftRotate(curr);
        }
        return leftRotate(curr);
    }

    return curr;
}

Food* searchFood(Food* curr, int id){
    if(!curr){
        return NULL;
    }else if (id < curr->id){
        return searchFood(curr->left, id);
    }else if(id > curr->id){
        return searchFood(curr->right, id);
    }else{
        return curr;
    }
}

Food* popAll(Food* curr){
    while(curr){
        curr = deleteFood(curr, curr->id);
    }
    return curr;
}

void inOrder(Food* curr){
    if(!curr){
        return;
    }

    inOrder(curr->left);
    printf("%-3d %-50s %-6d (%d)\n", curr->id, curr->name, curr->price, curr->height);
    inOrder(curr->right);
}

int main(){
    root = insertFood(root, 10, "Bakso", 20000);
    root = insertFood(root, 5, "Mie Ayam", 18000);
    root = insertFood(root, 20, "Martabak", 10000);
    root = insertFood(root, 3, "Nasi Padang", 8000);
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