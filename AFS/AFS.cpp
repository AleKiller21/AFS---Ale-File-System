#include "stdafx.h"
#include "AFS.h"

#define FIRST_BIT_WORD 2147483648
#define MAX_SIZE_WORD 4294967295

using namespace std;

AFS::AFS()
{
}

void AFS::mountNewFileSystem(string diskName, char partition, std::streamsize size)
{
	disk.open(diskName.c_str(), ios::binary | ios::out | ios::in);
	disk.seekp(0);

	initializeSuperBlock(size, partition);
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

	//cout << bitmap[0] << endl;

	delete[] directory;
	delete[] bitmap;
	delete[] inodes;
}

bool AFS::openDisk(std::string name)
{
	if (disk.is_open()) return false;

	disk.open(name.c_str(), ios::binary | ios::out | ios::in);
	loadStructuresToMemory();

	return true;
}

void AFS::loadStructuresToMemory()
{
	loadSuperBlock();
	loadBitmap();
	loadDirectory();
	loadInodeTable();
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

void AFS::initializeSuperBlock(streamsize partitionSize, char partition)
{
	super.blockSize = 4096;
	super.totalBlocks = partitionSize / super.blockSize;
	super.partitionSize = partitionSize;
	super.partition = partition;
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

int AFS::calculateDirectoryInitialBlock() const
{
	return ceil(super.bitmapSize / static_cast<double>(super.blockSize) + super.bitmapBlock);
}

int AFS::calculateInitialDataBlock() const
{
	return super.inodeTableBlock + static_cast<double>(super.inodeTableSize) / super.blockSize;
}

int AFS::calculateInodeTableInitialBlock() const
{
	int directoryBlocks = ceil((static_cast<double>(super.totalInodes) * sizeof(DirectoryEntry)) / super.blockSize);

	return directoryBlocks + super.directoryBlock;
}

AFS::~AFS()
{
}
