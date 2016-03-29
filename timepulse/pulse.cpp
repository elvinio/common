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
    char buf[512];
    socklen_t addrLen = sizeof(addr);
    if((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
        die("socket");
    }   
    memset((char *) &addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(19000);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(s, (struct sockaddr*)&addr, sizeof(sockaddr_in)) == 1){ 
        die("bind");
    }   

    struct timespec ts1, *ts2;
    int64_t tss1, tss2;
    int32_t diff;

    FILE *fp;
    fp = fopen("pulsediff", "w+");

    while(true){
        if(recvfrom(s, buf, 512, 0, (struct sockaddr *) &addr, &addrLen) == -1){
            die("recvfrom()");
        }
        clock_gettime(CLOCK_REALTIME, &ts1);
        tss1 = ts1.tv_sec * 1000000000 + ts1.tv_nsec;
        ts2 = (timespec *) buf;
        tss2 = ts2->tv_sec * 1000000000 + ts2->tv_nsec;
        diff = tss2 - tss1;

        fprintf(fp, "%lld %lld %d\n", tss1, tss2, diff);
    }   
    fclose(fp);
    return 0;
}
