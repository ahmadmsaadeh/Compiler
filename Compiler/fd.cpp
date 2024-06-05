#include "fd.h"
#include <iostream>
using namespace std;

FileDescriptor::FileDescriptor(char* FileName)
{
	file = _strdup(FileName);
	if (fopen_s(&fp, file, "r") !=0) {
		cerr << "File" << file << " Couldn't be opend 😢" << endl;
		exit(EXIT_FAILURE);
	}
	ungetBuffer = '\0';
	line_number = 1;
	char_number = 0;
	buf_size = BUFFER_SIZE;
	buffer = (char*)malloc(buf_size);
	if (!buffer) {
		cerr << "Buffer Can't Be Allocated 😢";
		exit(EXIT_FAILURE);
	}
}

char FileDescriptor::readChar(){
	if (ungetBuffer != '\0') {
		char ch = ungetBuffer;
		ungetBuffer = '\0';
		return ch;
	}

	char ch = fgetc(fp);
	if (ch == EOF) {
		// Handle end of file (EOF)
		return ch;
	}

	if (ch == '\n') {
		buffer[char_number] = '\0'; // Null-terminate the buffer to represent the end of the line
		char_number = 0; // Reset the character number for the next line
		line_number++;
		//std::cout << "\n--------" << buffer; // Process or print the line

	}
	else {
		buffer[char_number++] = ch; // Store the character in the buffer
		if (char_number >= buf_size - 1) {
			// Resize the buffer if it's about to overflow
			buf_size *= 2;
			buffer = (char*)realloc(buffer, buf_size * sizeof(char));
			if (buffer == nullptr) {
				perror("Error reallocating memory for buffer");
				exit(EXIT_FAILURE);
			}
		}
	}

	return ch;
}

void FileDescriptor::ungett() {
	
	if (ungetBuffer != '\0') {
		std::cerr << "Error: Two ungets in a row are not allowed." << std::endl;
		exit(EXIT_FAILURE);
	}

	if (char_number > 0) {
		ungetBuffer = buffer[char_number - 1]; // Store the last read character in the unget buffer
		char_number--; // Decrement char_number after using it

	}
}

char FileDescriptor::peek() {
	char ch = readChar();
	ungett();
	return ch;
}

int FileDescriptor::getLineNumber()
{
	return line_number;
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