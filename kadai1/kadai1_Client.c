
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>


#define SOCK_NAME "./socket"

int main(int argc, char *argv[])
{

  struct sockaddr_in saddr;
  int soc;
  char buf[1024];
  if(argc < 2){
    printf("You didnt input any information of oponent.\nusage: kadai1_Client.out IP_Address port_number\n");
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

  while( fgets( buf, 1024, stdin ) ) {
    if ( buf[strlen(buf)-1] == '\n' ) buf[strlen(buf)-1] = '\0';
    write( soc, buf, strlen(buf)+1 );
    fsync( soc );
    read( soc, buf, strlen(buf)+1 );
    fprintf( stdout, "%s\n", buf );
  }

  close( soc );

  return 0;
}
