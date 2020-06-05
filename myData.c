#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void reader(char **);

int main(int argc, char **argv)
{
   char filename[] = "";
   strcpy(filename, argv[1]);
   FILE *file = fopen(filename, "r");
   if (file != NULL)
   {
      char line[128];

      if (argv[2] == NULL)
      {
         while (fgets(line, sizeof line, file) != NULL)
         {
            fputs(line, stdout);
         }
      }
      else if (!strcmp(argv[2], "="))
      {
         pid_t pid;
         while (fgets(line, sizeof line, file) != NULL)
         {
            pid = fork();
            if (pid < 0)
            {
               printf("fork error\n");
            }
            else if (pid > 0)
            {
               char *args[] = {argv[3], line};
               execv(argv[3], args);
               perror("execv");
            }
            else
            {
            }
         }
      }
      else
      {
         printf("\nYanlis bir komut girdiniz !\n\n");
      }

      fclose(file);
   }
   else
   {
      perror(filename);
   }

   return 0;
}