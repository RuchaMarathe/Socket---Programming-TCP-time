/* CmpE 207  Lab Assignment #1
/* Topic : Client and server software design using sockets
/* Author: Group 8
/* This TCP Client program contacts two time servers and returns the time difference between the two servers. */

#define __USE_BSD 1
#include <stdarg.h>
#include <errno.h>
#include <netinet/in.h>
#include <time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define BUFSIZE 64
#define UNIXEPOCH 2208988800 /* UNIX epoch, in UCT secs : Number of seconds from1st January 1900 */
#define MSG "what time is it?\n"

#ifndef INADDR_NONE
#define INADDR_NONE 0xffffffff
#endif
 extern int errno;
typedef unsigned short u_short;
extern int errno;
int errexit(const char *format, ...);


typedef unsigned long u_long;
extern int errno;

int connectsock(const char *host, const char *service, const char *transport);
int connectTCP(const char *host, const char *service);

int errexit(const char *format, ...);

int main(int argc, char *argv[])
{
char *host1 = "localhost";
char *host2 = "localhost";
char *service = "time";
time_t now, next;
int s, s1, n, n1;
struct tm next2;
/* Two Localhosts and a service(time) are passed as arguments to the argument vector. */
switch (argc)
{
case 1:
host1 = "localhost";
host2 = "localhost";
break;
case 3:
host1 = argv[1];
host2 = argv[2];
break;
default:
fprintf(stderr, "usage: TCPtime [host [port]]\n");
exit(1);
}

/* Establishes connectivity with the server by choosing TCP protocol. Returns a positive Integer on connecting. */

s = connectTCP(host1, service);
if(s<0)
{
    printf("no service");
}


(void) write(s, MSG, strlen(MSG));
n = read(s, (char *)&now, sizeof(now));

if (n < 0)
printf("Error in reading");     //errexit("read failed: %s\n", strerror(errno));*/
now = ntohl((u_long)now);
now =now- UNIXEPOCH;
printf("\n%s", ctime(&now));


s1 = connectTCP(host2, service);
if(s1<0)
{
    printf("no service");
}
(void) write(s1, MSG, strlen(MSG));
n1 = read(s1, (char *)&next, sizeof(next));
if (n1 < 0)
printf("Error in reading");//errexit("read failed: %s\n", strerror(errno));*/
next = ntohl((u_long)next);
next =next- UNIXEPOCH;
printf("%s",ctime(&next));

double diff;
diff = difftime(next,now);
printf("%f seconds is the time difference between the servers\n\n",diff);
exit(0);
}

/* connectsock - allocate & connect a socket using TCP or UDP */

int connectTCP(const char *host, const char *service )
{
return connectsock(host, service, "tcp");
}

/*
* Arguments:
* host - name of host to which connection is desired
* service - service associated with the desired port
* transport - name of transport protocol to use ("tcp" or "udp")
*/

int connectsock(const char *host, const char *service, const char *transport )
{
struct hostent *phe;
struct servent *pse;
struct protoent *ppe;
struct sockaddr_in sin;
int s, type;

memset(&sin, 0, sizeof(sin));
sin.sin_family = AF_INET;

/* Map service name to port number */

if ( pse = getservbyname(service, transport) )
    sin.sin_port = pse->s_port;
else if ( (sin.sin_port = htons((u_short)atoi(service))) == 0 )
    errexit("can't get \"%s\" service entry\n", service);

 /* Map host name to IP address, allowing for dotted decimal */

if ( phe = gethostbyname(host) )
    memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
else if ( (sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE )
    errexit("can't get \"%s\" host entry\n", host);

/* Map transport protocol name to protocol number */

if ( (ppe = getprotobyname(transport)) == 0)
    errexit("can't get \"%s\" protocol entry\n", transport);

/* Use protocol to choose a socket type */

if (strcmp(transport, "udp") == 0)
    type = SOCK_DGRAM;
else
    type = SOCK_STREAM;

 /* Allocate a socket */

s = socket(PF_INET, type, ppe->p_proto);

if (s < 0)
    errexit("can't create socket: %s\n", strerror(errno));


/* Connect the socket */

if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
            errexit("can't connect to %s.%s: %s\n", host, service,

strerror(errno));

return s;
}

/* errexit - print an error message and exit */

errexit(const char *format, ...)
{
va_list args;
va_start(args, format);
vfprintf(stderr, format, args);
va_end(args);
exit(1);
}
