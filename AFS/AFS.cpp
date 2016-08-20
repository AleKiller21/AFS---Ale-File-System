#include "stdafx.h"
#include "AFS.h"

using namespace std;

AFS::AFS()
{
}

void AFS::mountFileSystem(string diskName, char partition, unsigned int size)
{
	ofstream out(diskName.c_str(), ios::binary);
	out.seekp(0);

	initializeSuperBlock(size, partition);
	initializeBitmap();
	out.write(reinterpret_cast<char*>(&super), sizeof(SuperBlock));
	out.seekp(super.bitmapBlock * super.blockSize);
	out.write(reinterpret_cast<char*>(bitmap), super.bitmapSize);
	out.close();
}

void AFS::initializeSuperBlock(unsigned int partitionSize, char partition)
{
	super.blockSize = 4096;
	super.totalBlocks = partitionSize / super.blockSize;
	super.freeBlocks = super.totalBlocks;
	super.usedBlocks = 0;
	super.totalSize = partitionSize;
	super.partition = partition;
	super.totalInodes = super.blockSize / sizeof(Inode);
	super.freeInodes = super.totalInodes;
	super.bitmapBlock = 1;
	super.bitmapSize = super.totalBlocks / 8;
	super.wordsInBitmap = super.bitmapSize / sizeof(int);
	super.directoryBlock = calculateDirectoryInitialBlock();
	super.inodeTableBlock = calculateInodeTableInitialBlock();
}

void AFS::initializeBitmap()
{
	bitmap = new unsigned int[super.wordsInBitmap];

	for (int i = 0; i < super.wordsInBitmap; i++)
	{
		bitmap[i] = 0;
	}
}

int AFS::calculateDirectoryInitialBlock() const
{
	return ceil(super.bitmapSize / static_cast<double>(super.blockSize) + super.bitmapBlock);
}

int AFS::calculateInodeTableInitialBlock() const
{
	int directoryBlocks = ceil((static_cast<double>(super.totalInodes) * sizeof(DirectoryEntry)) / super.blockSize);

	return directoryBlocks + super.directoryBlock;
}

AFS::~AFS()
{
}
