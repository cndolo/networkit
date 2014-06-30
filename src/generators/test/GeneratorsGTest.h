/*
 * GeneratorsGTest.h
 *
 *  Created on: 09.04.2013
 *      Author: cls
 */

#ifndef NOGTEST

#ifndef GENERATORSGTEST_H_
#define GENERATORSGTEST_H_

#include <gtest/gtest.h>
#include <cmath>

#include "../DynamicGraphSource.h"
#include "../DynamicBarabasiAlbertGenerator.h"
#include "../PubWebGenerator.h"
#include "../DynamicPubWebGenerator.h"
#include "../ErdosRenyiGenerator.h"
#include "../../viz/PostscriptWriter.h"
#include "../../clustering/ClusteringGenerator.h"
#include "../../community/PLP.h"
#include "../../community/PLM2.h"
#include "../../io/METISGraphWriter.h"
#include "../../io/DotGraphWriter.h"
#include "../BarabasiAlbertGenerator.h"
#include "../../io/GraphIO.h"
#include "../../io/METISGraphReader.h"
#include "../../properties/GraphProperties.h"
#include "../../clustering/Modularity.h"
#include "../../dynamics/GraphUpdater.h"
#include "../../auxiliary/MissingMath.h"

namespace NetworKit {

class GeneratorsGTest: public testing::Test {
public:
	GeneratorsGTest();
	virtual ~GeneratorsGTest();
};

} /* namespace NetworKit */
#endif /* GENERATORSGTEST_H_ */

#endif /*NOGTEST */