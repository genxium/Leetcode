Can I build 2 `Trie`s, `t1` using each `word` from the `dictionary` and `t2` using the `reverse of each word`, while marking the `containedWordIndices` at each `TrieNode` to speed up? 

With that structure, we can check each `splitting point of a word` to see whether or not the `ending of prefix TrieNode of t1` has intersecting `containedWordIndices` with the `ending of suffix TrieNode of t2`.
