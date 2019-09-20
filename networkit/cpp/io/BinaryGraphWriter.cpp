/*
 * BinaryGraphWriter.cpp
 *
 *@author Charmaine Ndolo <charmaine.ndolo@b-tu.de>
 */

#include "BinaryGraphWriter.h"

namespace NetworKit {
	
	void BinaryGraphWriter::write(const Graph& G, const std::string& path) {
	
		std::ofstream outfile(path, std::ios::binary);
		Aux::enforceOpened(outfile);
		PrefixVarint pv;
			
		std::vector<count> pref_sum;
		std::vector<count> node_pref_sum;

		uint64_t POS = 0; //position in file

		if(!G.isEmpty()){
			
			count prefSum = 0;
			nodes = G.numberOfNodes();
			chunks = nodes;
			
			std::vector<std::vector<uint64_t>>nbrs;
			std::vector<size_t> enc_arr_size; //  pref sum of size encoded adj arrays
			std::vector<size_t> enc_node_size; // size of encoded node ids

			/* GET ALL ADJ ARRAYS */
			size_t node_size = 0;
				count sum = 0;
			if(!G.isDirected()) {
				G.forNodes([&](node u){
					std::vector<uint64_t> nbr;
					uint8_t tmp [10];
					node_size = pv.encode(u,tmp);
					enc_node_size.push_back(node_size);
					G.forNeighborsOf(u,[&](node v){
					if (v <= u)
						nbr.push_back(v);
					});
					sum += nbr.size();
					node_pref_sum.push_back(sum);
					nbrs.push_back(nbr);
				});		
			} else {
				G.forNodes([&](node u){		
					std::vector<uint64_t> nbr;
					G.forNeighborsOf(u,[&](node v){
						nbr.push_back(v);
					});
					sum += nbr.size();
					node_pref_sum.push_back(sum);
					nbrs.push_back(nbr);
				});
			}	
			
			/* compute encoded size of arrays and store in vector */
			size_t prev = 0;
			
			for(auto nb_it = nbrs.begin(); nb_it != nbrs.end(); ++nb_it) {
				size_t adj_size = 0;
				for(auto it = (*nb_it).begin(); it!= (*nb_it).end(); ++it ) {
					uint8_t tmp [10];
					adj_size += pv.encode(*it, tmp);
				}
				adj_size += prev;
				enc_arr_size.push_back(adj_size);
				prev = adj_size;
			}
			
			/* WRITE TO FILE */
			outfile.write(reinterpret_cast<char*>(&nodes), sizeof(uint64_t));

			outfile.write(reinterpret_cast<char*>(&chunks), sizeof(uint64_t));
			
			/* PER VERTEX INFO */ 
			G.forNodes([&] (node u) {
				uint8_t tmp[10];
				prefSum += G.degree(u);
				pv.encode(prefSum, tmp);
				outfile.write(reinterpret_cast<char*>(&node_pref_sum.at(u)), sizeof(uint64_t));
				pref_sum.push_back(prefSum);
				POS = outfile.tellp();	
			});
		
			/* PER CHUNK INFO */
			for (count i = 0; i < nodes; i++) {
				uint64_t tmp_off;
				uint64_t START_ADJ;
				if(i == 0) {
					START_ADJ = POS + (chunks * (2*sizeof(uint64_t)));
					outfile.write(reinterpret_cast<char*>(&i), sizeof(uint64_t));
					outfile.write(reinterpret_cast<char*>(&START_ADJ),sizeof(uint64_t));
				} else {
					tmp_off = START_ADJ + (enc_arr_size.at(i-1)) + i;
					outfile.write(reinterpret_cast<char*>(&i), sizeof(uint64_t));
					outfile.write(reinterpret_cast<char*>(&tmp_off),sizeof(uint64_t));
				}			   
			}		
	
			for(auto x : nbrs) {
				size_t node_size = 0;
				for(unsigned i = 0; i < x.size(); i++) {
					uint8_t tmp [10];
					uint64_t node = x.at(i);
					node_size = pv.encode(node, tmp);
					uint64_t node_enc = pv.decode(tmp);
					outfile.write(reinterpret_cast<char*>(tmp), node_size);
				}
				outfile<<std::endl;
			}
			INFO("Written graph to ", path);

		} else {
			throw std::runtime_error("G is empty.");
		} 
	}
} /*namespace */
