# Future changes: search ALL files in a directory

import re
import prompt
from goody import safe_open

pattern_string = prompt.for_string('Enter Regular Expression pattern to search for')
pattern = re.compile(pattern_string)
file = safe_open('Enter file to search', 'r', 'Could not find that file')
for line,contents in enumerate(file,1):
    if pattern.search(contents):  # note search not match: pattern can start anywhere
        print('{:<5}: {}'.format(line,contents),end='')
file.close()