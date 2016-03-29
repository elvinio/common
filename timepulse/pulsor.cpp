#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

void die(char *s) 
{
        perror(s);
            exit(1);
}

int main(){
    struct sockaddr_in addr;
    int s = 0;
    socklen_t addrLen = sizeof(addr);
    if((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
        die("socket");
    }   
    memset((char *) &addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(19000);
    if(inet_aton("172.18.80.37", &addr.sin_addr) == 0){ 
        die("inet_aton failed");
    }   

    struct timespec ts; 

    while(1){
        clock_gettime(CLOCK_REALTIME, &ts);
        if(sendto(s, (void *) &ts, sizeof(ts), 0, (struct sockaddr *) &addr, addrLen) == -1){
            die("sendto()");
        }
        usleep(1000000);
    }
    return 0;
}
