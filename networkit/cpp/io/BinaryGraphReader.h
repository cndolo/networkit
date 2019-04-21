/*
 * BinaryGraphReader.h
 *
 *@author Charmaine Ndolo <charmaine.ndolo@b-tu.de>
 */
#ifndef BINARY_GRAPH_READER_H
#define BINARY_GRAPH_READER_H

#include "../graph/Graph.h"
#include "PrefixVarint.h"
#include "../auxiliary/Log.h"
#include "GraphReader.h"
#include <vector>
#include <fstream>

namespace NetworKit {
class BinaryGraphReader : public GraphReader {

public:		
	BinaryGraphReader(bool directed);
	
	Graph read(const std::string& path) override; 

private:
	count nodes;
	count chunks;
	bool directed;	
};	
} /* namespace */

#endif
