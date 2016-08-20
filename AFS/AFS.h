#pragma once

struct SuperBlock
{
	int totalBlocks;
	int freeBlocks;
	int usedBlocks;
	int blockSize;
	int bitmapSize;
	int wordsInBitmap;
	unsigned int totalSize;
	int totalInodes;
	int freeInodes;
	int bitmapBlock;
	int directoryBlock;
	int inodeTableBlock;
	char partition;
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
	int blockPointer;
};

class AFS
{
private:
	SuperBlock super;
	unsigned int* bitmap;


	void initializeSuperBlock(unsigned int partitionSize, char partition);
	void initializeBitmap();
	int calculateInodeTableInitialBlock() const;
	int calculateDirectoryInitialBlock() const;

public:

	AFS();
	void mountFileSystem(std::string diskName, char partition, unsigned int size);


	~AFS();
};

