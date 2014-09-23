#inlcude "stdio.h"
#include "stdlib.h"
#include "dberror.h"
#include "sys/stat.h"
#include "storage_mgr.h"
# include "fileio.h"

int main()
{
initStorageManager ()
{//initialize any of varibales if needed
}



RC createPageFile (char *fileName)
{
//Check for existence

//Create if not exists

//if exists say file already exists

//Return FIle create error if pointer is null or cant create RC_FILE_HANDLE_NOT_INIT 2

// open file

// Fill it with null bytes for first page (of PAGE_SIZE)

return RC;
}

RC openPageFile (char *fileName, SM_FileHandle *fHandle)
{
//Check for file name 

// if file doesnt exists then return file not found error RC_FILE_NOT_FOUND 1

// If file exists and pointer null and cant return then open error RC_FILE_HANDLE_NOT_INIT 2

// if opened get the entire file size using sys/stat

// Then divide by PAGE_SIZE

// Assign no fo pages to struct element

//initilize rest 2 and mgmt_Info to O

//return

return RC;
}

RC closePageFile (SM_FileHandle *fHandle);
{
//Check for filename 

//if exists, SET mgmt_Info to C and close

//If not,file not found exception RC_FILE_NOT_FOUND 1
return RC;
}

RC destroyPageFile (char *fileName);
{
// 
//Check for filename 

//if exists,check whether open or not using mgmt_Info and if it is C then destroy

// If anything else say File cant be destroyed as being used

//If not,file not found exception RC_FILE_NOT_FOUND 1
return RC;
}

/* reading blocks from disc */

RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage);
{
// Check for mgmt_Info and if its proper and check for no of pages (If more return RC_READ_NON_EXISTING_PAGE 4) then read block

// Seek it to no of pages passed fseek(file pointer,PAGE_SIZE(no of pages-1) + (pagenumblocks-1)*blk_size,SEEK_CUR))

// fread (page handle,sizeof page,no of blocks to read,fp)

// Set the position of page in a file
return RC;
}

int getBlockPos (SM_FileHandle *fHandle)
{
// Look at file handle,If null set then say file handle error

// Return position of page in a file (fhandle-> current position)
}

RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
// handle existnce and set 
// Seek to first block (fp,0,seek_set)
// read first block
// current postion of page to one
}

RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
// handle existence
// check for page if it is first then say RC_READ_NON_EXISTING_PAGE 4
// not first then fseek(fp,PAGE_SIZE*(currentpagepos-2),seek_set)
// Current page position to current-1
}

RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
// handle existence
// not first then fseek(fp,PAGE_SIZE*(currentpagepos-1),seek_set)
// Current page position to current-1
}

RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
// handle existence
// check for page if it is last then say RC_READ_NON_EXISTING_PAGE 4
// not first then fseek(fp,PAGE_SIZE*(currentpagepos),seek_set)
// Current page position to current+1
}

RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
// handle existnce and set 
// Seek to last block (fp,PAGE_SIZE,seek_end)
// read first block
// current postion of page to last
};

`
/* writing blocks to a page file */

extern RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage);

extern RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage);

extern RC appendEmptyBlock (SM_FileHandle *fHandle);

extern RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle);

