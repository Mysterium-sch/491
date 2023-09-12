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
	
	}
	
	int inter = -1;
	for(int why=-1; why<=reads; why++) {
	
	if(inter == 15) {
				inter = -1;
		                R_W = mosi[why-9]; 
                                stream = mosi[why-8];
				address = mosi[why-10] + 2*mosi[why-11] + 4*mosi[why-12] + 8*mosi[why-13] + 16*mosi[why-14] + 32*mosi[why-15]; 
				if(stream == 1) {
					//Pull next stream_length mosi/miso values
					stream_length = mosi[why] + 2*mosi[why-1] + 4*mosi[why-2] + 8*mosi[why-3] + 16*mosi[why-4] + 32*mosi[why-5] + 64*mosi[why-6] + 128*mosi[why-7];
					for(int tr=1; tr<stream_length+1; tr++) {
						value_wr[tr-1] = mosi[why+(tr*8)] + 2*mosi[why+(tr*8-1)] + 4*mosi[why+(tr*8-2)] + 8*mosi[why+(tr*8-3)] + 16*mosi[why+(tr*8-4)] + 32*mosi[why+(tr*8-5)] + 64*mosi[why+(tr*8-6)] + 128*mosi[why+(tr*8-7)];
						value_rd[tr-1] = miso[why+(tr*8)] + 2*miso[why+(tr*8-1)] + 4*miso[why+(tr*8-2)] + 8*miso[why+(tr*8-3)] + 16*miso[why+(tr*8-4)] + 32*miso[why+(tr*8-5)] + 64*miso[why+(tr*8-6)] + 128*miso[why+(tr*8-7)];
					}
					why = why + stream_length*8;
				} else {
				value_wr[0] = mosi[why] + 2*mosi[why-1] + 4*mosi[why-2] + 8*mosi[why-3] + 16*mosi[why-4] + 32*mosi[why-5] + 64*mosi[why-6] + 128*mosi[why-7];
				value_rd[0] = miso[why] + 2*miso[why-1] + 4*miso[why-2] + 8*miso[why-3] + 16*miso[why-4] + 32*miso[why-5] + 64*miso[why-6] + 128*miso[why-7];
				}
			// Print command
				if(R_W == 0) {
					if(stream_length>=1) {
						printf("RD STREAM %02x", address);
						for(int w=0; w<stream_length; w++) {
                                        	printf(" %02x", value_rd[w]);
                                	}
					}
					else {
						printf("RD %02x %02x", address, value_rd[0]);
					}
                                printf("\n");
				} else {
					if(stream_length>=1) {
                                        	printf("WR STREAM %02x", address);
                                        	for(int z=0; z<stream_length;z++) {
                                		printf(" %02x", value_wr[z]);
                                	}
                                        }
                                        else {
                                             printf("WR %02x %02x", address, value_wr[0]);
                                        }    
                     
                                	printf("\n");
				}
				why = why-1;
				stream_length = 0;
		} else {
		inter = inter +1;
		}
}
	free_waves(w);
	return 0;
}

