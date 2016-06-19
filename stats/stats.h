/*
 * stats.h
 *
 *  Created on: Oct 16, 2014
 *      Author: elvin.chua
 */

#ifndef STATS_H_
#define STATS_H_


#include <sys/time.h>
#include <stdint.h>
#include <stdio.h>

typedef struct{
	struct timeval start;
	struct timeval end;
	uint64_t totalAccum;
	uint32_t totalCount;
	uint64_t bucketAccum[51];
	uint32_t bucketCount[51];
} Histogram;

static Histogram histo[8];

static void HistoStart(int i){
		Histogram *h = histo + i;
		gettimeofday(&(h->start), 0);
		h->totalCount++;
	}

static void HistoStop(int i){
		Histogram *h = histo + i;
		gettimeofday(&(h->end), 0);
		h->totalCount++;
		
		uint64_t timeTaken = (h->end.tv_sec - h->start.tv_sec)*1000000UL + (h->end.tv_usec - h->start.tv_usec);
		h->totalAccum += timeTaken;

		uint32_t bucket = 50;
		if(timeTaken < 100UL){
			bucket = timeTaken / 5;
		}
		else if(timeTaken < 1000UL){
			bucket = (timeTaken / 100) + 20;
		}
		else if(timeTaken <= 1000000UL){
			bucket = (timeTaken / 50000) + 30;
		}

		h->bucketAccum[bucket] += timeTaken;
		h->bucketCount[bucket]++;
	}

static void HistoSummary(){
		uint32_t i = 1, j = 0;
		printf("Start - End: %% Total AvgTimingUs\n");
		for(; i<8; i++){
			Histogram h = histo[i];
			if(h.totalCount == 0) continue;

			for(j=0; j<20; ++j){
				if(h.bucketCount[j]>0){
					printf("%d - %dus: %d%% %d %.1f\n", (j*5)+1, (j+1)*5, h.bucketCount[j] * 100 /h.totalCount, h.bucketCount[j], (double) h.bucketAccum[j] / (double) h.bucketCount[j]);
				}
			}
			uint32_t x = 0;
			for(j=20; j<30; ++j){
				if(h.bucketCount[j]>0){
					printf("%d - %dus: %d%% %d %.1f\n", x+1, x+100, h.bucketCount[j] * 100 /h.totalCount, h.bucketCount[j], (double) h.bucketAccum[j] / (double) h.bucketCount[j]);
				}
				x+=100;
			}
			x=0;
			for(j=30; j<50; ++j){
				if(h.bucketCount[j] > 0){
					printf("%d - %dms: %d%% %d %.1f\n", x+1, x+50, h.bucketCount[j] * 100 /h.totalCount, h.bucketCount[j], (double) h.bucketAccum[j] / (double) h.bucketCount[j]);
				}
				x+=50;
			}

		}
}


#endif /* STATS_H_ */
