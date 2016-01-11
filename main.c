// listdir.c
#include <sys/types.h>
#include <dirent.h>

#include <stdio.h>

int main(int argc, char *argv[])
{
  struct dirent *de=NULL;
  DIR *d=NULL;

  if(argc != 2)
  {
    fprintf(stderr,"Usage: %s dirname\n",argv[0]);
    return(1);
  }

  d=opendir(".");
  if(d == NULL)
  {
    perror("Couldn't open directory");
    return(2);
  }

  // Loop while not NULL
  while(de = readdir(d))
    printf("%s\n",de->d_name);

  closedir(d);
  return(0);
}
