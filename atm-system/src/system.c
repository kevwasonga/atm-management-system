#include "header.h"

const char *RECORDS = "./data/records.txt";

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    return fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r->id,
		  &r->userId,
		  name,
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType) != EOF;
}

// Get the next available record ID
int getNextRecordId() {
    FILE *file = fopen(RECORDS, "r");
    if (!file) {
        return 0; // If file doesn't exist, start with ID 0
    }

    int maxId = -1;
    struct Record r;
    char userName[100];

    while (getAccountFromFile(file, userName, &r)) {
        if (r.id > maxId) {
            maxId = r.id;
        }
    }

    fclose(file);
    return maxId + 1;
}

void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
            r.id,
            u.id,
            u.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
    {
        system("clear");
        mainMenu(u);
    }
    else
    {
        system("clear");
        exit(1);
    }
}

void success(struct User u)
{
    int option;
    system("clear");
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);

    if (option == 1)
    {
        system("clear");
        mainMenu(u);
    }
    else if (option == 0)
    {
        system("clear");
        exit(1);
    }
    else
    {
        system("clear");
        printf("\n✔ Success!\n\n");
        printf("✖ Insert a valid operation!\n\n");
        goto invalid;
    }
}

void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    FILE *pf = fopen(RECORDS, "a+");

noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date(mm/dd/yyyy):");
    scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
    printf("\nEnter the account number:");
    scanf("%d", &r.accountNbr);

    // Check if account number already exists globally (for any user)
    rewind(pf); // Reset file pointer to beginning
    while (getAccountFromFile(pf, userName, &cr))
    {
        if (cr.accountNbr == r.accountNbr)
        {
            printf("✖ Account number %d already exists! Please choose a different account number.\n\n", r.accountNbr);
            goto noAccount;
        }
    }
    printf("\nEnter the country:");
    scanf("%s", r.country);
    printf("\nEnter the phone number:");
    scanf("%d", &r.phone);
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);

    int accountTypeChoice;
    printf("\nChoose the type of account:\n");
    printf("\t[1] -> saving\n");
    printf("\t[2] -> current\n");
    printf("\t[3] -> fixed01 (for 1 year)\n");
    printf("\t[4] -> fixed02 (for 2 years)\n");
    printf("\t[5] -> fixed03 (for 3 years)\n");
    printf("\n\tEnter your choice (1-5): ");
    scanf("%d", &accountTypeChoice);

    // Convert choice to account type string
    switch(accountTypeChoice) {
        case 1:
            strcpy(r.accountType, "saving");
            break;
        case 2:
            strcpy(r.accountType, "current");
            break;
        case 3:
            strcpy(r.accountType, "fixed01");
            break;
        case 4:
            strcpy(r.accountType, "fixed02");
            break;
        case 5:
            strcpy(r.accountType, "fixed03");
            break;
        default:
            printf("✖ Invalid choice! Defaulting to saving account.\n");
            strcpy(r.accountType, "saving");
            break;
    }

    // Set the record ID and user ID
    r.id = getNextRecordId();
    r.userId = u.id;

    saveAccountToFile(pf, u, r);

    fclose(pf);
    success(u);
}

void checkAllAccounts(struct User u)
{
    char userName[100];
    struct Record r;

    FILE *pf = fopen(RECORDS, "r");

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0)
        {
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
        }
    }
    fclose(pf);
    success(u);
}

// Validate phone number (7-15 digits, numbers only)
int isValidPhone(const char *phoneNumber) {
    int length = strlen(phoneNumber);
    if (length < 7 || length > 15) {
        return 0;
    }

    for (int i = 0; i < length; i++) {
        if (phoneNumber[i] < '0' || phoneNumber[i] > '9') {
            return 0;
        }
    }
    return 1;
}

// Update account information (country or phone number)
void updateAccountInfo(struct User currentUser) {
    int accountNumber;
    int fieldChoice;
    char newValue[100];
    struct Record record;
    char userName[100];
    int accountFound = 0;

    system("clear");
    printf("\t\t\t===== Update Account Information =====\n");

    // First, show user's accounts
    printf("\nYour accounts:\n");
    FILE *displayFile = fopen(RECORDS, "r");
    if (displayFile) {
        while (getAccountFromFile(displayFile, userName, &record)) {
            if (strcmp(userName, currentUser.name) == 0) {
                printf("Account Number: %d | Country: %s | Phone: %d\n",
                       record.accountNbr, record.country, record.phone);
            }
        }
        fclose(displayFile);
    }

    printf("\nEnter the account number you want to update: ");
    scanf("%d", &accountNumber);

    // Check if account exists and belongs to user
    FILE *checkFile = fopen(RECORDS, "r");
    if (!checkFile) {
        printf("✖ Error: Could not open records file!\n");
        stayOrReturn(0, updateAccountInfo, currentUser);
        return;
    }

    while (getAccountFromFile(checkFile, userName, &record)) {
        if (record.accountNbr == accountNumber && strcmp(userName, currentUser.name) == 0) {
            accountFound = 1;
            break;
        }
    }
    fclose(checkFile);

    if (!accountFound) {
        printf("✖ Account number %d not found or doesn't belong to you!\n", accountNumber);
        stayOrReturn(0, updateAccountInfo, currentUser);
        return;
    }

    printf("\nWhat would you like to update?\n");
    printf("[1] Country\n");
    printf("[2] Phone Number\n");
    printf("Enter your choice: ");
    scanf("%d", &fieldChoice);

    if (fieldChoice == 1) {
        printf("Enter new country: ");
        scanf("%s", newValue);

        if (strlen(newValue) == 0) {
            printf("✖ Country cannot be empty!\n");
            stayOrReturn(0, updateAccountInfo, currentUser);
            return;
        }
    } else if (fieldChoice == 2) {
        printf("Enter new phone number: ");
        scanf("%s", newValue);

        if (!isValidPhone(newValue)) {
            printf("✖ Invalid phone number! Must be 7-15 digits only.\n");
            stayOrReturn(0, updateAccountInfo, currentUser);
            return;
        }
    } else {
        printf("✖ Invalid choice!\n");
        stayOrReturn(0, updateAccountInfo, currentUser);
        return;
    }

    // Update the record in file
    FILE *originalFile = fopen(RECORDS, "r");
    FILE *tempFile = fopen("./data/temp_records.txt", "w");

    if (!originalFile || !tempFile) {
        printf("✖ Error: Could not open files for updating!\n");
        if (originalFile) fclose(originalFile);
        if (tempFile) fclose(tempFile);
        stayOrReturn(0, updateAccountInfo, currentUser);
        return;
    }

    // Copy all records, updating the target record
    while (getAccountFromFile(originalFile, userName, &record)) {
        if (record.accountNbr == accountNumber && strcmp(userName, currentUser.name) == 0) {
            // Update the specific field
            if (fieldChoice == 1) {
                strcpy(record.country, newValue);
            } else if (fieldChoice == 2) {
                record.phone = atoi(newValue);
            }
        }

        // Write record to temp file
        fprintf(tempFile, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
                record.id, record.userId, userName, record.accountNbr,
                record.deposit.month, record.deposit.day, record.deposit.year,
                record.country, record.phone, record.amount, record.accountType);
    }

    fclose(originalFile);
    fclose(tempFile);

    // Replace original file with updated file
    remove(RECORDS);
    rename("./data/temp_records.txt", RECORDS);

    printf("✔ Account information updated successfully!\n");
    success(currentUser);
}

// Calculate monthly interest for savings account (7% annually)
double calculateSavingsInterest(double balance) {
    return (balance * 0.07) / 12.0;
}

// Calculate annual interest for fixed accounts
double calculateFixedInterest(double balance, const char* accountType) {
    if (strcmp(accountType, "fixed01") == 0) {
        return balance * 0.04;
    } else if (strcmp(accountType, "fixed02") == 0) {
        return balance * 0.05;
    } else if (strcmp(accountType, "fixed03") == 0) {
        return balance * 0.08;
    }
    return 0.0;
}

// Calculate maturity date by adding years to deposit date
void calculateMaturityDate(struct Date depositDate, int years, struct Date* maturityDate) {
    maturityDate->day = depositDate.day;
    maturityDate->month = depositDate.month;
    maturityDate->year = depositDate.year + years;
}

// Check details of a specific account with interest calculations
void checkAccountDetails(struct User currentUser) {
    int accountNumber;
    struct Record record;
    char userName[100];
    int accountFound = 0;

    system("clear");
    printf("\t\t\t===== Account Details =====\n");

    // First, show user's accounts for easy selection
    printf("\nYour accounts:\n");
    FILE *displayFile = fopen(RECORDS, "r");
    if (displayFile) {
        printf("Account Number | Type | Balance\n");
        printf("--------------------------------\n");
        while (getAccountFromFile(displayFile, userName, &record)) {
            if (strcmp(userName, currentUser.name) == 0) {
                printf("%-14d | %-8s | $%.2f\n",
                       record.accountNbr, record.accountType, record.amount);
            }
        }
        fclose(displayFile);
    }

    printf("\nEnter the account number you want to check: ");
    scanf("%d", &accountNumber);

    // Find and validate the account
    FILE *checkFile = fopen(RECORDS, "r");
    if (!checkFile) {
        printf("✖ Error: Could not open records file!\n");
        stayOrReturn(0, checkAccountDetails, currentUser);
        return;
    }

    while (getAccountFromFile(checkFile, userName, &record)) {
        if (record.accountNbr == accountNumber && strcmp(userName, currentUser.name) == 0) {
            accountFound = 1;
            break;
        }
    }
    fclose(checkFile);

    if (!accountFound) {
        printf("✖ Account number %d not found or doesn't belong to you!\n", accountNumber);
        stayOrReturn(0, checkAccountDetails, currentUser);
        return;
    }

    // Display account details
    system("clear");
    printf("=============== Account Details ===============\n\n");
    printf("Account Number: %d\n", record.accountNbr);
    printf("Account Holder: %s\n", userName);
    printf("Deposit Date: %d/%d/%d\n", record.deposit.day, record.deposit.month, record.deposit.year);
    printf("Country: %s\n", record.country);
    printf("Phone Number: %d\n", record.phone);
    printf("Current Balance: $%.2f\n", record.amount);
    printf("Account Type: %s\n", record.accountType);

    printf("\n=============== Interest Information ===============\n");

    // Calculate and display interest based on account type
    if (strcmp(record.accountType, "saving") == 0) {
        double monthlyInterest = calculateSavingsInterest(record.amount);
        printf("You will get $%.2f as interest on day 10 of every month.\n", monthlyInterest);
    } else if (strcmp(record.accountType, "fixed01") == 0) {
        double annualInterest = calculateFixedInterest(record.amount, "fixed01");
        struct Date maturityDate;
        calculateMaturityDate(record.deposit, 1, &maturityDate);
        printf("You will get $%.2f as interest on %d/%d/%d.\n",
               annualInterest, maturityDate.day, maturityDate.month, maturityDate.year);
    } else if (strcmp(record.accountType, "fixed02") == 0) {
        double annualInterest = calculateFixedInterest(record.amount, "fixed02");
        struct Date maturityDate;
        calculateMaturityDate(record.deposit, 2, &maturityDate);
        printf("You will get $%.2f as interest on %d/%d/%d.\n",
               annualInterest, maturityDate.day, maturityDate.month, maturityDate.year);
    } else if (strcmp(record.accountType, "fixed03") == 0) {
        double annualInterest = calculateFixedInterest(record.amount, "fixed03");
        struct Date maturityDate;
        calculateMaturityDate(record.deposit, 3, &maturityDate);
        printf("You will get $%.2f as interest on %d/%d/%d.\n",
               annualInterest, maturityDate.day, maturityDate.month, maturityDate.year);
    } else if (strcmp(record.accountType, "current") == 0) {
        printf("You will not get interests because the account is of type current.\n");
    } else {
        printf("Interest calculation not available for account type: %s\n", record.accountType);
    }

    printf("\n===============================================\n");
    success(currentUser);
}

// Custom error handling for transaction errors (preserves custom error messages)
void transactionError(void f(struct User u), struct User u) {
    int option;
    printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
    scanf("%d", &option);
    if (option == 0)
        f(u);
    else if (option == 1)
        mainMenu(u);
    else if (option == 2)
        exit(0);
    else {
        printf("Insert a valid operation!\n");
        transactionError(f, u);
    }
}

// Check if transactions are allowed for account type
int isTransactionAllowed(const char* accountType) {
    return (strcmp(accountType, "saving") == 0 || strcmp(accountType, "current") == 0);
}

// Validate transaction amount
int validateTransactionAmount(double amount) {
    return (amount > 0.0);
}

// Check if account has sufficient balance for withdrawal
int hasSufficientBalance(double currentBalance, double withdrawAmount) {
    return (currentBalance >= withdrawAmount);
}

// Display transaction summary
void displayTransactionSummary(const char* type, int accountNumber, double amount, double oldBalance, double newBalance) {
    system("clear");
    printf("=============== Transaction Successful ===============\n\n");
    printf("Transaction Type: %s\n", type);
    printf("Account Number: %d\n", accountNumber);
    printf("Amount: $%.2f\n", amount);
    printf("Previous Balance: $%.2f\n", oldBalance);
    printf("New Balance: $%.2f\n", newBalance);
    printf("\n===============================================\n");
}

// Main transaction function : Transaction Management Feature
void makeTransaction(struct User currentUser) {
    int accountNumber;
    int transactionType;
    double transactionAmount;
    struct Record record;
    char userName[100];
    int accountFound = 0;
    double oldBalance, newBalance;

    system("clear");
    printf("\t\t\t===== Make Transaction =====\n");

    // Display user's accounts with balances
    printf("\nYour accounts:\n");
    FILE *displayFile = fopen(RECORDS, "r");
    if (displayFile) {
        printf("Account Number | Type     | Balance\n");
        printf("-----------------------------------\n");
        while (getAccountFromFile(displayFile, userName, &record)) {
            if (strcmp(userName, currentUser.name) == 0) {
                printf("%-14d | %-8s | $%.2f\n",
                       record.accountNbr, record.accountType, record.amount);
            }
        }
        fclose(displayFile);
    }

    printf("\nEnter the account number for transaction: ");
    scanf("%d", &accountNumber);

    // Find and validate the account
    FILE *checkFile = fopen(RECORDS, "r");
    if (!checkFile) {
        printf("✖ Error: Could not open records file!\n");
        stayOrReturn(0, makeTransaction, currentUser);
        return;
    }

    while (getAccountFromFile(checkFile, userName, &record)) {
        if (record.accountNbr == accountNumber && strcmp(userName, currentUser.name) == 0) {
            accountFound = 1;
            oldBalance = record.amount;
            break;
        }
    }
    fclose(checkFile);

    if (!accountFound) {
        printf("✖ Account number %d not found or doesn't belong to you!\n", accountNumber);
        stayOrReturn(0, makeTransaction, currentUser);
        return;
    }

    // Check if transactions are allowed for this account type
    if (!isTransactionAllowed(record.accountType)) {
        printf("✖ Transactions not allowed for %s accounts!\n", record.accountType);
        transactionError(makeTransaction, currentUser);
        return;
    }

    // Transaction type selection
    printf("\nSelect transaction type:\n");
    printf("[1] Deposit\n");
    printf("[2] Withdrawal\n");
    printf("[3] Return to main menu\n");
    printf("Enter your choice: ");
    scanf("%d", &transactionType);

    if (transactionType == 3) {
        mainMenu(currentUser);
        return;
    }

    if (transactionType != 1 && transactionType != 2) {
        printf("✖ Invalid transaction type!\n");
        transactionError(makeTransaction, currentUser);
        return;
    }

    // Amount input and validation
    printf("\nEnter transaction amount: $");
    scanf("%lf", &transactionAmount);

    if (!validateTransactionAmount(transactionAmount)) {
        printf("✖ Amount must be positive!\n");
        transactionError(makeTransaction, currentUser);
        return;
    }

    // Check sufficient balance for withdrawal
    if (transactionType == 2 && !hasSufficientBalance(oldBalance, transactionAmount)) {
        printf("✖ Insufficient balance for withdrawal!\n");
        printf("Current balance: $%.2f, Requested amount: $%.2f\n", oldBalance, transactionAmount);
        transactionError(makeTransaction, currentUser);
        return;
    }

    // Calculate new balance
    if (transactionType == 1) {
        newBalance = oldBalance + transactionAmount;
    } else {
        newBalance = oldBalance - transactionAmount;
    }

    // Update the record in file using temporary file
    FILE *originalFile = fopen(RECORDS, "r");
    FILE *tempFile = fopen("./data/temp_records.txt", "w");

    if (!originalFile || !tempFile) {
        printf("✖ Error: Could not open files for updating!\n");
        if (originalFile) fclose(originalFile);
        if (tempFile) fclose(tempFile);
        stayOrReturn(0, makeTransaction, currentUser);
        return;
    }

    // Copy all records, updating the target record
    while (getAccountFromFile(originalFile, userName, &record)) {
        if (record.accountNbr == accountNumber && strcmp(userName, currentUser.name) == 0) {
            record.amount = newBalance;
        }

        // Write record to temp file
        fprintf(tempFile, "%d %d %s %d %d/%d/%d %s %d %.2f %s\n\n",
                record.id, record.userId, userName, record.accountNbr,
                record.deposit.month, record.deposit.day, record.deposit.year,
                record.country, record.phone, record.amount, record.accountType);
    }

    fclose(originalFile);
    fclose(tempFile);

    // Replace original file with updated file
    remove(RECORDS);
    rename("./data/temp_records.txt", RECORDS);

    // Display transaction summary
    const char* transactionTypeName = (transactionType == 1) ? "Deposit" : "Withdrawal";
    displayTransactionSummary(transactionTypeName, accountNumber, transactionAmount, oldBalance, newBalance);

    success(currentUser);
}
