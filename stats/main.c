/*
 * gcc -std=gnu99 main.c -lrt -o stats
 */

#include "stats.h"

int main(){
	for(int i=0; i<100000; i++){
		HistoStart(1);
		for(int j=0; j<1000; j++){
			int x = i * j * 34 + i * 53 * j;
		}
		HistoStop(1);
	}
	HistoSummary();
}
