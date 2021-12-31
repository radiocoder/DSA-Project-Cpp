import json
from pathlib import Path
import string
from nltk.corpus import stopwords
from nltk.stem.snowball import SnowballStemmer

# function to perform the search recursively
def search_inverted(all_doc_ids, query_word_ids, last):
    if (last == -1): # base condition
        return all_doc_ids
    word_id = query_word_ids[last]
    doc_id_list = inverted_index[str(word_id)].keys()
    docs = set(doc_id_list)
    all_doc_ids = set(all_doc_ids) | docs # take union with all doc ids
    return docs & search_inverted(all_doc_ids, query_word_ids, last-1) # recursively take intersections


# load the inverted index, lexicon and document index in memory
inverted_index = json.loads(Path("inverted_index.json").read_text())
lexicon = json.loads(Path("lexicon.json").read_text())
doc_index = json.loads(Path("doc_index.json").read_text())


stemmer = SnowballStemmer('english') # construct a stemmer to stem the query
stop_words = set(stopwords.words('english')) # to remove stopwords from query
stop_words.add('the')

while True:
    query = input("Enter query: ") # take query from user

    query_words = query.split() # tokenize the query words
    # santize the query:
    # remove punctuations, stopwords
    # stem the query words
    query_words = [w.lower() for w in query_words]
    query_words = [w for w in query_words if w not in stop_words]
    table = str.maketrans('', '', string.punctuation)
    stripped = [w.translate(table) for w in query_words]
    query_words = [word for word in stripped if word.isalpha()]
    query_words = [stemmer.stem(w) for w in query_words]

    # get the word ids of the words from lexicon
    query_word_ids = []
    for word in query_words:
        query_word_ids.append(lexicon.get(word, ''))

    query_word_ids = [w for w in query_word_ids if w != '']
    if len(query_word_ids) == 0:
        print("No results")
    else:
        empty_set = {}
        docs = search_inverted(empty_set, query_word_ids, len(query_word_ids) - 1)
        while len(docs) == 0:
            empty_set = {}
            del query_word_ids[-1]
            if len(query_word_ids) == 0:
                break
            docs = search_inverted(empty_set, query_word_ids, len(query_word_ids) - 1)
        if len(docs) == 0:
            print('No results')
        # Calculate rank
        else:
            rank = {}
            for doc_id in docs:
                rank[doc_id] = 0
                for word_id in query_word_ids:
                    hit = inverted_index[str(word_id)][doc_id]
                    hit = int(bin(hit & 65535), 2) # to check if MSB is 1 for 16 bit num
                    if hit >= 32768: # if MSB == 1 i.e it is a fancy hit
                        rank[doc_id] += 50 # rank increment for fancy hit

                    hit = int(bin(hit & 32767), 2) # 32767 = 0111_1111_1111_1111
                    rank[doc_id] += hit # add the remaining number after setting MSB to 0
                    

            rank = sorted(rank.items(), key=lambda x: x[1], reverse=True) # return list of tuples
            #print(rank)
            for doc_id, rank_n in rank:
                print(doc_index[doc_id])

        #print(docs)
    ans = input("Do you wanna continue to search (Enter Y/N): ")
    if ans.lower() != "y":
        break