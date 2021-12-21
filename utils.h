#ifndef __UTILS_H
#define __UTILS_H

bool invalid_char(char c) {
	return !isprint( static_cast<unsigned char>( c ) );
}

void my_remove(std::string& s, char a)
{ 
	s.erase(std::remove(s.begin(),s.end(),a),s.end());
}

std::vector<std::string> split(const std::string &text, char sep) {
    std::vector<std::string> tokens;
    std::size_t start = 0, end = 0;
    while ((end = text.find(sep, start)) != std::string::npos) {
        if (end != start) {
          tokens.push_back(text.substr(start, end - start));
        }
        start = end + 1;
    }
    if (end != start) {
       tokens.push_back(text.substr(start));
    }
    return tokens;
}

void remove_stopwords(std::vector<std::string>& words) {
	words.erase(std::remove(words.begin(), words.end(), "a"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "of"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "the"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "an"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "to"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "on"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "does"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "with"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "other"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "their"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "when"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "both"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "these"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "here"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "this"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "that"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "who"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "have"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "has"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "such"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "which"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "or"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "most"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "over"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "if"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "was"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "were"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "but"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "will"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "would"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "should"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "shall"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "into"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "accord"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "at"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "away"), words.end());
	words.erase(std::remove(words.begin(), words.end(), "as"), words.end());
}


std::vector<std::string> sanitize_and_stem_list(std::vector<std::string> input) {
	std::vector<std::string> output;
	std::string word;
	//std::string chars = "\n{}[]()!@#$% ^&*-—_=+/?.>,<;:`\"1234567890";

	for (int i = 0; i < input.size(); i++) {
		word = input[i];

		size_t j = 0;
		size_t len = word.length();
		while(j < len){
			if (!std::isalpha(word[j]) || word[j] == '\n' || word[j] == '\t'){
				word.erase(j,1);
				len--;
			}else
				j++;
		}


		std::transform(word.begin(), word.end(), word.begin(),
    		[](unsigned char c){ return std::tolower(c); });

		std::wstring wstr (word.begin(), word.end());
		stemming::english_stem<> Stem;
		Stem(wstr);
		std::string stemmed = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(wstr);
		if(!(word.length() < 2 && word.length() > 30)) {
			//std::cout << temp << "\n";
			output.push_back(stemmed);
		}
	}

	return output;
}

#endif