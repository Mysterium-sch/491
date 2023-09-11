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
	bool read = false;
	int reads = -1;
	unsigned int CPOL = signal_at_idx(w,4,0);
	unsigned int clk = CPOL;
	bool first = false;
	unsigned int CPHA = signal_at_idx(w,5,0);
	int inter = -1;
	bool streaming = false;

	if(CPHA == 0) {
		first = true;
	}

	for (unsigned int j = 0; j < w->nsamples-4; j++) {
		int cur_sig = 0;
		for (unsigned int k = 0; k<w->nsignals; k++) {
			uint32_t value = signal_at_idx(w,cur_sig,cur);
			cur_sig = cur_sig + 1;
			if(k==0) {
				clk = value;
				if(first || (CPOL==CPHA && (clk==1 && signal_at_idx(w,0,cur+1)==0)) || (CPOL!=CPHA && (clk==0 && signal_at_idx(w,0,cur+1)==1))) {
					read = true;
					reads = reads + 1;
					inter = inter + 1;
					first = false;
				} else {
					read = false;
				}
			}
			if(k==1 && read) {
				mosi[reads] = value;
			} else if (k==2 && read) {
				miso[reads] = value;
			}
	
		}
		cur = cur + 1; 

		if(reads==15 || (streaming && inter/7==1)) {
		                int R_W = mosi[reads-9]; 
                                int stream = mosi[reads-8];
				if(stream == 1) {
					streaming = !streaming;
				}
                                int address = mosi[reads-10] + 2*mosi[reads-11] + 4*mosi[reads-12] + 8*mosi[reads-13] + 16*mosi[reads-14] + 32*mosi[reads-15]; 
                                int value_wr[w->nsamples];
				value_wr[((reads/2)/inter)==1] = mosi[reads] + 2*mosi[reads-1] + 4*mosi[reads-2] + 8*mosi[reads-3] + 16*mosi[reads-4] + 32*mosi[reads-5] + 64*mosi[reads-6] + 128*mosi[reads-7];
                                int value_rd[w->nsamples];
				value_rd[((reads/2)/inter)==1] = miso[reads] + 2*miso[reads-1] + 4*miso[reads-2] + 8*miso[reads-3] + 16*miso[reads-4] + 32*miso[reads-5] + 64*miso[reads-6] + 128*miso[reads-7];

				if(R_W == 0) {
					printf("RD %02x", address);
                                	for(int w=0; w<=((reads/2)/inter); w++) {
                                        	printf(" %02x", value_rd[w]);
                                	}
                                printf("\n");
				} else {
					printf("WR %02x", address);
                                	for(int z=0; z<=((reads/2)/inter);z++) {
                                		printf(" %02x", value_wr[z]);
                                	}
                                	printf("\n");
				}
			       if(stream == 0) {
			       	reads = -1;
				}
			       inter = -1;
		}
		
	}

	free_waves(w);
	return 0;
}

