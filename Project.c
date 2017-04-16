/*
c-finance
started on 25-3-17
by Abhinav,Sood
*/

#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<semaphore.h>
#include <time.h>

FILE *statementlog,*templog,*withdrawallog,*account,*depositlog,*transferlog;
pthread_mutex_t acc = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP;
pthread_mutex_t wd = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP;
pthread_mutex_t dep = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP;
pthread_mutex_t tra = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER_NP;
void transactions();
int main();

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

void newaccount(){
  account=fopen("Account","r+");
  int balance,acctemp;
  while(fscanf(account,"%d%d",&acctemp,&balance)!=EOF);
  acctemp++;
  fprintf(account,"%d %d\n",acctemp,0);
  printf("Your account has been created with an account no: %d and initial balance -0\n",acctemp);
  fclose(account);
  getchar();
  getchar();
  return;
}

void viewaccount(int accno){
  account=fopen("Account","r+");
  int balance,acctemp;
  while(fscanf(account,"%d%d",&acctemp,&balance)!=EOF)
  {
    if(acctemp==accno)
    {printf("Account details are:\nAccount no:%d\nBalance:%d\n",acctemp,balance);
      getchar();
      getchar();
      fclose(account);
      main();
    }
  }

}

int accountopen(int accno)
{
  //printf("a");
  pthread_mutex_lock(&acc);
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
  pthread_mutex_unlock(&acc);
  return balance;
}

void updateacc(int balance, int accno)
{ pthread_mutex_lock(&acc);
  account=fopen("Account","r+");
  int bal,acctemp,i=0,j;
  while(fscanf(account,"%d%d",&acctemp,&bal)!=EOF)
  {
    //printf("A");
    if(acctemp==accno)
    {
      //printf("A");
      while(bal>0){
        bal=bal/10;
        i++;
      }
      //printf("%d",i);
      fseek(account,-i,SEEK_CUR);
      for(j=0;j<i;j++)
      {
        fprintf(account, " ");
      }
      fseek(account,-(i+1),SEEK_CUR);
      fprintf(account, " %d", balance);
      break;
    }
  }
  fclose(account);
  pthread_mutex_unlock(&acc);
}

void *Transfer(void *t)
{ int length=0;
  int *text = (int *)t;
  time_t currtime;
  char date[100];
  //critical section starts
  pthread_mutex_lock(&tra);
  transferlog=fopen("Transferlog","a");
  int balance=accountopen(text[0]);
  int benficiarybal=accountopen(text[2]);

  if(text[1]<=balance)
  {
    balance=balance-text[1];
    updateacc(balance,text[0]);
    benficiarybal=benficiarybal+text[1];
    updateacc(benficiarybal,text[2]);

  }
  else
  {
    printf("Unsufficient Balance in account\n");
  }
  int val=text[1];

  while(val>0)
  {
    val=val/10;
    length++;
  }
  currtime=time(NULL);
  strcpy(date,ctime(&currtime));
  fprintf(transferlog,"%ld %d %d %d %d %s",currtime,text[0],text[2],text[1],length,date);
  fclose(transferlog);

  pthread_mutex_unlock(&tra);
  //critical section ends
  pthread_exit(NULL);
}

void *Withdrawl(void *t)
{
  int *text = (int *)t;
  int length=0;
  time_t currtime;
  char date[100];
  //critical section starts
  pthread_mutex_lock(&wd);
  withdrawallog=fopen("Withdrawallog","a");
  int balance=accountopen(text[0]);

  if(text[1]<=balance)
  {
    balance=balance-text[1];
    updateacc(balance,text[0]);
  }
  else
  {
    printf("Unsufficient Balance in account\n");
  }
  int val=text[1];

  while(val>0)
  {
    val=val/10;
    length++;
  }
  currtime=time(NULL);
  strcpy(date,ctime(&currtime));
  fprintf(withdrawallog,"%ld %d %d %d %s",currtime,text[0],text[1],length,date);
  fclose(withdrawallog);

  pthread_mutex_unlock(&wd);
  //critical section ends
  pthread_exit(NULL);
}

void *Statement(void *t)
{
  int *text=(int *)t,length=0;
  int count=0,i=0,k=1,val,acc,acc1;
  time_t curtime;
  char b,c[100],d[100],e[100],f[100],g[100];
  pthread_mutex_lock(&wd);
  withdrawallog = fopen("Withdrawallog","r");
  statementlog = fopen("Statementlog","w");
  while(fscanf(withdrawallog,"%ld %d %d %d %s %s %s %s %s",&curtime,&acc,&val,&length,c,d,e,f,g)!=EOF){count=count+1;}

  while(k<=count)
  {

      if(acc==text[0])
      {
        //fprintf(statementlog,"%ld %d %d\n",curtime,acc,val);
        fprintf(statementlog,"%s %ld %d %d %s %s %s %s %s\n","W",curtime,acc,val,c,d,e,f,g);
      }
       fseek(withdrawallog,-(length+45+26),SEEK_CUR);
       fscanf(withdrawallog,"%d",&length);
       fseek(withdrawallog,-(length+18),SEEK_CUR);

    fscanf(withdrawallog,"%ld %d %d %d %s %s %s %s %s",&curtime,&acc,&val,&length,c,d,e,f,g);
      k++;
  }
  pthread_mutex_unlock(&wd);
  count=0,k=1;
  pthread_mutex_lock(&dep);
  depositlog=fopen("Depositlog","r");
  while(fscanf(depositlog,"%ld %d %d %d %s %s %s %s %s",&curtime,&acc,&val,&length,c,d,e,f,g)!=EOF){count=count+1;}

  while(k<=count)
  {

      if(acc==text[0])
      {
        //fprintf(statementlog,"%ld %d %d\n",curtime,acc,val);
        fprintf(statementlog,"%s %ld %d %d %s %s %s %s %s\n","D",curtime,acc,val,c,d,e,f,g);
      }
       fseek(depositlog,-(length+45+26),SEEK_CUR);
       fscanf(depositlog,"%d",&length);
       fseek(depositlog,-(length+18),SEEK_CUR);

    fscanf(depositlog,"%ld %d %d %d %s %s %s %s %s",&curtime,&acc,&val,&length,c,d,e,f,g);
      k++;
  }
  pthread_mutex_unlock(&dep);
  count=0,k=1;
  pthread_mutex_lock(&tra);
  transferlog=fopen("Transferlog","r");
  while(fscanf(transferlog,"%ld %d %d %d %d %s %s %s %s %s",&curtime,&acc,&acc1,&val,&length,c,d,e,f,g)!=EOF){count=count+1;};
  while(k<=count)
  {
      if(acc==text[0])
      {
        fprintf(statementlog,"%s %ld %d %d %d %s %s %s %s %s\n","TT",curtime,acc,acc1,val,c,d,e,f,g);
      }
      else if(acc1==text[0])
      {
        fprintf(statementlog,"%s %ld %d %d %d %s %s %s %s %s\n","TF",curtime,acc,acc1,val,c,d,e,f,g);
      }
      fseek(transferlog,-(length+49+26),SEEK_CUR);
      fscanf(transferlog,"%d",&length);
      fseek(transferlog,-(length+23),SEEK_CUR);
      fscanf(transferlog,"%ld %d %d %d %d %s %s %s %s %s",&curtime,&acc,&acc1,&val,&length,c,d,e,f,g);
      k++;
  }
  fclose(statementlog);
  pthread_mutex_unlock(&tra);
  return NULL;
}

void *Deposit(void *t)
{ int length=0;
  int *text = (int *)t;
  time_t currtime;
  char date[100];

  //critical section starts
  pthread_mutex_lock(&dep);
  depositlog=fopen("Depositlog","a");
  int balance=accountopen(text[0]);
//  printf("%d",balance);
    balance=balance+text[1];
    updateacc(balance,text[0]);
    int val=text[1];

    while(val>0)
    {
      val=val/10;
      length++;
    }
    currtime=time(NULL);
    strcpy(date,ctime(&currtime));
    fprintf(depositlog,"%ld %d %d %d %s",currtime,text[0],text[1],length,date);
    //fprintf(depositlog,"%ld %d %d",currtime,text[0],text[1]);
    fclose(depositlog);
    pthread_mutex_unlock(&dep);

        //critical section ends

  return NULL;
  pthread_exit(NULL);
}

void clean_stdin(void)
{
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}


//For handling the transations
void transactions(){
  char text1[10],text2[10],text3[10],text4[10];
  pthread_t thread[100];
  int no=0;
  templog=fopen("templog","r");
  if(templog==NULL){
    printf("Unable to open templog\n");
    exit(1);
  }
  int text[3];
while(fscanf(templog,"%s %s %s %s",text1,text2,text3,text4)!=EOF){
  printf("%s %s %s %s\n",text1,text2,text3,text4);
  text[0]=text[1]=text[2]=0;
  if(strcmp(text4,"-")==0 && strcmp(text1,"STATEMENT")!=0)
  {
    text[0]=convert(text2);
    text[1]=convert(text3);
    if(strcmp(text1,"WITHDRAWL")==0){

      pthread_create(&thread[no],NULL,&Withdrawl,(void*)text);
      pthread_join(thread[no], NULL);
    }
    else if(strcmp(text1,"DEPOSIT")==0){

      pthread_create(&thread[no],NULL,&Deposit,(void*)text);
      pthread_join(thread[no], NULL);
    }
  }
  else if(strcmp(text4,"-")==0 && strcmp(text1,"STATEMENT")==0)
  {
    text[0]=convert(text2);
    pthread_create(&thread[no],NULL,&Statement,(void*)text);
    //pthread_join(thread[no], NULL);
  }
  else if (strcmp(text4,"-")!=0){
    text[0]=convert(text2);
    text[1]=convert(text3);
    text[2]=convert(text4);
    pthread_create(&thread[no],NULL,&Transfer,(void*)text);
    pthread_join(thread[no], NULL);
  }
no++;
}
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
printf("Enter the type of transaction\n");
printf("1.Withdrawl\n");
printf("2.Deposit\n");
printf("3.Account statement\n");
printf("4.Transfer Money\n");
printf("5.Start Consecutive transations\n");
printf("6.Return to main menu\n");
input=getchar();
switch (input) {
  case 49:{
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
          break;}
  case 51:{strcpy(text1,"STATEMENT");
          strcpy(text4,"-");
          strcpy(text3,"-");
          printf("Enter the account no for statement\n");
          scanf("%s",text2);
          fprintf(templog,"%s %s %s %s\n",text1,text2,text3,text4);
          break;}
  case 52:{strcpy(text1,"TRANSFER");
           printf("Enter the account no1 ,account no2 and amount to transfer\n");
           scanf("%s%s%s",text2,text4,text3);
           fprintf(templog,"%s %s %s %s \n",text1,text2,text3,text4);
           break;}
  case 53:{
          printf("Creating threads and performing transations\n");
          fclose(templog);
          transactions();
          banking();
          break;}
  case 54:  {printf("Returning to main menu\n");
                  main();
                  break;}
  default:{ printf("Invalid choice\n");
            break;}
}
system("tput clear");}
}


int main(){
system("tput clear");
int i,j,k,l,n,m,o,p;
//printf("%d",getpid());
while(1){
printf("FCFS BANKING SYSTEM\n\n\n");
printf("Enter the option\n");
printf("1.Create new account\n");
printf("2.View account\n");
printf("3.C banking\n");
printf("4.Exit\n");
n=getchar();
int accno;
switch (n) {
  case 49:{//clean_stdin();
          system("tput clear");
          newaccount();
          break;}
  case 50:{//clean_stdin();
          system("tput clear");
          printf("Enter the account no whose details you want to view:");
          scanf("%d",&accno);
          viewaccount(accno);
          break;}
  case 51:{system("tput clear");
          banking();
          break;}
  case 52:{printf("Exiting the program\n");
          exit(5);
          break;
          }
  default:{ //clean_stdin();
            system("tput clear");
            printf("Invalid choice\n");
            break;}
}
system("tput clear");
}

//printf("Enter the number of tasks to be done at initial bootup\n");

return 0;
}
