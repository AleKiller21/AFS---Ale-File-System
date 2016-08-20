#include "stdafx.h"
#include "AFS.h"

using namespace std;

void AFS::initializeSuperBlock(unsigned int partitionSize, char partition)
{
	super.blockSize = 4096;
	super.totalBlocks = partitionSize / super.blockSize;
	super.freeBlocks = super.totalBlocks;
	super.usedBlocks = 0;
	super.totalSize = partitionSize;
	super.partition = partition;
	super.totalInodes = calculateTotalInodes();
	super.freeInodes = super.totalInodes;
	super.bitmapBlock = 1;
	super.directoryBlock = calculateDirectoryInitialBlock();
	super.inodeTableBlock = calculateInodeTableInitialBlock();
}

int AFS::calculateTotalInodes() const
{
	return super.blockSize / sizeof(Inode);
}

int AFS::calculateDirectoryInitialBlock() const
{
	return ceil((super.totalBlocks / 8) / static_cast<double>(super.blockSize) + super.bitmapBlock);
}

int AFS::calculateInodeTableInitialBlock() const
{
	int directoryBlocks = ceil((static_cast<double>(super.totalInodes) * sizeof(DirectoryEntry)) / super.blockSize);

	return directoryBlocks + super.directoryBlock;
}

AFS::AFS()
{
}

void AFS::mountFileSystem(string diskName, char partition, unsigned int size)
{
	ofstream out(diskName.c_str(), ios::binary);
	out.seekp(0);

	initializeSuperBlock(size, partition);
	out.write((char*)&super, sizeof(SuperBlock));
	out.close();
}


AFS::~AFS()
{
}
