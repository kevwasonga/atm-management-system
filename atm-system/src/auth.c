#include <termios.h>
#include "header.h"

char *USERS = "./data/users.txt";

// Get the next available user ID
int getNextUserId() {
    FILE *file = fopen(USERS, "r");
    if (!file) {
        return 0; // If file doesn't exist, start with ID 0
    }

    int maxId = -1;
    struct User user;

    while (fscanf(file, "%d %s %s", &user.id, user.name, user.password) != EOF) {
        if (user.id > maxId) {
            maxId = user.id;
        }
    }

    fclose(file);
    return maxId + 1;
}

// Check if username is unique
int isUsernameUnique(const char *username) {
    FILE *file = fopen(USERS, "r");
    if (!file) {
        return 1; // If file doesn't exist, username is unique
    }

    struct User user;

    while (fscanf(file, "%d %s %s", &user.id, user.name, user.password) != EOF) {
        if (strcmp(user.name, username) == 0) {
            fclose(file);
            return 0; // Username already exists
        }
    }

    fclose(file);
    return 1; // Username is unique
}

void loginMenu(char username[50], char password[50])
{
    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login\n");
    printf("\n\t\t\t\tEnter username: ");
    scanf("%s", username);

    // Simple password input without terminal manipulation for now
    printf("\n\t\t\t\tEnter password: ");
    scanf("%s", password);
};

const char *getPassword(struct User u)
{
    FILE *fp;
    struct User userChecker;

    if ((fp = fopen("./data/users.txt", "r")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    while (fscanf(fp, "%d %s %s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, u.name) == 0)
        {
            fclose(fp);
            char *buff = userChecker.password;
            return buff;
        }
    }

    fclose(fp);
    return "no user found";
}

void registerMenu(char username[50], char password[50])
{
    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Registration\n");
    printf("\n\t\t\t\tEnter username: ");
    scanf("%s", username);

    // Check if username is unique
    if (!isUsernameUnique(username)) {
        printf("\n✖ Username '%s' already exists! Please choose a different username.\n", username);
        printf("Press Enter to continue...");
        getchar(); // consume newline
        getchar(); // wait for user input
        return;
    }

    // Simple password input without terminal manipulation
    printf("\n\t\t\t\tEnter password: ");
    scanf("%s", password);

    // Save user to file
    FILE *file = fopen(USERS, "a");
    if (!file) {
        printf("\n✖ Error: Could not open users file for writing!\n");
        return;
    }

    int userId = getNextUserId();
    fprintf(file, "%d %s %s\n", userId, username, password);
    fclose(file);

    printf("\n✔ User '%s' registered successfully with ID %d!\n", username, userId);
    printf("Press Enter to continue...");
    getchar(); // consume newline
    getchar(); // wait for user input
}