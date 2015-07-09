/*=============================
情報実験1
課題2-Server.ver1
BP13007
雨宮俊貴
©2015 雨宮俊貴
===============================*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define SOCK_NAME "./socket"
#define NO_MATCH_MSG "未登録"

FILE *func_InputFile(char *);

//socket のファイル名を定義
int main(int argc, char *argv[])
{
  FILE *fp;
  int i;
  int ret;
  int match_flag = 0;
  //ファイルディスクリプタ用アドレス
  int fd1,fd2;
  //サーバー側のbindで使うアドレス
  struct sockaddr_in saddr;
  //client側のacceptで使うアドレス
  struct sockaddr_in caddr;


  char dictionaly[256];
  char mean[256];
  int len;
  int red;
  char buf[1024];
  char no_match_msg[] = NO_MATCH_MSG;
  fp = func_InputFile("dic-w.txt");
  
  if ((fd1 = socket(AF_INET,SOCK_STREAM,0)) < 0) {
    perror("socket");
    exit(1);
  }
  //saddrを全て0にセットする
  memset((char *)&saddr, 0 ,sizeof( saddr ));
  //プロトコロルファミリー(種類)をAF_UNIXに初期化
  //saddr.sun_family = AF_INET;
  //ソケットのファイル名を指定
  saddr.sin_family=AF_INET;
  saddr.sin_addr.s_addr=INADDR_ANY;
  saddr.sin_port=htons(13007);
  

  //strcpy(saddr.sum_path,SOCK_NAME);

  unlink(SOCK_NAME);
  if (bind(fd1 , (struct sockaddr *)&saddr, (socklen_t)sizeof(saddr)) < 0) {
    perror("bind");
    exit(1);
  }

  if (listen(fd1,5) < 0) {
    perror("listen");
    exit(1);
  }
  
  while (1) {
    len = sizeof(caddr);
    if ((fd2 = accept(fd1 , (struct sockaddr *)&caddr , (socklen_t *)&len))<0) {
      perror("accept");
      exit(1);
    }
    fprintf(stderr,"Connection establish : socket %d used.\n",fd2);    
    //ret = 読み込み文字数
    while ((ret = read(fd2 , buf, 1024)) > 0) {
      fprintf(stderr,"read: %s\n",buf);
      /*for (i = 0; i < ret; i++) {
	if(isalpha(buf[i]))
	  buf[i] = toupper(buf[i]);
	  }*/
      /*辞書参照プロセス*/
      while(fscanf(fp,"%s %s",dictionaly,mean) != EOF){
	if (!strcmp(buf,dictionaly)) {
	  fprintf(stderr,"write: %s\n",mean);
	  write(fd2,mean,strlen(mean) + 1);
	  fsync(fd2);
	  match_flag = 1;
	  break;	  
	}
      }
      if(fscanf(fp,"%s %s",dictionaly,mean) == EOF && !match_flag){
	fprintf(stderr,"write: %s\n",no_match_msg);
	write(fd2,no_match_msg,strlen(no_match_msg)+1);
	fsync(fd2);	 
      }
      match_flag = 0;
      fseek(fp,0,SEEK_SET);
    }    
    close(fd2);
  }
  close(fd1);
  
  return 0;
}

	
	
FILE *func_InputFile(char *input){
  FILE *fp;
  if((fp = fopen(input,"r")) == NULL){
    printf("fuck off!!\n");
    exit(EXIT_FAILURE); //エラーなんで落ちます
  }
  return fp;
}
