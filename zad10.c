#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LEN 100

// Structure for a city
typedef struct City {
    char name[MAX_STRING_LEN];
    int population;
    struct City* left;
    struct City* right;
} City;

// Structure for a node in the linked list of countries
typedef struct CountryNode {
    char name[MAX_STRING_LEN];
    City* cityTree;
    struct CountryNode* next;
} CountryNode;

// Structure for a tree node (for the country tree)
typedef struct CountryTreeNode {
    char name[MAX_STRING_LEN];
    CountryNode* cityList;
    struct CountryTreeNode* left;
    struct CountryTreeNode* right;
} CountryTreeNode;

// Function to create a new city
City* createCity(char* name, int population) {
    City* newCity = (City*)malloc(sizeof(City));
    strcpy(newCity->name, name);
    newCity->population = population;
    newCity->left = newCity->right = NULL;
    return newCity;
}

// Function to insert a city into a BST sorted by population, then by city name
City* insertCity(City* root, char* name, int population) {
    if (root == NULL) {
        return createCity(name, population);
    }

    if (population < root->population || (population == root->population && strcmp(name, root->name) < 0)) {
        root->left = insertCity(root->left, name, population);
    } else {
        root->right = insertCity(root->right, name, population);
    }

    return root;
}

// Function to create a new country node for the linked list
CountryNode* createCountryNode(char* countryName) {
    CountryNode* newNode = (CountryNode*)malloc(sizeof(CountryNode));
    strcpy(newNode->name, countryName);
    newNode->cityTree = NULL;
    newNode->next = NULL;
    return newNode;
}

// Function to insert a country into the linked list in sorted order by country name
CountryNode* insertCountryNode(CountryNode* head, char* countryName) {
    CountryNode* newNode = createCountryNode(countryName);
    if (head == NULL || strcmp(countryName, head->name) < 0) {
        newNode->next = head;
        return newNode;
    }

    CountryNode* current = head;
    while (current->next != NULL && strcmp(countryName, current->next->name) > 0) {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;
    return head;
}

// Function to create a new country tree node
CountryTreeNode* createCountryTreeNode(char* countryName) {
    CountryTreeNode* newNode = (CountryTreeNode*)malloc(sizeof(CountryTreeNode));
    strcpy(newNode->name, countryName);
    newNode->cityList = NULL;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Function to insert a country into the country tree in sorted order by country name
CountryTreeNode* insertCountryTreeNode(CountryTreeNode* root, char* countryName) {
    if (root == NULL) {
        return createCountryTreeNode(countryName);
    }

    if (strcmp(countryName, root->name) < 0) {
        root->left = insertCountryTreeNode(root->left, countryName);
    } else {
        root->right = insertCountryTreeNode(root->right, countryName);
    }

    return root;
}

// Function to print cities from the city tree (inorder traversal)
void printCities(City* root) {
    if (root != NULL) {
        printCities(root->left);
        printf("  %s (Population: %d)\n", root->name, root->population);
        printCities(root->right);
    }
}

// Function to print the countries and their cities from the linked list
void printCountries(CountryNode* head) {
    CountryNode* current = head;
    while (current != NULL) {
        printf("Country: %s\n", current->name);
        printf("Cities:\n");
        printCities(current->cityTree);
        current = current->next;
    }
}

// Function to print the countries and their cities from the country tree
void printCountryTree(CountryTreeNode* root) {
    if (root != NULL) {
        printCountryTree(root->left);
        printf("Country: %s\n", root->name);
        // Assuming city list is sorted and we print cities from that list
        printCities(root->cityList);
        printCountryTree(root->right);
    }
}

// Function to search for cities with population greater than a given number
void searchCitiesByPopulation(CountryNode* head, int population) {
    CountryNode* current = head;
    while (current != NULL) {
        City* cityRoot = current->cityTree;
        printf("Country: %s\n", current->name);
        City* cityCurrent = cityRoot;
        while (cityCurrent != NULL) {
            if (cityCurrent->population > population) {
                printf("  %s (Population: %d)\n", cityCurrent->name, cityCurrent->population);
            }
            cityCurrent = cityCurrent->right;
        }
        current = current->next;
    }
}

// Function to read cities from a file and insert them into the city tree
void readCitiesFromFile(CountryNode* countryNode, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file %s\n", filename);
        return;
    }

    char cityName[MAX_STRING_LEN];
    int population;
    while (fscanf(file, "%s %d", cityName, &population) != EOF) {
        countryNode->cityTree = insertCity(countryNode->cityTree, cityName, population);
    }

    fclose(file);
}

int main() {
    // Open the "drzave.txt" file to read countries and their cities files
    FILE* countryFile = fopen("drzave.txt", "r");
    if (!countryFile) {
        printf("Failed to open drzave.txt\n");
        return 1;
    }

    CountryNode* countryList = NULL;
    CountryTreeNode* countryTree = NULL;

    char countryName[MAX_STRING_LEN];
    char citiesFileName[MAX_STRING_LEN];
    while (fscanf(countryFile, "%s %s", countryName, citiesFileName) != EOF) {
        // Insert the country into the linked list
        countryList = insertCountryNode(countryList, countryName);

        // Insert the country into the country tree
        countryTree = insertCountryTreeNode(countryTree, countryName);

        // Read cities from the respective file and add them to the city tree or linked list
        readCitiesFromFile(countryList, citiesFileName);
    }

    fclose(countryFile);

    // Print countries and cities
    printf("Countries and their cities (linked list):\n");
    printCountries(countryList);

    printf("\nCountries and their cities (country tree):\n");
    printCountryTree(countryTree);

    // User input for searching cities with population greater than a given value
    int populationThreshold;
    printf("\nEnter a population threshold for searching cities: ");
    scanf("%d", &populationThreshold);

    searchCitiesByPopulation(countryList, populationThreshold);

    return 0;
}


    fclose(file);
    printf("Inorder output written to 'inorder_output.txt'.\n");

    return 0;
}
