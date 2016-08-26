#pragma once

struct SuperBlock
{
	int totalBlocks;
	int freeBlocks;
	int usedBlocks;
	int blockSize;
	int bitmapSize;
	int directorySize;
	int wordsInBitmap;
	std::streamsize partitionSize;
	int totalInodes;
	int freeInodes;
	int bitmapBlock;
	int directoryBlock;
	int inodeTableBlock;
	int firstDataBlock;
	char partition;
	int inodeTableSize;
};

struct DirectoryEntry
{
	char name[50];//char name[256]
	int inode;
	bool available;
};

struct Inode
{
	bool available;
	std::streamsize size;
	int dataBlocks;
	//DateCreated
	//DateLastModified
	unsigned int blockPointer;
};

class AFS
{
private:
	std::fstream disk;
	SuperBlock super;
	DirectoryEntry* directory;
	Inode* inodes;
	unsigned int* bitmap;
	bool structuresInMemory;

	void loadStructuresToMemory();
	void loadSuperBlock();
	void loadBitmap();
	void loadDirectory();
	void loadInodeTable();
	void initializeSuperBlock(std::streamsize partitionSize, char partition);
	void initializeBitmap();
	void initializeDirectory();
	void initializeInodeTable();
	int calculateInodeTableInitialBlock() const;
	int calculateDirectoryInitialBlock() const;
	int calculateInitialDataBlock() const;
	int createNewFile(std::streamsize size, std::string name);
	int checkIfEnoughFreeBlocks(std::streamsize fileSize) const;
	int* getBlocksForFile(std::streamsize size);
	int calculateBlockNumberInBitmap(int wordsOccupied, int blockPositionInWord);
	int assignInodeToFile(std::streamsize fileSize, int* dataBlocks);
	int saveFileInDirectoryEntry(const char* name, int inode) const;

public:

	AFS();
	void mountNewFileSystem(std::string diskName, char partition, std::streamsize size);
	int openDisk(std::string name);
	int importFile(std::string filePath, std::string name);

	~AFS();
};

