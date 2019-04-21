#include "PrefixVarint.h"

namespace NetworKit {

PrefixVarint::PrefixVarint() {}

std::vector<uint8_t> PrefixVarint::encode(const std::vector<uint64_t> &in) {
	
	for(auto y : in) {
		std::cout<<"in: "<<y<<std::endl;
	}
	std::vector<uint8_t> out;
	for (auto x : in) {
    	unsigned bits = 64 - count_leading_zeros_64(x | 1);
    	unsigned bytes = 1 + (bits - 1) / 7;
    	if (bits > 56) {
      		out.push_back(0);
      		bytes = 8;
    	} else {
      		x = (2 * x + 1) << (bytes - 1);
    	}
    	for (unsigned n = 0; n < bytes; n++) {
    		out.push_back(x & 0xff);
      		x >>= 8;
		}
  	}
	return out;
}

size_t PrefixVarint::encode(uint64_t value, uint8_t* buffer) {
    
    //While more than 7 bits of data are left, occupy the last output byte
    // and set the next byte flag
    size_t outputSize = 0;
    while (value > 127) {
        buffer[outputSize] = ((uint8_t)(value & 127)) | 128;
        value >>= 7;
        outputSize++;
    }

    buffer[outputSize++] = ((uint8_t)value) & 127;
    return outputSize;
}

uint64_t PrefixVarint::decode(uint8_t* data) {
		
    int i = 0;
    uint64_t decoded = 0;
    int shift = 0;

    do {
        decoded |= (uint64_t)(data[i] & 0x7F) << shift;     
        shift += 7;
    } while ((data[i++] & 0x80) != 0 );

    return decoded;
}

} /* namespace */
