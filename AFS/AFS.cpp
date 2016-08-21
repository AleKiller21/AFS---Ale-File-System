#include "stdafx.h"
#include "AFS.h"

using namespace std;

AFS::AFS()
{
}

void AFS::mountFileSystem(string diskName, char partition, unsigned int size)
{
	disk.open(diskName.c_str(), ios::binary | ios::out | ios::in);
	disk.seekp(0);

	initializeSuperBlock(size, partition);
	initializeBitmap();
	initializeDirectory();

	disk.write(reinterpret_cast<char*>(&super), sizeof(SuperBlock));

	disk.seekp(super.bitmapBlock * super.blockSize);
	disk.write(reinterpret_cast<char*>(bitmap), super.bitmapSize);

	disk.seekp(super.directoryBlock * super.blockSize);
	disk.write(reinterpret_cast<char*>(directory), super.directorySize);

	disk.close();
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
	super.directorySize = super.totalInodes * sizeof(DirectoryEntry);
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
