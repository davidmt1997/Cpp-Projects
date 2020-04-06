# mmap

This was an assignment given to me in my Operating Systems class.

We were loading a file to virtual memory using the mmap function and then bynary search in the file to look up for a word entered by the user. The purpose of these exercise is to not load the file to memory, since it's a really big file, instead just load it to virtual memory. This way we can still operate through it without wasting memory, just as if we were using an array, but the file was never loaded into memory.
