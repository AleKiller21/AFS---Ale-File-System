#pragma once

struct SuperBlock
{
	int totalBlocks;
	int freeBlocks;
	int usedBlocks;
	int blockSize;
	int totalSize;
	int totalInodes;
	int freeInodes;
	int bitmapBlock;
	int directoryBlock;
	int inodeTableBlock;
	char fileSystemName[16];
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
public:
	AFS();



	~AFS();
};

