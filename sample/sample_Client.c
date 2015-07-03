#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>


#define SOCK_NAME "./socket"

int main()
{
  struct sockaddr_in saddr;
  int soc;
  char buf[1024];

  if ( ( soc = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 ) {
    perror( "socket" );
    exit( 1 );
  }
  struct hostent *hp;
  if((hp=gethostbyname(”www.shibaura-it.ac.jp”))==NULL){
    perror(”connect”);
    exit(1);
  }
  memcpy(&addr.sin_addr, hp->h_addr, hp->h_length);

  
  memset( (char *)&saddr, 0, sizeof( saddr ) );
  saddr.sin_family=AF_INET;
  saddr.sin_addr.s_addr = inet_addr("172.24.213.89");
  saddr.sin_port = htons(1357);
  //strcpy( saddr.sun_path, SOCK_NAME );

  if ( connect( soc, ( struct sockaddr * )&saddr, ( socklen_t )sizeof( saddr ) ) < 0 ) {
    perror( "connect" );
    exit( 1 );
  }
  fprintf( stderr, "Connection established: socket %d used.\n", soc );

  while( fgets( buf, 1024, stdin ) ) {
    if ( buf[strlen(buf)-1] == '\n' ) buf[strlen(buf)-1] = '\0';
    write( soc, buf, 1024 );
    fsync( soc );
    read( soc, buf, 1024 );
    fprintf( stdout, "%s\n", buf );
  }

  close( soc );

  return 0;
}
