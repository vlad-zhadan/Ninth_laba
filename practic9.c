#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "hash_table.h"

#define ESC 27

FILE *filePointer;

typedef struct state
{
    char name[40];
    int population;
    float square;
} state;

// in other file
int verify_int(int *);
int verify_float(float *);
void clear();
// new functions
int verify_main_menu(int (*function)(char *), char *);
int create_file(char *);
void create_file_path_name(char *, char *);
void input_file_name(char *);
int create_file(char *);
int verify_can_create_file(char *, FILE *);
int verify_can_read_file(char *, FILE *);
int open_file(char *);
int delete_file(char *);
int make_record(char *);
void input_record(state *);
int find_record(char *);
int read_records(char *);
int verify_file_not_empty(char *);
int read_data(char *, state *, int *);
int cmpfunc(const void *, const void *);
void display(char array[][40], int);
int binary_search(int n, char array[n][40], char *);
int find_data(state *, int *, int);
int append_data(char *, int, state *);
int delete_structure(state *, int *, int);
int delete_record(char *);
int write_data(char *, int, state *);
int edit_records(char *);
int arrange_data(state *, int structureNumber, char state_name[structureNumber + 1][40], char array_value[structureNumber + 1][40]);
int arrange_record(char *);

int main()
{
    int choise_file, choise_action, main_menu = 1, entry_menu = 1;
    char name_file_path[230];
    char name_file[30];

    do
    {
        // Main menu
        do
        {
            // clean arrays
            strcpy(name_file_path, "");
            strcpy(name_file, "");

            system("cls");
            printf("\tMAIN MENU\n\n");
            do
            {
                printf("\t1.CREATE FILE\n\t2.OPEN THE FILE\n\t3.DELETE THE FILE\n\n\tANY OTHER NUMBER TO ESCAPE\n");
                //
                printf("\n\tYOUR CHOISE: ");
            } while (verify_int(&choise_file) == 1);
            // user enter the name

            input_file_name(name_file);
            create_file_path_name(name_file_path, name_file);

            switch (choise_file)
            {
            case 1:
                main_menu = verify_main_menu(create_file, name_file_path);
                entry_menu = 1;
                break;
            case 2:
                main_menu = verify_main_menu(open_file, name_file_path);
                entry_menu = 1;
                break;
            case 3:
                delete_file(name_file_path);
                break;
            default:
                exit(0);
                break;
            }

        } while (main_menu == 1);

        // Entry menu
        do
        {
            system("cls");
            printf("\tENTRY MENU\n\n");
            do
            {
                printf("\t1.MAKE THE RECORD\n\t2.DELETE THE RECORD \n\t3.READ THE DATA\n\t4.FIND THE DATA\n\t5.EDIT THE RECORD\n\t6.ARRANGE RECORDS\n\nANY OTHER NUMBER TO BACK TO THE MAIN MENU");
                printf("\n\nYOU WORK WITH %s\n", name_file_path);
                printf("\n\tYOUR CHOISE: ");
            } while (verify_int(&choise_action) == 1);

            system("cls");
            switch (choise_action)
            {
            case 1:
                make_record(name_file_path);
                break;
            case 2:
                // printf("\n\tcChouse by what parameter you want to delete record: ");
                delete_record(name_file_path);
                break;
            case 3:
                read_records(name_file_path);
                break;
            case 4:
                find_record(name_file_path);
                break;
            case 5:
                edit_records(name_file_path);
                break;
            case 6:
                arrange_record(name_file_path);
                break;
            default:
                main_menu = 1;
                entry_menu = 0;
                break;
            }
            getch();
        } while (entry_menu == 1);

    } while (main_menu = 1);

    return 0;
}

int verify_main_menu(int (*function)(char *name_file_path), char *name_file_path)
{
    if ((function)(name_file_path) == 1) // function does not work
    {
        return 1;
    }
    return 0;
}

// need to create a file-----------------------------------
int create_file(char *name_file_path)
{
    // create a file
    if (verify_can_read_file(name_file_path, filePointer) == 1)
    {
        printf("Can't create file. File already exist!\n");
        return 1;
    }

    // fopen() return NULL if last operation was unsuccessful
    if (verify_can_create_file(name_file_path, filePointer) == 1)
    {
        printf("Can't create file\n");
        return 1;
    }

    fclose(filePointer);
    printf("File is created successfully\n");
    return 0;
}

int verify_can_create_file(char *name_file_path, FILE *filePointer)
{
    if ((filePointer = fopen(name_file_path, "wb")) == NULL)
    {
        fclose(filePointer);
        return 1;
    }
    fclose(filePointer);
    return 0;
}
// we want to work only in specific directory
void create_file_path_name(char *name_file_path, char *name_file)
{
    char default_file_path[100];

    strcpy(default_file_path, "");
    strcat(default_file_path, "F:\\KPI\\programing\\C\\practics\\practic9\\"); // create a default file path in case we would want to use other directorys in the future

    strcat(name_file_path, default_file_path);
    strcat(name_file_path, name_file); // now we have full path: directory + name of file
}

void input_file_name(char *name_file)
{
    printf("\nEnter the name of the file: ");
    fgets(name_file, 30, stdin);
    fflush(stdin);
    strtok(name_file, "\n");   // delete \n character in the end
    strcat(name_file, ".bin"); // add our new type of variable pgp
}

// void edit_file_type(){

int verify_can_read_file(char *name_file_path, FILE *filePointer)
{
    // if we cann`t open and read than the file with the same name doesn`t exist
    if ((filePointer = fopen(name_file_path, "rb")) == NULL)
    {
        fclose(filePointer);
        return 0; // can`t read
    }

    fclose(filePointer);
    return 1; // can read
}

int open_file(char *name_file_path)
{
    // verication that we can open/read the file
    if (verify_can_read_file(name_file_path, filePointer) == 0)
    {
        printf("Can't open the file. \n");
        return 1;
    }

    fclose(filePointer);
    printf("File opened!\n");
    return 0;
}

int delete_file(char *name_file_path)
{
    // verication that we can open/read the file
    if (verify_can_read_file(name_file_path, filePointer) == 0)
    {
        printf("Can't delete the file. It doesn't exist!\n");
        return 1;
    }

    if (remove(name_file_path) == 0)
    {
        printf("The file is deleted successfully.");
        return 0;
    }

    printf("The file is not deleted.Try again!\n");
    return 1;
}

int make_record(char *name_file_path)
{
    state to_record[1];

    input_record(to_record);

    if (append_data(name_file_path, 1, to_record) == 1)
    {
        printf("\nError adding the record\n");
        return 1;
    }
    printf("\nThe record is added successfully\n");
    return 0;
}

int append_data(char *name_file_path, int times_to_record, state *data)
{

    // verication that we can open/read the file
    if (verify_can_read_file(name_file_path, filePointer) == 0)
    {
        printf("Can't open the file. \n");
        return 1;
    }
    filePointer = fopen(name_file_path, "ab");

    for (int i = 0; i < times_to_record; i++)
    {
        // fseek(filePointer, 0, SEEK_END); // move to the end
        if ((fwrite(data, sizeof(state), 1, filePointer)) == 0)
        {
            fclose(filePointer);
            return 1;
        }
        data++;
    }
    fclose(filePointer);
    return 0;
}

int write_data(char *name_file_path, int times_to_record, state *data)
{

    // verication that we can open/read the file
    if (verify_can_read_file(name_file_path, filePointer) == 0)
    {
        printf("Can't open the file. \n");
        return 1;
    }
    filePointer = fopen(name_file_path, "wb");

    for (int i = 0; i <= times_to_record; i++)
    {
        fseek(filePointer, 0, SEEK_END); // move to the end
        if ((fwrite(data, sizeof(state), 1, filePointer)) == 0)
        {
            fclose(filePointer);
            return 1;
        }
        data++;
    }
    fclose(filePointer);
    return 0;
}

void input_record(state *record)
{
    printf("\nEnter the name of the state: ");
    fgets((record->name), 40, stdin);
    fflush(stdin);
    strtok(record->name, "\n"); // delete \n character in the end
    do
    {
        printf("Enter the population of the state: ");
    } while ((verify_int(&(record->population)) == 1));
    do
    {
        printf("Enter the square of the state: ");
    } while ((verify_float(&(record->square)) == 1));
}

int delete_record(char *name_file_path)
{
    state to_delete[50];                       // create structure to delete
    int structureNumber = 0;                   // number of element in structure
    int number;                                // position of record in structure that we try to find
    char array_value[structureNumber + 1][40]; // array of values
    char state_name[structureNumber + 1][40];  // create an array of names from structre

    if (read_data(name_file_path, to_delete, &structureNumber) == 1)
    {
        return 1;
    }
    if (find_data(to_delete, &number, structureNumber) == 1)
    {
        return 1;
    }

    delete_structure(to_delete, &structureNumber, number);

    if (write_data(name_file_path, structureNumber, to_delete) == 1)
    {
        printf("Can't record data back to the file. \n");
        return 1;
    }
    printf("Record deleted!\n");
}

int delete_structure(state *data, int *number_of_element, int number_of_structure)
{
    for (int i = number_of_structure; i < *number_of_element; i++)
    {
        strcpy(data[i].name, ""); // clear the i element
        strcpy(data[i].name, data[i + 1].name);
        data[i].population = data[i + 1].population;
        data[i].square = data[i + 1].square;
    }
    *number_of_element -= 1;
}

int find_data(state *data, int *number, int structureNumber)
{
    // take a name of state
    char stateName[40];
    printf("\n\tEnter the name of state: ");
    fgets(stateName, 40, stdin);
    fflush(stdin);
    strtok(stateName, "\n"); // delete \n character in the end

    // create an array of names from structre
    char state_name[structureNumber + 1][40];
    for (int i = 0; i < (structureNumber + 1); i++)
    {
        for (int j = 0; j < 40; j++)
        {
            state_name[i][j] = data[i].name[j];
        }
    }
    // display(state_name, structureNumber + 1);

    // create a hash table
    char c[40];
    HashTable *ht = create_table(CAPACITY);
    for (int i = 0; i < structureNumber + 1; i++)
    {
        strcpy(c, "");
        sprintf(c, "%d", i); // convert int in char
        ht_insert(ht, data[i].name, c);
    }

    // print_table(ht);

    // sort copy array with quick sort
    qsort(state_name, structureNumber + 1, 40 * sizeof(char), cmpfunc);
    // display(state_name, structureNumber + 1);

    // find  element in array = number of structure
    if (binary_search(structureNumber + 1, state_name, stateName) == 1)
    {
        printf("There is no record with such name!");
        return 1;
    }

    // to print text
    printf("\nThere is a record with such name!\n\n");

    char *val = ht_search(ht, stateName);
    // position of record in structure
    *number = atoi(val);
    free_table(ht);
    return 0;
}

int find_record(char *name_file_path) // need to add 1) find record/s by different parameters 3)show all infomation about record to confirm
{
    state to_find[50];       // create array of structure to read
    int structureNumber = 0; // number of element in structure
    int number;              // position of record in structure

    if (verify_can_read_file(name_file_path, filePointer) == 0)
    {
        printf("Can't open the file. \n");
        return 1;
    }

    if (verify_file_not_empty(name_file_path) == 1)
    {
        printf("This file is empty!");
        return 1;
    }

    if (read_data(name_file_path, to_find, &structureNumber) == 1)
    {
        return 1;
    }

    if (find_data(to_find, &number, structureNumber) == 1)
    {
        return 1;
    }

    printf("State: %s\n", to_find[number].name);
    printf("Population: %d\n", to_find[number].population);
    printf("Square: %f\n", to_find[number].square);
}

int cmpfunc(const void *a, const void *b)
{
    return (*(char *)a - *(char *)b);
}

int cmpfunc_int(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

void display(char array[][40], int size_of_array)
{
    for (int i = 0; i < size_of_array; i++)
    {
        printf("%s ", array[i]);
    }
    printf("\n");
}

int binary_search(int n, char array[n][40], char *key)
{
    int low = 0, mid;
    int high = n - 1;
    while (low <= high)
    {
        mid = (low + high) / 2;
        if (strcmp(key, array[mid]) == 0)
        {
            mid += 1;
            return 0;
        }
        else if (strcmp(key, array[mid]) > 0)
        {
            high = high;
            low = mid + 1;
        }
        else
        {
            low = low;
            high = mid - 1;
        }
    }
    return 1;
}

int verify_file_not_empty(char *name_file_path)
{
    filePointer = fopen(name_file_path, "rb"); // open file for read
    fseek(filePointer, 0, SEEK_END);           // move to the end
    if (ftell(filePointer) == 0)               // and if we still on 0 position that means file is empty
    {
        return 1;
    }
    return 0;
}

int read_data(char *name_file_path, state *data, int *structureNumber)
{

    if (verify_can_read_file(name_file_path, filePointer) == 0)
    {
        printf("Can't open the file. \n");
        return 1;
    }

    if (verify_file_not_empty(name_file_path) == 1)
    {
        printf("This file is empty!");
        return 1;
    }

    filePointer = fopen(name_file_path, "rb");
    fseek(filePointer, 0, SEEK_SET); // read from the start

    while (fread(data, sizeof(state), 1, filePointer) != 0)
    {
        data++;
        *structureNumber = *structureNumber + 1;
    }

    *structureNumber = *structureNumber - 1;
    fclose(filePointer);
    return 0;
}

int read_records(char *name_file_path) // need to add read as table or individualy !
{
    state to_read[50]; // create array of structure to read
    int structureNumber = 0;

    if (verify_can_read_file(name_file_path, filePointer) == 0)
    {
        printf("Can't open the file. \n");
        return 1;
    }

    if (verify_file_not_empty(name_file_path) == 1)
    {
        printf("This file is empty!");
        return 1;
    }

    read_data(name_file_path, to_read, &structureNumber);

    for (int i = 0; i < structureNumber + 1; i++)
    {
        printf("\nState:%s\nSquare:%f\nPopulation:%d\n", to_read[i].name, to_read[i].square, to_read[i].population);
    }
}

int edit_records(char *name_file_path)
{
    state to_edit[50];       // create structure to delete
    int structureNumber = 0; // number of element in structure
    int number;              // position of record in structure that we try to find
    int choise;

    if (read_data(name_file_path, to_edit, &structureNumber) == 1)
    {
        return 1;
    }

    if (find_data(to_edit, &number, structureNumber) == 1)
    {
        return 1;
    }

    do
    {
        printf("\nWhat do you want to change?\n1)Name of the state\n2)Population of the state\n3)Square od the state\n\n Your choise:");
    } while (verify_int(&choise) == 1);

    switch (choise)
    {
    case 1:
        strcpy(to_edit[number].name, ""); // clear the i element
        printf("\nEnter the name of the state: ");
        fgets((to_edit[number].name), 40, stdin);
        fflush(stdin);
        strtok(to_edit[number].name, "\n"); // delete \n character in the end
        break;
    case 2:
        do
        {
            printf("Enter the population of the state: ");
        } while ((verify_int(&(to_edit[number].population)) == 1));
        break;
    case 3:
        do
        {
            printf("Enter the square of the state: ");
        } while ((verify_float(&(to_edit[number].square)) == 1));
        break;
    default:
        break;
    }

    if (write_data(name_file_path, structureNumber, to_edit) == 1)
    {
        printf("Can't record data back to the file. \n");
        return 1;
    }
    printf("Record edited!\n");
}

int arrange_record(char *name_file_path) // arrange for defernt category
{
    state to_arrange[50];    // create structure to delete
    int structureNumber = 0; // number of element in structure
    int number;              // position of record in structure that we try to find
    int choise = 0;
    char int_char[40];

    if (read_data(name_file_path, to_arrange, &structureNumber) == 1)
    {
        return 1;
    }

    do
    {
        printf("Enter the parameter by which you want to sort:\n1)Name of the state \n2)Population of the state\nYOUR CHOISE: ");
    } while ((verify_int(&choise) == 1));

    structureNumber++;
    char state_info[structureNumber][40];
    int state_info_int[structureNumber];
    switch (choise)
    {
    case 1: // by name
        // create an array of names from structre
        for (int i = 0; i < (structureNumber); i++)
        {
            for (int j = 0; j < 40; j++)
            {
                state_info[i][j] = to_arrange[i].name[j];
            }
        }

        break;
    case 2: // by population
        for (int i = 0; i < structureNumber; i++)
        {
            strcpy(int_char, "");
            sprintf(int_char, "%d", to_arrange[i].population);
            strcpy(state_info[i], int_char);
            state_info_int[i] = to_arrange[i].population;
        }

    default:
        break;
    }

    // create a hash table
    char c[40];
    HashTable *ht = create_table(CAPACITY);
    for (int i = 0; i < structureNumber; i++)
    {
        strcpy(c, "");
        sprintf(c, "%d", i); // convert int in char
        if (choise == 1)
        {
            ht_insert(ht, to_arrange[i].name, c);
        }
        else if (choise == 2)
        {
            strcpy(int_char, "");
            sprintf(int_char, "%d", to_arrange[i].population);
            ht_insert(ht, int_char, c);
        }
    }

    // sort copy array with quick sort
    if (choise == 1)
    {
        qsort(state_info, structureNumber, 40 * sizeof(char), cmpfunc);
    }
    else if (choise == 2)
    {
        qsort(state_info_int, structureNumber, sizeof(int), cmpfunc_int);
        for (int loop = 0; loop < structureNumber; loop++)
            printf("%d ", state_info_int[loop]);
    }

    // print_table(ht);
    // display(state_info_int, structureNumber);

    for (int i = 0; i < structureNumber; i++)
    {
        char *val;
        if (choise == 1)
        {
            val = ht_search(ht, state_info[i]);
        }
        else if (choise == 2)
        {
            char d[40];
            char *token = NULL;
            strcpy(d, "");
            sprintf(d, "%d", state_info_int[i]);
            val = ht_search(ht, d);
        }
        number = atoi(val);

        printf("\nState:%s\nSquare:%f\nPopulation:%d\n", to_arrange[number].name, to_arrange[number].square, to_arrange[number].population);
    }

    free_table(ht);
}

//-----------

int verify_int(int *variable_int)
{
    char ch;
    scanf("%d%c", variable_int, &ch);
    if (ch == '\n')
    {
        return 0;
    }
    printf("Invalid number. Try again!\n");
    clear();
    return 1;
}

int verify_float(float *variable_float)
{
    char ch;
    scanf("%f%c", variable_float, &ch);
    if (ch == '\n')
    {
        return 0;
    }
    printf("Invalid number. Try again!\n");
    clear();
    return 1;
}

void clear()
{
    int character;
    while ((character = getchar()) != '\n' && character != EOF) // end of file
        ;
}
