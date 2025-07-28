# ATM Management System

A comprehensive ATM management system implemented in C that provides secure banking operations including account management, transactions, and user authentication.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Project Structure](#project-structure)
- [Installation](#installation)
- [Usage](#usage)
- [Data Format](#data-format)
- [Account Types](#account-types)
- [Security Features](#security-features)
- [Testing](#testing)
- [Contributing](#contributing)
- [License](#license)

## Overview

This ATM Management System is a terminal-based application that simulates real-world ATM operations. Users can register, login, create multiple accounts, perform transactions, and manage their banking information securely. The system uses file-based storage for persistence and implements various account types with different interest rates and transaction rules.

## Features

### Core Features
- **User Authentication**: Secure login and registration system
- **Account Management**: Create, update, view, and delete bank accounts
- **Transaction Processing**: Deposit and withdraw money with validation
- **Account Types**: Support for current, savings, and fixed-term accounts
- **Interest Calculation**: Automatic interest calculation based on account type
- **Ownership Transfer**: Transfer account ownership between users
- **Data Persistence**: File-based storage system

### Account Operations
1. **Login/Register**: Secure user authentication with unique usernames
2. **Create New Account**: Set up different types of bank accounts
3. **Check Account Details**: View account information and interest calculations
4. **Update Account Information**: Modify country and phone number
5. **Make Transactions**: Deposit/withdraw money with account type restrictions
6. **Remove Account**: Delete existing accounts
7. **Transfer Ownership**: Transfer accounts to other users
8. **List Accounts**: View all accounts owned by the user

## Project Structure

```
atm-management-system/
├── atm-system/
│   ├── src/
│   │   ├── main.c          # Main program entry and menu system
│   │   ├── auth.c          # Authentication functions
│   │   ├── system.c        # Core system operations
│   │   └── header.h        # Function declarations and structures
│   ├── data/
│   │   ├── users.txt       # User account database
│   │   ├── records.txt     # Account records database
│   │   ├── users.txt.template    # Template for user data
│   │   └── records.txt.template  # Template for record data
│   ├── Makefile           # Build configuration
│   └── atm                # Compiled executable
├── README.md              # This file
├── problemstatement.txt   # Original project requirements
└── breakdown.txt          # Detailed implementation plan
```

## Installation

### Prerequisites
- GCC compiler
- Make utility
- Linux/Unix environment (recommended)

### Build Instructions

1. **Clone the repository**:
   ```bash
   git clone <repository-url>
   cd atm-management-system
   ```

2. **Navigate to the project directory**:
   ```bash
   cd atm-system
   ```

3. **Compile the project**:
   ```bash
   make
   ```

4. **Run the application**:
   ```bash
   ./atm
   ```

### Clean Build
To remove compiled object files:
```bash
make clean
```

## Quick Start Guide

### 1. Build and Run
```bash
cd atm-system
make
./atm
```

### 2. First Time User
```
Welcome to ATM System
[1] Login
[2] Register
[3] Exit

Choose option: 2
Enter username: john_doe
Enter password: secure123
Registration successful!
```

### 3. Create Your First Account
```
ATM Main Menu
[1] Create a new account
Choose option: 1

Enter account number: 12345
Select account type:
[1] Current
[2] Savings
[3] Fixed01 (1 year)
[4] Fixed02 (2 years)
[5] Fixed03 (3 years)
Choose: 2

Enter country: USA
Enter phone: 1234567890
Enter initial deposit: 1000.00
Account created successfully!
```

## Usage

### Starting the Application
Run the compiled executable:
```bash
./atm
```

### First Time Setup
1. Choose "Register" to create a new user account
2. Enter a unique username and secure password
3. Login with your credentials
4. Create your first bank account

### Main Menu Options
1. **Create a new account** - Set up different types of bank accounts
2. **Update account information** - Modify country or phone number
3. **Check accounts** - View detailed account information
4. **Check list of owned accounts** - See all your accounts
5. **Make Transaction** - Deposit or withdraw money
6. **Remove existing account** - Delete an account
7. **Transfer ownership** - Transfer account to another user
8. **Exit** - Close the application

### Example Workflows

#### Making a Transaction
```
[5] Make Transaction
Enter account ID: 12345
[1] Deposit
[2] Withdraw
Choose: 1
Enter amount: 500.00
Transaction successful! New balance: $1500.00
```

#### Checking Account Details
```
[3] Check accounts
Enter account ID: 12345

Account Details:
- Account ID: 12345
- Type: Savings
- Balance: $1500.00
- Country: USA
- Phone: 1234567890
- Interest Rate: 7%
- Monthly Interest: You will get $8.75 as interest on day 10 of every month
```

#### Transferring Ownership
```
[7] Transfer ownership
Enter account ID to transfer: 12345
Enter recipient username: jane_doe
Transfer successful! Account 12345 now belongs to jane_doe
```

## Data Format

### User Data (users.txt)
Format: `id name password`
```
0 Alice 1234password
1 Michel password1234
```

### Account Records (records.txt)
Format: `id user_id username account_id date country phone balance account_type`
```
0 0 Alice 0 10/02/2020 Germany 986134231 11090830.00 current
1 1 Michel 2 10/10/2021 Portugal 914134431 1920.42 savings
```

## Account Types

### Current Account
- **Interest Rate**: 0% (No interest)
- **Transactions**: Unlimited deposits and withdrawals
- **Purpose**: Daily banking operations

### Savings Account
- **Interest Rate**: 7% annually
- **Transactions**: Unlimited deposits and withdrawals
- **Interest**: Calculated monthly on the 10th of each month

### Fixed-Term Accounts

#### Fixed01 (1 Year)
- **Interest Rate**: 4% annually
- **Transactions**: No withdrawals or deposits allowed
- **Term**: 1 year lock-in period

#### Fixed02 (2 Years)
- **Interest Rate**: 5% annually
- **Transactions**: No withdrawals or deposits allowed
- **Term**: 2 year lock-in period

#### Fixed03 (3 Years)
- **Interest Rate**: 8% annually
- **Transactions**: No withdrawals or deposits allowed
- **Term**: 3 year lock-in period

## Security Features

- **Input Validation**: Comprehensive validation for all user inputs
- **Password Protection**: Secure password storage and verification
- **Account Ownership**: Users can only access their own accounts
- **Transaction Limits**: Prevents overdrafts and invalid transactions
- **Data Integrity**: File-based storage with error handling

## Testing

The project includes comprehensive testing documentation:

- `automated_validation_tests.txt` - Automated test scenarios
- `complete_procedural_tests.txt` - Manual testing procedures
- `comprehensive_testing_validation.txt` - Full validation suite
- `security_penetration_tests.txt` - Security testing scenarios

### Running Tests
Execute the test scenarios documented in the testing files to validate functionality.

## Development Process

### Implementation Phases

The project was developed in structured phases following a feature-driven approach:

1. **Phase 1**: Core Infrastructure
   - Basic file structure setup
   - User authentication system
   - Account creation functionality

2. **Phase 2**: Account Management
   - Account details viewing
   - Account information updates
   - Account removal functionality

3. **Phase 3**: Transaction System
   - Deposit/withdrawal operations
   - Transaction validation
   - Account type restrictions

4. **Phase 4**: Advanced Features
   - Ownership transfer system
   - Interest calculations
   - Enhanced input validation

5. **Phase 5**: Testing & Validation
   - Comprehensive test suite
   - Security testing
   - Edge case handling

### Quality Assurance

The project includes extensive testing documentation:
- **Unit Tests**: Individual function validation
- **Integration Tests**: System-wide functionality testing
- **Security Tests**: Penetration testing scenarios
- **Edge Case Tests**: Boundary condition validation
- **User Acceptance Tests**: Real-world usage scenarios

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/new-feature`)
3. Commit your changes (`git commit -am 'Add: New feature'`)
4. Push to the branch (`git push origin feature/new-feature`)
5. Create a Pull Request

### Coding Standards
- Follow modular programming principles
- Implement comprehensive error handling
- Add comments for complex logic
- Validate all user inputs
- Maintain consistent code formatting

### Development Guidelines
- **Branching Strategy**: Use feature branches for new functionality
- **Commit Messages**: Follow format `<Action>: <Description>`
- **Code Review**: All changes require review before merging
- **Testing**: Add tests for new features
- **Documentation**: Update README for significant changes

## API Reference

### Core Functions

#### Authentication Functions (`auth.c`)
- `void loginMenu(char username[50], char password[50])` - Handle user login
- `void registerMenu(char username[50], char password[50])` - Handle user registration
- `const char *getPassword(struct User u)` - Retrieve user password

#### System Functions (`system.c`)
- `void createNewAcc(struct User u)` - Create new bank account
- `void mainMenu(struct User u)` - Display main menu interface
- `void checkAllAccounts(struct User u)` - List all user accounts
- `void updateAccountInfo(struct User u)` - Update account information
- `void checkAccountDetails(struct User u)` - View specific account details
- `void makeTransaction(struct User u)` - Process deposits/withdrawals
- `void removeAccount(struct User u)` - Delete account
- `void transferOwnership(struct User u)` - Transfer account ownership

#### Utility Functions
- `int isValidPhone(const char *phoneNumber)` - Validate phone numbers
- `int isValidDate(int month, int day, int year)` - Validate dates
- `int getValidDate(struct Date *date)` - Get valid date input
- `int getValidInteger(const char *prompt, int minValue, int maxValue)` - Get valid integer input
- `double getValidAmount(const char *prompt, double minValue, double maxValue)` - Get valid amount input

### Data Structures

#### User Structure
```c
struct User {
    int id;
    char name[50];
    char password[50];
};
```

#### Record Structure
```c
struct Record {
    int id;
    int userId;
    char name[100];
    char country[100];
    int phone;
    char accountType[10];
    int accountNbr;
    double amount;
    struct Date deposit;
    struct Date withdraw;
};
```

#### Date Structure
```c
struct Date {
    int month, day, year;
};
```

## Troubleshooting

### Common Issues

#### Compilation Errors
- **Issue**: `gcc: command not found`
  - **Solution**: Install GCC compiler: `sudo apt-get install gcc` (Ubuntu/Debian)

- **Issue**: `make: command not found`
  - **Solution**: Install make utility: `sudo apt-get install make` (Ubuntu/Debian)

#### Runtime Errors
- **Issue**: "Permission denied" when running `./atm`
  - **Solution**: Make executable: `chmod +x atm`

- **Issue**: "No such file or directory" for data files
  - **Solution**: Ensure `data/users.txt` and `data/records.txt` exist

#### Data Issues
- **Issue**: Corrupted data files
  - **Solution**: Restore from templates in `data/` directory

- **Issue**: Login failures
  - **Solution**: Check user credentials in `data/users.txt`

### Debug Mode
To enable debug output, modify the source code to include debug prints:
```c
#define DEBUG 1
#if DEBUG
    printf("Debug: %s\n", debug_message);
#endif
```

## Performance Considerations

- **File I/O**: The system uses text files for storage, which may be slower for large datasets
- **Memory Usage**: Structures are loaded into memory during operations
- **Scalability**: Current implementation is suitable for small to medium user bases
- **Concurrency**: Single-user access only; no multi-threading support

## Future Enhancements

### Planned Features
- [ ] Database integration (SQLite)
- [ ] Password encryption
- [ ] Real-time notifications for transfers
- [ ] Enhanced terminal UI (TUI)
- [ ] Multi-user concurrent access
- [ ] Transaction history logging
- [ ] Account statements generation
- [ ] PIN-based authentication

### Bonus Features (Available)
- **Real-time Transfer Notifications**: Using pipes and child processes
- **Enhanced Terminal Interface**: Improved user experience
- **Password Encryption**: Secure password storage
- **Custom Makefile**: Optimized build process

## License

This project is part of an educational programming exercise focusing on C language fundamentals, file manipulation, and system design.

---

**Note**: This is an educational project designed to demonstrate C programming concepts including file I/O, data structures, memory management, and terminal user interfaces.
