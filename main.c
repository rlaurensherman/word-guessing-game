/* Lauren Sherman */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#define MAX_BUFFER_SIZE 128

char *answer;
char *answercpy; 

void alarmHandler(int sig); 
void intHandler(int sig);
void play(char* inputWord);
void updateCpy();

int main(int argc, char* argv[])
{
	/*if there are not enough input arguments*/
	if(argc < 2) 
	{
		printf("%s: usage %s inputFile outFile \n", argv[0], argv[0]);
		return 2; 
	}

	play(argv[1]);

	return 0;
}

/* Handler for the alarm */
void alarmHandler(int sig)
{
	system("clear");
	updateCpy();
	printf("TOO SLOW!!!\n%s\n", answercpy);
	alarm(5);
}

/* Handler for ctrl-z signal */
void intHandler(int sig)
{
	int guessLen = strlen(answercpy);
	int n;

	n = rand() % guessLen;

	while (answercpy[n] != '-')
	{
		n = rand() % guessLen;
	}

	if(answercpy[n] == '-')
	{
		printf("\nHint: one of the letters is %c\n", answer[n]);
	}
}

/* Function for playing the game */
void play(char* inputWord)
{
	int i;
	char buffer[MAX_BUFFER_SIZE];
	int wordLen;
	char blank = '-';

	struct sigaction action; 
	struct sigaction action2;

	memset(&action, '\0', sizeof(struct sigaction));
	action.sa_handler = alarmHandler;
	action.sa_flags = SA_RESTART;
	sigaction(SIGALRM, &action, NULL);

	memset(&action2, '\0', sizeof(struct sigaction));
	action2.sa_handler = intHandler;
	action2.sa_flags = SA_RESTART;
	sigaction(SIGINT, &action2, NULL);

	/* Set the answer to the user's word */
	answer = inputWord;
	wordLen = strlen(answer);

	/* Set answercpy to all dashes for the length of the answer */
	answercpy = (char*)malloc(sizeof(char)*(wordLen+1));
	for (i = 0; i < wordLen; i++)
	{
		answercpy[i] = blank;
	}
	answercpy[wordLen] = '\0';

	system("clear");
	printf("%s", answercpy);
	printf("\nYour guess -> ");
	alarm(5);
	scanf("%s", buffer);

	/* Repeat while user has not guessed correctly and all letter have not been revealed */
	while(strcmp(answer, buffer) != 0)
	{
		system("clear");
		printf("Incorrect.\n");
		updateCpy();
		printf("%s", answercpy);
		printf("\nYour guess -> ");
		alarm(5);
		scanf("%s", buffer);
	}

	/* If user guesses correctly */
	if(strcmp(answer, buffer) == 0)
	{
		printf("\nCongratulations! You guessed correctly!\n");
		free(answercpy);
		exit(0);
	}

}

/* Function to update one of the dashes in answercpy to the correct letter */
void updateCpy()
{
	int guessLen = strlen(answercpy);
	int n;

	n = rand() % guessLen;

	/* If that letter has already been revealed, try another position */
	while (answercpy[n] != '-')
	{
		n = rand() % guessLen;
	}

	if(answercpy[n] == '-')
	{
		answercpy[n] = answer[n];
	}

	/* If all the letters have been revealed, user cannot guess anymore and loses */
	if(strcmp(answer, answercpy) == 0)
	{
		printf("Uh oh! You lose. Answer is: %s\n", answer);
		free(answercpy);
		exit(1);
	}

}
