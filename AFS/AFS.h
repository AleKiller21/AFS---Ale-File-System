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
	unsigned int partitionSize;
	int totalInodes;
	int freeInodes;
	int bitmapBlock;
	int directoryBlock;
	int inodeTableBlock;
	int firstDataBlock;
	int inodeTableSize;
};

struct DirectoryEntry
{
	char name[256];
	int inode;
	bool available;
};

struct Inode
{
	bool available;
	unsigned int size;
	int dataBlocks;
	//DateCreated
	//DateLastModified
	unsigned int blockPointer;
};

struct FileInfo
{
	char name[256];
	int inode;
	unsigned int sizeInBytes;
	unsigned int sizeInDisk;
};

class AFS
{

	std::fstream disk;
	SuperBlock super;
	DirectoryEntry* directory;
	Inode* inodes;
	unsigned int* bitmap;

	void loadSuperBlock();
	void loadBitmap();
	void loadDirectory();
	void loadInodeTable();
	void initializeSuperBlock(unsigned int partitionSize);
	void initializeBitmap();
	void initializeDirectory();
	void initializeInodeTable();
	void updateStructuresInDisk();
	void updateSuperBlock(unsigned int size);
	int calculateInodeTableInitialBlock() const;
	int calculateDirectoryInitialBlock() const;
	int calculateInitialDataBlock() const;
	int createNewFile(unsigned int size, std::string name);
	int checkIfEnoughFreeBlocks(unsigned int fileSize) const;
	int* getBlocksForFile(unsigned int size);
	int calculateBlockNumberInBitmap(int wordsOccupied, int blockPositionInWord);
	int assignInodeToFile(unsigned int fileSize, int* dataBlocks) const;
	void saveFileInDirectoryEntry(const char* name, int inode) const;
	void restoreBitmap();
	bool isFileSystemMounted() const;
	bool checkFileExist(std::string name) const;

public:

	AFS();
	int writeFileSystemStructuresToDisk(std::string diskName);
	int mountFileSystem();
	int unmountFileSystem();
	//int validateFileSystemMount();
	int createEmptyFile(std::string name);
	int openDisk(std::string name);
	int closeDisk();
	int importFile(std::string filePath, std::string name);
	std::list<unsigned int>* getFileSystemInfo() const;
	std::list<FileInfo>* listFiles() const;

	~AFS();
};

