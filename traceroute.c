#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>


#define DEBUG 1

char *get_ip_str(const struct sockaddr *sa, char *s, size_t maxlen){
  switch(sa->sa_family) {
    case AF_INET:
      inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr),s, maxlen);
      break;

    case AF_INET6:
      inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr), s, maxlen);
      break;

    default:
      strncpy(s, "Unknown AF", maxlen);
      return NULL;
  }
  return s;
}

int traceroute(char* dest_hostname){
  struct addrinfo*        dest_addrinfo_collection;
  struct addrinfo*        dest_addrinfo_item;
  struct sockaddr*        dest_addr;
  int                     recv_socket;
  int                     send_socket;
  char*                   buf = "";
  char                    dest_addr_str[INET_ADDRSTRLEN];
  int                     error;
  unsigned short          iter;
  unsigned short          ttl = 1;

  /* resolve the domain name into a list of addresses */
  error = getaddrinfo(dest_hostname, NULL, NULL, &dest_addrinfo_collection);
  if (error != 0){
      fprintf(stderr, "error in getaddrinfo: %s\n", gai_strerror(error));
      return EXIT_FAILURE;
  }

  /* loop over returned results */
  for (dest_addrinfo_item = dest_addrinfo_collection, iter=1;
       dest_addrinfo_item != NULL; 
       dest_addrinfo_item = dest_addrinfo_item->ai_next, iter++){
    if(iter==1){
      dest_addr = dest_addrinfo_item->ai_addr;
      get_ip_str(dest_addr, dest_addr_str, INET_ADDRSTRLEN);
    }
    else{
      printf("\nWarning: %s has multiple addresses; using %s", dest_hostname,
                                                               dest_addr_str);
      break;
    }
  }
  freeaddrinfo(dest_addrinfo_collection);
  
  /* loop until you reach the reach the destination;
     or your TTL exceeds MAX HOPS */
  
  while(1){
    int recv_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    //int recv_socket = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
    if(recv_socket == -1){
      fprintf(stderr, "\ncannot create receive socket");
      return EXIT_FAILURE;
    }
    int send_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(send_socket == -1){
       fprintf(stderr, "\ncannot create send socket");
       return EXIT_FAILURE;
     }
    error = setsockopt(send_socket, IPPROTO_IP, IP_TTL, &ttl, sizeof(int));
    if(error != 0){
       perror("\nerror setting socket options");
       return EXIT_FAILURE;
     }
    struct sockaddr_in *dest_addr_in = (struct sockaddr_in*) dest_addr;
    dest_addr_in->sin_family = PF_INET;
    dest_addr_in->sin_port = htons(33434);
    error = sendto(send_socket, &buf, sizeof(buf),
                   0, (struct sockaddr *)dest_addr_in, sizeof(*dest_addr_in));
    if(error == -1){
       perror("\nerror sending data to destination");
       return EXIT_FAILURE;
     }

    //recvfrom()
    break;
  }

  close(recv_socket);
  close(send_socket);
  
  return EXIT_SUCCESS;
}

int main(void){
  int status = traceroute("www.facebook.com");
  return status;
}