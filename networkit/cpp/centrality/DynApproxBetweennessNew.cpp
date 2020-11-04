#include <vector>

#include "DynApproxBetweennessImpl.hpp"
#include <networkit/auxiliary/Log.hpp>
#include <networkit/centrality/DynApproxBetweennessNew.hpp>
#include <networkit/dynamics/GraphEvent.hpp>

// networkit-format

namespace NetworKit {

DynApproxBetweennessNew::DynApproxBetweennessNew(const Graph &G, const double epsilon,
                                                 const double delta, const bool storePredecessors,
                                                 const double universalConstant)
    : impl(new DynApproxBetweennessImpl(Centrality(G, true), G, true, G, epsilon, delta,
                                        storePredecessors, universalConstant)) {}

DynApproxBetweennessNew::~DynApproxBetweennessNew() = default;

void DynApproxBetweennessNew::run() {
    impl->run();
}

void DynApproxBetweennessNew::update(GraphEvent e) {
    impl->update(e);
}

void DynApproxBetweennessNew::updateBatch(const std::vector<GraphEvent> &e) {
    impl->updateBatch(e);
}

count DynApproxBetweennessNew::getNumberOfSamples() {
    return impl->getNumberOfSamples();
}
} // namespace NetworKit
