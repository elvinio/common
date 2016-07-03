/*
 * gcc -std=gnu99 example.c -lrt -o example
 */

#include "timer.h"

int main(){
	HistoInit();
	for(int i=0; i<100000; i++){
		HistoStart(1);
		for(int j=0; j<1000; j++){
			int x = i * j * 34 + i * 53 * j / 86513;
		}
		HistoStop(1);
	}
}
