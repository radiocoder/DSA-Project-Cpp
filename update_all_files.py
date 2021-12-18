from pathlib import Path

paths = Path('newsdata')
all_files = [p for p in paths.glob('*.json')]

f = open('files.txt', 'w')

for path in all_files:
    f.write("newsdata/" + path.name + "\n")

f.close()