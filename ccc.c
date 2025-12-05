#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> 

#define STUDENT_FILE "students.txt"
#define CREDENTIAL_FILE "credentials.txt"

struct Student {
    int roll;
    char name[50];
    float marks;
};

char currentRole[20];
char currentUser[50];

void initializeFiles();
int loginSystem();
void mainMenu();
void adminMenu();
void staffMenu();
void guestMenu();

void addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

void searchUser();

int strCaseCmp(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        if (tolower((unsigned char)*s1) != tolower((unsigned char)*s2)) {
            return (int)tolower((unsigned char)*s1) - (int)tolower((unsigned char)*s2);
        }
        s1++;
        s2++;
    }
    return (int)tolower((unsigned char)*s1) - (int)tolower((unsigned char)*s2);
}

int main() {
    printf("=================Student Management System Login=====================\n");
    initializeFiles(); 

    if (loginSystem()) {
        printf("\nWelcome, %s (%s)!\n", currentUser, currentRole);
        mainMenu();
    } else {
        printf("\nToo many failed attempts! Exiting...\n");
    }

    return 0;
}

void initializeFiles() {
    FILE *fp;
    fp = fopen(CREDENTIAL_FILE, "a");
    if (fp) fclose(fp);
    fp = fopen(STUDENT_FILE, "a");
    if (fp) fclose(fp);
}

int loginSystem() {
    char username[20], password[20];
    char fileUser[20], filePass[20], fileRole[20];
    int attempts = 0;

    while (attempts < 3) {
        printf("\n============== LOGIN ==============\n");
        printf("Attempt %d of 3\n", attempts + 1);
        printf("Username: ");
        scanf("%s", username);

        printf("Password: ");
        scanf("%s", password);

        FILE *fp = fopen(CREDENTIAL_FILE, "r");
        if (!fp) {
            printf("Error: credentials file missing! Please create it.\n");
            return 0;
        }

        int found = 0;
        fseek(fp, 0, SEEK_SET); 

        while (fscanf(fp, "%s %s %s", fileUser, filePass, fileRole) == 3) {
            if (strcmp(username, fileUser) == 0 &&
                strcmp(password, filePass) == 0) {

                strcpy(currentRole, fileRole);
                strcpy(currentUser, fileUser);

                fclose(fp);
                return 1; 
            }
        }

        fclose(fp);
        attempts++;
        printf("\nInvalid Login! Attempts Remaining: %d\n", 3 - attempts);
    }

    return 0; 
}

void mainMenu() {
    if (strcmp(currentRole, "ADMIN") == 0) {
        adminMenu();
    } else if (strcmp(currentRole, "STAFF") == 0) {
        staffMenu();
    } else if (strcmp(currentRole, "GUEST") == 0) { 
        guestMenu();
    } else {
        printf("\nError: Unknown role '%s'. Logging out.\n", currentRole);
    }
}

void adminMenu() {
    int choice;

    do {
        printf("\n================ ADMIN MENU (User: %s) ================\n", currentUser);
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student (By Name)\n"); 
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Search Any User (Admin/Staff/Guest)\n");
        printf("7. Logout\n");
        printf("Enter Choice: ");
        
        if (scanf("%d", &choice) != 1) { 
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n'); 
            continue;
        }

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: searchUser(); break;
            case 7: return;
            default: printf("Invalid Choice!\n");
        }

    } while (1);
}

void staffMenu() {
    int choice;

    do {
        printf("\n================ STAFF MENU (User: %s) ================\n", currentUser);
        printf("1. Display All Students\n");
        printf("2. Search Student (By Name)\n"); 
        printf("3. Search Any User\n"); 
        printf("4. Logout\n");
        printf("Enter Choice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: searchUser(); break;
            case 4: return;
            default: printf("Invalid Choice!\n");
        }

    } while (1);
}

void guestMenu() {
    int choice;

    do {
        printf("\n================ GUEST MENU (User: %s) ================\n", currentUser);
        printf("1. Display All Students\n");
        printf("2. Search Student (By Name)\n"); 
        printf("3. Logout\n");
        printf("Enter Choice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break; 
            case 3: return;
            default: printf("Invalid Choice!\n");
        }

    } while (1);
}

void addStudent() {
    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "a");
    if (!fp) { printf("Error: Could not open student file.\n"); return; }

    printf("\n--- Adding New Student ---\n");
    printf("Enter Roll: ");
    if (scanf("%d", &s.roll) != 1) { printf("Invalid Roll input.\n"); while(getchar() != '\n'); fclose(fp); return; }
    printf("Enter Name: ");
    scanf("%s", s.name);
    printf("Enter Marks: ");
    if (scanf("%f", &s.marks) != 1) { printf("Invalid Marks input.\n"); while(getchar() != '\n'); fclose(fp); return; }

    fprintf(fp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    fclose(fp);
    printf("Student Added Successfully!\n");
}

void displayStudents() {
    struct Student s;
    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) { printf("\nNo student records found.\n"); return; }

    printf("\n--- ALL STUDENT RECORDS ---\n");
    printf("ROLL\tNAME\t\tMARKS\n");
    printf("----------------------------------\n");
    int count = 0;
    while (fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3) {
        printf("%d\t%s\t\t%.2f\n", s.roll, s.name, s.marks);
        count++;
    }
    printf("----------------------------------\n");
    if (count == 0) printf("No records available.\n");

    fclose(fp);
}

void searchStudent() {
    char searchName[50];
    struct Student s;
    int found = 0;

    printf("\n--- Search Student Record by Name ---\n");
    printf("Enter Name to Search: ");
    scanf("%s", searchName); 

    FILE *fp = fopen(STUDENT_FILE, "r");
    if (!fp) { printf("Error: Could not open student file.\n"); return; }

    printf("\nSearching for students with name '%s'...\n", searchName);

    while (fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3) {
        
        if (strcmp(s.name, searchName) == 0) {
                    
            if (!found) {
                printf("----------------------------------------------------\n");
                printf("| Roll No | Name           | Marks |\n");
                printf("----------------------------------------------------\n");
            }

            printf("| %-7d | %-14s | %.2f  |\n", s.roll, s.name, s.marks);
            found = 1;
        }
    }
    
    if (found) {
        printf("----------------------------------------------------\n");
        printf("Search Complete. Found %s.\n", found > 1 ? "multiple records" : "one record");
    } else {
        printf("Record Not Found for Name '%s'!\n", searchName);
    }

    fclose(fp);
}

void updateStudent() {
    int r, found = 0;
    struct Student s;

    printf("\n--- Update Student Record ---\n");
    printf("Enter Roll to Update: ");
    if (scanf("%d", &r) != 1) { 
        printf("Invalid Roll input.\n"); 
        while(getchar() != '\n'); 
        return; 
    }

    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) {
         printf("Error opening files.\n"); 
         if (fp) fclose(fp);
          if (temp) fclose(temp); 
          return;
         }

    while (fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3) {

        if (s.roll == r) {
            found = 1;
            printf("Enter New Name (Current: %s): ", s.name);
            scanf("%s", s.name);
            printf("Enter New Marks (Current: %.2f): ", s.marks);
            if (scanf("%f", &s.marks) != 1) { 
                printf("Invalid Marks input. Aborting update for this record.\n"); 
                while(getchar() != '\n');
             }
        }

        fprintf(temp, "%d %s %.2f\n", s.roll, s.name, s.marks);
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found) printf("Record Updated!\n");
    else printf("Record Not Found!\n");
}

void deleteStudent() {
    int r, found = 0;
    struct Student s;

    printf("\n--- Delete Student Record ---\n");
    printf("Enter Roll to Delete: ");
    if (scanf("%d", &r) != 1) { 
        printf("Invalid Roll input.\n"); 
        while(getchar() != '\n'); 
        return; 
    }

    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (!fp || !temp) { 
        printf("Error opening files.\n"); 
        if (fp) fclose(fp); 
        if (temp) fclose(temp);
         return;
         }

    while (fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3) {
        if (s.roll != r) {
            fprintf(temp, "%d %s %.2f\n", s.roll, s.name, s.marks);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found) printf("Record Deleted!\n");
    else printf("Record Not Found!\n");
}

void searchUser() {
    char searchName[20];
    char fileUser[20], filePass[20], fileRole[20];

    printf("\n--- Search User Credential ---\n");
    printf("Enter username to search: ");
    scanf("%s", searchName);

    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    if (!fp) { 
        printf("Error: Could not open credentials file.\n"); 
        return; 
    }

    int found = 0;
    while (fscanf(fp, "%s %s %s", fileUser, filePass, fileRole) == 3) {

        if (strcmp(searchName, fileUser) == 0) {
            printf("\nUser Found!\n");
            printf("Username: %s\n", fileUser);
            printf("Role: %s\n", fileRole);
            found = 1;
            break;
        }
    }

    fclose(fp);
    if (!found)
        printf("User Not Found!\n");
}