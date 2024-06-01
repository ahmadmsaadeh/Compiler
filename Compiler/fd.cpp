#include "fd.h"
#include <iostream>
using namespace std;

FileDescriptor::FileDescriptor()
{
	file = nullptr;
	fp = stdin;
	line_number = 0;
	char_number = 0;
	flag = UNSET;
	buf_size = BUFFER_SIZE;
	buffer = (char*)malloc(buf_size);
	if (!buffer) {
		cerr << "Buffer Can't Be Allocated 😢";
		exit(EXIT_FAILURE);
	}

}

FileDescriptor::FileDescriptor(char* FileName)
{
	file = _strdup(FileName);
	if (fopen_s(&fp, file, "r") !=0) {
		cerr << "File" << file << " Couldn't be opend 😢" << endl;
		exit(EXIT_FAILURE);
	}
	line_number = 0;
	char_number = 0;
	flag = UNSET;
	buf_size = BUFFER_SIZE;
	buffer = (char*)malloc(buf_size);
	if (!buffer) {
		cerr << "Buffer Can't Be Allocated 😢";
		exit(EXIT_FAILURE);
	}
}


char* FileDescriptor::GetFileName()
{
	return file;
}

bool FileDescriptor::IsOpen()
{
	return fp != nullptr;
}

char* FileDescriptor::GetCurrLine()
{
	if (fgets(buffer, buf_size, fp)) {
		line_number++;
		char_number = 0;
		return buffer;
	}
	return nullptr;
}

int FileDescriptor::GetLineNum()
{
	return line_number;
}

int FileDescriptor::GetCharNum()
{
	return char_number;
}

void FileDescriptor::Close()
{
	if (fp && fp != stdin) {
		fclose(fp);
		fp = nullptr;
	}
}

char FileDescriptor::GetChar()
{
	if (flag == SET) {
		flag = UNSET;
		char_number--;
		return buffer[char_number];
	}

	if (char_number == 0 || buffer[char_number] == '\0') {
		if (!GetCurrLine()) {
			return EOF;
		}
	}

	return buffer[char_number++];
}

void FileDescriptor::ReportError(char* msg)
{
	cerr << buffer << endl;
	for (int i = 0; i < char_number - 1; ++i) {
		cerr << ' ';
	}
	cerr << '^' << endl;
	cerr << "Error: \"" << msg << "\" on line " << line_number << " of " << file << endl;

}

void FileDescriptor::UngetChar(char c)
{
	if (flag == UNSET) {
		if (char_number > 0) {
			char_number--;
			buffer[char_number] = c;
			flag = SET;
		}
	}
}

FileDescriptor::~FileDescriptor()
{
	if (fp && fp != stdin) {
		fclose(fp);
	}
	free(buffer);
	if (file) {
		free(file);
	}
}

void FileDescriptor::unget() {
	if (flag == UNSET) {
		cerr << "Error: Two ungets in a row are not allowed." << endl;
		exit(EXIT_FAILURE);
	}

	if (char_number > 0) {
		flag = SET; // Store the last read character in the unget buffer
		char_number--; // Decrement char_number after using it

	}
}

char FileDescriptor::peek() {
	char ch = GetChar();
	unget();
	return ch;
}
