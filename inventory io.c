#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define structure for inventory items
typedef struct Item {
    char name[50];
    int quantity;
    float price;
    struct Item *next;
} Item;

// Function prototypes
void add_item(Item **head);
void view_items(Item *head);
Item* find_item(Item *head, const char *name);
void purchase_item(Item *head);
void save_to_file(Item *head, const char *filename); //Dario
Item* load_from_file(const char *filename); //Dario
void free_list(Item **head); //Dario

int main() {
    Item *inventory = load_from_file("inventory.dat");
    int choice;

    do {
        printf("\nInventory Management System\n");
        printf("1. Add Item\n2. View Items\n3. Find Item\n4. Purchase Item\n5. Exit\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); // Clear invalid input
            printf("Invalid input! Please enter a number.\n");
            continue;
        }

        switch (choice) {
            case 1: add_item(&inventory); break;
            case 2: view_items(inventory); break;
            case 3: /* TODO: Implement find_item functionality */ break;
            case 4: purchase_item(inventory); break;
            case 5: save_to_file(inventory, "inventory.dat"); break;
            default: printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 5);

    free_list(&inventory);
    return 0;
}

// TODO: Implement functions such as add_item(), view_items(), etc.
