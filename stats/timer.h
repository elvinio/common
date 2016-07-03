/*
 * time.h
 * Times the difference and sends the data out via UDP
 *
 *  Created on: July 2, 2016
 *      Author: elvin.chua
 */

#ifndef STATS_H_
#define STATS_H_


#include <sys/time.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

typedef struct{
	uint32_t id;
	uint32_t timeTaken;
} TimePacket;

static struct timeval startTime[32];
static int histoUdpSock;
static struct sockaddr_in histoAddr;

static void HistoInit(){
    histoUdpSock = 0;
    histoUdpSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    memset((char *) &histoAddr, 0, sizeof(histoAddr));
    histoAddr.sin_family = AF_INET;
    histoAddr.sin_port = htons(19000);
    inet_aton("127.0.0.1", &histoAddr.sin_addr);
}
static void HistoStart(int i){
	gettimeofday(&(startTime[i]), 0);
}

static void HistoStop(int i){
	struct timeval end;
	gettimeofday(&end, 0);
	TimePacket p;
	p.id = i;
	p.timeTaken = (end.tv_sec - startTime[i].tv_sec) * 1000000 + (end.tv_usec - startTime[i].tv_usec);
	if(histoUdpSock > 0){
        sendto(histoUdpSock, (void *) &p, sizeof(p), 0, (struct sockaddr *) &histoAddr, sizeof(histoAddr));
	}
}


#endif /* STATS_H_ */
