#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SUGGESTIONS 5
#define MAX_NAME_LENGTH 100
#define MAX_PHONE_LENGTH 15
#define FILE_NAME "contacts.txt"

typedef struct Node {
    char name[MAX_NAME_LENGTH];
    char phone[MAX_PHONE_LENGTH];
    struct Node* left;
    struct Node* right;
} Node;

Node* createNode(char* name, char* phone) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->name, name);
    strcpy(newNode->phone, phone);
    newNode->left = newNode->right = NULL;
    return newNode;
}

Node* insert(Node* root, char* name, char* phone) {
    if (root == NULL) {
        return createNode(name, phone);
    }
    
    int cmp = strcmp(name, root->name);
    if (cmp < 0) {
        root->left = insert(root->left, name, phone);
    } else if (cmp > 0) {
        root->right = insert(root->right, name, phone);
    } else {
        printf("Contact with name '%s' already exists. Do you want to update the contact? (y/n): ", name);
        char choice;
        scanf(" %c", &choice);
        if (choice == 'y' || choice == 'Y') {
            strcpy(root->phone, phone);  // Update phone number
        }
    }
    return root;
}

void printSuggestions(Node* root, const char* name) {
    static int suggestionsCount = 0;
    if (root == NULL || suggestionsCount >= MAX_SUGGESTIONS) {
        return;
    }

    if (strstr(root->name, name) != NULL) {
        printf("Did you mean: %s\n", root->name);
        suggestionsCount++;
    }
    
    printSuggestions(root->left, name);
    printSuggestions(root->right, name);
}

void search(Node* root, const char* name) {
    printf("Searching for contacts matching '%s'...\n", name);
    printSuggestions(root, name);
}

void saveContactsToFile(Node* root) {
    if (root == NULL) {
        return;
    }

    FILE* file = fopen(FILE_NAME, "a");  // Append mode
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    fprintf(file, "%s,%s\n", root->name, root->phone);
    fclose(file);

    saveContactsToFile(root->left);
    saveContactsToFile(root->right);
}

void displayContacts(Node* root) {
    if (root == NULL) {
        return;
    }
    displayContacts(root->left);
    printf("Name: %s, Phone: %s\n", root->name, root->phone);
    displayContacts(root->right);
}

int isValidPhoneNumber(const char* phone) {
    if (strlen(phone) < 10 || strlen(phone) > 15) return 0;
    for (int i = 0; phone[i] != '\0'; i++) {
        if (!isdigit(phone[i])) return 0;
    }
    return 1;
}

int isValidName(const char* name) {
    if (strlen(name) == 0) return 0;
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i]) && name[i] != ' ') return 0;
    }
    return 1;
}

int main() {
    Node* root = NULL;
    int choice;
    
    while (1) {
        printf("\n--- Contact Management ---\n");
        printf("1. Add Contact\n");
        printf("2. Search Contacts\n");
        printf("3. Display All Contacts\n");
        printf("4. Save Contacts to File\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // To consume newline character

        switch (choice) {
            case 1: {
                char name[MAX_NAME_LENGTH];
                char phone[MAX_PHONE_LENGTH];
                
                printf("Enter contact name: ");
                fgets(name, MAX_NAME_LENGTH, stdin);
                name[strcspn(name, "\n")] = 0;  // Remove newline from fgets
                
                if (!isValidName(name)) {
                    printf("Invalid name. Please enter a valid name containing only letters and spaces.\n");
                    break;
                }
                
                printf("Enter contact phone number: ");
                fgets(phone, MAX_PHONE_LENGTH, stdin);
                phone[strcspn(phone, "\n")] = 0;  // Remove newline from fgets
                
                if (!isValidPhoneNumber(phone)) {
                    printf("Invalid phone number. Please enter a valid phone number.\n");
                    break;
                }
                
                root = insert(root, name, phone);
                break;
            }
            case 2: {
                char searchTerm[MAX_NAME_LENGTH];
                printf("Enter search term: ");
                fgets(searchTerm, MAX_NAME_LENGTH, stdin);
                searchTerm[strcspn(searchTerm, "\n")] = 0;  // Remove newline from fgets
                
                search(root, searchTerm);
                break;
            }
            case 3:
                printf("\nDisplaying all contacts:\n");
                displayContacts(root);
                break;
            case 4:
                saveContactsToFile(root);
                printf("Contacts saved to file.\n");
                break;
            case 5:
                printf("Exiting the program...\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
        
        printf("\n--- End of Operation ---\n");
    }

    return 0;
}
