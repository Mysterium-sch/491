/*
 * Copyright 2021 Jason Bakos, Philip Conrad, Charles Daniels
 *
 * Distributed as part of the University of South Carolina CSCE317 course
 * materials. Please do not redistribute without written authorization.
 */

#include <stdio.h>
#include "waves.h"

int hex(int,int,int,int,int,int);

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
	int cur = 3;
	int mosi[w->nsamples];
	int miso[w->nsamples];
	int count_mosi = 0;
	int count_miso = 0;
	bool read = false;
	unsigned int clk = signal_at_idx(w,o,4);
	bool CPHA_first = false;
		if (signal_at_idx(w,0,5) == 0) { CPHA_first = true; }
	unsigned int CPHA = signal_at_idx(w,5,0);
	for (unsigned int j = 0; j < w->nsamples-4; j++) {
		int cur_sig = 0;
		for (unsigned int k = 0; k<w->nsignals; k++) {
			uint32_t value = signal_at_idx(w,cur_sig,cur);
			cur_sig = cur_sig + 1;
			if(k==0) {
				clk = value;
				if(CPHA_first || (clk==(1-CPHA) && signal_at_idx(w,0,cur+1)==CPHA)) {
					read = true;
					CPHA_first = false;
				} else {
					read = false;
				}
			}
			if(k==1 && read) {
				mosi[count_mosi] = value;
				count_mosi = count_mosi + 1;
			} else if (k==2 && read) {
				miso[count_miso] = value;
				count_miso = count_miso +1;
			}
	
		}
		cur = cur + 1; 
		                        if(count_mosi % 16 == 0 && read) {
                                int R_W = mosi[count_mosi-10]; 
                                //int stream = mosi[count_mosi-9];
                                int address = mosi[count_mosi-16] + 2*mosi[count_mosi-15] + 4*mosi[count_mosi-14] + 8*mosi[count_mosi-13] + 16*mosi[count_mosi-12] + 32*mosi[count_mosi-11];
				int value_wr = mosi[count_mosi-8] + 2*mosi[count_mosi-7] + 4*mosi[count_mosi-6] + 8*mosi[count_mosi-5] + 16*mosi[count_mosi-4] + 32*mosi[count_mosi-3] + 64*mosi[count_mosi-3] + 128*mosi[count_mosi-1];
				int value_rd = miso[count_miso-8] + 2*miso[count_miso-7] + 4*miso[count_miso-6] + 8*miso[count_miso-5] + 16*miso[count_miso-4] + 32*miso[count_miso-3] + 64*miso[count_miso-3] + 128*miso[count_miso-1];
                                if(R_W == 0) {
					printf("RD %x %x\n", address, value_rd);
				} else {
					printf("WR %x %x\n", address, value_wr);
				}
				}
	}
	printf("end program\n");

	free_waves(w);
	return 0;
}

