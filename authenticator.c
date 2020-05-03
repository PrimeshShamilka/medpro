#include <stdio.h>
#include <termios.h> // in gcc, use < conio.h > in turboc
#include <string.h>
#include <stdlib.h>
#include "config_rw_module.c"
#include <openssl/md5.h>

/************************************************
THIS CODE IS USED IN GCC LINUX
because getch() is not defined in library file
************************************************/
 
static struct termios old, new;
 
/* Initialize new terminal i/o settings */
void initTermios(int echo)
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  new = old; /* make new settings same as old settings */
  new.c_lflag &= ~ICANON; /* disable buffered i/o */
  new.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
  tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */
}
 
/* Restore old terminal i/o settings */
void resetTermios(void)
{
  tcsetattr(0, TCSANOW, &old);
}
 
/* Read 1 character - echo defines echo mode */
char getch_(int echo)
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}
 
/* Read 1 character without echo */
char getch(void)
{
  return getch_(0);
}
 
/* Read 1 character with echo */
char getche(void)
{
  return getch_(1);
}
 
/* function :   getPassword(),
   to get password from keyboard    */
 
void getPassword(char *pass)
{
  int c=0;
  char buff[30]={0},ch;
  int len=0;
  while((ch=getch())!='\n')
  {
      if(ch==0x7f)    // use 0x08 in turboc (WINDOWS)
      {
          if(len==0)  continue;
          printf("\b \b"); len--; continue;
      }
      printf("%c",'*');
      pass[len]=ch;
      len++;
  }
  pass[len]='\0';
}



/*
*
* HASH PASSWORD WITH MD5 HASH 
* 
*/

char* convert_to_md5(char* plain_text){

  int i;
  unsigned char result[MD5_DIGEST_LENGTH];
  MD5(plain_text, strlen(plain_text), result);
  char *converted = malloc(MD5_DIGEST_LENGTH*2 + 1);

  for(i=0;i<MD5_DIGEST_LENGTH;i++) {
    sprintf(&converted[i*2], "%02X", result[i]);

    /* equivalent using snprintf, notice len field keeps reducing
       with each pass, to prevent overruns

    snprintf(&converted[i*2], sizeof(converted)-(i*2),"%02X", buffer[i]);
    */

  }
  return converted;
}



/*
*
* AUTHENTICATE USER 
* 
*/
 
struct user authenticate()
{
  char user[30],pass[30];
  printf("Enter User Name :");
  fgets(user, 10, stdin);
  strtok(user, "\n");
  printf("Enter Password  :");
  getPassword(pass);
  
  char *username, *password, *user_type;
  struct user current_user = get_user_cfg(user);
  username = current_user.username;
  password = current_user.password;
  user_type = current_user.user_type;

  char *hashed_password = convert_to_md5(pass);

  if(username == NULL){
    printf("\nUSER DOES NOT EXIST.\n");
  }else{
    if(strcmp(hashed_password,password) == 0)
    {
      printf("\nLOGIN SUCCESS.\n");
      if(strcmp(user_type,"admin") == 0){
        printf("\nYOU ARE LOGGED IN AS AN ADMIN.\n");
      }
    }
    else
    {
      printf("\nLOGIN FAILED.\n");
    }
  }

  struct user result = {username, password, user_type};
  return result;
}



/*
*
* ADD NEW USER 
* 
*/

void add_user(){

  char new_username[30], new_user_type[30], new_password[30], new_password_re[30];

  printf("Enter user name :\n");
  fgets(new_username, 30, stdin);
  strtok(new_username, "\n");

  while(1){
    printf("Enter user password :\n");
    getPassword(new_password);
    printf("\nRe-Enter user password :\n");
    getPassword(new_password_re);

    if(strcmp(new_password,new_password_re)== 0){
        break;
    }
    printf("\nYOUR PASSWORDS DO NOT MATCH. RE-ENTER THE PASSWORD.\n");
  }
  printf("\nEnter user type :\n");
  fgets(new_user_type, 30, stdin);
  strtok(new_user_type, "\n");

  char *hashed_password = convert_to_md5(new_password);
  add_user_cfg(new_username, hashed_password, new_user_type);
  free(hashed_password);
}



