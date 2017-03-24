/*
c finance
started on 25-3-17
by Abhinav,Sood
*/

#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//#include<conio.h>
#include<unistd.h>

void clean_stdin(void)
{
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

void banking(){
int input;
char text1[10],text2[10],text3[10],text4[10];
FILE *templog;
templog=fopen("templog","w");
if(templog==NULL){
  printf("Unable to open templog\n");
  exit(1);
}

pthread_t deposit,withd,statement;
while(1){
//text1[10]="";text2[10]="";text3[10]="",text4[10]="";
printf("Enter the type of transaction\n");
printf("1.Withdrawl\n");
printf("2.Deposit\n");
printf("3.Account statement\n");
printf("4.Transfer Money\n");
printf("5.Start Consecutive transations\n");
printf("6.Return to main menu\n");
input=getchar();
switch (input) {
  case 49:{clean_stdin();
          strcpy(text1,"WITHDRAWL");
          printf("Enter the account no and amount to withdrawl\n");
          scanf("%s%s",text2,text3);
          fprintf(templog,"%s %s %s \n",text1,text2,text3);
          //Withdrawl();
          break;}
  case 50:{strcpy(text1,"DEPOSIT");
          printf("Enter the account no and amount to Deposit\n");
          scanf("%s%s",text2,text3);
          fprintf(templog,"%s %s %s \n",text1,text2,text3);
          clean_stdin();
          break;}
  case 51:{strcpy(text1,"STATEMENT");
          printf("Enter the account no for statement\n");
          scanf("%s%s",text2,text3);
          fprintf(templog,"%s %s %s \n",text1,text2,text3);
          clean_stdin();
          break;}
  case 52:{strcpy(text1,"TRANSFER");
           printf("Enter the account no1 ,account no2 and amount to transfer\n");
           scanf("%s%s%s",text2,text3,text4);
           fprintf(templog,"%s %s %s %s \n",text1,text2,text3,text4);
           clean_stdin();
           break;}
  case 53:{printf("Creating threads and performing transations\n");
          transations();
          return;
          break;}
  case 54:{printf("Returning to main menu\n");
                  return;
                  break;}
  default:{ clean_stdin();
            printf("Invalid choice\n");
            break;}
}
system("tput clear");}
}


//For Withdrawl
void Withdrawl(){

}


int main(){
int i,j,k,l,n,m,o,p;

while(1){
printf("Enter the option\n");
printf("1.Create new account\n");
printf("2.View account\n");
printf("3.Edit details\n");
printf("4.C banking\n");
printf("5.Exit\n");
clean_stdin();
n=getchar();
switch (n) {
  case 49:{clean_stdin();//newaccount();
          break;}
  case 50:{clean_stdin();//viewaccount();
          break;}
  case 51:{clean_stdin();//editdetails();
          break;}
  case 52:{clean_stdin();banking();
          break;}
  case 53:{printf("Exiting the program\n");
          exit(5);
          break;}
  default:{ clean_stdin();
            printf("Invalid choice\n");
            break;}
}
system("tput clear");
}

//printf("Enter the number of tasks to be done at initial bootup\n");

return 0;
}
