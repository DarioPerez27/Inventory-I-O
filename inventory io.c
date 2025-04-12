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
Item* find_item(Item *head);
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
            case 3: find_item(inventory); break;
            case 4: purchase_item(inventory); break;
            case 5: save_to_file(inventory, "inventory.dat"); break;
            default: printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 5);

    free_list(&inventory);
    return 0;
}

void add_item(Item **head) {
    Item *new_item = (Item *)malloc(sizeof(Item));
    if (!new_item) {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("Enter item name: ");
    scanf(" %[^\n]", new_item->name); // Read string with spaces
    printf("Enter item quantity: ");
    scanf("%d", &new_item->quantity);
    printf("Enter item price: ");
    scanf("%f", &new_item->price);

    new_item->next = *head;
    *head = new_item;

    printf("Item added successfully!\n");
}

void save_to_file(Item *head, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Error opening file for writing!\n");
        return;
    }

    Item *current = head;
    while (current) {
        fwrite(current, sizeof(Item) - sizeof(Item *), 1, file); // Exclude the pointer
        current = current->next;
    }

    fclose(file);
    printf("Inventory saved to file successfully!\n");
}

Item* load_from_file(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Error opening file for reading!\n");
        return NULL;
    }

    Item *head = NULL;
    Item *current = NULL;

    while (1) {
        Item *new_item = (Item *)malloc(sizeof(Item));
        if (!new_item) {
            printf("Memory allocation failed!\n");
            break;
        }

        size_t read_size = fread(new_item, sizeof(Item) - sizeof(Item *), 1, file);
        if (read_size == 0) {
            free(new_item);
            break; // End of file or error
        }

        new_item->next = NULL;

        if (head == NULL) {
            head = new_item;
            current = new_item;
        } else {
            current->next = new_item;
            current = new_item;
        }
    }

    fclose(file);
    printf("Inventory loaded from file successfully!\n");
    return head;
}

void view_items(Item *head) {
    if (head == NULL) {
        printf("No items in inventory!\n");
        return;
    }

    printf("\nInventory Items:\n");
    Item *current = head;
    while (current) {
        printf("Name: %s, Quantity: %d, Price: %.2f\n", current->name, current->quantity, current->price);
        current = current->next;
    }
}

Item* find_item(Item *head) {
    char name[50];
    printf("Enter item name to find: ");
    scanf(" %[^\n]", name);

    Item *current = head;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            printf("Item found: Name: %s, Quantity: %d, Price: %.2f\n", current->name, current->quantity, current->price);
            return current;
        }
        current = current->next;
    }

    printf("Item not found!\n");
    return NULL;
}

void purchase_item(Item *head) {
    char name[50];
    printf("Enter item name to purchase: ");
    scanf(" %[^\n]", name);

    Item *current = head;
    while (current) {
        if (strcmp(current->name, name) == 0) {
            int quantity;
            printf("Enter quantity to purchase: ");
            scanf("%d", &quantity);

            if (quantity > current->quantity) {
                printf("Not enough stock available!\n");
            } else {
                current->quantity -= quantity;
                printf("Purchased %d of %s. Remaining stock: %d\n", quantity, current->name, current->quantity);
            }
            return;
        }
        current = current->next;
    }

    printf("Item not found!\n");
}

void free_list(Item **head) {
    Item *current = *head;
    Item *next;

    while (current) {
        next = current->next;
        free(current);
        current = next;
    }

    *head = NULL;
}