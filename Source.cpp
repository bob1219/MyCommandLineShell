#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "Functions.h"
#include "Macros.h"

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