#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Same Node structure as before
typedef struct Node {
    int value;
    struct Node* left;
    struct Node* right;
} Node;

// Function to create a new node
Node* createNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->value = value;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Function to insert a new value into the binary search tree
Node* insert(Node* root, int value) {
    if (root == NULL) {
        return createNode(value);
    }

    if (value < root->value) {
        root->left = insert(root->left, value);
    } else {
        root->right = insert(root->right, value);
    }

    return root;
}

// Function to calculate the sum of all nodes in the subtree rooted at 'node'
int subtreeSum(Node* node) {
    if (node == NULL) {
        return 0;
    }
    return node->value + subtreeSum(node->left) + subtreeSum(node->right);
}

// Function to replace each node with the sum of its left and right subtrees
void replace(Node* root) {
    if (root == NULL) {
        return;
    }

    int leftSum = subtreeSum(root->left);
    int rightSum = subtreeSum(root->right);

    // Replace the current node's value with the sum of its children
    root->value = leftSum + rightSum;

    replace(root->left);
    replace(root->right);
}

// Helper function for inorder traversal
void inorder(Node* root, FILE* file) {
    if (root != NULL) {
        inorder(root->left, file);
        fprintf(file, "%d ", root->value);
        inorder(root->right, file);
    }
}

int main() {
    srand(time(NULL));  // Seed for random number generation

    int n = 10;  // Number of random numbers to generate
    Node* root = NULL;

    // Insert random values in the range [10, 90]
    for (int i = 0; i < n; i++) {
        int randomValue = 10 + rand() % 81;  // Random value between 10 and 90
        root = insert(root, randomValue);
    }

    // Open a file to write the inorder traversal
    FILE* file = fopen("inorder_output.txt", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    // Write the inorder traversal before replace
    fprintf(file, "Inorder before replace:\n");
    inorder(root, file);

    // Replace values with the sum of left and right subtrees
    replace(root);

    // Write the inorder traversal after replace
    fprintf(file, "\nInorder after replace:\n");
    inorder(root, file);

    fclose(file);
    printf("Inorder output written to 'inorder_output.txt'.\n");

    return 0;
}
