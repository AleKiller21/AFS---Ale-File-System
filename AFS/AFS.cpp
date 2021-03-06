#include "stdafx.h"
#include "AFS.h"
#include "Parser.h"
#include <ctime>

#define FIRST_BIT_WORD 2147483648
#define MAX_SIZE_WORD 4294967295
#define SUCCESS 0
#define DISK_NOT_OPEN  1
#define DISK_ALREADY_OPEN 2
#define NOT_ENOUGH_BLOCKS 3
#define FILE_ALREADY_EXISTS 4
#define NO_FREE_INODE 5
#define NO_FREE_DIRECTORY_ENTRY 6
#define FILE_SYSTEM_NOT_MOUNTED 7
#define FILE_SYSTEM_ALREADY_MOUNTED 8
#define DISK_NOT_EXIST 9
#define DISK_ALREADY_FORMATTED 12
#define FILE_NOT_FOUND 13
#define DISK_NOT_FOUND 14

using namespace std;

AFS::AFS()
{
	directory = nullptr;
	bitmap = nullptr;
	inodes = nullptr;
}

int AFS::writeFileSystemStructuresToDisk(string diskName)
{
	disk.open(diskName.c_str(), ios::binary | ios::out | ios::in | ios::ate);
	unsigned int diskSize = disk.tellg();

	disk.seekp(0);
	initializeSuperBlock(diskSize);
	initializeBitmap();
	initializeDirectory();
	initializeInodeTable();

	disk.write(reinterpret_cast<char*>(&super), sizeof(SuperBlock));

	disk.seekp(super.bitmapBlock * super.blockSize);
	disk.write(reinterpret_cast<char*>(bitmap), super.bitmapSize);

	disk.seekp(super.directoryBlock * super.blockSize);
	disk.write(reinterpret_cast<char*>(directory), super.directorySize);

	disk.seekp(super.inodeTableBlock * super.blockSize);
	disk.write(reinterpret_cast<char*>(inodes), super.inodeTableSize);
	disk.close();

	cout << sizeof(DirectoryEntry) << endl;

	delete[] directory;
	delete[] bitmap;
	delete[] inodes;

	directory = nullptr;
	bitmap = nullptr;
	inodes = nullptr;

	return SUCCESS;
}

void AFS::saveBytesIntoDataBlocks(unsigned int* fileBlocks, int inumber, string sourceFilePath)
{
	unsigned int fileSize = inodes[inumber].size;
	ifstream file(sourceFilePath.c_str(), ios::binary);

	for (int i = 0; i < inodes[inumber].dataBlocks; i++)
	{
		char* buffer = new char[super.bytesAvailablePerBlock];
		setUpBuffer(buffer, super.bytesAvailablePerBlock);
		if (fileSize > super.bytesAvailablePerBlock)
		{
			file.read(buffer, super.bytesAvailablePerBlock);
		}
		else
		{
			file.read(buffer, fileSize);
		}

		unsigned int pointer = i == inodes[inumber].dataBlocks - 1 ? 0 : fileBlocks[i+1];
		disk.seekp(fileBlocks[i] * super.blockSize);
		disk.write(buffer, super.bytesAvailablePerBlock);
		disk.write(reinterpret_cast<char*>(&pointer), sizeof(int));
		fileSize -= super.bytesAvailablePerBlock;
		//buffer += super.bytesAvailablePerBlock;
		delete[] buffer;
	}

	file.close();
}

void AFS::setUpBuffer(char* buffer, unsigned int sizeOfBuffer)
{
	for (int i = 0; i < sizeOfBuffer; i++)
	{
		buffer[i] = '\0';
	}
}

unsigned int AFS::convertFileSizeToBlocks(unsigned size) const
{
	return ceil(static_cast<double>(size) / super.bytesAvailablePerBlock);
}

int AFS::searchFileInDirectory(std::string fileName) const
{
	for (int i = 0; i < super.totalInodes; i++)
	{
		if (directory[i].available) continue;

		if (!fileName.compare(directory[i].name)) return i;
	}

	return -1;
}

void AFS::getFileData(int inode, string targetFilePath)
{
	unsigned int pointer = inodes[inode].blockPointer;
	unsigned int fileSize = inodes[inode].size;
	ofstream file(targetFilePath.c_str(), ios::binary);

	for (int i = 0; i < inodes[inode].dataBlocks; i++)
	{
		disk.seekg(pointer * super.blockSize);
		char* buffer;

		if (fileSize > super.bytesAvailablePerBlock)
		{
			buffer = new char[super.bytesAvailablePerBlock];
			disk.read(buffer, super.bytesAvailablePerBlock);
			file.write(buffer, super.bytesAvailablePerBlock);
		}

		else
		{
			buffer = new char[fileSize];
			disk.read(buffer, fileSize);
			file.write(buffer, fileSize);
		}

		disk.read(reinterpret_cast<char*>(&pointer), sizeof(int));
		fileSize -= super.bytesAvailablePerBlock;
		delete[] buffer;
	}

	file.close();
}

void AFS::freeBlocksOnBitmap(std::list<unsigned>* blocks) const
{
	for (list<unsigned int>::iterator it = blocks->begin(); it != blocks->end(); ++it)
	{
		int word = *it / (sizeof(int) * 8);
		int offset = *it - (word * sizeof(int) * 8);
		bitmap[word] ^= (FIRST_BIT_WORD >> offset);
	}
}

int AFS::extractFileNamesFromPath(std::list<std::string>* path, std::list<std::string>* originNameList, std::list<std::string>* destinyNameList)
{
	list<string>::iterator it;

	for (it = path->begin(); it != path->end(); ++it)
	{
		if (*it == ";") break;
	}

	if (it == path->end()) return 201;
	originNameList->splice(originNameList->begin(), *path, path->begin(), it++);
	destinyNameList->splice(destinyNameList->begin(), *path, it, path->end());

	return SUCCESS;
}

std::list<unsigned int>* AFS::getFileBlocks(int inode)
{
	list<unsigned int>* pointers = new list<unsigned int>();
	unsigned int pointer = inodes[inode].blockPointer;

	for (int i = 0; i < inodes[inode].dataBlocks; i++)
	{
		pointers->push_back(pointer);
		disk.seekg(pointer * super.blockSize);
		disk.seekg(super.bytesAvailablePerBlock, ios::cur);
		disk.read(reinterpret_cast<char*>(&pointer), sizeof(int));
	}

	return pointers;
}

int AFS::createEmptyFile(list<string>* path)
{
	if (path->begin() != path->end() && *(path->begin()) == "") return 201;
	string name = Parser::constructPath(path);
	return createNewFile(1, name, "");
}

int AFS::openDisk(string name)
{
	if (disk.is_open()) return DISK_ALREADY_OPEN;

	disk.open(name.c_str(), ios::binary | ios::out | ios::in);
	mountFileSystem();

	return SUCCESS;
}

int AFS::closeDisk()
{
	if (!disk.is_open()) return DISK_NOT_OPEN;
	
	unmountFileSystem();
	disk.close();
	return SUCCESS;
}

int AFS::importFile(list<string>* path)
{
	if (!isFileSystemMounted()) return FILE_SYSTEM_NOT_MOUNTED;

	string sourceFilePath = "";
	string targetFileName = "";
	list<string>* sourcePathList = new list<string>();
	list<string>* targetNameList = new list<string>();

	if (extractFileNamesFromPath(path, sourcePathList, targetNameList)) return 201;
	sourceFilePath = Parser::constructPath(sourcePathList);
	targetFileName = Parser::constructPath(targetNameList);

	list<string>::iterator it = targetNameList->begin();
	if (it != targetNameList->end() && *it == "") return 201;
	if (!targetFileName.compare("")) targetFileName = sourceFilePath;

	ifstream file(sourceFilePath.c_str(), ios::binary | ios::ate);
	if (!file)
	{
		file.close();
		return FILE_NOT_FOUND;
	}

	string name = Parser::extractNameFromPath(targetFileName);
	unsigned int size = file.tellg();
	file.close();
	int state = createNewFile(size, name, sourceFilePath);

	sourcePathList->clear();
	targetNameList->clear();
	delete sourcePathList;
	delete targetNameList;

	return state;
}

int AFS::exportFile(list<string>* path)
{
	if (!isFileSystemMounted()) return FILE_SYSTEM_NOT_MOUNTED;

	string sourceFileName = "";
	string targetFilePath = "";
	list<string>* sourceNameList = new list<string>();
	list<string>* targetPathList = new list<string>();

	if (extractFileNamesFromPath(path, sourceNameList, targetPathList)) return 201;
	sourceFileName = Parser::constructPath(sourceNameList);
	targetFilePath = Parser::constructPath(targetPathList);

	list<string>::iterator it = targetPathList->begin();
	if (it != targetPathList->end() && *it == "") return 201;
	if (!targetFilePath.compare("")) targetFilePath = sourceFileName;

	int entry = searchFileInDirectory(sourceFileName);
	if (entry == -1) return FILE_NOT_FOUND;
	int inode = directory[entry].inode;

	getFileData(inode, targetFilePath);
	
	sourceNameList->clear();
	targetPathList->clear();
	delete sourceNameList;
	delete targetPathList;

	return SUCCESS;
}

int AFS::renameFile(list<string>* path)
{
	if (!isFileSystemMounted()) return FILE_SYSTEM_NOT_MOUNTED;

	string currentName;
	string newName;
	list<string>* currentNameList = new list<string>();
	list<string>* newNameList = new list<string>();
	bool found = false;

	if (extractFileNamesFromPath(path, currentNameList, newNameList)) return 201;
	currentName = Parser::constructPath(currentNameList);
	newName = Parser::constructPath(newNameList);	

	for (int i = 0; i < super.totalInodes; i++)
	{
		if (directory[i].available) continue;
		if (currentName.compare(directory[i].name)) continue;

		strcpy_s(directory[i].name, newName.c_str());
		found = true;
		break;
	}

	if (!found) return FILE_NOT_FOUND;

	disk.seekp(super.directoryBlock * super.blockSize);
	disk.write(reinterpret_cast<char*>(directory), super.directorySize);

	currentNameList->clear();
	newNameList->clear();
	delete currentNameList;
	delete newNameList;

	return SUCCESS;
}

int AFS::deleteFile(list<string>* path)
{
	if (!isFileSystemMounted()) return FILE_SYSTEM_NOT_MOUNTED;

	string fileName = Parser::constructPath(path);
	int entry = searchFileInDirectory(fileName);

	if (entry == -1) return FILE_NOT_FOUND;

	directory[entry].available = true;
	inodes[directory[entry].inode].available = true;
	list<unsigned int>* pointers = getFileBlocks(directory[entry].inode);
	freeBlocksOnBitmap(pointers);
	super.freeBlocks += pointers->size();
	super.usedBlocks -= pointers->size();
	super.freeInodes++;

	updateStructuresInDisk();
	pointers->clear();
	delete pointers;
	return SUCCESS;
}

int AFS::deleteDisk(std::string diskName)
{
	if (disk.is_open()) return DISK_ALREADY_OPEN;
	if (!remove(diskName.c_str())) return SUCCESS;
	
	return DISK_NOT_FOUND;
}

list<unsigned int>* AFS::getFileSystemInfo() const
{
	if (!isFileSystemMounted()) return nullptr;

	list<unsigned int>* info = new list<unsigned int>();

	info->push_back(super.partitionSize);
	info->push_back(super.totalBlocks);
	info->push_back(super.freeBlocks);
	info->push_back(super.usedBlocks);
	info->push_back(super.blockSize);
	info->push_back(super.bytesAvailablePerBlock);
	info->push_back(super.bitmapSize);
	info->push_back(super.bitmapBlock);
	info->push_back(super.wordsInBitmap);
	info->push_back(super.directorySize);
	info->push_back(super.directoryBlock);
	info->push_back(super.totalInodes);
	info->push_back(super.freeInodes);
	info->push_back(super.inodeTableSize);
	info->push_back(super.inodeTableBlock);
	info->push_back(super.firstDataBlock);
	info->push_back(sizeof(SuperBlock));
	info->push_back(sizeof(DirectoryEntry));
	info->push_back(sizeof(Inode));

	return info;
}

list<FileInfo>* AFS::listFiles() const
{
	if (!isFileSystemMounted()) return nullptr;

	list<FileInfo>* files = new list<FileInfo>();

	for (int i = 0; i < super.totalInodes; i++)
	{
		if (directory[i].available) continue;

		FileInfo info;
		strcpy_s(info.name, directory[i].name);
		info.inode = directory[i].inode;
		strcpy_s(info.DateCreated, inodes[directory[i].inode].DateCreated);
		info.sizeInBytes = inodes[directory[i].inode].size;
		info.sizeInDisk = inodes[directory[i].inode].dataBlocks * super.blockSize;
		files->push_back(info);
	}

	return files;
}

int AFS::mountFileSystem()
{
	loadSuperBlock();
	loadBitmap();
	loadDirectory();
	loadInodeTable();

	return SUCCESS;
}

int AFS::unmountFileSystem()
{
	delete[] bitmap;
	delete[] directory;
	delete[] inodes;

	bitmap = nullptr;
	directory = nullptr;
	inodes = nullptr;

	return SUCCESS;
}

void AFS::loadSuperBlock()
{
	disk.seekg(0);
	disk.read(reinterpret_cast<char*>(&super), sizeof(SuperBlock));
}

void AFS::loadBitmap()
{
	bitmap = new unsigned int[super.wordsInBitmap];

	disk.seekg(super.bitmapBlock * super.blockSize);
	disk.read(reinterpret_cast<char*>(bitmap), super.bitmapSize);
}

void AFS::loadDirectory()
{
	directory = new DirectoryEntry[super.totalInodes];

	disk.seekg(super.directoryBlock * super.blockSize);
	disk.read(reinterpret_cast<char*>(directory), super.directorySize);
}

void AFS::loadInodeTable()
{
	inodes = new Inode[super.totalInodes];

	disk.seekg(super.inodeTableBlock * super.blockSize);
	disk.read(reinterpret_cast<char*>(inodes), super.inodeTableSize);
}

void AFS::initializeSuperBlock(unsigned int partitionSize)
{
	super.blockSize = 4096;
	super.bytesAvailablePerBlock = super.blockSize - sizeof(int);
	super.totalBlocks = partitionSize / super.blockSize;
	super.partitionSize = partitionSize;
	super.totalInodes = super.blockSize / sizeof(Inode);
	super.freeInodes = super.totalInodes;
	super.bitmapBlock = 1;
	super.bitmapSize = super.totalBlocks / 8;
	super.wordsInBitmap = super.bitmapSize / sizeof(int);
	super.directorySize = super.totalInodes * sizeof(DirectoryEntry);
	super.inodeTableSize = super.totalInodes * sizeof(Inode);
	super.directoryBlock = calculateDirectoryInitialBlock();
	super.inodeTableBlock = calculateInodeTableInitialBlock();
	super.firstDataBlock = calculateInitialDataBlock();
	super.usedBlocks = super.firstDataBlock;
	super.freeBlocks = super.totalBlocks - super.usedBlocks;
}

void AFS::initializeBitmap()
{
	bitmap = new unsigned int[super.wordsInBitmap];
	int blocksUsedCounter = 0;

	for (int i = 0; i < super.wordsInBitmap; i++)
	{
		bitmap[i] = 0;
	}

	for (int i = 0; i < super.wordsInBitmap; i++)
	{
		for (int bit = 0; bit < sizeof(int) * 8; bit++)
		{
			if (blocksUsedCounter == super.usedBlocks) break;

			bitmap[i] ^= (FIRST_BIT_WORD >> bit);
			blocksUsedCounter++;
		}

		if (blocksUsedCounter == super.usedBlocks) break;
	}
}

void AFS::initializeDirectory()
{
	directory = new DirectoryEntry[super.totalInodes];

	for (int i = 0; i < super.totalInodes; i++)
	{
		DirectoryEntry entry;
		entry.inode = -1;
		entry.available = true;

		for (int x = 0; x < sizeof(entry.name); x++)
		{
			entry.name[x] = '\0';
		}

		directory[i] = entry;
	}
}

void AFS::initializeInodeTable()
{
	inodes = new Inode[super.totalInodes];

	for (int i = 0; i < super.totalInodes; i++)
	{
		Inode inode;
		inode.available = true;
		inode.blockPointer = 0;
		inode.dataBlocks = 0;
		inode.size = 0;

		inodes[i] = inode;
	}
}

void AFS::updateStructuresInDisk()
{
	disk.seekp(0);

	disk.write(reinterpret_cast<char*>(&super), sizeof(SuperBlock));

	disk.seekp(super.bitmapBlock * super.blockSize);
	disk.write(reinterpret_cast<char*>(bitmap), super.bitmapSize);

	disk.seekp(super.directoryBlock * super.blockSize);
	disk.write(reinterpret_cast<char*>(directory), super.directorySize);

	disk.seekp(super.inodeTableBlock * super.blockSize);
	disk.write(reinterpret_cast<char*>(inodes), super.inodeTableSize);
}

void AFS::updateSuperBlock(unsigned int size)
{
	int sizeInBlocks = convertFileSizeToBlocks(size);
	super.freeBlocks -= sizeInBlocks;
	super.usedBlocks += sizeInBlocks;
	super.freeInodes--;
}

int AFS::calculateDirectoryInitialBlock() const
{
	return ceil(super.bitmapSize / static_cast<double>(super.blockSize) + super.bitmapBlock);
}

int AFS::calculateInitialDataBlock() const
{
	return super.inodeTableBlock + ceil(static_cast<double>(super.inodeTableSize) / super.blockSize);
}

int AFS::calculateInodeTableInitialBlock() const
{
	int directoryBlocks = ceil((static_cast<double>(super.totalInodes) * sizeof(DirectoryEntry)) / super.blockSize);

	return directoryBlocks + super.directoryBlock;
}

int AFS::createNewFile(unsigned int size, std::string name, string sourceFilePath)
{
	if (!isFileSystemMounted()) return FILE_SYSTEM_NOT_MOUNTED;
	if (!checkIfEnoughFreeBlocks(size)) return NOT_ENOUGH_BLOCKS;
	if (checkFileExist(name)) return FILE_ALREADY_EXISTS;

	unsigned int* fileBlocks = getBlocksForFile(size);
	int inode = assignInodeToFile(size, fileBlocks);
	if (inode == -1)
	{
		restoreBitmap();
		return NO_FREE_INODE;
	}

	saveFileInDirectoryEntry(name.c_str(), inode);
	if (sourceFilePath != "") saveBytesIntoDataBlocks(fileBlocks, inode, sourceFilePath);

	updateSuperBlock(size);
	updateStructuresInDisk();

	delete[] fileBlocks;
	return SUCCESS;
}

int AFS::checkIfEnoughFreeBlocks(unsigned int fileSize) const
{
	int sizeInBlocks = convertFileSizeToBlocks(fileSize);
	return sizeInBlocks < super.freeBlocks;
}

unsigned int* AFS::getBlocksForFile(unsigned int size)
{
	unsigned int sizeInBlocks = convertFileSizeToBlocks(size);
	unsigned int* blocks = new unsigned int[sizeInBlocks];
	int iterator = 0;
	int wordsOccupied = 0;
	unsigned int bitResult;

	for (int i = 0; i < super.wordsInBitmap; i++)
	{
		if (bitmap[i] == MAX_SIZE_WORD) { wordsOccupied++; continue; };

		for (int position = 0; position < sizeof(int) * 8; position++)
		{
			bitResult = (FIRST_BIT_WORD >> position) & bitmap[i];
			if (bitResult) continue;

			blocks[iterator] = calculateBlockNumberInBitmap(wordsOccupied, position);
			bitmap[i] ^= (FIRST_BIT_WORD >> position);
			iterator++;
			if (iterator == sizeInBlocks) break;
		}

		if (iterator == sizeInBlocks) break;
		wordsOccupied++;
	}

	return blocks;
}

unsigned int AFS::calculateBlockNumberInBitmap(int wordsOccupied, int blockPositionInWord)
{
	unsigned int bitsPerWord = sizeof(int) * 8;

	return bitsPerWord * wordsOccupied + blockPositionInWord;
}

int AFS::assignInodeToFile(unsigned int fileSize, unsigned int* dataBlocks) const
{
	time_t raw;
	int sixHoursSeconds = 21600;

	for (int i = 0; i < super.totalInodes; i++)
	{
		if (!inodes[i].available) continue;

		inodes[i].available = false;
		inodes[i].blockPointer = dataBlocks[0];
		inodes[i].dataBlocks = convertFileSizeToBlocks(fileSize);
		inodes[i].size = fileSize == 1 ? 0: fileSize;
		time(&raw);
		//raw -= sixHoursSeconds;
		//raw-= 43200;
		//strcpy_s(inodes[i].DateCreated, ctime(&raw));
		ctime_s(inodes[i].DateCreated, sizeof(inodes[i].DateCreated), &raw);
		return i;
	}

	return -1;
}

void AFS::saveFileInDirectoryEntry(const char* name, int inode) const
{
	for (int i = 0; i < super.totalInodes; i++)
	{
		if (!directory[i].available) continue;

		directory[i].available = false;
		directory[i].inode = inode;
		strcpy_s(directory[i].name, name);
		break;
	}
}

void AFS::restoreBitmap()
{
	delete[] bitmap;
	bitmap = nullptr;
	loadBitmap();
}

bool AFS::isFileSystemMounted() const
{
	return directory && bitmap && inodes;
}

bool AFS::checkFileExist(std::string name) const
{
	for (int i = 0; i < super.totalInodes; i++)
	{
		if (directory[i].available) continue;

		if (!name.compare(directory[i].name)) return true;
	}

	return false;
}

AFS::~AFS()
{
}
