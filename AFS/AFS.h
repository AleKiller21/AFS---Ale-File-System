#pragma once

struct SuperBlock
{
	int totalBlocks;
	int freeBlocks;
	int usedBlocks;
	int blockSize;
	int bytesAvailablePerBlock;
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
	unsigned int dataBlocks;
	char DateCreated[32];
	//DateLastModified
	unsigned int blockPointer;
};

struct FileInfo
{
	char name[256];
	char DateCreated[32];
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
	int createNewFile(unsigned int size, std::string name, std::string sourceFilePath);
	int checkIfEnoughFreeBlocks(unsigned int fileSize) const;
	unsigned int* getBlocksForFile(unsigned int size);
	unsigned int calculateBlockNumberInBitmap(int wordsOccupied, int blockPositionInWord);
	int assignInodeToFile(unsigned int fileSize, unsigned int* dataBlocks) const;
	void saveFileInDirectoryEntry(const char* name, int inode) const;
	void restoreBitmap();
	bool isFileSystemMounted() const;
	bool checkFileExist(std::string name) const;
	void saveBytesIntoDataBlocks(unsigned int* fileBlocks, int inumber, std::string sourceFilePath);
	void setUpBuffer(char* buffer, unsigned int sizeOfBuffer);
	unsigned int convertFileSizeToBlocks(unsigned int size) const;
	int searchFileInDirectory(std::string fileName) const;
	void getFileData(int inode, std::string targetFilePath);
	void freeBlocksOnBitmap(std::list<unsigned int>* blocks) const;
	int extractFileNamesFromPath(std::list<std::string>* path, std::list<std::string>* originNameList, std::list<std::string>* destinyNameList);
	std::list<unsigned int>* getFileBlocks(int inode);

public:

	AFS();
	int writeFileSystemStructuresToDisk(std::string diskName);
	int mountFileSystem();
	int unmountFileSystem();
	int createEmptyFile(std::list<std::string>* path);
	int openDisk(std::string name);
	int closeDisk();
	int importFile(std::list<std::string>* path);
	int exportFile(std::list<std::string>* path);
	int renameFile(std::list<std::string>* path);
	int deleteFile(std::list<std::string>* path);
	int deleteDisk(std::string diskName);
	std::list<unsigned int>* getFileSystemInfo() const;
	std::list<FileInfo>* listFiles() const;

	~AFS();
};

