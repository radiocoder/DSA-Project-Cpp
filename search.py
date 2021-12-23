import json
from pathlib import Path
import string
from nltk.corpus import stopwords
from nltk.stem.snowball import SnowballStemmer


def search_inverted(all_doc_ids, query_word_ids, last):
    if (last == -1): # base condition
        return all_doc_ids
    word_id = query_word_ids[last]
    #print(word_id)
    doc_id_list = inverted_index[str(word_id)].keys()
    docs = set(doc_id_list)
    all_doc_ids = set(all_doc_ids) | docs # take union with all doc ids
    return docs & search_inverted(all_doc_ids, query_word_ids, last-1) # recursively take intersections


inverted_index = json.loads(Path("inverted_index.json").read_text())
lexicon = json.loads(Path("lexicon.json").read_text())
del lexicon['']
doc_index = json.loads(Path("doc_index.json").read_text())

stemmer = SnowballStemmer('english')
stop_words = set(stopwords.words('english'))
stop_words.add('the')

while True:
    query = input("Enter query: ")

    query_words = query.split()
    query_words = [w.lower() for w in query_words]
    query_words = [w for w in query_words if w not in stop_words]
    table = str.maketrans('', '', string.punctuation)
    stripped = [w.translate(table) for w in query_words]
    query_words = [word for word in stripped if word.isalpha()]
    query_words = [stemmer.stem(w) for w in query_words]

    query_word_ids = []
    for word in query_words:
        query_word_ids.append(lexicon.get(word, ''))

    query_word_ids = [w for w in query_word_ids if w != '']
    if len(query_word_ids) == 0:
        print("No results")
    else:
        empty_set = {}
        docs = search_inverted(empty_set, query_word_ids, len(query_word_ids) - 1)
        if len(docs) == 0:
            print("No results")
        # Calculate rank
        else:
            rank = {}
            for doc_id in docs:
                rank[doc_id] = 0
                for word_id in query_word_ids:
                    if inverted_index[str(word_id)][doc_id][0] == -1: # if fancy hit
                        rank[doc_id] += 50 # rank increment for fancy hit
                        rank[doc_id] += inverted_index[str(word_id)][doc_id][1]
                    
                    rank[doc_id] += inverted_index[str(word_id)][doc_id][0]

            rank = sorted(rank.items(), key=lambda x: x[1], reverse=True) # return list of tuples

            for doc_id, rank_n in rank:
                print(doc_index[doc_id])

        #print(docs)
    ans = input("Do you wanna continue to search (Enter Y/N): ")
    if ans.lower() != "y":
        break