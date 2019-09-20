/*
 * BinaryGraphReader.cpp
 *
 *@author Charmaine Ndolo <charmaine.ndolo@b-tu.de>
 */

#include "BinaryGraphReader.h"

namespace NetworKit {

	BinaryGraphReader::BinaryGraphReader(bool directed):directed(directed) { }
	
	Graph BinaryGraphReader::read(const std::string& path) {
		
		PrefixVarint pv;
		std::vector<uint64_t> prefSum;
		std::vector<std::vector<uint64_t>> adj;
		std::vector<std::pair<uint64_t, uint64_t>> chunk_info;
		bool weighted = false;
		
		char c_path[path.length()];
		strcpy(c_path, path.c_str()); 
		FILE* infile;
		infile = fopen(c_path, "rb");


		if(infile) {
			/* read number of nodes */
			unsigned char tmp[10];

			fread(&nodes,sizeof(uint64_t),sizeof(char),infile);
			DEBUG("# nodes here = ", nodes);

			/* read number of chunks */ 
			fread(&chunks,sizeof(uint64_t),sizeof(char),infile);
			DEBUG("# chunks here = ", chunks);

			Graph G(nodes, weighted, directed);
			
			/*read prefix sum of nodes */
			for (uint64_t i = 0; i < nodes; i++) {
				uint64_t sum = 0;
				fread(&sum,sizeof(uint64_t),sizeof(char),infile);
				prefSum.push_back(sum);
			}
	
			/* read chunk info */ 
			for (uint64_t i = 0; i < chunks; i++) {
				uint64_t ch = 0;
				uint64_t off = 0;

				fread(&ch,sizeof(uint64_t),sizeof(char),infile);
				fread(&off,sizeof(uint64_t),sizeof(char),infile);
				chunk_info.push_back(std::make_pair(ch, off));
			}
	
			/*read adj arrays */ 
			node nbr;
			uint8_t tmp_node;

			for(node c = 0; c < chunks; c++) {
				std::vector<node>tmp_array;			
				uint64_t pos = chunk_info.at(c).second;
				fseek(infile, pos, SEEK_SET);	
				uint64_t size = 0;
				if (c == 0) {
					size = prefSum.at(c);
					for(uint64_t i = 0; i < size; i++) {
						fread(&tmp_node, sizeof(uint8_t),sizeof(char), infile);
						nbr = pv.decode(&tmp_node);
						tmp_array.push_back(nbr);
					}
					adj.push_back(tmp_array);
				} else {	
					size = prefSum.at(c) - prefSum.at(c-1);	
					for(uint64_t i = 0; i < size; i++) {
						fread(&tmp_node, sizeof(uint8_t),sizeof(char), infile);
						nbr = pv.decode(&tmp_node);
						tmp_array.push_back(nbr);	
					}
					adj.push_back(tmp_array);
				}
			}
			DEBUG("READ ADJ ARRAYS");
	
			/* create graph */			
			count i = 0;
			for(auto arr : adj) {
				DEBUG("ARRAY: ", i);
				node curr;
				node nxt;
				curr = chunk_info.at(i).first;	//TODO: fix this some day, only works with chunks == nodes
				i++;
				for(auto &nd: arr) {
					nxt = nd;
					G.addEdge(curr,nxt);
				}
			}
			DEBUG("reader #edges: ", G.numberOfEdges());	
			G.shrinkToFit();	
			fclose(infile);
			return G;
		} else {
			throw std::runtime_error("error opening file for reading!");
		}
	}	
} /* namespace */


