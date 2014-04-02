/*
 * PostscriptWriter.h
 *
 *  Created on: Apr 10, 2013
 *      Author: Henning
 */

#ifndef POSTSCRIPTWRITER_H_
#define POSTSCRIPTWRITER_H_

#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <cassert>
#include <cstdio>
#include <climits>

#include "../graph/Graph.h"
#include "../structures/Partition.h"
#include "../community/ClusteringGenerator.h"
#include "../distmeasures/AlgebraicDistance.h"

namespace NetworKit {

/**
 * TODO: class documentation
 * TODO: decouple coordinates used here from those in graph
 */
class PostscriptWriter {

	struct float_triple {
		float r;
		float g;
		float b;
	};

protected:
	Graph g;
	bool wrapAround;

	count numColors;
	std::vector<float_triple> psColor;

	float ps_sizex = 1020.0;
	float ps_sizey = 1020.0;
	float ps_borderx = 10.0;
	float ps_bordery = 10.0;

	float ps_scale;
	float ps_minx;
	float ps_maxx;
	float ps_miny;
	float ps_maxy;
	float ps_minz;
	float ps_maxz;

	void init(std::string filename, std::ofstream& file);
	void writeHeader(std::ofstream& file);
	void writeMacros(std::ofstream& file);
	void writeClustering(Partition& clustering, std::ofstream& file);

public:
	PostscriptWriter(const Graph& graph, bool isTorus = false);
	~PostscriptWriter();

	void write(Partition& clustering, std::string filename);
	void write(std::string filename);
};

} /* namespace NetworKit */
#endif /* POSTSCRIPTWRITER_H_ */