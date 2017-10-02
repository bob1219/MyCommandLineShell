#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
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

void CommandLine(void) {
	time_t timer;
	struct tm now;
	char command[COMMAND_MAX];

	while (1) {
		timer = time(NULL);
		localtime_s(&now, &timer);

		printf("[%d-%d-%d %d:%d]", now.tm_mon + 1, now.tm_mday, now.tm_year + 1900, now.tm_hour,
			now.tm_min);
		printf("[%s]\n", Username);
		putchar('>');

		if (fgets(command, COMMAND_MAX, stdin) == NULL) {
			Error("Failed input.");
			_getch();
			exit(EXIT_FAILURE);
		}
		if (command[strlen(command) - 1] == '\n')
			command[strlen(command) - 1] = '\0';
		else
			while (getchar() != '\n');

		if (!CommandProcess(command))printf("succeed.");
		else printf("failed.");

		putchar('\n');
	}
}

unsigned int CommandProcess(const char *command) {
	char command1[COMMAND_MAX], command2[COMMAND_MAX], command3[COMMAND_MAX];

	sscanf_s(command, "%s %s %s", command1, COMMAND_MAX, command2, COMMAND_MAX, command3, COMMAND_MAX);

	if (!strcmp(command1, "date")) {
		time_t timer;
		struct tm now;

		timer = time(NULL);
		localtime_s(&now, &timer);

		printf("%d-%d-%d\n", now.tm_mon + 1, now.tm_mday, now.tm_year + 1900);

		return 0;
	}
	else if (!strcmp(command1, "time")) {
		time_t timer;
		struct tm now;

		timer = time(NULL);
		localtime_s(&now, &timer);

		printf("%d:%d\n", now.tm_hour, now.tm_min);

		return 0;
	}
	else if (!strcmp(command1, "now")) {
		time_t timer;
		struct tm now;

		timer = time(NULL);
		localtime_s(&now, &timer);

		printf("%d-%d-%d %d:%d\n", now.tm_mon + 1, now.tm_mday, now.tm_year + 1900, now.tm_hour,
			now.tm_min);

		return 0;
	}
	else if (!strcmp(command1, "mf")) {
		char filename[FILE_NAME_MAX], filebox_name[FILEBOX_NAME_MAX], filepath[FILENAME_MAX];
		FILE *FilePointer;

		sscanf_s(command2, "%[^/]/%s", filebox_name, FILEBOX_NAME_MAX, filename, FILE_NAME_MAX);
		sprintf_s(filepath, FILENAME_MAX, "files\\%s;%s\\%s", Username, filebox_name, filename);

		if (fopen_s(&FilePointer, filepath, "w") != 0)return 1;
		fclose(FilePointer);

		return 0;
	}
	else if (!strcmp(command1, "rf")) {
		char filename[FILE_NAME_MAX], filebox_name[FILEBOX_NAME_MAX], filepath[FILENAME_MAX];

		sscanf_s(command2, "%[^/]/%s", filebox_name, FILEBOX_NAME_MAX, filename, FILE_NAME_MAX);
		sprintf_s(filepath, FILENAME_MAX, "files\\%s;%s\\%s", Username, filebox_name, filename);

		if (!remove(filepath))return 0;
		else return 1;
	}
	else if (!strcmp(command1, "mfb")) {
		char filepath[FILENAME_MAX];

		sprintf_s(filepath, FILENAME_MAX, "files\\%s;%s", Username, command2);

		if (!_mkdir(filepath))return 0;
		else return 1;
	}
	else if (!strcmp(command1, "rfb")) {
		char filepath[FILENAME_MAX];

		sprintf_s(filepath, FILENAME_MAX, "files\\%s;%s", Username, command2);

		if (!_rmdir(filepath))return 0;
		else return 1;
	}
	else if (!strcmp(command1, "rnf")) {
		char filepath[FILENAME_MAX], filebox_name[FILEBOX_NAME_MAX], filename[FILE_NAME_MAX];

		sscanf_s(command2, "%[^/]/%s", filebox_name, FILEBOX_NAME_MAX, filename, FILE_NAME_MAX);
		sprintf_s(filepath, FILENAME_MAX, "files\\%s;%s\\%s", Username, filebox_name, filename);

		if (!rename(filepath, command3))return 0;
		else return 1;
	}
	else if (!strcmp(command1, "pfc")) {
		char filebox_name[FILEBOX_NAME_MAX], filename[FILE_NAME_MAX], filepath[FILENAME_MAX];
		FILE *FilePointer;
		int c;

		sscanf_s(command2, "%[^/]/%s", filebox_name, FILEBOX_NAME_MAX, filename, FILE_NAME_MAX);
		sprintf_s(filepath, FILENAME_MAX, "files\\%s;%s\\%s", Username, filebox_name, filename);

		if (fopen_s(&FilePointer, filepath, "r") != 0)return 1;
		while (c = fgetc(FilePointer) != EOF)putchar(c);
		fclose(FilePointer);

		return 0;
	}
	else if (!strcmp(command1, "wf")) {
		char filebox_name[FILEBOX_NAME_MAX], filename[FILE_NAME_MAX], filepath[FILENAME_MAX],
			fileline[FILE_LINE_MAX];
		FILE *FilePointer;

		sscanf_s(command2, "%[^/]/%s", filebox_name, FILEBOX_NAME_MAX, filename, FILE_NAME_MAX);
		sprintf_s(filepath, FILENAME_MAX, "files\\%s;%s\\%s", Username, filebox_name, filename);

		if (fopen_s(&FilePointer, filepath, "w") != 0)return 1;
		while (1) {
			putchar('>');
			if (fgets(fileline, FILE_LINE_MAX, stdin) == NULL) {
				Error("Failed input.");
				_getch();
				exit(EXIT_FAILURE);
			}
			if (fileline[strlen(fileline) - 1] == '\n')
				fileline[strlen(fileline) - 1] = '\0';
			else
				while (getchar() != '\n');

			if (!strcmp(fileline, "\\"))break;
			else fprintf(FilePointer, "%s\n", fileline);
		}
		fclose(FilePointer);

		return 0;
	}
	else {
		char message[100];

		sprintf_s(message, 100, "Unknown command \"%s\".", command1);
		Error(message);

		return 1;
	}
}

void Error(const char *message) {
	fprintf(stderr, "Error:%s\n", message);
}
