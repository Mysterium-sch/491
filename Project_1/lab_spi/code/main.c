/*
 * Copyright 2021 Jason Bakos, Philip Conrad, Charles Daniels
 *
 * Distributed as part of the University of South Carolina CSCE317 course
 * materials. Please do not redistribute without written authorization.
 */

#include <stdio.h>
#include "waves.h"

int main(int argc, char** argv) {
	/* This macro silences compiler errors about unused variables. */
	UNUSED(argc);
	UNUSED(argv);

	/* Read the input for the test case into a new waves object. */
	waves* w = parse_file(stdin);
	/* We can use the 'log' function like printf() to record information to
	 * standard error, this way it will not interfere with the data sent to
	 * standard out we wish for grade.sh to interpret as our solution to
	 * the testcase. */
	log("read a waves file with %i signals and %i samples\n", w->nsignals, w->nsamples);
	log("input has these signals:\n");
	for (unsigned int i = 0 ; i < w->nsignals ; i++) {
		// index2signal() lets us get the human-readable name of
		// the signal from its index.
		//
		// w->widths[i] tells us the number of bits in signal i.
		log("\t* %s (%i bits)\n", index2signal(w, i), w->widths[i]);
		//printf("Nsignals = %d, nsamples = %d\n", w->nsignals, w->nsamples);
	}
	int cur = 0;
	int mosi[w->nsamples];
	int miso[w->nsamples];
	int count_mosi = 0;
	int count_miso = 0;
	for (unsigned int j = 0; j < w->nsamples; j++) {
		int cur_sig = 0;
		for (unsigned int k = 0; k<w->nsignals; k++) {
			uint32_t value = signal_at_idx(w,cur_sig,cur);
			cur_sig = cur_sig + 1;
			if(k==1) {
				mosi[count_mosi] = value;
				count_mosi = count_mosi + 1;
			} else if (k==2) {
				miso[count_miso] = value;
				printf("%i\t", miso[count_miso]);
			}
			if(count_mosi % 8 == 0) {
				int R_W = mosi[count_mosi-1];
				int stream = mosi[count_mosi];
				int address = hex(mosi[count_mosi-7], mosi[count_mosi-6],mosi[count_mosi-5], mosi[count_mosi-4],mosi[count_mosi-3], mosi[count_mosi-2]);
				if(R_W == 0) {
				}
			}
		}
		cur = cur + 1; 
	
	}

	free_waves(w);
	return 0;
}

int hex(int b1, int b2, int b3, int b4, int b5, int b6) {
	int total


	return total;
}
