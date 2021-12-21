#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <cctype>
#include <codecvt>
#include <locale>
#include "lib/json.hpp"
#include "lib/english_stem.hpp"
#include "utils.h"

using json = nlohmann::json;



int main() {
	
	std::unordered_map<std::string, int> lexicon;
	std::unordered_map<int, std::string> doc_index;
	std::unordered_map<int, std::unordered_map<int, std::vector<short int>>> forward_index;
	int num = 0;
	int doc_id = 0;
	
	std::vector<std::string> words;
	//std::vector<int> word_id_list;
	// get all the files from the folder
	std::ifstream files_names_file;
	files_names_file.open("files.txt");

	std::string temp_file_name;
	std::vector<std::string> all_files_names;
	while(std::getline(files_names_file, temp_file_name)) {
		all_files_names.push_back(temp_file_name);
	}

	files_names_file.close();

	std::cout << "Building lexicon, doc_index, forward_index...." << "\n";
	for (int a = 0; a < all_files_names.size(); a++) {
		std::ifstream ifs(all_files_names[a]);
		nlohmann::json jf = nlohmann::json::parse(ifs);
		
		for (int i = 0; i < jf.size(); i++) {
			//std::cout << i << std::endl;
			//word_id_list.clear();

			std::string doc_id_string = jf[i]["url"];
			doc_index[doc_id] = doc_id_string;
			//std::cout << doc_id << std::endl;
			

			words = split(jf[i]["content"], ' ');
			words = sanitize_and_stem_list(words);

			remove_stopwords(words);
			//std::sort(words.begin(), words.end()); 
			words.erase(std::remove(words.begin(), words.end(), ","), words.end());
			//words.erase(std::unique(words.begin(), words.end()), words.end()); // remove duplicate words

			//words.erase(words.begin()); // remove the comma (because it is the first element)

			// remove stopwords
			

			for (int j = 0; j < words.size(); j++) {
				if(lexicon.find(words[j]) == lexicon.end()) { // if not in lexicon
					lexicon[words[j]] = num++;
				}
			}

			std::unordered_map<int, std::vector<short int>> hit_list;
			std::vector<short int> temp_list;
			for(short int j = 0; j < words.size(); j++) {
				temp_list.clear();
				if (hit_list.find(lexicon[words[j]]) == hit_list.end()) {
					std::string title = jf[i]["title"];
					if (title.find(words[j]) != std::string::npos) {
    					temp_list.push_back(-1); // fancy
					} /*else {
						temp_list.push_back(-2);
					}*/
					temp_list.push_back(j);
					hit_list[lexicon[words[j]]] = temp_list;
				} else {
					hit_list[lexicon[words[j]]].push_back(j);
				}
			}

			/*int temp_word_id = 0;
			for (int j = 0; j < words.size(); j++) {
				temp_word_id = lexicon[words[j]];
				word_id_list.push_back(temp_word_id);
				
			}*/

			forward_index[doc_id] = hit_list;
			doc_id++;
			
		}
	}


	//std::wstring tname = L"Organization";
	//stemming::english_stem<> obj;
	//obj(tname);

	std::cout << "Writing lexicon to files...." << "\n";

    nlohmann::json lex_writer;
    std::ofstream lex_file("lexicon.json");
    
	for (auto it : lexicon) 
        lex_writer[it.first] = it.second;
    	
    lex_file << lex_writer.dump(4) << std::endl;
    lex_file.close();

	std::cout << "Writing doc_index to files...." << "\n";

    nlohmann::json doc_index_writer;
    std::ofstream doc_index_file("doc_index.json");

	for (auto it : doc_index)
		doc_index_writer[std::to_string(it.first)] = it.second;
    
    doc_index_file << doc_index_writer.dump(4) << std::endl;
    doc_index_file.close();
    
	std::cout << "Writing forward_index to files...." << "\n";
	
    json for_index_writer;
    std::ofstream for_index_file("forward_index.json");
	
	for (auto it : forward_index) {
        
		json temp_hit_list_fi;
		
		for (auto it2 : it.second) {
			temp_hit_list_fi[std::to_string(it2.first)] = it2.second;
			
		}
        for_index_writer[std::to_string(it.first)] = temp_hit_list_fi;
	}

    for_index_file << for_index_writer.dump(-1) << std::endl;
    for_index_file.close();
	std::cout << "Finishing.." << "\n";

	abort();
	return 0;

}