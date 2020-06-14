#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define BufferSize 500
#define MaxLineSize 250
#define PrintLineCount 24

int main(int argc, char *argv[])
{
	char pidIndex[10];
	int pidValue = getpid();
	sprintf(pidIndex, "%d", pidValue);

	char ppidIndex[10];
	int ppidValue = getppid();
	sprintf(ppidIndex, "%d", ppidValue);

	char *pidIndexParent = argv[2];  // Parent üzerinden parametre olarak gönderilen pid id'ler alınır.
	char *ppidIndexParent = argv[3]; // Process çalışan process id alınır.

	if (strcmp(pidIndex, pidIndexParent) != 0 && strcmp(ppidIndex, ppidIndexParent) != 0)  // Parent ve child process id'ler kontrol edilir.
	{
		printf("myMore dosyasi parenti olmadan calisamaz...\n");
	}
	else
	{
		int readEnd = atoi(argv[1]);
		char readMessage[MaxLineSize][BufferSize];

		read(readEnd, &readMessage, PrintLineCount * BufferSize);

		for (int i = 0; i < PrintLineCount; ++i)
		{
			printf("%s", readMessage[i]);
		}

		close(readEnd);
		return 0;
	}
}
