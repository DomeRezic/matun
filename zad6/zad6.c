#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LENGTH 256

// Struct to store item information (name, quantity, price)
typedef struct Item {
    char name[100];
    int quantity;
    float price;
    struct Item *next;  // Points to the next item in the list
} Item;

// Struct to store account information (date and list of items)
typedef struct Account {
    struct tm date;  // Date when the invoice was issued
    Item *items_head;  // Head of the list of items
    struct Account *next;  // Points to the next account
} Account;

// Function to read the date from a string (YYYY-MM-DD)
struct tm parse_date(char *date_str) {
    struct tm date = {0};
    sscanf(date_str, "%d-%d-%d", &date.tm_year, &date.tm_mon, &date.tm_mday);
    date.tm_year -= 1900;  // tm_year is years since 1900
    date.tm_mon -= 1;  // tm_mon is months since January
    return date;
}

// Function to add an item to the linked list of items, sorted by item name
void add_item(Account *account, char *name, int quantity, float price) {
    Item *new_item = (Item *)malloc(sizeof(Item));
    strcpy(new_item->name, name);
    new_item->quantity = quantity;
    new_item->price = price;
    new_item->next = NULL;

    // Insert into sorted list of items
    if (account->items_head == NULL || strcmp(account->items_head->name, new_item->name) > 0) {
        new_item->next = account->items_head;
        account->items_head = new_item;
    } else {
        Item *current = account->items_head;
        while (current->next != NULL && strcmp(current->next->name, new_item->name) < 0) {
            current = current->next;
        }
        new_item->next = current->next;
        current->next = new_item;
    }
}

// Function to add an account to the linked list of accounts, sorted by date
void add_account(Account **accounts, struct tm date) {
    Account *new_account = (Account *)malloc(sizeof(Account));
    new_account->date = date;
    new_account->items_head = NULL;
    new_account->next = NULL;

    // Insert into sorted list of accounts
    if (*accounts == NULL || difftime(mktime(&(*accounts)->date), mktime(&date)) > 0) {
        new_account->next = *accounts;
        *accounts = new_account;
    } else {
        Account *current = *accounts;
        while (current->next != NULL && difftime(mktime(&current->next->date), mktime(&date)) < 0) {
            current = current->next;
        }
        new_account->next = current->next;
        current->next = new_account;
    }
}

// Function to read an account file and add the account to the linked list
void process_account_file(Account **accounts, char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Could not open file %s\n", filename);
        return;
    }

    char date_str[MAX_LINE_LENGTH];
    fgets(date_str, sizeof(date_str), file);
    struct tm date = parse_date(date_str);

    // Create and add account to the linked list
    add_account(accounts, date);

    // Read items and add them to the account's item list
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        char name[MAX_LINE_LENGTH];
        int quantity;
        float price;
        sscanf(line, "%[^,], %d, %f", name, &quantity, &price);

        // Add item to the account
        Account *account = *accounts;
        while (account && difftime(mktime(&account->date), mktime(&date)) != 0) {
            account = account->next;
        }
        if (account) {
            add_item(account, name, quantity, price);
        }
    }

    fclose(file);
}

// Function to query total money spent and quantity purchased for an item in a date range
void query_spent_on_item(Account *accounts, char *item_name, struct tm start_date, struct tm end_date) {
    float total_spent = 0;
    int total_quantity = 0;

    Account *account = accounts;
    while (account) {
        if (difftime(mktime(&account->date), mktime(&start_date)) >= 0 &&
            difftime(mktime(&account->date), mktime(&end_date)) <= 0) {

            Item *item = account->items_head;
            while (item) {
                if (strcmp(item->name, item_name) == 0) {
                    total_spent += item->quantity * item->price;
                    total_quantity += item->quantity;
                }
                item = item->next;
            }
        }
        account = account->next;
    }

    if (total_quantity > 0) {
        printf("Total spent on '%s': %.2f\n", item_name, total_spent);
        printf("Total quantity purchased: %d\n", total_quantity);
    } else {
        printf("No purchases found for item '%s' in the given date range.\n", item_name);
    }
}

int main() {
    Account *accounts = NULL;

    // Read the main file racuni.txt
    FILE *file = fopen("racuni.txt", "r");
    if (!file) {
        printf("Could not open racuni.txt\n");
        return 1;
    }

    char account_file[MAX_LINE_LENGTH];
    while (fgets(account_file, sizeof(account_file), file)) {
        account_file[strcspn(account_file, "\n")] = '\0';  // Remove newline character
        process_account_file(&accounts, account_file);
    }
    fclose(file);

    // User input for query
    char item_name[MAX_LINE_LENGTH];
    char start_date_str[MAX_LINE_LENGTH], end_date_str[MAX_LINE_LENGTH];

    printf("Enter the item name: ");
    fgets(item_name, sizeof(item_name), stdin);
    item_name[strcspn(item_name, "\n")] = '\0';  // Remove newline

    printf("Enter start date (YYYY-MM-DD): ");
    fgets(start_date_str, sizeof(start_date_str), stdin);
    start_date_str[strcspn(start_date_str, "\n")] = '\0';  // Remove newline

    printf("Enter end date (YYYY-MM-DD): ");
    fgets(end_date_str, sizeof(end_date_str), stdin);
    end_date_str[strcspn(end_date_str, "\n")] = '\0';  // Remove newline

    struct tm start_date = parse_date(start_date_str);
    struct tm end_date = parse_date(end_date_str);

    // Perform the query
    query_spent_on_item(accounts, item_name, start_date, end_date);

    return 0;
}
