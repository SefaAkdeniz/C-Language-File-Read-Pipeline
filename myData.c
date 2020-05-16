#include <stdio.h>
#include <string.h>s
int main (int argc, char** argv)
{
   char filename[] = "";
   strcpy(filename, argv[1]);
   FILE *file = fopen ( filename, "r" );
   if ( file != NULL )
   {
      char line [ 128 ];
      while ( fgets ( line, sizeof line, file ) != NULL )
      {
         fputs ( line, stdout );
      }
      fclose ( file );
   }
   else
   {
      perror ( filename );
   }
   return 0;
}
