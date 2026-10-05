#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

#include "../include/file_operations.h"
#include "../include/fd_manager.h"

#define DATA_DIRECTORY "../data"
#define MAX_FILES 100
#define MAX_FILENAME 256

static void pause_screen(void)
{
    int ch;

    printf("\n\nPress ENTER to continue...");

    fflush(stdout);

    while ((ch = getchar()) != '\n' && ch != EOF)
    {
        /* Clear remaining input */
    }

    getchar();
}
static void clear_input_buffer(void)
{
    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
}

static int get_saved_files(char files[MAX_FILES][MAX_FILENAME])
{
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    char path[512];
    int count = 0;

    dir = opendir(DATA_DIRECTORY);

    if (dir == NULL)
    {
        perror("Unable to open data directory");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        snprintf(path, sizeof(path),
                 "%s/%s",
                 DATA_DIRECTORY,
                 entry->d_name);

        if (stat(path, &file_stat) == -1)
        {
            continue;
        }

        if (!S_ISREG(file_stat.st_mode))
        {
            continue;
        }

        if (count >= MAX_FILES)
        {
            break;
        }

        strncpy(files[count],
                entry->d_name,
                MAX_FILENAME - 1);

        files[count][MAX_FILENAME - 1] = '\0';

        count++;
    }

    closedir(dir);

    return count;
}

void list_saved_files_menu(void)
{
    char files[MAX_FILES][MAX_FILENAME];
    int count;
    int i;

    printf("\n");
    printf("===============================================================\n");
    printf("                       SAVED FILES\n");
    printf("===============================================================\n");

    count = get_saved_files(files);

    if (count == -1)
    {
        pause_screen();
        return;
    }

    if (count == 0)
    {
        printf("\nNo saved files found in the data directory.\n");
        pause_screen();
        return;
    }

    printf("\nAvailable Saved Files:\n\n");

    for (i = 0; i < count; i++)
    {
        printf("%d. %s\n", i + 1, files[i]);
    }

    printf("\nTotal Saved Files : %d\n", count);
    pause_screen();
}

void create_new_file_menu(void)
{
    char filename[MAX_FILENAME];
    char filepath[512];
    int fd;

    printf("\n");
    printf("===============================================================\n");
    printf("                       CREATE NEW FILE\n");
    printf("===============================================================\n");

    printf("\nEnter new file name: ");

    clear_input_buffer();

    if (fgets(filename, sizeof(filename), stdin) == NULL)
    {
        printf("\nInvalid input.\n");
        pause_screen();
        return;
    }

    filename[strcspn(filename, "\n")] = '\0';

    if (strlen(filename) == 0)
    {
        printf("\nFile name cannot be empty.\n");
        pause_screen();
        return;
    }

    snprintf(filepath,
             sizeof(filepath),
             "%s/%s",
             DATA_DIRECTORY,
             filename);

    fd = open(filepath,
              O_RDWR | O_CREAT | O_EXCL,
              0644);

    if (fd == -1)
    {
        if (errno == EEXIST)
        {
            printf("\nFile already exists!\n");
        }
        else
        {
            perror("\nUnable to create file");
        }

        pause_screen();
        return;
    }

    register_file_descriptor(fd, filepath);
    set_active_fd(fd);

    printf("\nFile created successfully!\n");
    printf("\nFile Name       : %s\n", filename);
    printf("File Path       : %s\n", filepath);
    printf("File Descriptor : %d\n", fd);
    printf("Current Position: 0\n");
    printf("Status          : ACTIVE\n");

    pause_screen();
}

void open_saved_file_menu(void)
{
    char files[MAX_FILES][MAX_FILENAME];
    char filepath[512];

    int count;
    int choice;
    int fd;

    printf("\n");
    printf("===============================================================\n");
    printf("                    OPEN SAVED FILE\n");
    printf("===============================================================\n");

    count = get_saved_files(files);

    if (count == -1)
    {
        pause_screen();
        return;
    }

    if (count == 0)
    {
        printf("\nNo saved files found.\n");
        printf("Create a file first using File Manager.\n");

        pause_screen();
        return;
    }

    printf("\nAvailable Saved Files:\n\n");

    for (int i = 0; i < count; i++)
    {
        printf("%d. %s\n", i + 1, files[i]);
    }

    printf("\n0. Back\n");

    printf("\nSelect file: ");

    if (scanf("%d", &choice) != 1)
    {
        clear_input_buffer();

        printf("\nInvalid choice.\n");
        pause_screen();
        return;
    }

    if (choice == 0)
    {
        clear_input_buffer();
        return;
    }

    if (choice < 1 || choice > count)
    {
        clear_input_buffer();

        printf("\nInvalid file selection.\n");
        pause_screen();
        return;
    }

    clear_input_buffer();

    snprintf(filepath,
             sizeof(filepath),
             "%s/%s",
             DATA_DIRECTORY,
             files[choice - 1]);

    fd = open(filepath, O_RDWR);

    if (fd == -1)
    {
        perror("\nUnable to open file");
        pause_screen();
        return;
    }

    register_file_descriptor(fd, filepath);
    set_active_fd(fd);

    printf("\nFile opened successfully!\n");
    printf("\nSelected File   : %s\n", files[choice - 1]);
    printf("File Path       : %s\n", filepath);
    printf("File Descriptor : %d\n", fd);
    printf("Current Position: 0\n");
    printf("Status          : ACTIVE\n");

    pause_screen();
}

void create_if_not_exists_menu(void)
{
    char filename[MAX_FILENAME];
    char filepath[512];
    int fd;

    printf("\n");
    printf("===============================================================\n");
    printf("                 CREATE IF FILE DOES NOT EXIST\n");
    printf("===============================================================\n");

    printf("\nEnter file name: ");

    clear_input_buffer();

    if (fgets(filename, sizeof(filename), stdin) == NULL)
    {
        printf("\nInvalid input.\n");
        pause_screen();
        return;
    }

    filename[strcspn(filename, "\n")] = '\0';

    if (strlen(filename) == 0)
    {
        printf("\nFile name cannot be empty.\n");
        pause_screen();
        return;
    }

    snprintf(filepath,
             sizeof(filepath),
             "%s/%s",
             DATA_DIRECTORY,
             filename);

    fd = open(filepath,
              O_RDWR | O_CREAT,
              0644);

    if (fd == -1)
    {
        perror("\nUnable to create/open file");
        pause_screen();
        return;
    }

    register_file_descriptor(fd, filepath);
    set_active_fd(fd);

    printf("\nOperation completed successfully!\n");
    printf("\nFile Name       : %s\n", filename);
    printf("File Path       : %s\n", filepath);
    printf("File Descriptor : %d\n", fd);
    printf("Status          : OPEN AND ACTIVE\n");

    pause_screen();
}

void select_active_file_menu(void)
{
    int fd;

    printf("\n");
    printf("===============================================================\n");
    printf("                     SELECT ACTIVE FILE\n");
    printf("===============================================================\n");

    display_fd_table();

    printf("\nEnter FD to make active: ");

    if (scanf("%d", &fd) != 1)
    {
        clear_input_buffer();

        printf("\nInvalid input.\n");
        pause_screen();
        return;
    }

    clear_input_buffer();

    if (!is_fd_registered(fd))
    {
        printf("\nFD %d is not registered.\n", fd);
        pause_screen();
        return;
    }

    set_active_fd(fd);

    printf("\nActive file selected successfully!\n");
    printf("Active FD   : %d\n", fd);
    printf("Active File : %s\n", get_fd_filename(fd));

    pause_screen();
}
void write_file_menu(void)
{
    int fd;
    char data[1024];
    ssize_t bytes_written;

    printf("\n");
    printf("===============================================================\n");
    printf("                         WRITE FILE\n");
    printf("===============================================================\n");

    display_fd_table();

    if (get_active_fd() == -1)
    {
        printf("\nNo active file is selected.\n");
        printf("Please open or select a file first.\n");

        pause_screen();
        return;
    }

    printf("\nActive File Information\n");
    printf("File Descriptor : %d\n", get_active_fd());
    printf("File            : %s\n",
           get_fd_filename(get_active_fd()));

    printf("\nEnter FD to write to: ");

    if (scanf("%d", &fd) != 1)
    {
        clear_input_buffer();

        printf("\nInvalid FD input.\n");
        pause_screen();
        return;
    }

    clear_input_buffer();

    if (!is_fd_registered(fd))
    {
        printf("\nFD %d is not registered.\n", fd);
        pause_screen();
        return;
    }

    printf("\nEnter data to write:\n");
    printf("> ");

    if (fgets(data, sizeof(data), stdin) == NULL)
    {
        printf("\nUnable to read input.\n");
        pause_screen();
        return;
    }

    data[strcspn(data, "\n")] = '\0';

    if (strlen(data) == 0)
    {
        printf("\nNo data entered. Nothing was written.\n");
        pause_screen();
        return;
    }

    bytes_written = write(fd, data, strlen(data));

    if (bytes_written == -1)
    {
        perror("\nwrite() failed");
        pause_screen();
        return;
    }

    printf("\n===============================================================\n");
    printf("                    WRITE SUCCESSFUL\n");
    printf("===============================================================\n");

    printf("\nFile Descriptor : %d\n", fd);
    printf("File            : %s\n", get_fd_filename(fd));
    printf("Bytes Written   : %zd\n", bytes_written);
    printf("Operation       : write()\n");
    printf("Status          : SUCCESS\n");

    printf("\nData Written:\n");
    printf("%s\n", data);

    pause_screen();
}
void read_file_menu(void)
{
    int fd;
    char buffer[1025];
    ssize_t bytes_read;

    printf("\n");
    printf("===============================================================\n");
    printf("                          READ FILE\n");
    printf("===============================================================\n");

    display_fd_table();

    if (get_active_fd() == -1)
    {
        printf("\nNo active file is selected.\n");
        printf("Please open or select a file first.\n");

        pause_screen();
        return;
    }

    printf("\nActive File Information\n");
    printf("File Descriptor : %d\n", get_active_fd());
    printf("File            : %s\n",
           get_fd_filename(get_active_fd()));

    printf("\nEnter FD to read from: ");

    if (scanf("%d", &fd) != 1)
    {
        clear_input_buffer();

        printf("\nInvalid FD input.\n");
        pause_screen();
        return;
    }

    clear_input_buffer();

    if (!is_fd_registered(fd))
    {
        printf("\nFD %d is not registered.\n", fd);
        pause_screen();
        return;
    }

    /*
       Move to the beginning of the file so that
       the complete stored content can be displayed.
    */
    if (lseek(fd, 0, SEEK_SET) == -1)
    {
        perror("\nlseek() failed");
        pause_screen();
        return;
    }

    bytes_read = read(fd, buffer, sizeof(buffer) - 1);

    if (bytes_read == -1)
    {
        perror("\nread() failed");
        pause_screen();
        return;
    }

    buffer[bytes_read] = '\0';

    printf("\n===============================================================\n");
    printf("                     READ SUCCESSFUL\n");
    printf("===============================================================\n");

    printf("\nFile Descriptor : %d\n", fd);
    printf("File            : %s\n", get_fd_filename(fd));
    printf("Operation       : read()\n");
    printf("Bytes Read      : %zd\n", bytes_read);

    printf("\nFile Contents:\n");
    printf("---------------------------------------------------------------\n");

    if (bytes_read == 0)
    {
        printf("[File is empty]\n");
    }
    else
    {
        printf("%s\n", buffer);
    }

    printf("---------------------------------------------------------------\n");

    printf("Current Position: %ld\n", lseek(fd, 0, SEEK_CUR));
    printf("Status          : SUCCESS\n");

    pause_screen();
}
void append_file_menu(void)
{
    int fd;
    char data[1024];
    ssize_t bytes_written;
    off_t old_position;
    off_t new_position;

    printf("\n");
    printf("===============================================================\n");
    printf("                         APPEND FILE\n");
    printf("===============================================================\n");

    display_fd_table();

    if (get_active_fd() == -1)
    {
        printf("\nNo active file is selected.\n");
        printf("Please open or select a file first.\n");

        pause_screen();
        return;
    }

    printf("\nActive File Information\n");
    printf("File Descriptor : %d\n", get_active_fd());
    printf("File            : %s\n",
           get_fd_filename(get_active_fd()));

    printf("\nEnter FD to append to: ");

    if (scanf("%d", &fd) != 1)
    {
        clear_input_buffer();

        printf("\nInvalid FD input.\n");
        pause_screen();
        return;
    }

    clear_input_buffer();

    if (!is_fd_registered(fd))
    {
        printf("\nFD %d is not registered.\n", fd);
        pause_screen();
        return;
    }

    old_position = lseek(fd, 0, SEEK_CUR);

    if (old_position == -1)
    {
        perror("\nlseek() failed");
        pause_screen();
        return;
    }

    if (lseek(fd, 0, SEEK_END) == -1)
    {
        perror("\nUnable to move to file end");
        pause_screen();
        return;
    }

    printf("\nEnter data to append:\n");
    printf("> ");

    if (fgets(data, sizeof(data), stdin) == NULL)
    {
        printf("\nUnable to read input.\n");
        pause_screen();
        return;
    }

    data[strcspn(data, "\n")] = '\0';

    if (strlen(data) == 0)
    {
        printf("\nNo data entered. Nothing was appended.\n");
        pause_screen();
        return;
    }

    bytes_written = write(fd, data, strlen(data));

    if (bytes_written == -1)
    {
        perror("\nwrite() failed");
        pause_screen();
        return;
    }

    new_position = lseek(fd, 0, SEEK_CUR);

    printf("\n===============================================================\n");
    printf("                   APPEND SUCCESSFUL\n");
    printf("===============================================================\n");

    printf("\nFile Descriptor : %d\n", fd);
    printf("File            : %s\n", get_fd_filename(fd));
    printf("Previous Position: %ld\n", (long)old_position);
    printf("Bytes Appended  : %zd\n", bytes_written);
    printf("New Position    : %ld\n", (long)new_position);
    printf("Operation       : lseek() + write()\n");
    printf("Status          : SUCCESS\n");

    printf("\nData Appended:\n");
    printf("%s\n", data);

    pause_screen();
}
void seek_file_menu(void)
{
    int fd;
    int choice;
    long offset;
    int whence;
    off_t new_position;

    printf("\n");
    printf("===============================================================\n");
    printf("                    SEEK FILE POSITION\n");
    printf("===============================================================\n");

    display_fd_table();

    if (get_active_fd() == -1)
    {
        printf("\nNo active file is selected.\n");
        printf("Please open or select a file first.\n");

        pause_screen();
        return;
    }

    printf("\nActive File Information\n");
    printf("File Descriptor : %d\n", get_active_fd());
    printf("File            : %s\n",
           get_fd_filename(get_active_fd()));

    printf("\nEnter FD: ");

    if (scanf("%d", &fd) != 1)
    {
        clear_input_buffer();

        printf("\nInvalid FD input.\n");
        pause_screen();
        return;
    }

    clear_input_buffer();

    if (!is_fd_registered(fd))
    {
        printf("\nFD %d is not registered.\n", fd);
        pause_screen();
        return;
    }

    printf("\nSelect seek location:\n");
    printf("1. Beginning of File (SEEK_SET)\n");
    printf("2. Current Position (SEEK_CUR)\n");
    printf("3. End of File (SEEK_END)\n");

    printf("\nEnter choice: ");

    if (scanf("%d", &choice) != 1)
    {
        clear_input_buffer();

        printf("\nInvalid choice.\n");
        pause_screen();
        return;
    }

    clear_input_buffer();

    switch (choice)
    {
        case 1:
            whence = SEEK_SET;
            break;

        case 2:
            whence = SEEK_CUR;
            break;

        case 3:
            whence = SEEK_END;
            break;

        default:
            printf("\nInvalid seek option.\n");
            pause_screen();
            return;
    }

    printf("\nEnter offset: ");

    if (scanf("%ld", &offset) != 1)
    {
        clear_input_buffer();

        printf("\nInvalid offset.\n");
        pause_screen();
        return;
    }

    clear_input_buffer();

    new_position = lseek(fd, (off_t)offset, whence);

    if (new_position == (off_t)-1)
    {
        perror("\nlseek() failed");
        pause_screen();
        return;
    }

    printf("\n===============================================================\n");
    printf("                  SEEK SUCCESSFUL\n");
    printf("===============================================================\n");

    printf("\nFile Descriptor : %d\n", fd);
    printf("File            : %s\n",
           get_fd_filename(fd));

    printf("Offset          : %ld\n", offset);

    if (choice == 1)
        printf("Reference       : Beginning of File\n");
    else if (choice == 2)
        printf("Reference       : Current Position\n");
    else
        printf("Reference       : End of File\n");

    printf("New Position    : %ld\n", (long)new_position);
    printf("Operation       : lseek()\n");
    printf("Status          : SUCCESS\n");

    pause_screen();
}
void read_specific_position_menu(void)
{
    int fd;
    long position;
    ssize_t bytes_read;
    char buffer[1025];
    off_t current_position;

    printf("\n");
    printf("===============================================================\n");
    printf("                 READ FROM SPECIFIC POSITION\n");
    printf("===============================================================\n");

    display_fd_table();

    if (get_active_fd() == -1)
    {
        printf("\nNo active file is selected.\n");
        printf("Please open or select a file first.\n");

        pause_screen();
        return;
    }

    printf("\nActive File Information\n");
    printf("File Descriptor : %d\n", get_active_fd());
    printf("File            : %s\n",
           get_fd_filename(get_active_fd()));

    printf("\nEnter FD: ");

    if (scanf("%d", &fd) != 1)
    {
        clear_input_buffer();

        printf("\nInvalid FD input.\n");
        pause_screen();
        return;
    }

    clear_input_buffer();

    if (!is_fd_registered(fd))
    {
        printf("\nFD %d is not registered.\n", fd);
        pause_screen();
        return;
    }

    printf("\nEnter position from where to read: ");

    if (scanf("%ld", &position) != 1)
    {
        clear_input_buffer();

        printf("\nInvalid position.\n");
        pause_screen();
        return;
    }

    clear_input_buffer();

    if (position < 0)
    {
        printf("\nPosition cannot be negative.\n");
        pause_screen();
        return;
    }

    if (lseek(fd, (off_t)position, SEEK_SET) == (off_t)-1)
    {
        perror("\nlseek() failed");
        pause_screen();
        return;
    }

    bytes_read = read(fd, buffer, sizeof(buffer) - 1);

    if (bytes_read == -1)
    {
        perror("\nread() failed");
        pause_screen();
        return;
    }

    buffer[bytes_read] = '\0';

    current_position = lseek(fd, 0, SEEK_CUR);

    printf("\n===============================================================\n");
    printf("             READ FROM POSITION SUCCESSFUL\n");
    printf("===============================================================\n");

    printf("\nFile Descriptor : %d\n", fd);
    printf("File            : %s\n",
           get_fd_filename(fd));
    printf("Starting Position: %ld\n", position);
    printf("Bytes Read      : %zd\n", bytes_read);
    printf("Current Position: %ld\n",
           (long)current_position);
    printf("Operation       : lseek() + read()\n");
    printf("Status          : SUCCESS\n");

    printf("\nData Read From Position %ld:\n", position);
    printf("---------------------------------------------------------------\n");

    if (bytes_read == 0)
    {
        printf("[No data available from this position]\n");
    }
    else
    {
        printf("%s\n", buffer);
    }

    printf("---------------------------------------------------------------\n");

    pause_screen();
}
void write_specific_position_menu(void)
{
    int fd;
    long position;
    char data[1024];
    ssize_t bytes_written;
    off_t new_position;

    printf("\n");
    printf("===============================================================\n");
    printf("                 WRITE AT SPECIFIC POSITION\n");
    printf("===============================================================\n");

    display_fd_table();

    if (get_active_fd() == -1)
    {
        printf("\nNo active file is selected.\n");
        printf("Please open or select a file first.\n");

        pause_screen();
        return;
    }

    printf("\nActive File Information\n");
    printf("File Descriptor : %d\n", get_active_fd());
    printf("File            : %s\n",
           get_fd_filename(get_active_fd()));

    printf("\nEnter FD: ");

    if (scanf("%d", &fd) != 1)
    {
        clear_input_buffer();

        printf("\nInvalid FD input.\n");
        pause_screen();
        return;
    }

    clear_input_buffer();

    if (!is_fd_registered(fd))
    {
        printf("\nFD %d is not registered.\n", fd);
        pause_screen();
        return;
    }

    printf("\nEnter position where data should be written: ");

    if (scanf("%ld", &position) != 1)
    {
        clear_input_buffer();

        printf("\nInvalid position.\n");
        pause_screen();
        return;
    }

    clear_input_buffer();

    if (position < 0)
    {
        printf("\nPosition cannot be negative.\n");
        pause_screen();
        return;
    }

    if (lseek(fd, (off_t)position, SEEK_SET) == (off_t)-1)
    {
        perror("\nlseek() failed");
        pause_screen();
        return;
    }

    printf("\nEnter data to write:\n");
    printf("> ");

    if (fgets(data, sizeof(data), stdin) == NULL)
    {
        printf("\nUnable to read input.\n");
        pause_screen();
        return;
    }

    data[strcspn(data, "\n")] = '\0';

    if (strlen(data) == 0)
    {
        printf("\nNo data entered. Nothing was written.\n");
        pause_screen();
        return;
    }

    bytes_written = write(fd, data, strlen(data));

    if (bytes_written == -1)
    {
        perror("\nwrite() failed");
        pause_screen();
        return;
    }

    new_position = lseek(fd, 0, SEEK_CUR);

    printf("\n===============================================================\n");
    printf("             WRITE AT POSITION SUCCESSFUL\n");
    printf("===============================================================\n");

    printf("\nFile Descriptor : %d\n", fd);
    printf("File            : %s\n",
           get_fd_filename(fd));
    printf("Starting Position: %ld\n", position);
    printf("Bytes Written   : %zd\n", bytes_written);
    printf("New Position    : %ld\n", (long)new_position);
    printf("Operation       : lseek() + write()\n");
    printf("Status          : SUCCESS\n");

    printf("\nData Written:\n");
    printf("---------------------------------------------------------------\n");
    printf("%s\n", data);
    printf("---------------------------------------------------------------\n");

    pause_screen();
}
