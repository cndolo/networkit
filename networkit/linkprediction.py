
from _NetworKit import KatzIndex, CommonNeighborsIndex, JaccardIndex, PreferentialAttachmentIndex, AdamicAdarIndex, UDegreeIndex, VDegreeIndex, AlgebraicDistanceIndex, NeighborhoodDistanceIndex, TotalNeighborsIndex, NeighborsMeasureIndex, SameCommunityIndex, AdjustedRandIndex, ResourceAllocationIndex, RandomLinkSampler, ROCMetric, KFoldCrossValidator, PrecisionRecallMetric, MissingLinksFinder, LinkThresholder, PredictionsSorter

from .graph import Graph

import numpy as np
# Only necessary to fix GML, remove afterwards
import re, string

try:
  import sklearn
except ImportError:
  print(""" WARNING: module 'sklearn' not found, supervised link prediction won't be available """)

# Maybe the methods below should be encapsulated into a "SupervisedPrediction" class?

def trainClassifier(trainingSet, trainingGraph, classifier, *linkPredictors):
  """ Trains the given classifier with the feature-vectors generated by the given linkPredictors.

  Parameters
  ----------
  trainingSet : vector[pair[node, node]]
    Vector of node-pairs to generate features for,
  trainingGraph : Graph
    Training graph containing all edges from the training set.
  classifier:
    Scikit-learn classifier to train.
  linkPredictors:
    Predictors used for the generation of feature-vectors.
  """
  # Make sure the set is sorted because the samples will be sorted by node-pairs (asc.)
  # and the labels would be sorted by the initial order. That would lead to an incorrect
  # matching between labels and samples.
  trainingSet.sort()
  trainingLabels = getLabels(trainingSet, trainingGraph)
  trainingFeatures = getFeatures(trainingSet, *linkPredictors)
  classifier.fit(trainingFeatures, trainingLabels)
  
def getFeatures(nodePairs, *linkPredictors):
  """ Returns a numpy-array containing the generated scores from the predictors for the given node-pairs.

  Parameters
  ----------
  nodePairs : vector[pair[node, node]]
    Node-pairs to get the samples for.
  *linkPredictors
    List of link predictors to use for sample-generation.

  Returns
  -------
  A numpy-array of shape (#nodePairs, #linkPredictors) containing the generated scores
  from the predictors for the given node-pairs.
  """
  return np.column_stack(([list(zip(*p.runOnParallel(nodePairs)))[1] for p in linkPredictors]))

def getLabels(nodePairs, G):
  """ Returns a numpy-array containing the labels of the given node-pairs.

  The labels are defined as follows: 1 = link, 0 = absent link.

  Parameters
  ----------
  nodePairs : vector[pair[node, node]]
    Node-pairs to get the labels for.
  G : Graph
    Graph which provides ground truth for the labels.

  Returns
  -------
  A numpy-array containing the labels of the given node-pairs.
  """
  return np.array(list(map(lambda p: 1 if G.hasEdge(p[0], p[1]) else 0, nodePairs)))

# This should probably move to graphio or sth. similar
def readGraph(file, percentLinks):
  """ Reads a time-based graph and returns a pair of graphs encompassing the original graph and a subgraph.

  The subgraph is constructed by copying the first percentLinks percent of links from the original graph
  to the subgraph with respect to the date of creation.

  Parameters
  ----------
  file : str
    Path to the file to read the graph from.
  percentLinks : double
    Percentage of links to store in the training graph.

  Returns
  -------
  A pair of graphs where the first graph contains all the links from the file whereas
  the second graph contains only the first (timewise) percentLinks percent of links.
  """
  # reads the konect file contained in "file" and returns two graphs: G (full
  # graph read from the file) and G1 (equal to G, but without the last nEdges
  # edges, i.e. the ones that have to be predicted)
  f = open(file, "r")
  n = 0
  filelist = []
  #first scan to find out the number of nodes
  for line in f:
    fields = line.strip().split()
    if fields[0].startswith("%"):
      continue
    (u, v, weight, time) = [int(i) for i in fields]
    if u == v:
      continue
    filelist.append((time, u-1, v-1))
    n = max(u, v, n)
  # we sort filelist by time
  filelist.sort()
  G = Graph(n)
  # we create the graph. if an edge is created between a pair of nodes that were already connected by an edge, it is ignored and removed from the list
  filelist2 = []
  for index, (time, u, v) in enumerate(filelist):
    if not G.hasEdge(u, v):
      G.addEdge(u, v)
      filelist2.append([time, u, v])
  filelist = filelist2
  # now G is the graph with all the edges. we want to remove the last nEdges edges, creating G1
  G1 = Graph(G)
  nEdges = int(percentLinks * G.numberOfEdges())
  for (time, u, v) in filelist[-nEdges:]:
    G1.removeEdge(u, v)
  return G, G1

def readTestTrainFeature(file, percentTrain, percentFeature):
  """ Reads a time-based graph and returns a pair of graphs encompassing the original graph and a subgraph.

  The subgraph is constructed by copying the first percentLinks percent of links from the original graph
  to the subgraph with respect to the date of creation.

  Parameters
  ----------
  file : str
    Path to the file to read the graph from.
  percentLinks : double
    Percentage of links to store in the training graph.

  Returns
  -------
  A pair of graphs where the first graph contains all the links from the file whereas
  the second graph contains only the first (timewise) percentLinks percent of links.
  """
  # reads the konect file contained in "file" and returns two graphs: G (full
  # graph read from the file) and G1 (equal to G, but without the last nEdges
  # edges, i.e. the ones that have to be predicted)
  f = open(file, "r")
  n = 0
  filelist = []
  #first scan to find out the number of nodes
  for line in f:
    fields = line.strip().split()
    if fields[0].startswith("%"):
      continue
    (u, v, weight, time) = [int(i) for i in fields]
    if u == v:
      continue
    filelist.append((time, u-1, v-1))
    n = max(u, v, n)
  # we sort filelist by time
  filelist.sort()
  G = Graph(n)
  # we create the graph. if an edge is created between a pair of nodes that were already connected by an edge, it is ignored and removed from the list
  filelist2 = []
  for index, (time, u, v) in enumerate(filelist):
    if not G.hasEdge(u, v):
      G.addEdge(u, v)
      filelist2.append([time, u, v])
  filelist = filelist2
  # now G is the graph with all the edges. we want to remove the last nEdges edges, creating G1
  G1 = Graph(G)
  nEdges = int(percentTrain * G.numberOfEdges())
  for (time, u, v) in filelist[-nEdges:]:
    G1.removeEdge(u, v)

  filelist = filelist[:len(filelist) - nEdges]
  G2 = Graph(G1)
  nEdges = int(percentFeature * G1.numberOfEdges())
  for (time, u, v) in filelist[-nEdges:]:
    G2.removeEdge(u, v)
  return G, G1, G2


def fixGML(filepath):
  directed = 0
  f = open(filepath, "r")
  i = 0
  state = 0
  directed = '0'
  awaitNodeId = False
  awaitEdgeId = False
  nodes = []
  edges = []
  source = -1
  for line in f:
    cleanedLine = re.sub(r'[^\s\w_]+', '', line).strip()
    if state == 0: # graph key
      if cleanedLine == 'graph':
        state += 1
    elif state == 1: # directed
      p = cleanedLine.split()
      if len(p) > 0:
        if p[0] == 'directed':
          directed = p[1]
          state += 1
    else: # nodes or edges
      p = cleanedLine.split()
      if len(p) == 0:
        continue
      if p[0] == 'node':
        awaitNodeId = True
        awaitEdgeId = False
      if p[0] == 'edge':
        awaitNodeId = False
        awaitEdgeId = True
      if awaitNodeId and p[0] == 'id':
        nodes.append(int(p[1]))
        awaitNodeId = False
      if awaitEdgeId and p[0] == 'source':
        source = int(p[1])
      elif awaitEdgeId and p[0] == 'target':
        edges.append((source, int(p[1])))
        awaitEdgeId = False
  f.close()
  if state != 2:
    return
  n = open(filepath + "_new", "w")
  n.write("graph [\n")
  n.write("  directed " + directed + "\n")
  for node in nodes:
    n.write("  node [\n")
    n.write("    id " + str(node) + "\n")
    n.write("  ]\n")
  for edge in edges:
    n.write("  edge [\n")
    n.write("    source " + str(edge[0]) + "\n")
    n.write("    target " + str(edge[1]) + "\n")
    n.write("  ]\n")

  n.write("]")
  n.close()