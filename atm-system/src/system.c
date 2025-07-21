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

    // Get valid date with proper validation
    if (!getValidDate(&r.deposit)) {
        printf("\n✖ Failed to get valid date. Returning to main menu.\n");
        printf("Press Enter to continue...");
        getchar(); // consume newline
        getchar(); // wait for user input
        fclose(pf);
        success(u);
        return;
    }
    r.accountNbr = getValidInteger("\nEnter the account number: ", 1, 999999999);
    if (r.accountNbr == -1) {
        printf("\n✖ Failed to get valid account number. Returning to main menu.\n");
        printf("Press Enter to continue...");
        getchar();
        fclose(pf);
        success(u);
        return;
    }

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
    r.phone = getValidInteger("\nEnter the phone number: ", 1000000, 999999999);
    if (r.phone == -1) {
        printf("\n✖ Failed to get valid phone number. Returning to main menu.\n");
        printf("Press Enter to continue...");
        getchar();
        fclose(pf);
        success(u);
        return;
    }
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);

    int accountTypeChoice;
    printf("\nChoose the type of account:\n");
    printf("\t[1] -> saving\n");
    printf("\t[2] -> current\n");
    printf("\t[3] -> fixed01 (for 1 year)\n");
    printf("\t[4] -> fixed02 (for 2 years)\n");
    printf("\t[5] -> fixed03 (for 3 years)\n");
    accountTypeChoice = getValidInteger("\n\tEnter your choice (1-5): ", 1, 5);
    if (accountTypeChoice == -1) {
        printf("\n✖ Failed to get valid account type. Returning to main menu.\n");
        printf("Press Enter to continue...");
        getchar();
        fclose(pf);
        success(u);
        return;
    }

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

// Check if a year is a leap year
int isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Validate date (month, day, year)
int isValidDate(int month, int day, int year) {
    // Check year range (reasonable range for banking system)
    if (year < 1900 || year > 2100) {
        return 0;
    }

    // Check month range
    if (month < 1 || month > 12) {
        return 0;
    }

    // Days in each month
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Adjust for leap year
    if (isLeapYear(year)) {
        daysInMonth[1] = 29;
    }

    // Check day range
    if (day < 1 || day > daysInMonth[month - 1]) {
        return 0;
    }

    return 1;
}

// Get valid date input with proper format validation
int getValidDate(struct Date *date) {
    char dateInput[20];
    int attempts = 0;
    const int maxAttempts = 3;

    while (attempts < maxAttempts) {
        printf("\nEnter date (mm/dd/yyyy): ");
        scanf("%s", dateInput);

        // Try to parse the date
        int month, day, year;
        int parsed = sscanf(dateInput, "%d/%d/%d", &month, &day, &year);

        // Check if parsing was successful and format is correct
        if (parsed == 3) {
            // Validate the date components
            if (isValidDate(month, day, year)) {
                date->month = month;
                date->day = day;
                date->year = year;
                return 1; // Success
            } else {
                printf("✖ Invalid date! Please check:\n");
                printf("  • Month must be 1-12\n");
                printf("  • Day must be valid for the month\n");
                printf("  • Year must be between 1900-2100\n");
            }
        } else {
            printf("✖ Invalid format! Please use mm/dd/yyyy format (e.g., 12/25/2023)\n");
        }

        attempts++;
        if (attempts < maxAttempts) {
            printf("Attempts remaining: %d\n", maxAttempts - attempts);
        }
    }

    printf("✖ Too many invalid attempts. Please try again later.\n");
    return 0; // Failed after max attempts
}

// Get valid integer input with range validation and input buffer clearing
int getValidInteger(const char *prompt, int minValue, int maxValue) {
    char input[100];
    int value;
    int attempts = 0;
    const int maxAttempts = 3;

    while (attempts < maxAttempts) {
        printf("%s", prompt);

        // Read input as string first
        if (scanf("%s", input) != 1) {
            printf("✖ Error reading input. Please try again.\n");
            // Clear input buffer
            while (getchar() != '\n');
            attempts++;
            continue;
        }

        // Try to convert string to integer
        char *endptr;
        value = strtol(input, &endptr, 10);

        // Check if conversion was successful (entire string was converted)
        if (*endptr == '\0') {
            // Check if value is in valid range
            if (value >= minValue && value <= maxValue) {
                return value; // Success
            } else {
                printf("✖ Please enter a number between %d and %d.\n", minValue, maxValue);
            }
        } else {
            printf("✖ Invalid input! Please enter a valid number.\n");
        }

        attempts++;
        if (attempts < maxAttempts) {
            printf("Attempts remaining: %d\n", maxAttempts - attempts);
        }
    }

    printf("✖ Too many invalid attempts.\n");
    return -1; // Failed after max attempts
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

    accountNumber = getValidInteger("\nEnter the account number you want to update: ", 1, 999999999);
    if (accountNumber == -1) {
        printf("\n✖ Failed to get valid account number. Returning to main menu.\n");
        printf("Press Enter to continue...");
        getchar();
        success(currentUser);
        return;
    }

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
    fieldChoice = getValidInteger("Enter your choice (1-2): ", 1, 2);
    if (fieldChoice == -1) {
        printf("\n✖ Failed to get valid choice. Returning to main menu.\n");
        printf("Press Enter to continue...");
        getchar();
        success(currentUser);
        return;
    }

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

// Count total accounts for a user
int countUserAccounts(struct User user) {
    FILE *file = fopen(RECORDS, "r");
    if (!file) return 0;

    int count = 0;
    struct Record record;
    char userName[100];

    while (getAccountFromFile(file, userName, &record)) {
        if (strcmp(userName, user.name) == 0) {
            count++;
        }
    }

    fclose(file);
    return count;
}

// Display account details for confirmation before deletion
void displayAccountForConfirmation(struct Record record, const char* userName) {
    printf("\n=============== Confirm Account Deletion ===============\n\n");
    printf("Account Number: %d\n", record.accountNbr);
    printf("Account Holder: %s\n", userName);
    printf("Account Type: %s\n", record.accountType);
    printf("Current Balance: $%.2f\n", record.amount);
    printf("Country: %s\n", record.country);
    printf("Phone: %d\n", record.phone);
    printf("Deposit Date: %d/%d/%d\n", record.deposit.day, record.deposit.month, record.deposit.year);
    printf("\n⚠️  WARNING: This action cannot be undone!\n");
}

// Get confirmation from user for account deletion
int confirmDeletion() {
    char confirmation;
    printf("\nAre you sure you want to delete this account? (y/n): ");
    scanf(" %c", &confirmation);

    if (confirmation == 'y' || confirmation == 'Y') {
        return 1;
    } else if (confirmation == 'n' || confirmation == 'N') {
        return 0;
    } else {
        printf("✖ Please enter 'y' for yes or 'n' for no.\n");
        return confirmDeletion(); // Ask again for valid input
    }
}

// Remove account from file safely using temporary file
int removeAccountFromFile(int accountNumber, const char* userName) {
    FILE *originalFile = fopen(RECORDS, "r");
    if (!originalFile) {
        printf("✖ Error: Could not open records file for deletion!\n");
        return 0;
    }

    FILE *tempFile = fopen("./data/temp_records.txt", "w");
    if (!tempFile) {
        fclose(originalFile);
        printf("✖ Error: Could not create temporary file!\n");
        return 0;
    }

    struct Record record;
    char currentUserName[100];
    int accountFound = 0;

    // Copy all records except the one to be deleted
    while (getAccountFromFile(originalFile, currentUserName, &record)) {
        if (record.accountNbr == accountNumber && strcmp(currentUserName, userName) == 0) {
            accountFound = 1;
            // Skip this record (don't write to temp file)
            continue;
        }

        // Write all other records to temp file
        fprintf(tempFile, "%d %d %s %d %d/%d/%d %s %d %.2f %s\n\n",
                record.id, record.userId, currentUserName, record.accountNbr,
                record.deposit.month, record.deposit.day, record.deposit.year,
                record.country, record.phone, record.amount, record.accountType);
    }

    fclose(originalFile);
    fclose(tempFile);

    if (!accountFound) {
        remove("./data/temp_records.txt");
        return 0;
    }

    // Replace original file with updated file
    if (remove(RECORDS) != 0 || rename("./data/temp_records.txt", RECORDS) != 0) {
        printf("✖ Error: Failed to update records file!\n");
        return 0;
    }

    return 1;
}

// Main remove account function
void removeAccount(struct User currentUser) {
    int accountNumber;
    struct Record record;
    char userName[100];
    int accountFound = 0;

    system("clear");
    printf("\t\t\t===== Remove Account =====\n");

    // Check if user has any accounts
    int accountCount = countUserAccounts(currentUser);
    if (accountCount == 0) {
        printf("\n✖ You have no accounts to remove!\n");
        success(currentUser);
        return;
    }

    // Display user's accounts
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

    accountNumber = getValidInteger("\nEnter the account number you want to remove: ", 1, 999999999);
    if (accountNumber == -1) {
        printf("\n✖ Failed to get valid account number. Returning to main menu.\n");
        printf("Press Enter to continue...");
        getchar();
        success(currentUser);
        return;
    }

    // Find and validate the account
    FILE *checkFile = fopen(RECORDS, "r");
    if (!checkFile) {
        printf("✖ Error: Could not open records file!\n");
        stayOrReturn(0, removeAccount, currentUser);
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
        stayOrReturn(0, removeAccount, currentUser);
        return;
    }

    // Display account details for confirmation
    displayAccountForConfirmation(record, userName);

    // Get user confirmation
    if (!confirmDeletion()) {
        printf("\n✖ Account deletion cancelled by user.\n");
        success(currentUser);
        return;
    }

    // Remove the account from file
    if (removeAccountFromFile(accountNumber, currentUser.name)) {
        system("clear");
        printf("=============== Account Deleted Successfully ===============\n\n");
        printf("Account Number: %d has been permanently removed.\n", accountNumber);

        int remainingAccounts = countUserAccounts(currentUser);
        printf("Your remaining accounts: %d\n", remainingAccounts);

        printf("\n===============================================\n");
        success(currentUser);
    } else {
        printf("✖ Failed to remove account. Please try again.\n");
        stayOrReturn(0, removeAccount, currentUser);
    }
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

    accountNumber = getValidInteger("\nEnter the account number you want to check: ", 1, 999999999);
    if (accountNumber == -1) {
        printf("\n✖ Failed to get valid account number. Returning to main menu.\n");
        printf("Press Enter to continue...");
        getchar();
        success(currentUser);
        return;
    }

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

    accountNumber = getValidInteger("\nEnter the account number for transaction: ", 1, 999999999);
    if (accountNumber == -1) {
        printf("\n✖ Failed to get valid account number. Returning to main menu.\n");
        printf("Press Enter to continue...");
        getchar();
        success(currentUser);
        return;
    }

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
    transactionType = getValidInteger("Enter your choice (1-3): ", 1, 3);
    if (transactionType == -1) {
        printf("\n✖ Failed to get valid choice. Returning to main menu.\n");
        printf("Press Enter to continue...");
        getchar();
        success(currentUser);
        return;
    }

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

// Get user ID by username from users.txt
int getUserIdByName(const char* username) {
    FILE *file = fopen(USERS, "r");
    if (!file) return -1;

    struct User user;
    while (fscanf(file, "%d %s %s", &user.id, user.name, user.password) == 3) {
        if (strcmp(user.name, username) == 0) {
            fclose(file);
            return user.id;
        }
    }

    fclose(file);
    return -1;
}

// Validate target user exists and is different from current user
int isValidTargetUser(const char* targetUsername, const char* currentUsername) {
    // Check if trying to transfer to self
    if (strcmp(targetUsername, currentUsername) == 0) {
        return 0;
    }

    // Check if target user exists
    return (getUserIdByName(targetUsername) != -1);
}

// Display transfer confirmation details
void displayTransferConfirmation(struct Record record, const char* currentOwner, const char* newOwner) {
    printf("\n=============== Confirm Account Transfer ===============\n\n");
    printf("Account Number: %d\n", record.accountNbr);
    printf("Account Type: %s\n", record.accountType);
    printf("Current Balance: $%.2f\n", record.amount);
    printf("Country: %s\n", record.country);
    printf("Phone: %d\n", record.phone);
    printf("Current Owner: %s\n", currentOwner);
    printf("New Owner: %s\n", newOwner);
    printf("\n⚠️  WARNING: This will permanently transfer ownership!\n");
}

// Get confirmation from user for account transfer
int confirmTransfer() {
    char confirmation;
    printf("\nAre you sure you want to transfer this account? (y/n): ");
    scanf(" %c", &confirmation);

    if (confirmation == 'y' || confirmation == 'Y') {
        return 1;
    } else if (confirmation == 'n' || confirmation == 'N') {
        return 0;
    } else {
        printf("✖ Please enter 'y' for yes or 'n' for no.\n");
        return confirmTransfer(); // Ask again for valid input
    }
}

// Transfer account ownership in file safely using temporary file
int transferAccountOwnership(int accountNumber, const char* currentOwner, const char* newOwner, int newUserId) {
    FILE *originalFile = fopen(RECORDS, "r");
    if (!originalFile) {
        printf("✖ Error: Could not open records file for transfer!\n");
        return 0;
    }

    FILE *tempFile = fopen("./data/temp_records.txt", "w");
    if (!tempFile) {
        fclose(originalFile);
        printf("✖ Error: Could not create temporary file!\n");
        return 0;
    }

    struct Record record;
    char currentUserName[100];
    int accountFound = 0;

    // Copy all records, updating the target record
    while (getAccountFromFile(originalFile, currentUserName, &record)) {
        if (record.accountNbr == accountNumber && strcmp(currentUserName, currentOwner) == 0) {
            accountFound = 1;
            // Update ownership information
            record.userId = newUserId;
            strcpy(currentUserName, newOwner);
        }

        // Write record to temp file
        fprintf(tempFile, "%d %d %s %d %d/%d/%d %s %d %.2f %s\n\n",
                record.id, record.userId, currentUserName, record.accountNbr,
                record.deposit.month, record.deposit.day, record.deposit.year,
                record.country, record.phone, record.amount, record.accountType);
    }

    fclose(originalFile);
    fclose(tempFile);

    if (!accountFound) {
        remove("./data/temp_records.txt");
        return 0;
    }

    // Replace original file with updated file
    if (remove(RECORDS) != 0 || rename("./data/temp_records.txt", RECORDS) != 0) {
        printf("✖ Error: Failed to update records file!\n");
        return 0;
    }

    return 1;
}

// Main transfer ownership function
void transferOwnership(struct User currentUser) {
    int accountNumber;
    char targetUsername[50];
    struct Record record;
    char userName[100];
    int accountFound = 0;

    system("clear");
    printf("\t\t\t===== Transfer Account Ownership =====\n");

    // Check if user has any accounts
    int accountCount = countUserAccounts(currentUser);
    if (accountCount == 0) {
        printf("\n✖ You have no accounts to transfer!\n");
        success(currentUser);
        return;
    }

    // Display user's accounts available for transfer
    printf("\nYour accounts available for transfer:\n");
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

    accountNumber = getValidInteger("\nEnter the account number you want to transfer: ", 1, 999999999);
    if (accountNumber == -1) {
        printf("\n✖ Failed to get valid account number. Returning to main menu.\n");
        printf("Press Enter to continue...");
        getchar();
        success(currentUser);
        return;
    }

    // Find and validate the account
    FILE *checkFile = fopen(RECORDS, "r");
    if (!checkFile) {
        printf("✖ Error: Could not open records file!\n");
        stayOrReturn(0, transferOwnership, currentUser);
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
        stayOrReturn(0, transferOwnership, currentUser);
        return;
    }

    // Get target username
    printf("\nEnter the username to transfer this account to: ");
    scanf("%s", targetUsername);

    // Validate target user
    if (!isValidTargetUser(targetUsername, currentUser.name)) {
        if (strcmp(targetUsername, currentUser.name) == 0) {
            printf("✖ Cannot transfer account to yourself!\n");
        } else {
            printf("✖ User '%s' does not exist in the system!\n", targetUsername);
        }
        stayOrReturn(0, transferOwnership, currentUser);
        return;
    }

    // Get target user ID
    int targetUserId = getUserIdByName(targetUsername);
    if (targetUserId == -1) {
        printf("✖ Error: Could not access user database!\n");
        stayOrReturn(0, transferOwnership, currentUser);
        return;
    }

    // Display transfer details for confirmation
    displayTransferConfirmation(record, currentUser.name, targetUsername);

    // Get user confirmation
    if (!confirmTransfer()) {
        printf("\n✖ Account transfer cancelled by user.\n");
        success(currentUser);
        return;
    }

    // Transfer the account ownership
    if (transferAccountOwnership(accountNumber, currentUser.name, targetUsername, targetUserId)) {
        system("clear");
        printf("=============== Account Transfer Successful ===============\n\n");
        printf("Account Number: %d has been successfully transferred.\n", accountNumber);
        printf("Previous Owner: %s\n", currentUser.name);
        printf("New Owner: %s\n", targetUsername);

        int remainingAccounts = countUserAccounts(currentUser);
        printf("Your remaining accounts: %d\n", remainingAccounts);

        printf("\n===============================================\n");
        success(currentUser);
    } else {
        printf("✖ Failed to transfer account. Please try again.\n");
        stayOrReturn(0, transferOwnership, currentUser);
    }
}
