#include "header.h"

void mainMenu(struct User u)
{
    int option;
    system("clear");
    printf("\n\n\t\t======= ATM =======\n\n");
    printf("\n\t\t-->> Feel free to choose one of the options below <<--\n");
    printf("\n\t\t[1]- Create a new account\n");
    printf("\n\t\t[2]- Update account information\n");
    printf("\n\t\t[3]- Check accounts\n");
    printf("\n\t\t[4]- Check list of owned account\n");
    printf("\n\t\t[5]- Make Transaction\n");
    printf("\n\t\t[6]- Remove existing account\n");
    printf("\n\t\t[7]- Transfer ownership\n");
    printf("\n\t\t[8]- Exit\n");

    option = getValidInteger("\n\t\tEnter your choice (1-8): ", 1, 8);
    if (option == -1) {
        printf("\n✖ Too many invalid attempts. Returning to login menu.\n");
        printf("Press Enter to continue...");
        getchar();
        return;
    }

    switch (option)
    {
    case 1:
        createNewAcc(u);
        break;
    case 2:
        updateAccountInfo(u);
        break;
    case 3:
        checkAccountDetails(u);
        break;
    case 4:
        checkAllAccounts(u);
        break;
    case 5:
        makeTransaction(u);
        break;
    case 6:
        removeAccount(u);
        break;
    case 7:
        transferOwnership(u);
        break;
    case 8:
        exit(1);
        break;
    }
};

void initMenu(struct User *u)
{
    int r = 0;
    int option;

    while (!r)
    {
        system("clear");
        printf("\n\n\t\t======= ATM =======\n");
        printf("\n\t\t-->> Feel free to login / register :\n");
        printf("\n\t\t[1]- login\n");
        printf("\n\t\t[2]- register\n");
        printf("\n\t\t[3]- exit\n");

        option = getValidInteger("\n\t\tEnter your choice (1-3): ", 1, 3);
        if (option == -1) {
            printf("\n✖ Too many invalid attempts. Exiting application.\n");
            exit(1);
        }
        switch (option)
        {
        case 1:
            loginMenu(u->name, u->password);
            if (strcmp(u->password, getPassword(*u)) == 0)
            {
                printf("\n\nPassword Match!");
            }
            else
            {
                printf("\nWrong password!! or User Name\n");
                exit(1);
            }
            r = 1;
            break;
        case 2:
            registerMenu(u->name, u->password);
            // After registration, provide clear navigation options
            r = 0; // Reset to continue the loop and show menu again
            break;
        case 3:
            exit(1);
            break;
        }
    }
};

int main()
{
    struct User u;
    
    initMenu(&u);
    mainMenu(u);
    return 0;
}
