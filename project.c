#include <stdio.h>
#define ALLOWED_WORKER_COUNT 3

int WORKER_COUNT = 0;
int foundIndexByEmail = -1;
int foundIndexByPwd = -1;

struct Worker {
    int age;
    char name[50];
    char email[50];
    char password[50];
};

struct Worker workers[ALLOWED_WORKER_COUNT];

void createWorkers();
void printWorkerInfo();

// *database
void saveAllChanges();
void saveData();
void loadData();

// *auth
void signup();
void login();
int isInputEmail(char input[50]);
int getUserIndexByEmail(char inputEmail[50]);
int getUserIndexByPwd(char inputPwd[50]);
int checkEmailExists(char inputEmail[50]);
int isPasswordValid(char inputPwd[50]);
int checkEmailsEqual(char inputEmail[50], char userEmail[50]);
int checkPasswordsEqual(char inputPassword[50], char userPassword[50]);

// *app
void lobby();

// *util
int getStrLength(char str[50]);

int main() {
    // struct Worker *worker_ptr = workers;
    // printf("%d", worker_ptr->age);

    // create workers
    // createWorkers();

    // load data
    loadData();

    // save initial data
    // saveInitialData();
    
    // print worker info
    printWorkerInfo();

    // lobby
    lobby();

    return 0;
}

void saveAllChanges() {
    FILE *fptr = fopen("./data/workers.txt", "w");

    if(fptr == NULL) {
        printf("File Not Found!");
        return;
    }

    printf("\n==== Saving Initial Data ====\n");

    for (int i = 0; i < WORKER_COUNT; i++)
    {
        fprintf(fptr, "%d %s %s %s\n", workers[i].age, workers[i].name, workers[i].email, workers[i].password);
    }
    
    fclose(fptr);
}

void saveData() {

}

void loadData() {
    FILE *fptr = fopen("./data/workers.txt", "r");

    if(fptr == NULL) {
        printf("File not found!");
        return;
    }

    for(int i = 0; i < ALLOWED_WORKER_COUNT; i++) {

        if(fscanf(fptr, "%d %s %s %s", &workers[i].age, workers[i].name, workers[i].email, workers[i].password) == EOF) {
            return;
        }
        WORKER_COUNT++;
    }


    fclose(fptr);
}

void createWorkers() {
    int count = 0;
    while (count < ALLOWED_WORKER_COUNT)
    {
        printf("Enter Age: ");
        scanf("%d", &workers[count].age);

        printf("Enter Name: ");
        scanf(" %[^\n]", workers[count].name);
        
        printf("Enter Email: ");
        scanf(" %[^\n]", workers[count].email);

        printf("Enter Password: ");
        scanf(" %[^\n]", workers[count].password);

        printf("=================\n");

        WORKER_COUNT++;
        count++;
    }
};

void printWorkerInfo() {
    int count = 0;

    printf("==== Printing Data ==== (%d)\n", WORKER_COUNT);

    while(count < WORKER_COUNT) {
        printf("Name: %s  Age: %d  Email: %s  Password: %s\n", workers[count].name, workers[count].age, workers[count].email, workers[count].password);

        count++;
    }
}

void signup() {
    int ageInput = 0;
    char nameInput[50];
    char emailInput[50];
    char pwdInput[50];

    printf("\n==== SIGN UP FORM ====\n");

    do {
        printf("Enter your age: ");
        scanf("%d", &ageInput);
    } while(ageInput == 0 || ageInput < 16);

    // TODO: check username already existed
    do {
        printf("Enter your name: ");
        scanf(" %[^\n]", nameInput);
    } while(0);

    do {
        printf("Enter your email: ");
        scanf(" %[^\n]", emailInput);

    } while(!isInputEmail(emailInput) || checkEmailExists(emailInput));
    
    do {
        printf("Enter your password: ");
        scanf(" %[^\n]", pwdInput);
    } while(!isPasswordValid(pwdInput));

    printf("name: %s, age: %d, email: %s, pwd: %s", nameInput, ageInput, emailInput, pwdInput);
    // TODO: save data to file!
}

void login() {
    char emailInput[50];
    char pwdInput[50];
    foundIndexByEmail = -1;
    foundIndexByPwd = -1;

    printf("\n===== LOGIN FORM =====\n");

    printf("Enter your email: ");
    scanf(" %[^\n]", emailInput);

    if(!isInputEmail(emailInput)) {
        printf("Invalid Email!\n");
        printf("=================================\n");
        login();
        return;
    }
    
    printf("Enter your password: ");
    scanf(" %[^\n]", pwdInput);

    foundIndexByEmail = getUserIndexByEmail(emailInput);
    
    if(foundIndexByEmail == -1) {
        printf("Invalid Email %d!\n", foundIndexByEmail);
        printf("=================================\n");
        login();
        return;
    }

    foundIndexByPwd = getUserIndexByPwd(pwdInput);

    if(foundIndexByPwd == -1) {
        printf("Wrong Password %d!\n", foundIndexByPwd);
        printf("=================================\n");
        login();
        return;
    }

    if(foundIndexByEmail != foundIndexByPwd) {
        login();
        return;
    }

    printf("email: %d, password: %d\n", foundIndexByEmail, foundIndexByPwd);
    printf("\nWelcome Sir: %s\n\n", workers[foundIndexByEmail].name);
}

int getStrLength(char str[50]) {
    // In C, a String is an array of characters that is terminated with a null character "\0" to indicate the end of the string.
    int count = 0;

    for(int i = 0; i < 50; i++) {
        if(str[i] == '\0') {
            return count;
        }
        count++;
    }

    return count;
}

int checkEmailExists(char inputEmail[50]) {
    int count = 0;

    while(count < WORKER_COUNT) {
        if(checkEmailsEqual(inputEmail, workers[count].email)) {
            return 1;
        }

        count++;
    }

    return 0;
}

int isInputEmail(char input[50]) {
    // @gmail.com = 10
    char MAIL_EXTENSION[10] = "@gmail.com";
    // int inputLength = strlen(input);
    int inputLength = getStrLength(input);

    if(inputLength <= 10) {
        return 0;
    }

    for(int i = (inputLength - 10); i < inputLength; i++) {
        int mailIndex = (i + 10) - inputLength;

        if(input[i] != MAIL_EXTENSION[mailIndex]) {
            return 0;
        }
    }

    return 1;
}

// TODO: check password strength and password length must be at least 8
int isPasswordValid(char inputPwd[50]) {
    // length must be at least 8

    int pwdLength = getStrLength(inputPwd);
    if(pwdLength < 7) {
        return 0;
    }

    return 1;
}

int getUserIndexByEmail(char inputEmail[50]) {
    int userIndex = -1;
    int count = 0;

    while(count < WORKER_COUNT) {
        if(checkEmailsEqual(inputEmail, workers[count].email)) {
            userIndex = count;
            return userIndex;
        }
        count++;
    }

    return userIndex;
}

int getUserIndexByPwd(char inputPwd[50]) {
    int userIndex = -1;
    int count = 0;

    while(userIndex < WORKER_COUNT) {
        if(checkPasswordsEqual(inputPwd, workers[count].password)) {
            userIndex = count;
            return userIndex;
        }

        count++;
    }

        return userIndex;
}

int checkEmailsEqual(char inputEmail[50], char userEmail[50]) {
    int inputEmailLength = getStrLength(inputEmail);
    int userEmailLength = getStrLength(userEmail);

    if(inputEmailLength != userEmailLength) {
        return 0;
    }

    for (int i = 0; i < inputEmailLength; i++)
    {
        if (inputEmail[i] != userEmail[i]) {
            return 0;
        }
    }
    
    return 1;
}

int checkPasswordsEqual(char inputPassword[50], char userPassword[50]) {
    int inputPwdLength = getStrLength(inputPassword);
    int userPwdLength = getStrLength(userPassword);

    if(inputPwdLength != userPwdLength) {
        return 0;
    }

    for (int i = 0; i < inputPwdLength; i++)
    {
        if (inputPassword[i] != userPassword[i]) {
            return 0;
        }
    }
    
    return 1;
}

void lobby() {
    int lobbyIndex = 0;

    printf("\n==== Welcome to Lobby ====\n");
    printf("Press 1 to Register\n");
    printf("Press 2 to Login\n");
    printf("Press 3 to Exit\n");

    do {
        printf("Press number: ");
        scanf(" %d", &lobbyIndex);
    } while (lobbyIndex != 1 && lobbyIndex != 2 && lobbyIndex != 3);

    if(lobbyIndex == 1) {
        signup();
    } else if(lobbyIndex == 2) {
        login();
    } else if (lobbyIndex == 3) {
        // exit
    }


}