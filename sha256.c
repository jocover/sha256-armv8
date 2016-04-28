#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

void sha256_block_data_order (uint32_t *ctx, const void *in, size_t num);

// SHA-256 initial hash value
const uint32_t H_0[8] = {
	0x6a09e667,
	0xbb67ae85,
	0x3c6ef372,
	0xa54ff53a,
	0x510e527f,
	0x9b05688c,
	0x1f83d9ab,
	0x5be0cd19,
};


void sha256_print_hash(uint32_t *H, const char *title) {
	printf("-----------------------------------------------------------------------------------------\n");
	printf("| %-85s |\n", title);
	printf("-----------------------------------------------------------------------------------------\n");
	printf("|   H[0]   |   H[1]   |   H[2]   |   H[3]   |   H[4]   |   H[5]   |   H[6]   |   H[7]   |\n");
	printf("| %08x | %08x | %08x | %08x | %08x | %08x | %08x | %08x |\n", H[0], H[1], H[2], H[3], H[4], H[5], H[6], H[7]);
	printf("-----------------------------------------------------------------------------------------\n");
}


// initialize hash value
void sha256_init(uint32_t *H) {
	H[0] = 0x6a09e667;
	H[1] = 0xbb67ae85;
	H[2] = 0x3c6ef372;
	H[3] = 0xa54ff53a;
	H[4] = 0x510e527f;
	H[5] = 0x9b05688c;
	H[6] = 0x1f83d9ab;
	H[7] = 0x5be0cd19;
}


int main(int argc, char **argv) {
	unsigned int i;

	// check arguments
	if (argc != 2) {
		printf("Usage: %s <input file>\n", argv[0]);
		return -1;
	}

	// open input file
	FILE *fp = fopen(argv[1], "r");
	if (!fp) {
		printf("Error opening file '%s' for reading.\n", argv[1]);
		return -1;
	}

	// initialize hash value
	uint32_t H[8];
	memcpy(H, H_0, 8*4);

	// read file and calculate hash
	uint64_t bits = 0;
	unsigned char buffer[64];
	size_t len;
	while (len = fread(buffer, 1, sizeof(buffer), fp)) {
		bits += len * 8;

		// preserve value of len by breaking on end of file (or error)
		if (len < 64) {
			break;
		}
		sha256_block_data_order(H, buffer,1);
	}

	// add padding
	if (len < 56) {
		// padd current block to 56 byte
		buffer[len] = 0x80;
		i = len + 1;
	} else {
		// fill up current block and update hash
		buffer[len] = 0x80;
		for (i = len + 1; i < 64; i++) {
			buffer[i] = 0x00;
		}

		sha256_block_data_order(H, buffer,1);

		// add (almost) one block of zero bytes
		i = 0;
	}
	for (; i < 56; i++) {
		buffer[i] = 0x00;
	}

	// add message length in bits in big endian
	for (i = 0; i < 8; i++) {
		buffer[63 - i] = bits >> (i * 8);
	}

	sha256_block_data_order(H, buffer,1);

	// print hash
	sha256_print_hash(H, "Final Hash");

	// convert hash to char array (in correct order)
	for (i = 0; i < 8; i++) {
		buffer[i*4 + 0] = H[i] >> 24;
		buffer[i*4 + 1] = H[i] >> 16;
		buffer[i*4 + 2] = H[i] >>  8;
		buffer[i*4 + 3] = H[i];
	}

	// print hash
	printf("Hash:\t");
	for (i = 0; i < 32; i++) {
		printf("%02x", buffer[i]);
	}
	printf("\n");

	return 0;
}

