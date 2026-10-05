#include <stdio.h>
#include <stdlib.h>

#include "../include/file_operations.h"
#include "../include/fd_manager.h"
#include "../include/file_info.h"
#include "../include/file_management.h"
#include "../include/advanced_io.h"
#include "../include/system_info.h"
static void pause_screen(void)
{
    int ch;

    printf("\nPress ENTER to continue...");
    fflush(stdout);

    while ((ch = getchar()) != '\n' && ch != EOF)
    {
        /* Clear input */
    }

    getchar();
}
/* ============================================================
   FILE MANAGER SUBMENU
   ============================================================ */

void file_manager_menu(void)
{
    int choice;

    while (1)
    {
        printf("\n");
        printf("===============================================================\n");
        printf("                         FILE MANAGER\n");
        printf("===============================================================\n");

        printf("\n");
        printf("1. Create New File\n");
        printf("2. Open Existing Saved File\n");
        printf("3. Create If File Does Not Exist\n");
        printf("4. List Saved Files\n");
        printf("5. Select Active File\n");
        printf("6. Back\n");

        printf("\n===============================================================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n')
                ;

            printf("\nInvalid input!");
            printf("\nPress ENTER to continue...");
            getchar();
            continue;
        }

        switch (choice)
        {
            case 1:
                create_new_file_menu();
                break;

            case 2:
                open_saved_file_menu();
                break;

            case 3:
                create_if_not_exists_menu();
                break;

            case 4:
                list_saved_files_menu();
                break;

            case 5:
                select_active_file_menu();
                break;

            case 6:
                return;

            default:
                printf("\nInvalid choice! Please select 1 to 6.");
                printf("\nPress ENTER to continue...");
                getchar();
                getchar();
                break;
        }
    }
}

void fd_management_menu(void)
{
    int choice;

    while (1)
    {
        printf("\n");
        printf("===============================================================\n");
        printf("                 FILE DESCRIPTOR MANAGEMENT\n");
        printf("===============================================================\n");

        printf("\n");
        printf("1. Display Open File Descriptors\n");
        printf("2. Duplicate File Descriptor (dup)\n");
        printf("3. Duplicate to Specific FD (dup2)\n");
        printf("4. Close File Descriptor\n");
        printf("5. Close All File Descriptors\n");
        printf("6. Select Active FD\n");
        printf("7. Back\n");

        printf("\n===============================================================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n')
                ;

            printf("\nInvalid input!");
            printf("\nPress ENTER to continue...");
            getchar();
            continue;
        }

        switch (choice)
        {
            case 1:
                display_fd_table();

                printf("\nPress ENTER to continue...");
                getchar();
                getchar();
                break;

            case 2:
                duplicate_fd_menu();
                break;

            case 3:
                duplicate_fd2_menu();
                break;

            case 4:
                close_fd_menu();
                break;

            case 5:
                close_all_fds();
                break;

            case 6:
                set_active_fd_menu();
                break;

            case 7:
                return;

            default:
                printf("\nInvalid choice! Please select 1 to 7.");
                printf("\nPress ENTER to continue...");
                getchar();
                getchar();
                break;
        }
    }
}
/* ============================================================
   FILE I/O OPERATIONS SUBMENU
   ============================================================ */
void file_information_menu(void)
{
    int choice;

    while (1)
    {
        printf("\n");
        printf("===============================================================\n");
        printf("                     FILE INFORMATION\n");
        printf("===============================================================\n");

        printf("\n");
        printf("1. Display Complete File Information\n");
        printf("2. Display File Permissions\n");
        printf("3. Display File Size\n");
        printf("4. Display Current File Position\n");
        printf("5. Display File Type\n");
        printf("6. Back\n");

        printf("\n===============================================================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n')
                ;

            printf("\nInvalid input!");
            printf("\nPress ENTER to continue...");
            getchar();
            continue;
        }

        switch (choice)
        {
            case 1:
                display_complete_file_info_menu();
                break;

            case 2:
                display_file_permissions_menu();
                break;

            case 3:
                display_file_size_menu();
                break;

            case 4:
                display_current_position_menu();
                break;

            case 5:
                display_file_type_menu();
                break;
            case 6:
                return;

            default:
                printf("\nInvalid choice! Please select 1 to 6.");
                printf("\nPress ENTER to continue...");
                getchar();
                getchar();
                break;
        }
    }
}
void file_io_menu(void)
{
    int choice;

    while (1)
    {
        printf("\n");
        printf("===============================================================\n");
        printf("                    FILE I/O OPERATIONS\n");
        printf("===============================================================\n");

        printf("\n");
        printf("1. Read File\n");
        printf("2. Write File\n");
        printf("3. Append File\n");
        printf("4. Seek File Position\n");
        printf("5. Read From Specific Position\n");
        printf("6. Write At Specific Position\n");
        printf("7. Back\n");

        printf("\n===============================================================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n')
                ;

            printf("\nInvalid input! Please enter a number.");
            printf("\nPress ENTER to continue...");
            getchar();
            continue;
        }

        switch (choice)
        {

            case 1:
                read_file_menu();
                break;

            case 2:
                write_file_menu();
                break;

            case 3:
                append_file_menu();
                break;

            case 4:
                seek_file_menu();
                break;

            case 5:
                read_specific_position_menu();
                break;

            case 6:
                write_specific_position_menu();
                break;

            case 7:
                return;

            default:
                printf("\nInvalid choice! Please select 1 to 7.");
                printf("\nPress ENTER to continue...");
                getchar();
                getchar();
                break;
        }
    }
}

void file_management_menu(void)
{
    int choice;

    while (1)
    {
        printf("\n");
        printf("===============================================================\n");
        printf("                     FILE MANAGEMENT\n");
        printf("===============================================================\n");

        printf("\n");
        printf("1. List Saved Files\n");
        printf("2. Rename File\n");
        printf("3. Delete File\n");
        printf("4. Create Directory\n");
        printf("5. Change File Permissions\n");
        printf("6. View Operation History\n");
        printf("7. Back\n");

        printf("\n===============================================================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n')
                ;

            printf("\nInvalid input!");
            printf("\nPress ENTER to continue...");
            getchar();
            continue;
        }

        switch (choice)
        {
            case 1:
                list_saved_files_management_menu();
                break;

            case 2:
                rename_file_menu();
                break;

            case 3:
                delete_file_menu();
                break;

            case 4:
                create_directory_menu();
                break;

            case 5:
                change_file_permissions_menu();
                break;

            case 6:
                view_operation_history_menu();
                break;

            case 7:
                return;

            default:
                printf("\nInvalid choice! Please select 1 to 7.");
                printf("\nPress ENTER to continue...");
                getchar();
                getchar();
                break;
        }
    }
}
/* ============================================================
   MAIN MENU
   ============================================================ */

int main(void)
{
    int choice;

    while (1)
    {
        printf("\n");
        printf("===============================================================\n");
        printf("        LINUX FILE DESCRIPTOR & FILE I/O MANAGEMENT SYSTEM\n");
        printf("===============================================================\n");

        printf("\n");
        printf("1. File Manager\n");
        printf("2. File I/O Operations\n");
        printf("3. File Descriptor Management\n");
        printf("4. File Information\n");
        printf("5. File Management\n");
        printf("6. Advanced I/O\n");
        printf("7. System Information\n");
        printf("8. Exit\n");

        printf("\n===============================================================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n')
                ;

            printf("\nInvalid input! Please enter a number.");
            printf("\nPress ENTER to continue...");
            getchar();
            continue;
        }

        switch (choice)
        {
            case 1:
                file_manager_menu();
                break;

            case 2:
                file_io_menu();
                break;

            case 3:
                fd_management_menu();
                break;

            case 4:
                file_information_menu();
                break;

            case 5:
                file_management_menu();
                break;

            case 6:
                advanced_io_menu();
                break;

            case 7:
                system_info_menu();
                break;

            case 8:
                printf("\n");
                printf("===============================================================\n");
                printf("Exiting Linux File Descriptor & File I/O Management System...\n");
                printf("===============================================================\n");
                return 0;

            default:
                printf("\nInvalid choice! Please select 1 to 8.");
                printf("\nPress ENTER to continue...");
                getchar();
                getchar();
                break;
        }
    }

    return 0;
}
void advanced_io_menu(void)
{
    int choice;

    while (1)
    {
        printf("\n");
        printf("===============================================================\n");
        printf("                         ADVANCED I/O\n");
        printf("===============================================================\n");

        printf("\n");
        printf("1. Display FD Flags\n");
        printf("2. File Locking\n");
        printf("3. Synchronize File\n");
        printf("4. Truncate File\n");
        printf("5. Back\n");

        printf("\n===============================================================\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n')
                ;

            printf("\nInvalid input.");
            pause_screen();
            continue;
        }

        switch (choice)
        {
            case 1:
                display_fd_flags_menu();
                break;

            case 2:
                file_locking_menu();
                break;

            case 3:
                synchronize_file_menu();
                break;

            case 4:
                truncate_file_menu();
                break;

            case 5:
                return;

            default:
                printf("\nInvalid choice! Please select 1 to 5.");
                pause_screen();
                break;
        }
    }
}
