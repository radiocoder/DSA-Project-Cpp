import json
import sys
from pathlib import Path

forward_index = json.loads(Path('forward_index.json').read_text())
inverted_index = {}

for doc_id in forward_index.keys():
    for word_id in forward_index[doc_id]:
        if word_id in inverted_index:
            inverted_index[word_id][doc_id] = forward_index[doc_id][word_id]
            
        else:
            temp_dict = {}
            temp_dict[doc_id] = forward_index[doc_id][word_id]
            inverted_index[word_id] = temp_dict
    
    if (int(doc_id) % 1000 == 0):
        print(doc_id)


inv_data = json.dumps(inverted_index)
Path('inverted_index.json').write_text(inv_data)