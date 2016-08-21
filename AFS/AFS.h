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
	char partition;
	int inodeTableSize;
};

struct DirectoryEntry
{
	char name[50];
	int inode;
	bool available;
};

struct Inode
{
	bool available;
	int size;
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


	void initializeSuperBlock(std::streamsize partitionSize, char partition);
	void initializeBitmap();
	void initializeDirectory();
	void initializeInodeTable();
	int calculateInodeTableInitialBlock() const;
	int calculateDirectoryInitialBlock() const;

public:

	AFS();
	void mountFileSystem(std::string diskName, char partition, std::streamsize size);


	~AFS();
};

