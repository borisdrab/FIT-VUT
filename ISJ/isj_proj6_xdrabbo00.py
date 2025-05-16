from collections import Counter

class ValidationError(Exception):
    """Exception for invalid word counts."""
    pass

def upd_word_counts(sentence, word_counts=None, *, to_upper=False):
    """Funkcia na aktualizáciu počtov slov.

    >>> upd_word_counts('That that exists exists in that that that that exists exists in', to_upper=True)
    Counter({'THAT': 6, 'EXISTS': 4, 'IN': 2})

    >>> upd_word_counts('du du du du po le du do pre du du i do za du du', Counter(['po', 'le']))
    Counter({'du': 9, 'po': 2, 'le': 2, 'do': 2, 'pre': 1, 'i': 1, 'za': 1})

    >>> upd_word_counts('du du du du po le du do pre du du i do za du du', Counter(['po', 'le']), True)
    Traceback (most recent call last):
    ...
    TypeError: upd_word_counts() takes from 1 to 2 positional arguments but 3 were given

    >>> upd_word_counts('du du du du po le du do pre du du i do za du du', Counter(['PO', 'LE']), to_upper=True)
    Counter({'DU': 9, 'PO': 2, 'LE': 2, 'DO': 2, 'PRE': 1, 'I': 1, 'ZA': 1})

    >>> all_words = Counter(('a', 'b', 'c', 'a')); upd_word_counts('du du du du po le du do pre du du i do za du du', all_words, to_upper=True)
    Traceback (most recent call last):
    ...
    ValidationError: All letters must be uppercase.

    >>> all_words = Counter(('A', 'B', 'C', 'A')); upd_word_counts('du du du du po le du do pre du du i do za du du', all_words, to_upper=True)
    Counter({'DU': 9, 'DO': 2, 'A': 2, 'PO': 1, 'LE': 1, 'PRE': 1, 'I': 1, 'ZA': 1, 'B': 1, 'C': 1})

    >>> all_words = Counter(('A', 'B', 'C', 'A')); upd_word_counts('du du du du po le du do pre du du i do za du du', all_words, to_upper=True)
    Counter({'DU': 9, 'DO': 2, 'A': 2, 'PO': 1, 'LE': 1, 'PRE': 1, 'I': 1, 'ZA': 1, 'B': 1, 'C': 1})

    >>> all_words = Counter(('A', 'B', 'C', 'A')); print(all_words)
    Counter({'A': 2, 'B': 1, 'C': 1})
    """
    
    word_counts = Counter() if word_counts is None else Counter(word_counts)
    if to_upper:
        for index in word_counts:
            if not index.isupper():
                raise ValidationError("All letters must be uppercase.")
    
    
    words = sentence.split()
    if to_upper:
        words = [w.upper() for w in words]
    
    
    merged = Counter(words) + word_counts
    
    
    index = {}
    for i, w in enumerate(words):
        if w not in index:
            index[w] = i

    sorted_keys = sorted(merged.keys(), key=lambda k: (-merged[k], index.get(k, float('inf')), k))
    
    return Counter({k: merged[k] for k in sorted_keys})

if __name__ == "__main__":
    import doctest
    doctest.testmod()


