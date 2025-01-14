#include <stdio.h>
#include <stdlib.h>

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

// Helper function to perform inorder traversal of the tree and print the values
void inorder(Node* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->value);
        inorder(root->right);
    }
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

// Helper function to perform inorder traversal after replacement
void inorderAfterReplace(Node* root) {
    if (root != NULL) {
        inorderAfterReplace(root->left);
        printf("%d ", root->value);
        inorderAfterReplace(root->right);
    }
}

// Driver function to demonstrate replace
int main() {
    int values[] = {2, 5, 7, 8, 11, 1, 4, 2, 3, 7};  // Values from your sequence
    int n = sizeof(values) / sizeof(values[0]);

    Node* root = NULL;
    for (int i = 0; i < n; i++) {
        root = insert(root, values[i]);
    }

    printf("Inorder traversal before replace:\n");
    inorder(root);

    // Replace values with the sum of left and right subtrees
    replace(root);

    printf("\nInorder traversal after replace:\n");
    inorderAfterReplace(root);
    return 0;
}
