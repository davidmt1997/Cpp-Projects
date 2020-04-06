#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
#include <string>
#include <assert.h>
#include <algorithm>

using namespace std;

#define FILEPATH "/usr/share/dict/words"

bool isInStartOfFile(long index, char* arr) {
	return (index == 0);
}

bool isInStartOfLine(long index, char* arr) {
	index--;
	return (arr[index] == '\n');
}

bool isInEOL(long index, char* arr) {
	return (arr[index] == '\n');
}

string getWordAtStartOfLine(long index, char* arr) {
	string ret = "";
	while (arr[index] != '\n') {
		ret += arr[index];
		index++;
	}
	return ret;
}

string getWordAt(long index, char* arr) {
	string ret = "";
	//is in start of file
	if (isInStartOfFile(index, arr)) {
		cout << "is in start of file" << endl;
		ret = getWordAtStartOfLine(index, arr);
		return ret;
	}
	//is in start of line
	if (isInStartOfLine(index, arr)) {
		cout << "in start of line" << endl;
		ret = getWordAtStartOfLine(index, arr);
		return ret;
	}
	//is in end on line
	if (isInEOL(index, arr)) {
		cout << "in end of line" << endl;
		index--;
		while ((!isInEOL(index, arr)) && (!isInStartOfFile(index, arr))) {
			index--;
		}
		if (!(isInStartOfFile(index, arr)))
			index++;
		ret = getWordAtStartOfLine(index, arr);
		return ret;
	}
	//is in middle of line
	if (!(isInStartOfLine(index, arr)) && !(isInStartOfFile(index, arr)) && !(isInEOL(index, arr))) {
		cout << "in middle of line" << endl;
		while ((!isInEOL(index, arr)) && (!isInStartOfFile(index, arr))) {
			index--;
		}
		if (!(isInStartOfFile(index, arr)))
			index++;
		ret = getWordAtStartOfLine(index, arr);
		return ret;
	}
}

bool bSearch(string value, void *arr, long start, long end, long size){
	char *words = (char *)arr;
	long first = start;
	long last = end;
	string begining = getWordAt(0, words);
	begining.erase(begining.find('\r'));
	string ending = getWordAt(size-2, words);
	ending.erase(ending.find('\r'));
	if(value < begining||value > ending){
		cout << "Error, bad input" << endl;
		return false;
	}
	while(last >= first){
		long mid = (first+last)/2;
		cout <<mid<<endl;
		string word = getWordAt(mid, words);
		word.erase(word.find('\r'));
		cout << "word is \"" << word<<  "\"" << endl;
		if(value == word) return true;
		if(value > word){
			first = mid+1;
		}
		if(value < word){
			last = mid-1;
		} 
	}
	cout << value << " " << value.length() << endl;
	return false;
}

void *getFilePointer(const char *fname, long *size){
	struct stat buf;
	stat(fname, &buf);
	int fd1 = open(fname, O_RDONLY);
	assert(fd1 != -1);
	void *in1 = mmap((void *)0, buf.st_size, PROT_READ, MAP_SHARED, fd1, 0);
	*size = buf.st_size;
	return in1;
}

int main(){
	cout << "Enter word to search: ";
	string input;
	getline(cin , input);
	long size;
	void *in = getFilePointer("words.txt", &size);
	if(bSearch(input, in, 0, size-1, size)){
		cout << "word found" << endl;
	}
	else cout << "word not found" << endl;
	return 0;
}

