#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <cctype>
#include <codecvt>
#include <locale>
#include "lib/json.hpp"
#include "lib/english_stem.hpp"
#include "utils.h"


int main() {
	int num = 0;
	

	std::unordered_map<std::string, int> lexicon;
	std::unordered_map<std::string, int> doc_index;
	std::unordered_map<int, std::vector<int>> forward_index;

	std::ofstream lex_file;
  	lex_file.open("lexicon.txt");
	std::ofstream doc_index_file;
	doc_index_file.open("doc_index.txt");
	int doc_id = 0;
	std::ofstream forward_index_file;
	forward_index_file.open("forward_index.txt");
	
	
	std::vector<std::string> words;
	std::vector<int> word_id_list;
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
			word_id_list.clear();

			std::string doc_id_string = jf[i]["id"];
			doc_index[doc_id_string] = doc_id;
			//std::cout << doc_id << std::endl;
			

			words = split(jf[i]["content"], ' ');
			words = sanitize_and_stem_list(words);

			std::sort(words.begin(), words.end()); 
			words.erase(std::unique(words.begin(), words.end()), words.end()); // remove duplicate words

			words.erase(words.begin()); // remove the comma (because it is the first element)

			// remove stopwords
			words.erase(std::remove(words.begin(), words.end(), "a"), words.end());
			words.erase(std::remove(words.begin(), words.end(), "of"), words.end());
			words.erase(std::remove(words.begin(), words.end(), "the"), words.end());
			words.erase(std::remove(words.begin(), words.end(), "an"), words.end());
			words.erase(std::remove(words.begin(), words.end(), "to"), words.end());
			words.erase(std::remove(words.begin(), words.end(), "on"), words.end());

			for (int j = 0; j < words.size(); j++) {
				if(lexicon.find(words[j]) == lexicon.end()) { // if not in lexicon
					lexicon[words[j]] = num++;
				}
			}

			int temp_word_id = 0;
			for (int j = 0; j < words.size(); j++) {
				temp_word_id = lexicon[words[j]];
				word_id_list.push_back(temp_word_id);
				
			}

			forward_index[doc_id] = word_id_list;
			doc_id++;
			
		}
	}


	//std::wstring tname = L"Organization";
	//stemming::english_stem<> obj;
	//obj(tname);

	std::cout << "Writing lexicon, doc_index, forward_index to files...." << "\n";

	for (auto it : lexicon) 
    	lex_file << it.first << ", " << it.second << "\n";

	for (auto it : doc_index)
		doc_index_file << "\"" << it.first << "\", " << it.second << "\n";

	
	for (auto it : forward_index) {
		forward_index_file << it.first << ","; // << " " << it.second << "\n";
		for (int i = 0; i < it.second.size(); i++) {
			forward_index_file << " " << it.second[i];
		}
		forward_index_file << "\n";
		//for (auto it2 : it.second)
	}

  	lex_file.close();
	doc_index_file.close();
	forward_index_file.close();

	// Building inverted index
	std::cout << "Building inverted index..." << "\n";

	std::ofstream inv_index_file;
  	inv_index_file.open("inverted_index.txt");

	std::unordered_map<int, std::vector<int>> inverted_index;

	for (auto it : forward_index) {
		for (int i = 0; i < it.second.size(); i++) {
			if(inverted_index.find(it.second[i]) != inverted_index.end()) {
				inverted_index[it.second[i]].push_back(it.first);
			} else {
				std::vector<int> temp_doc_list;
				temp_doc_list.push_back(it.first);
				inverted_index[it.second[i]] = temp_doc_list;
			}
		}
	}

	std::cout << "Writing inverted index to a file..." << "\n";


	for (auto it : inverted_index) {
		inv_index_file << it.first << ","; // << " " << it.second << "\n";
		for (int i = 0; i < it.second.size(); i++) {
			inv_index_file << " " << it.second[i];
		}
		inv_index_file << "\n";
		//for (auto it2 : it.second)
	}

	inv_index_file.close();

	return 0;

}