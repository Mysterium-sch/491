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
	int stream = 0;
	int R_W = 0;                   
	int address = 0;     
        int value_wr[w->nsamples];
        int value_rd[w->nsamples];
	int stream_length = 0;
	int cur_stream = -1;

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
					if(reads > 15) {
						cur_stream = cur_stream + 1;
					}
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

		if(reads==15 || ((cur_stream/7)>=1 && reads<15)) {
			if(reads == 15) {
		                R_W = mosi[reads-9]; 
                                stream = mosi[reads-8];
				if(stream == 1) {
					stream_length = mosi[reads] + 2*mosi[reads-1] + 4*mosi[reads-2] + 8*mosi[reads-3] + 16*mosi[reads-4] + 32*mosi[reads-5] + 64*mosi[reads-6] + 128*mosi[reads-7];
				}
                                address = mosi[reads-10] + 2*mosi[reads-11] + 4*mosi[reads-12] + 8*mosi[reads-13] + 16*mosi[reads-14] + 32*mosi[reads-15]; 
			}
			if(stream == 0) {
				value_wr[cur_stream/7] = mosi[reads] + 2*mosi[reads-1] + 4*mosi[reads-2] + 8*mosi[reads-3] + 16*mosi[reads-4] + 32*mosi[reads-5] + 64*mosi[reads-6] + 128*mosi[reads-7];
				value_rd[cur_stream/7] = miso[reads] + 2*miso[reads-1] + 4*miso[reads-2] + 8*miso[reads-3] + 16*miso[reads-4] + 32*miso[reads-5] + 64*miso[reads-6] + 128*miso[reads-7];
			}
				if(R_W == 0 && (cur_stream/7)==stream_length) {
					printf("RD %02x", address);
                                	for(int w=0; w<=(cur_stream/7); w++) {
                                        	printf(" %02x", value_rd[w]);
                                	}
                                printf("\n");
				} else if ((cur_stream/7)==stream_length) {
					printf("WR %02x", address);
                                	for(int z=0; z<=(cur_stream/7);z++) {
                                		printf(" %02x", value_wr[z]);
                                	}
                                	printf("\n");
				}
			       if((cur_stream/7) == stream_length) {
			       	reads = -1;
				cur_stream = -1;
				stream_length = 0;
				}
		}
		
	}

	free_waves(w);
	return 0;
}

