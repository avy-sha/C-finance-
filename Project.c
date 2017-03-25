/*
c-finance
started on 25-3-17
by Abhinav,Sood
*/

#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//#include<conio.h>
#include<unistd.h>

FILE *templog,*withdrawallog,*account;

void transactions();

int convert(char *a)
{
  int  i, len;
  int result=0;
  len = strlen(a);
  for(i=0; i<len; i++){
  result = result * 10 + ( a[i] - '0' );
  }
  return result;
}

int accountopen(int accno)
{
  printf("a");
  account=fopen("Account","r");
  int balance,acctemp;
  while(fscanf(account,"%d%d",&acctemp,&balance)!=EOF)
  {
    if(acctemp==accno)
    {
      break;
    }
  }
  fclose(account);
  return balance;
}

void *withdrawl(void *t)
{
  int *text = (int *)t;
  printf("A");
  withdrawallog=fopen("Withdrawallog","w");
  int balance=accountopen(text[0]);
  printf("%d",balance);
  return NULL;
}

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
templog=fopen("templog","w");
if(templog==NULL){
  printf("Unable to open templog\n");
  exit(1);
}

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
          strcpy(text4,"-");
          printf("Enter the account no and amount to withdrawl\n");
          scanf("%s%s",text2,text3);
          fprintf(templog,"%s %s %s %s \n",text1,text2,text3,text4);
          //Withdrawl();
          break;}
  case 50:{strcpy(text1,"DEPOSIT");
           strcpy(text4,"-");
          printf("Enter the account no and amount to Deposit\n");
          scanf("%s%s",text2,text3);
          fprintf(templog,"%s %s %s %s \n",text1,text2,text3,text4);
          clean_stdin();
          break;}
  case 51:{strcpy(text1,"STATEMENT");
          strcpy(text4,"-");
          strcpy(text3,"-");
          printf("Enter the account no for statement\n");
          scanf("%s",text2);
          fprintf(templog,"%s %s %s %s\n",text1,text2,text3,text4);
          clean_stdin();
          break;}
  case 52:{strcpy(text1,"TRANSFER");
           printf("Enter the account no1 ,account no2 and amount to transfer\n");
           scanf("%s%s%s",text2,text3,text4);
           fprintf(templog,"%s %s %s %s \n",text1,text2,text3,text4);
           clean_stdin();
           break;}
  case 53:{clean_stdin();
          printf("Creating threads and performing transations\n");
          fclose(templog);
          transactions();
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


//For handling the transations
void transactions(){
  char text1[10],text2[10],text3[10],text4[10];
  pthread_t deposit,withd,statement,transfer;
  templog=fopen("templog","r");
  if(templog==NULL){
    printf("Unable to open templog\n");
    exit(1);
  }
  int text[3];
while(fscanf(templog,"%s %s %s %s",text1,text2,text3,text4)!=EOF){
  printf("%s %s %s %s\n",text1,text2,text3,text4);
  if(strcmp(text4,"-")==0 && strcmp(text1,"STATEMENT")!=0)
  {
    //printf("A");
    text[0]=convert(text2);
    text[1]=convert(text3);
    if(strcmp(text1,"WITHDRAWL")==0){
      //printf("a");
      pthread_create(&withd,NULL,&withdrawl,(void*)text);
    }
    else if(strcmp(text1,"DEPOSIT")==0){}
  }
  //printf("%s",text4);
}
getchar();
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
