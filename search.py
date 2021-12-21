import json
from pathlib import Path
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
doc_index = json.loads(Path("doc_index.json").read_text())

stemmer = SnowballStemmer('english')
stop_words = set(stopwords.words('english'))
stop_words.add('the')

query = input("Enter query: ")

query_words = query.split()
query_words = [w.lower() for w in query_words]
query_words = [w for w in query_words if w not in stop_words]
query_words = [stemmer.stem(w) for w in query_words]

query_word_ids = []
for word in query_words:
    query_word_ids.append(lexicon[word])

empty_set = {}
docs = search_inverted(empty_set, query_word_ids, len(query_word_ids) - 1)

# Calculate rank
rank = {}
for doc_id in docs:
    rank[doc_id] = 0
    for word_id in query_word_ids:
        if inverted_index[str(word_id)][doc_id][0] == -1: # if fancy hit
            rank[doc_id] += 100 # rank increment for fancy hit
        
        rank[doc_id] += len(inverted_index[str(word_id)][doc_id])

rank = sorted(rank.items(), key=lambda x: x[1], reverse=True) # return list of tuples

for doc_id, rank_n in rank:
    print(doc_index[doc_id])

#print(docs)