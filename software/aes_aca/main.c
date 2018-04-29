/* LAP-I&C-EPFL
*  ACA HW3, April 2015
*
*  Encryption and Decryption with the Advanced Encryption standard
*
*  Implement a hardware accelerated version of the AES blockenc:
*    void aes_blockenc_HWacc(uint8* src, uint8* dest, uint8* keyexp);
*  according to exercise.
*
*  ALL FILES ARE COURSE-CONFIDENTIAL: DO NOT SHARE
*
*/

#include <stdio.h>
#include <sys/alt_cache.h>
#include "system.h"
#include "timer.h"
#include "utils.h"
#include "leds96_rgb.h"
#include "aes.h"
#include "altera_avalon_performance_counter.h"

#define INPUT_LENGTH 256
#define BLOCK_SZ (128 / 8)
#define BUF_SZ 64
#define TEXT_DELAY_MS 80

int main(int argc, char *argv[])
{
    int i, j;
    uint8 outputdata[BLOCK_SZ];
    uint8 decodedata[BLOCK_SZ];
    uint8 keyexp[aes_keyexpsize128];
    char buf[BUF_SZ] = {0};
	int done = 0;

    unsigned long long tick_count = 0, tick_count_baseline = 0;
    void* PERF_UNIT_BASE = (void*)COUNTER_0_BASE;

    // Init the input and key to a known test vector (See FIPS197, Appendix B)
    // Warning: The SWacc version only works if its arguments are aligned
    // to word-boundaries. That should be the case by default, but be wary
    // if you add declarations in main()
    uint8 inputdata[BLOCK_SZ] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};
    uint8 key[BLOCK_SZ] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};

    // clear leds
    leds96_reset();

    // Reset the performance counter
    PERF_RESET (PERF_UNIT_BASE);
	// Get the baseline target:
	{
    	printf("Starting Baseline (SWacc) Measurement\n");
        // Start the performance counter
        PERF_START_MEASURING (PERF_UNIT_BASE);

        aes_keyexpand128(key, keyexp);
        for(i = 0; i < INPUT_LENGTH; i++)
        {
            aes_blockenc_SWacc(inputdata, outputdata, keyexp);
        }

        // Stop the performance counter
        PERF_STOP_MEASURING (PERF_UNIT_BASE);

        // Show the performance data
        tick_count_baseline = perf_get_section_time(PERF_UNIT_BASE, 0);
        printf("\t-> operation took %llu cycles\n", tick_count_baseline);
	}

    // Reset the performance counter
    PERF_RESET (PERF_UNIT_BASE);
	// Do the student version:
	{
		printf("Starting HWacc Measurement\n");
		// Start the performance counter
		PERF_START_MEASURING (PERF_UNIT_BASE);

		alt_dcache_flush_all();
		IOWR(AES_ACCELERATOR_0_BASE,3,0);	//stop accelerator
		IOWR(AES_ACCELERATOR_0_BASE,2,INPUT_LENGTH);
		IOWR(AES_ACCELERATOR_0_BASE,0,inputdata);
		IOWR(AES_ACCELERATOR_0_BASE,1,outputdata);
		IOWR(AES_ACCELERATOR_0_BASE,4,*(uint32 *)(key));
		IOWR(AES_ACCELERATOR_0_BASE,5,*(uint32 *)(key + 4));
		IOWR(AES_ACCELERATOR_0_BASE,6,*(uint32 *)(key + 8));
		IOWR(AES_ACCELERATOR_0_BASE,7,*(uint32 *)(key + 12));
		IOWR(AES_ACCELERATOR_0_BASE,3,1);	//start accelerator

		while (done == 0) {
			done = IORD(AES_ACCELERATOR_0_BASE,3);//check if all numbers are done
		}

		// Stop the performance counter
		PERF_STOP_MEASURING (PERF_UNIT_BASE);

		printf("Key %x %x %x %x\n", *(uint32 *)(key),*(uint32 *)(key+4),*(uint32 *)(key+8),*(uint32 *)(key+12));
		printf("Data %x %x %x %x\n", *(uint32 *)(inputdata),*(uint32 *)(inputdata+4),*(uint32 *)(inputdata+8),*(uint32 *)(inputdata+12));
		printf("Data %x %x %x %x\n", *(uint32 *)(outputdata),*(uint32 *)(outputdata+4),*(uint32 *)(outputdata+8),*(uint32 *)(outputdata+12));
		printf("Data %x\n", (uint32)outputdata);
		printf("Data %x\n", (uint32)inputdata);
		// Show the performance data
		tick_count = perf_get_section_time(PERF_UNIT_BASE, 0);
		printf("\t-> operation took %llu cycles\n", tick_count);
	}


    // Test correctness of a block
    Decrypt(outputdata, keyexp, decodedata);  // decrypt
    j = 0;
    for(i = 0; i < 16; i++)
    {
        if(inputdata[i] != decodedata[i])
        {
            printf("Data error at index %i, in = 0x%X, dec = 0x%X\n", i,
                    inputdata[i], decodedata[i]);
            j = 1;
        }
    }

    /*------------------------------------------------------------------*/
    // If there's not enough code space, comment from here ...
    if(j == 0){
        printf("Correct!\n");

    	// init the global timer
    	timer_start_global();

        unsigned long t1 = (unsigned long)(tick_count_baseline & 0xFFFFFFFF);
        unsigned long t2 = (unsigned long)(tick_count & 0xFFFFFFFF);
        float speedup = ((float)t1) / ((float)t2);
        unsigned long whole = (unsigned long)speedup;
        unsigned long mantissa2 = (speedup * 100.0) - (whole * 100);

        snprintf(buf, BUF_SZ, "%lu cycles -> %lu.%.2lux faster", t2, whole, mantissa2);
    }

    else{
    	snprintf(buf, BUF_SZ, "%s", "Data error!");
    }

    unsigned long color;
    for(color = 0xF33333; ; color += 0x2222){
    	utils_display_text(buf, TEXT_DELAY_MS, color, 0x00000000);
    	printf("Finished. %s\n", buf);
    	timer_wait(1000);
    }
    // ... to here.
    /*------------------------------------------------------------------*/

    return 0;
}

/* Student TODO: Implement me! */
void aes_blockenc_HWacc(uint8* src, uint8* dest, uint8* key){

}

