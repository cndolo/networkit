/*
 * BinaryGraphWriter.h
 *
 *@author Charmaine Ndolo <charmaine.ndolo@b-tu.de>
 */

#ifndef BINARYGRAPHWRITER_H
#define BINARYGRAPHWRITER_H

#include "../graph/Graph.h"
#include "GraphWriter.h"
#include "../auxiliary/Enforce.h"
#include "PrefixVarint.h"

#include <fstream>

namespace NetworKit {
	
class BinaryGraphWriter : public GraphWriter {
	
public:
		
	void write(const Graph& G, const std::string& path);

protected:
	count nodes;
	count chunks;	
	
};
} /* namespace */
#endif
