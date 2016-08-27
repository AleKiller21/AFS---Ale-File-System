#include "stdafx.h"
#include "AFS.h"

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

//int AFS::validateFileSystemMount()
//{
//	int totalBlocks;
//
//	disk.seekg(0);
//	disk.read(reinterpret_cast<char*>(&totalBlocks), sizeof(int));
//
//	return totalBlocks;
//}

int AFS::createEmptyFile(std::string name)
{
	return createNewFile(1, name);
}

int AFS::openDisk(string name)
{
	if (disk.is_open()) return DISK_ALREADY_OPEN;

	disk.open(name.c_str(), ios::binary | ios::out | ios::in);
	if (!disk) return DISK_NOT_EXIST;

	return SUCCESS;
}

int AFS::closeDisk()
{
	if (!disk.is_open()) return DISK_NOT_OPEN;

	if (isFileSystemMounted()) unmountFileSystem();

	disk.close();
	return SUCCESS;
}

int AFS::importFile(string filePath, string name)
{
	if (!isFileSystemMounted()) return FILE_SYSTEM_NOT_MOUNTED;

	ifstream file(filePath.c_str(), ios::binary | ios::ate);
	unsigned int size = file.tellg();
	createNewFile(size, name);

	file.seekg(0);
	file.close();

	return 0;
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
		info.sizeInBytes = inodes[directory[i].inode].size;
		info.sizeInDisk = inodes[directory[i].inode].dataBlocks * super.blockSize;
		files->push_back(info);
	}

	return files;
}

int AFS::mountFileSystem()
{
	if (!disk.is_open()) return DISK_NOT_OPEN;
	if (isFileSystemMounted()) return FILE_SYSTEM_ALREADY_MOUNTED;

	loadSuperBlock();
	loadBitmap();
	loadDirectory();
	loadInodeTable();

	return SUCCESS;
}

int AFS::unmountFileSystem()
{
	if (!isFileSystemMounted()) return FILE_SYSTEM_NOT_MOUNTED;

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
	int sizeInBlocks = ceil(static_cast<double>(size) / super.blockSize);
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

int AFS::createNewFile(unsigned int size, std::string name)
{
	if (!isFileSystemMounted()) return FILE_SYSTEM_NOT_MOUNTED;
	if (!checkIfEnoughFreeBlocks(size)) return NOT_ENOUGH_BLOCKS;
	if (checkFileExist(name)) return FILE_ALREADY_EXISTS;

	int* fileBlocks = getBlocksForFile(size);
	int inode = assignInodeToFile(size, fileBlocks);
	if (inode == -1)
	{
		restoreBitmap();
		return NO_FREE_INODE;
	}

	saveFileInDirectoryEntry(name.c_str(), inode);
	updateSuperBlock(size);
	updateStructuresInDisk();

	delete[] fileBlocks;
	return SUCCESS;
}

int AFS::checkIfEnoughFreeBlocks(unsigned int fileSize) const
{
	int sizeInBlocks = ceil(static_cast<double>(fileSize) / super.blockSize);
	return sizeInBlocks < super.freeBlocks;
}

int* AFS::getBlocksForFile(unsigned int size)
{
	int sizeInBlocks = ceil(static_cast<double>(size) / super.blockSize);
	int* blocks = new int[sizeInBlocks];
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

int AFS::calculateBlockNumberInBitmap(int wordsOccupied, int blockPositionInWord)
{
	int bitsPerWord = sizeof(int) * 8;

	return bitsPerWord * wordsOccupied + blockPositionInWord;
}

int AFS::assignInodeToFile(unsigned int fileSize, int* dataBlocks) const
{
	for (int i = 0; i < super.totalInodes; i++)
	{
		if (!inodes[i].available) continue;

		inodes[i].available = false;
		inodes[i].blockPointer = dataBlocks[0];
		inodes[i].dataBlocks = ceil(static_cast<double>(fileSize) / super.blockSize);
		inodes[i].size = fileSize;
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
