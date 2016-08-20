#pragma once

struct SuperBlock
{
	int totalBlocks;
	int freeBlocks;
	int usedBlocks;
	int blockSize;
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
	void initializeSuperBlock(unsigned int partitionSize, char partition);
	int calculateTotalInodes() const;
	int calculateInodeTableInitialBlock() const;
	int calculateDirectoryInitialBlock() const;

public:
	SuperBlock super;
	DirectoryEntry entry;
	Inode inode;

	AFS();
	void mountFileSystem(std::string diskName, char partition, unsigned int size);


	~AFS();
};

