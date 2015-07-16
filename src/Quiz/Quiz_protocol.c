/*=============================
情報実験1
Quiz_Client ver 3
BP13007
雨宮俊貴
©2015 雨宮俊貴
===============================*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>


#define SOCK_NAME "./socket"

void func_write_and_read(int soc,char *buf,int len);
void func_inputUserId(int);
int func_inputPassWord(int);
void func_wait_answer(int);

int main(int argc, char *argv[])
{

  struct sockaddr_in saddr;

  
  int soc;
  char buf[1024];
  char temp[256];
  int passwordMissFlag = 1;
  int exitflag = 0;
  if(argc < 2){
    printf("You didnt input any information of oponent.\nusage: ./~this~.out IP_Address port_number\n");
    exit(1);
  }

  if ( ( soc = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 ) {
    perror( "socket" );
    exit( 1 );
  }
  /*
  struct hostent *hp;
  if((hp=gethostbyname("localhost"))==NULL){
    perror("connect");
    exit(1);
  }

  memcpy(&saddr.sin_addr, hp->h_addr, hp->h_length);
  */
  
  memset( (char *)&saddr, 0, sizeof( saddr ) );
  saddr.sin_family=AF_INET;
  saddr.sin_addr.s_addr = inet_addr(argv[1]);
  saddr.sin_port = htons(atoi(argv[2]));
  //strcpy( saddr.sun_path, SOCK_NAME );

  if ( connect( soc, ( struct sockaddr * )&saddr, ( socklen_t )sizeof( saddr ) ) < 0 ) {
    perror( "connect" );
    exit( 1 );
  }
  fprintf( stderr, "Connection established: socket %d used.\n", soc );
  printf("This is Quiz Client.");
  /*ユーザー名受付状態*/
  printf("This is Quiz Client.\n");  
      strcpy(buf,"USER bp13007");
      func_write_and_read(soc,buf,strlen(buf));
      strcpy(buf,"PASS 70031pb");
      func_write_and_read(soc,buf,strlen(buf));
  /*クイズ,回答受付状態*/
  func_wait_answer(soc);

  printf("Congratulation! You answered right 5 quiz!\n");
  strcpy(buf,"GET MESSAGE");
  //if ( buf[strlen(buf)-1] == '\n' ) buf[strlen(buf)-1] = '\0';
  func_write_and_read(soc,buf,strlen(buf));
  printf("this is secret message\n");
  printf("%s\n",buf);
  
  


    

  close( soc );

  return 0;
}


void func_write_and_read(int soc,char *buf,int len){
  write( soc, buf, strlen(buf)+1 );
  fsync( soc );
  read( soc, buf, 1024);
}


void func_inputUserId(int soc){
  char temp[256];
  char buf[1024];
  
  while(1) {
    printf("Input your User ID\n");
    printf("::>");
    scanf("%s",temp);
    if(!strcmp(temp,"QUIT")){
      strcpy(buf,"QUIT");
      func_write_and_read(soc,buf,strlen(buf));
      fprintf(stderr,"%sを受信したため切断します\n",buf);
      close(soc);
      exit(1);
    }
    sprintf(buf,"USER %s",temp);
    //if ( buf[strlen(buf)-1] == '\n' ) buf[strlen(buf)-1] = '\0';
    func_write_and_read(soc,buf,strlen(buf));
    if(!strcmp(buf,"OK")){
      printf("Accepted your user id\n");
      break;
    }else if(!strcmp(buf,"NG"))
      printf("Invalid ID\n");    
    else if(!strcmp(buf,"ERROR"))
      printf("Invalid Input\n");
  }
}

int func_inputPassWord(int soc){
  char temp[256];
  char buf[1024];
  while(1) {
    
    printf("%s\n",buf);
    printf("Input password\n");    
    printf("::>");
    scanf("%s",temp);
    
    if(!strcmp(temp,"QUIT")){
      strcpy(buf,temp);
      func_write_and_read(soc,buf,strlen(buf));
      fprintf(stderr,"%sを受信したため切断します\n",buf);
      close(soc);
      exit(1);
    }
    sprintf(buf,"PASS %s",temp);
    //if ( buf[strlen(buf)-1] == '\n' ) buf[strlen(buf)-1] = '\0';
    func_write_and_read(soc,buf,strlen(buf));
    if(!strcmp(buf,"OK")){
      printf("Accepted your Password\n");
      return 0;
     
    }else if(!strcmp(buf,"NG")){
      printf("Invalid password\n");    
      return 1;
    }
  }
}

void func_wait_answer(int soc){
  char temp[256];
  char buf[1024];
  int i=0;
  while(i < 5) {
    
    sprintf(buf,"QUIZ %d",i);
      func_write_and_read(soc,buf,strlen(buf));
      if(!strcmp(buf,"NG"))
	printf("Wrong number of right answers\n");
      else if(!strcmp(buf,"ERROR"))
	printf("Wrong input");
      else{

	printf("%s\n",buf);
	printf("Input answer\n");    
	printf("::>");
	scanf("%s",temp);

	if(!strcmp(temp,"QUIT")){
	  strcpy(buf,temp);
	  func_write_and_read(soc,buf,strlen(buf));
	  fprintf(stderr,"%sを受信したため切断します\n",buf);
	  close(soc);
	  exit(1);
	}
	sprintf(buf,"ANSR %s",temp);
	//if ( buf[strlen(buf)-1] == '\n' ) buf[strlen(buf)-1] = '\0';
	func_write_and_read(soc,buf,strlen(buf));
	if(!strcmp(buf,"OK")){
	  printf("Exactly!\n");
	  i++;
	}else if(!strcmp(buf,"NG"))
	printf("Wrong answer\n");    
	
    }
  }
}


void func_STAT(int soc){  
  char buf[1024];
  strcpy(buf,"STAT");
  func_write_and_read(soc,buf,strlen(buf));
  printf("%s\n",buf);
  
}
