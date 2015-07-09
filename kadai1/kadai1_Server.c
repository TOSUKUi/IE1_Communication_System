/*=============================
情報実験1
課題1-Server.ver1
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
//socket のファイル名を定義
int main(int argc, char *argv[])
{
  int i;
  int ret;
  //ファイルディスクリプタ用アドレス
  int fd1,fd2;
  //サーバー側のbindで使うアドレス
  struct sockaddr_in saddr;
  //client側のacceptで使うアドレス
  struct sockaddr_in caddr;

  int len;
  int red;
  char buf[1024];

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
      for (i = 0; i < ret; i++) {
	if(isalpha(buf[i]))//大文字小文字変換
	   buf[i] = toupper(buf[i]);
      }

      fprintf(stderr,"write: %s\n",buf);
      write(fd2,buf,strlen(buf) + 1);
      fsync(fd2);
    }
    

    
    close(fd2);
  }
  close(fd1);
  
  return 0;
}

	
	
