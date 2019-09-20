#ifndef PREFIXVARINT_H 
#define PREFIXVARINT_H

#include "Compiler.h"
#include <vector>

namespace NetworKit {

class PrefixVarint {
	
public:
	PrefixVarint();
	
	std::vector<uint8_t> encode(const std::vector<uint64_t> &in);

	size_t encode(uint64_t value, uint8_t* buffer);
			
	uint64_t decode(uint8_t *data);
		
		
private:
	inline size_t prefix_length(const uint8_t *p) {
		return 1 + count_trailing_zeros_32(*p | 0x100);
	}

	inline uint64_t prefix_get(const uint8_t *p, size_t length) {
		if (length < 9) {
    		size_t unused = 64 - 8 * length;
    		return unaligned_load_u64(p) << unused >> (unused + length);
  		} else {
    		return unaligned_load_u64(p + 1);
  		}
	}
};
} /*namespace */
#endif
