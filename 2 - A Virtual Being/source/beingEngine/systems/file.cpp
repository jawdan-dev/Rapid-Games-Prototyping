#include "file.hpp"

const String File::readAll(const String& filePath) {
	FILE* const file = fopen(filePath.c_str(), "rb");
	if (file == nullptr)
		BEING_ERROR("Failed to open file.");

	fseek(file, 0, SEEK_END);
	const long fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* rawFileString = new char[fileSize + 1];
	fread(rawFileString, sizeof(char), fileSize, file);
	fclose(file);

	rawFileString[fileSize] = '\0';
	const String fileString(rawFileString);

	delete[] rawFileString;

	return fileString;
}