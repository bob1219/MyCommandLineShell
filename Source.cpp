#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "Functions.h"
#include "Macros.h"

char Username[USERNAME_MAX];

int main(void) {
	time_t timer;
	struct tm now;
	char username[USERNAME_MAX], password[PASSWORD_MAX];

	PrintVersion();

	_getch();
	putchar('\n');

	// Print the current time
	timer = time(NULL);
	localtime_s(&now, &timer);
	printf("%d-%d-%d %d:%d\n", now.tm_mon + 1, now.tm_mday, now.tm_year + 1900, now.tm_hour, now.tm_min);

	putchar('\n');

	// login
	printf("Welcome to the MyCommandLineShell.\n");
	printf("Please login.\n");
	while (1) {
		// Input the username
		printf("username>");
		if (fgets(username, USERNAME_MAX, stdin) == NULL) {
			Error("Failed input.");
			_getch();
			return EXIT_FAILURE;
		}
		if (username[strlen(username) - 1] == '\n')
			username[strlen(username) - 1] = '\0';
		else
			while (getchar() != '\n');

		// Input the password
		printf("password>");
		if (fgets(password, PASSWORD_MAX, stdin) == NULL) {
			Error("Failed input.");
			_getch();
			return EXIT_FAILURE;
		}
		if (password[strlen(password) - 1] == '\n')
			password[strlen(password) - 1] = '\0';
		else
			while (getchar() != '\n');

		switch (Login(username, password)) {
		case 0:goto endloop;

		case 1:
			Error("Failed login.");
			putchar('\n');
			continue;

		default:
			Error("Return value of function \"login\" is invalid.");
			_getch();
			return EXIT_FAILURE;
		}
	}
endloop:;

	putchar('\n');

	CommandLine();
}

void PrintVersion(void) {
	printf("MyCommandLineShell\n");
	printf("Version %s\n", VERSION);
}

unsigned int Login(const char *username, const char *password) {
	FILE *FilePointer;
	char username2[USERNAME_MAX], password2[PASSWORD_MAX];
	short count = 0, EqualCount = 0, count2 = 0, equal = 0;

	// Check username
	if (fopen_s(&FilePointer, "files\\system\\usernames", "r") != 0) {
		Error("Cannot open file.");
		_getch();
		exit(EXIT_FAILURE);
	}
	while (fgets(username2, USERNAME_MAX, FilePointer) != NULL) {
		count++;
		if (!strcmp(username, username2)) {
			EqualCount++;
			break;
		}
	}
	if (EqualCount == 0)return 1;
	fclose(FilePointer);

	// Check password
	if (fopen_s(&FilePointer, "files\\system\\passwords", "r") != 0) {
		Error("Cannot open file.");
		_getch();
		exit(EXIT_FAILURE);
	}
	while (fgets(password2, PASSWORD_MAX, FilePointer) != NULL) {
		count2++;
		if ((count == count2) && (!strcmp(password, password2)))equal = 1;
	}
	fclose(FilePointer);

	if (equal) {
		strcpy_s(Username, USERNAME_MAX, username);
		return 0;
	}
	else return 1;
}