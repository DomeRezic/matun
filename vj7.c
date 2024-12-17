#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Directory structure
typedef struct Directory {
    char name[100];
    struct Directory *parent;
    struct Directory *subDir;
    struct Directory *nextSibling;
} Directory;

// Global variable to store the current directory
Directory *currentDirectory;

// Function to create a new directory
Directory* createDirectory(const char *name, Directory *parent) {
    Directory *newDir = (Directory*)malloc(sizeof(Directory));
    strcpy(newDir->name, name);
    newDir->parent = parent;
    newDir->subDir = NULL;
    newDir->nextSibling = NULL;
    return newDir;
}

// Function to change the current directory to a sub-directory
void changeDirectory(const char *dirName) {
    Directory *temp = currentDirectory->subDir;
    while (temp != NULL) {
        if (strcmp(temp->name, dirName) == 0) {
            currentDirectory = temp;
            printf("Changed to directory: %s\n", currentDirectory->name);
            return;
        }
        temp = temp->nextSibling;
    }
    printf("Directory not found!\n");
}

// Function to change the current directory to the parent directory
void changeToParentDirectory() {
    if (currentDirectory->parent != NULL) {
        currentDirectory = currentDirectory->parent;
        printf("Changed to parent directory: %s\n", currentDirectory->name);
    } else {
        printf("You are already at the root directory!\n");
    }
}

// Function to list the contents of the current directory
void listDirectoryContents() {
    Directory *temp = currentDirectory->subDir;
    if (temp == NULL) {
        printf("No subdirectories.\n");
    } else {
        printf("Directories in %s:\n", currentDirectory->name);
        while (temp != NULL) {
            printf("- %s\n", temp->name);
            temp = temp->nextSibling;
        }
    }
}

// Function to create a new directory in the current directory
void makeDirectory(const char *dirName) {
    Directory *newDir = createDirectory(dirName, currentDirectory);
    if (currentDirectory->subDir == NULL) {
        currentDirectory->subDir = newDir;
    } else {
        Directory *temp = currentDirectory->subDir;
        while (temp->nextSibling != NULL) {
            temp = temp->nextSibling;
        }
        temp->nextSibling = newDir;
    }
    printf("Directory '%s' created.\n", dirName);
}

// Function to print the menu
void printMenu() {
    printf("\nChoose an option:\n");
    printf("1. md <directory> - Create a new directory\n");
    printf("2. cd <directory> - Change to a sub-directory\n");
    printf("3. cd.. - Move to the parent directory\n");
    printf("4. dir - List the contents of the current directory\n");
    printf("5. exit - Exit the program\n");
}

int main() {
    // Initialize the root directory
    currentDirectory = createDirectory("C:", NULL); // Root directory

    while (1) {
        printMenu();

        char command[200];
        printf("Enter command: ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0; // Remove newline

        if (strncmp(command, "md ", 3) == 0) {
            // "md <directory>"
            char dirName[100];
            sscanf(command + 3, "%s", dirName);
            makeDirectory(dirName);

        } else if (strncmp(command, "cd ", 3) == 0) {
            // "cd <directory>"
            char dirName[100];
            sscanf(command + 3, "%s", dirName);
            changeDirectory(dirName);

        } else if (strcmp(command, "cd..") == 0) {
            // "cd.."
            changeToParentDirectory();

        } else if (strcmp(command, "dir") == 0) {
            // "dir"
            listDirectoryContents();

        } else if (strcmp(command, "exit") == 0) {
            // "exit"
            printf("Exiting the program.\n");
            break;

        } else {
            printf("Invalid command!\n");
        }
    }

    return 0;
}
