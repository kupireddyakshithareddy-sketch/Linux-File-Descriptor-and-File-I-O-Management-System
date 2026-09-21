#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    int choice, fd;
    char data[100];

    while (1) {
        printf("\n===== Linux File I/O Management System =====\n");
        printf("1. Create/Open File\n");
        printf("2. Write File\n");
        printf("3. Read File\n");
        printf("4. Append File\n");
        printf("5. Display File Descriptor\n");
        printf("6. Close File\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            fd = open("data.txt", O_CREAT | O_RDWR, 0644);

            if (fd == -1)
                printf("Error opening file\n");
            else
                printf("File opened successfully!\n");
        }

        else if (choice == 2) {
            fd = open("data.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);

            printf("Enter data: ");
            fgets(data, sizeof(data), stdin);

            write(fd, data, strlen(data));
            printf("Data written successfully!\n");

            close(fd);
        }

        else if (choice == 3) {
            char buffer[100];
            int n;

            fd = open("data.txt", O_RDONLY);

            n = read(fd, buffer, sizeof(buffer) - 1);
            buffer[n] = '\0';

            printf("Data from file: %s", buffer);

            close(fd);
        }

        else if (choice == 4) {
            fd = open("data.txt", O_WRONLY | O_APPEND);

            printf("Enter data to append: ");
            fgets(data, sizeof(data), stdin);

            write(fd, data, strlen(data));
            printf("Data appended successfully!\n");

            close(fd);
        }

        else if (choice == 5) {
            fd = open("data.txt", O_RDONLY);

            printf("File Descriptor: %d\n", fd);

            close(fd);
        }

        else if (choice == 6) {
            printf("File closed successfully!\n");
        }

        else if (choice == 7) {
            printf("Exiting program...\n");
            break;
        }

        else {
            printf("Invalid choice!\n");
        }
    }

    return 0;
}
