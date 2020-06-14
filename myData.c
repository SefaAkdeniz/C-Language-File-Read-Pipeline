#include <stdlib.h>
#include <stdio.h>
#include <string.h> // Gerekli Kütüphaneler
#include <unistd.h>
#include <termio.h>
#include <sys/wait.h> 

#define READ 0
#define WRITE 1
#define PrintLineCount 24 // Basılacak satır sayısı
#define BufferSize 500    // Limitlemeyi kullanmazsak, satır sayısı artar ve bunu kontrol edemeyiz.Satır çok uzun olduğunda terminal satırı ikiye bölüyor.
#define MaxLineSize 250   // .txt max satır sayısı


// Fonksiyonun amacı kullanıcının devam etmek için girdiği tuşu kontrol eder.
int InputControl(void);
// Fonksiyonun amacı dosyanın işaretçisini verir.
FILE *OpenFile(char *);
// Fonksiyonun amacı dosyayı okurken aynı anda ekran bastırır.
int ReadFile(FILE *);

int main(int argc, char *argv[])
{
   FILE *fp = OpenFile(argv[1]);

   // Eğer girdi "./myData <filename> = myMore" ise dosyamızı 24'er satır satır okur.
   if (!strcmp(argv[2], "=") && !strcmp(argv[3], "myMore") && argc == 4)
   {
      char buffer[BufferSize];
      char inputGetChar;                          // Bu değişken girdileri tutar.
      int isReadEnd = 0;                          // Bu değişken bitip bitmediğini tutar.
      char writeMessage[MaxLineSize][BufferSize]; // Bu değişken pipe aracılığıyla gönderilen değişkeni tutar.

      do
      {
         pid_t pid;
         int fd[2];
         pipe(fd);
         pid = fork(); // Program fork methodu ile çatallandırıldı.

         // Çatallandırma başarısız olursa bu if koşuluna girecek ve hata verilecek.
         if (pid < 0)
         {
            fprintf(stderr, "Çatallandırma başarısız oldu.");
            return 1;
         }

         
         if (pid > 0)      // Parent Process
         {
            int lineCount = 0; // Değişkenimiz kaç satır okunduğu bilgisini tutar.

            //24 satır okuruz eğer verimiz 24 satırdan daha az kaldıysa kalan verimizin tamamını okur.
            for (int i = 0; i < PrintLineCount; ++i)
            {
               // Satırlarımızı okur
               if (fgets(buffer, sizeof(buffer), fp) > 0)
               {
                  strcpy(writeMessage[i], buffer);
                  lineCount++;
               }
               // Dosyada veri kalmadıysa ve okuma bittiyse döngüden çıkar
               else
               {
                  isReadEnd = 1;
                  break;
               }
            }

            close(fd[READ]);
            write(fd[WRITE], &writeMessage, lineCount * BufferSize);
            close(fd[WRITE]);

            wait(NULL);
         }

         if (pid == 0)     // Child Process
         {
            close(fd[WRITE]);

            char read_pipe[10]; // Bu değişken pipe'ın okuma ucunu tutar.
            sprintf(read_pipe, "%d", fd[READ]);

            char pid_index[10];
            char ppid_index[10];  
           
            int pidDegeri = getpid();   // olusturulan processimizin pid ve ppid degerlerini aliyoruz
            int ppidDegeri = getppid();

            sprintf(pid_index, "%d", pidDegeri);              //alinan degerleri sprintf yardimi ile icine yazdiriyoruz
            sprintf(ppid_index, "%d", ppidDegeri);

            char *arguments[5] = {argv[3], read_pipe, pid_index, ppid_index, NULL}; // Bu değişken myMore Bloğuna gönderilecek olan parametrelerin dizisini tutar.

            
            if (execv(argv[3], arguments) == -1)  // Execv fonksiyonu yardımıyla  myMore'u bloğunu çalıştırırız.
            {
               perror("myMore programı çalıştırılamadı.");
               return -1;
            }
         }

         inputGetChar = InputControl(); //Kullanıcının girdisi

      } while (isReadEnd != 1 && inputGetChar != 'q');  // Dosyada veri bittiyse yadq "q" basıldıysa döngüden çıkılır.
   }
   else if (argc == 2)    // Eğer girdi "./myData <filename>" ise bütün dosyayı okur.
   {
      ReadFile(fp);
   }
   else
   {
      printf("\nKullanım Hatası!\nDosyanın tamamını okumak için:\t\t\"./myData <Metin Dosyası Yolu>\"\nDosyayı satır satır okumak için:\t\"./myData <Metin Dosyası Yolu> = myMore\"\n");
      return 1;
   }

   return 0;
}

// Fonksiyonun amacı kullanıcının devam etmek için girdiği tuşu kontrol eder. Linux'da termio.h kütüphaneleri bu fonksiyon için kullanılır.
int InputControl(void)
{
   struct termios oldattr, newattr;
   int ch;
   tcgetattr(STDIN_FILENO, &oldattr);
   newattr = oldattr;
   newattr.c_lflag &= ~(ICANON | ECHO);
   tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
   ch = getchar();
   tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);

   return ch;
}

// Fonksiyonun amacı dosyanın işaretçisini verir.
FILE *OpenFile(char *filePath)
{
   FILE *fp;
   fp = fopen(filePath, "r");

   if (fp != NULL)
      return fp;

   else
   {
      perror("Error");
      exit(1);
   }
}

// Fonksiyonun amacı dosyayı okurken aynı anda ekran bastırır.
int ReadFile(FILE *fp)
{
   char *records = malloc(BufferSize + 1);

   if (!fp)
      return -1;

   while (fgets(records, BufferSize, fp) != NULL) // Dosyanın sonuna(\0) kadar okur.
      printf("%s", records);

   fclose(fp);

   return 0;
}