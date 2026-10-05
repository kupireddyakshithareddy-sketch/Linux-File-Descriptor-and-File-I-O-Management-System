#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>

#include "../include/file_management.h"

#define DATA_DIRECTORY "../data"
#define MAX_FILENAME 512

#define LOG_FILE "../logs/operations.log"

void log_operation(const char *operation, const char *details)
{
    FILE *log_file;
    time_t current_time;
    struct tm *time_info;
    char timestamp[64];

    log_file = fopen(LOG_FILE, "a");

    if (log_file == NULL)
    {
        return;
    }

    current_time = time(NULL);
    time_info = localtime(&current_time);

    if (time_info == NULL)
    {
        fclose(log_file);
        return;
    }

    strftime(timestamp,
             sizeof(timestamp),
             "%Y-%m-%d %H:%M:%S",
             time_info);

    fprintf(log_file,
            "[%s] %-20s %s\n",
            timestamp,
            operation,
            details);

    fclose(log_file);
}
void list_saved_files_management_menu(void)
{
    DIR *directory;
    struct dirent *entry;
    struct stat file_stat;
    char filepath[MAX_FILENAME];
    int count = 0;

    printf("\n");
    printf("===============================================================\n");
    printf("                       SAVED FILES\n");
    printf("===============================================================\n");

    directory = opendir(DATA_DIRECTORY);

    if (directory == NULL)
    {
        printf("\nERROR: Unable to open data directory.\n");
        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    printf("\n");

    while ((entry = readdir(directory)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        snprintf(filepath,
                 sizeof(filepath),
                 "%s/%s",
                 DATA_DIRECTORY,
                 entry->d_name);

        if (stat(filepath, &file_stat) == -1)
        {
            continue;
        }

        if (S_ISREG(file_stat.st_mode))
        {
            count++;

            printf("%d. %s\n", count, entry->d_name);
        }
    }

    closedir(directory);

    if (count == 0)
    {
        printf("No saved files found.\n");
    }
    else
    {
        printf("\nTotal Saved Files : %d\n", count);
    }

    printf("\nPress ENTER to continue...");
    getchar();
}
void rename_file_menu(void)
{
    DIR *directory;
    struct dirent *entry;
    struct stat file_stat;

    char files[100][256];
    char old_path[512];
    char new_path[512];
    char new_name[256];

    int count = 0;
    int choice;

    printf("\n");
    printf("===============================================================\n");
    printf("                         RENAME FILE\n");
    printf("===============================================================\n");

    directory = opendir(DATA_DIRECTORY);

    if (directory == NULL)
    {
        printf("\nERROR: Unable to open data directory.\n");
        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    while ((entry = readdir(directory)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        snprintf(old_path,
                 sizeof(old_path),
                 "%s/%s",
                 DATA_DIRECTORY,
                 entry->d_name);

        if (stat(old_path, &file_stat) == -1)
        {
            continue;
        }

        if (S_ISREG(file_stat.st_mode))
        {
            if (count < 100)
            {
                strncpy(files[count],
                        entry->d_name,
                        sizeof(files[count]) - 1);

                files[count][sizeof(files[count]) - 1] = '\0';

                count++;
            }
        }
    }

    closedir(directory);

    if (count == 0)
    {
        printf("\nNo saved files available to rename.\n");
        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    printf("\nAvailable Saved Files:\n");

    for (int i = 0; i < count; i++)
    {
        printf("%d. %s\n", i + 1, files[i]);
    }

    printf("\nSelect file: ");

    if (scanf("%d", &choice) != 1)
    {
        while (getchar() != '\n')
            ;

        printf("\nInvalid selection.\n");
        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    while (getchar() != '\n')
        ;

    if (choice < 1 || choice > count)
    {
        printf("\nInvalid file selection.\n");
        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    printf("\nSelected File: %s\n", files[choice - 1]);

    printf("\nEnter new file name: ");

    if (fgets(new_name, sizeof(new_name), stdin) == NULL)
    {
        printf("\nUnable to read new file name.\n");
        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    new_name[strcspn(new_name, "\n")] = '\0';

    if (strlen(new_name) == 0)
    {
        printf("\nNew file name cannot be empty.\n");
        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    snprintf(old_path,
             sizeof(old_path),
             "%s/%s",
             DATA_DIRECTORY,
             files[choice - 1]);

    snprintf(new_path,
             sizeof(new_path),
             "%s/%s",
             DATA_DIRECTORY,
             new_name);

    if (rename(old_path, new_path) == -1)
    {
        perror("\nrename() failed");
        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    printf("\n===============================================================\n");
    printf("                    RENAME SUCCESSFUL\n");
    printf("===============================================================\n");

    printf("\nOld Name : %s\n", files[choice - 1]);
    printf("New Name : %s\n", new_name);
    printf("Operation: rename()\n");
    printf("Status   : SUCCESS\n");

    printf("\nPress ENTER to continue...");
    getchar();
}
void delete_file_menu(void)
{
    DIR *directory;
    struct dirent *entry;
    struct stat file_stat;

    char files[100][256];
    char filepath[512];

    int count = 0;
    int choice;
    int confirm;

    printf("\n");
    printf("===============================================================\n");
    printf("                         DELETE FILE\n");
    printf("===============================================================\n");

    directory = opendir(DATA_DIRECTORY);

    if (directory == NULL)
    {
        printf("\nERROR: Unable to open data directory.\n");
        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    while ((entry = readdir(directory)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        snprintf(filepath,
                 sizeof(filepath),
                 "%s/%s",
                 DATA_DIRECTORY,
                 entry->d_name);

        if (stat(filepath, &file_stat) == -1)
        {
            continue;
        }

        if (S_ISREG(file_stat.st_mode))
        {
            if (count < 100)
            {
                strncpy(files[count],
                        entry->d_name,
                        sizeof(files[count]) - 1);

                files[count][sizeof(files[count]) - 1] = '\0';

                count++;
            }
        }
    }

    closedir(directory);

    if (count == 0)
    {
        printf("\nNo saved files available to delete.\n");
        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    printf("\nAvailable Saved Files:\n");

    for (int i = 0; i < count; i++)
    {
        printf("%d. %s\n", i + 1, files[i]);
    }

    printf("\nSelect file: ");

    if (scanf("%d", &choice) != 1)
    {
        while (getchar() != '\n')
            ;

        printf("\nInvalid selection.\n");
        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    while (getchar() != '\n')
        ;

    if (choice < 1 || choice > count)
    {
        printf("\nInvalid file selection.\n");
        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    printf("\nSelected File: %s\n", files[choice - 1]);

    printf("\nWARNING! This file will be permanently deleted.\n");
    printf("\n1. Yes, Delete\n");
    printf("2. No, Cancel\n");

    printf("\nEnter your choice: ");

    if (scanf("%d", &confirm) != 1)
    {
        while (getchar() != '\n')
            ;

        printf("\nInvalid choice. Delete cancelled.\n");
        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    while (getchar() != '\n')
        ;

    if (confirm != 1)
    {
        printf("\nDelete operation cancelled.\n");
        printf("File remains safe.\n");

        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    snprintf(filepath,
             sizeof(filepath),
             "%s/%s",
             DATA_DIRECTORY,
             files[choice - 1]);

    if (unlink(filepath) == -1)
    {
        perror("\nunlink() failed");
        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    printf("\n===============================================================\n");
    printf("                     DELETE SUCCESSFUL\n");
    printf("===============================================================\n");

    printf("\nDeleted File : %s\n", files[choice - 1]);
    printf("Operation    : unlink()\n");
    printf("Status       : SUCCESS\n");

    printf("\nPress ENTER to continue...");
    getchar();
}
void create_directory_menu(void)
{
    char directory_name[256];
    char directory_path[512];

    printf("\n");
    printf("===============================================================\n");
    printf("                       CREATE DIRECTORY\n");
    printf("===============================================================\n");

    printf("\nEnter directory name: ");

    while (getchar() != '\n')
        ;

    if (fgets(directory_name,
              sizeof(directory_name),
              stdin) == NULL)
    {
        printf("\nUnable to read directory name.\n");
        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    directory_name[strcspn(directory_name, "\n")] = '\0';

    if (strlen(directory_name) == 0)
    {
        printf("\nDirectory name cannot be empty.\n");
        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    snprintf(directory_path,
             sizeof(directory_path),
             "%s/%s",
             DATA_DIRECTORY,
             directory_name);

    if (mkdir(directory_path, 0755) == -1)
    {
        if (errno == EEXIST)
        {
            printf("\nDirectory already exists.\n");
        }
        else
        {
            perror("\nmkdir() failed");
        }

        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    printf("\n===============================================================\n");
    printf("                  DIRECTORY CREATED SUCCESSFULLY\n");
    printf("===============================================================\n");

    printf("\nDirectory Name : %s\n", directory_name);
    printf("Directory Path : %s\n", directory_path);
    printf("Permissions    : 0755\n");
    printf("Operation      : mkdir()\n");
    printf("Status         : SUCCESS\n");

    printf("\nPress ENTER to continue...");
    getchar();
}
void change_file_permissions_menu(void)
{
    DIR *directory;
    struct dirent *entry;
    struct stat file_stat;

    char files[100][256];
    char filepath[512];
    char permission_input[20];

    int count = 0;
    int choice;
    int permission_value;
    char *endptr;

    printf("\n");
    printf("===============================================================\n");
    printf("                  CHANGE FILE PERMISSIONS\n");
    printf("===============================================================\n");

    directory = opendir(DATA_DIRECTORY);

    if (directory == NULL)
    {
        printf("\nERROR: Unable to open data directory.\n");
        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    while ((entry = readdir(directory)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        snprintf(filepath,
                 sizeof(filepath),
                 "%s/%s",
                 DATA_DIRECTORY,
                 entry->d_name);

        if (stat(filepath, &file_stat) == -1)
        {
            continue;
        }

        if (S_ISREG(file_stat.st_mode))
        {
            if (count < 100)
            {
                strncpy(files[count],
                        entry->d_name,
                        sizeof(files[count]) - 1);

                files[count][sizeof(files[count]) - 1] = '\0';

                count++;
            }
        }
    }

    closedir(directory);

    if (count == 0)
    {
        printf("\nNo saved files available.\n");
        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    printf("\nAvailable Saved Files:\n");

    for (int i = 0; i < count; i++)
    {
        printf("%d. %s\n", i + 1, files[i]);
    }

    printf("\nSelect file: ");

    if (scanf("%d", &choice) != 1)
    {
        while (getchar() != '\n')
            ;

        printf("\nInvalid file selection.\n");
        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    while (getchar() != '\n')
        ;

    if (choice < 1 || choice > count)
    {
        printf("\nInvalid file selection.\n");
        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    snprintf(filepath,
             sizeof(filepath),
             "%s/%s",
             DATA_DIRECTORY,
             files[choice - 1]);

    printf("\nSelected File : %s\n", files[choice - 1]);

    printf("\nCurrent Permissions:\n");

    if (stat(filepath, &file_stat) == -1)
    {
        perror("\nstat() failed");
        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    printf("Current Mode  : %04o\n",
           file_stat.st_mode & 0777);

    printf("\nEnter new permissions in octal format.\n");
    printf("Examples: 644, 600, 755\n");
    printf("Enter permissions: ");

    if (fgets(permission_input,
              sizeof(permission_input),
              stdin) == NULL)
    {
        printf("\nUnable to read permissions.\n");
        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    permission_input[strcspn(permission_input, "\n")] = '\0';

    endptr = NULL;

    permission_value =
        (int)strtol(permission_input, &endptr, 8);

    if (endptr == permission_input ||
        *endptr != '\0' ||
        permission_value < 0 ||
        permission_value > 0777)
    {
        printf("\nInvalid permission value.\n");
        printf("Please use an octal value such as 644 or 755.\n");

        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    if (chmod(filepath, (mode_t)permission_value) == -1)
    {
        perror("\nchmod() failed");
        printf("\nPress ENTER to continue...");
        getchar();
        return;
    }

    printf("\n===============================================================\n");
    printf("                PERMISSIONS UPDATED SUCCESSFULLY\n");
    printf("===============================================================\n");

    printf("\nFile              : %s\n", files[choice - 1]);
    printf("Old Permissions   : %04o\n",
           file_stat.st_mode & 0777);
    printf("New Permissions   : %04o\n",
           permission_value);
    printf("Operation         : chmod()\n");
    printf("Status            : SUCCESS\n");

    printf("\nPress ENTER to continue...");
    getchar();
}
void view_operation_history_menu(void)
{
    FILE *log_file;
    char line[512];
    int count = 0;
    int ch;

    printf("\n");
    printf("===============================================================\n");
    printf("                    OPERATION HISTORY\n");
    printf("===============================================================\n");

    /*
       Record that the history was viewed.
    */
    log_operation("VIEW_HISTORY",
                  "Operation history viewed");

    log_file = fopen(LOG_FILE, "r");

    if (log_file == NULL)
    {
        printf("\nNo operation history available yet.\n");
        printf("\nHistory File : %s\n", LOG_FILE);

        printf("\nPress ENTER to continue...");
        fflush(stdout);

        while ((ch = getchar()) != '\n' && ch != EOF)
        {
            /* Clear input */
        }

        getchar();
        return;
    }

    printf("\nHistory File : %s\n", LOG_FILE);
    printf("---------------------------------------------------------------\n");

    while (fgets(line, sizeof(line), log_file) != NULL)
    {
        printf("%s", line);
        count++;
    }

    fclose(log_file);

    printf("---------------------------------------------------------------\n");

    if (count == 0)
    {
        printf("\nNo operations have been recorded yet.\n");
    }
    else
    {
        printf("\nTotal History Entries : %d\n", count);
    }

    printf("\nPress ENTER to continue...");
    fflush(stdout);

    while ((ch = getchar()) != '\n' && ch != EOF)
    {
        /* Clear input */
    }

    getchar();
}
