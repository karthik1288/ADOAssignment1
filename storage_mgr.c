#include "stdio.h"
#include "stdlib.h"
#include "dberror.h"
#include "sys/stat.h"
#include "storage_mgr.h"      // Storage manager interfacing
#include "fcntl.h"            // File control
#include "unistd.h"           // For access and file existence

void initStorageManager (void)
{//initialize any of varibales if needed
}

int check_access (char *fn,char type)
{
int amode,rval;
switch ( type ) 
{
case 'e':
// for existence
amode = 00;
break;

case 'r':
// for reading
amode = 04;
break;

case 'w':
// for writing
amode = 02;
break;

case 'x':
// for reading and writing
amode = 06;
break;

default:
// return -1
return -1;
break;
}
return access(fn, amode);
}


RC createPageFile (char *fileName)
{
RC Return_Code;
FILE *File_Pointer; 
int rval,ctr; 
//Check for existence
rval = check_access(fileName,'e');
	if( rval != -1)
	{
	//if exists say file already exists
	printf("\n The file exists already");
	Return_Code = RC_OK;
	}
// Open file
File_Pointer = fopen (fileName,"wb");
	// Check for return code when file creates
	if (File_Pointer == NULL)
	{
	printf("\n File creation error");
	Return_Code = RC_FILE_HANDLE_NOT_INIT;
	}
	else
	{
	// File opened
	// Create if not exists and pointer returns not null
	printf("\nFile created and opened to write null characters");
	// Fill it with null bytes for first page (of PAGE_SIZE)
	for(ctr=0 ; ctr<PAGE_SIZE; ctr++)
	fprintf(File_Pointer,"%c",'\0');
	Return_Code = RC_OK;
	}
fclose(File_Pointer);
return Return_Code;
}


RC openPageFile (char *fileName, SM_FileHandle *fHandle)
{
RC Return_Code;
FILE *File_Pointer; 
int rval;
struct stat stat_sample;
unsigned long int Page_Blocks=0,FILE_SIZE;
rval = check_access(fileName,'r');
	//Check fo existence
	if (rval != -1)
	{
	printf("\nFile exists and has read access");
	File_Pointer = fopen(fileName, "rb+");
		if(File_Pointer == NULL)
		{
		printf("\nYou have no read access to this file");
		Return_Code = RC_FILE_HANDLE_NOT_INIT;
		goto exit;
		}
		else
		{
		// if opened get the entire file size using sys/stat
		printf("\nFile named %s opened successfully",fileName);
		FILE_SIZE = stat_sample.st_size;
		printf("\nFile opened successfully");
		if (stat(fileName, &stat_sample) == 0)
		{
		// Then divide by PAGE_SIZE
		Page_Blocks = (FILE_SIZE)/PAGE_SIZE;
			// Assign no of pages to struct element
			if ((FILE_SIZE%PAGE_SIZE) > 0)
			{
			fHandle -> totalNumPages = (Page_Blocks+1);
			}
			else
			{
			fHandle -> totalNumPages = 1 ;
			}
		// initilize rest 2 and mgmt_Info to pointer 
        	fHandle -> mgmtInfo = File_Pointer;
		fHandle -> fileName = fileName;
        	fHandle -> curPagePos = 0;
        	Return_Code = RC_OK;
		}
		}
	}
	else
	{
	// if file doesnt exists then return file not found error RC_FILE_NOT_FOUND 1 or no read access
	printf("\n File doesnt exists");
	Return_Code = RC_FILE_NOT_FOUND;
	}
exit:
return Return_Code;
}

RC closePageFile (SM_FileHandle *fHandle)
{
RC Return_Code;
int rval;
printf("\n Entered Closefile method");
//Check for file existence
//Check fo existence
if (fHandle == NULL)
{
printf("not init");
Return_Code = RC_FILE_HANDLE_NOT_INIT; 
}
//if exists, check mgmtInfo  
else if (fclose (fHandle -> mgmtInfo) == 0)  
{
printf("\nFile named %s closed successfully",fHandle->fileName);
Return_Code = RC_OK;
}
else 
{
printf("\nFile closing error");
Return_Code = RC_FILE_HANDLE_NOT_INIT;
}
return Return_Code;
}

RC destroyPageFile (char *fileName)
{
RC Return_Code;
FILE *File_Pointer; 
int rval;

//Check for file existence
rval = check_access(fileName,'e');
//Check fo existence
if (rval != -1)
	{
	printf("\n File exists");
	// if exists ,try deleting
	if(remove(fileName) == 0)
	{
	printf("\nFile named %s deleted", fileName); 
	Return_Code = RC_OK;
	}
	else
	{
	// If anything else say File cant be destroyed as being used
	printf("\nFile named %s cant be deleted", fileName); 	 
	Return_Code = RC_FILE_HANDLE_NOT_INIT;
	}
}
else
{
//If not,file not found exception RC_FILE_NOT_FOUND 1 
printf("\n File doesnt exists");
Return_Code = RC_FILE_NOT_FOUND;
}
return Return_Code;
}

/* reading blocks from disc */

RC readBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
RC Return_Code;
// Check for mgmt_Info and if its proper 
if ( fHandle -> mgmtInfo = NULL)
{
Return_Code = RC_FILE_HANDLE_NOT_INIT;
printf("\nHandle seeking error");
}
else
{
	// check for no of pages (If more return RC_READ_NON_EXISTING_PAGE 4) then read block
	if( pageNum > fHandle-> totalNumPages )
	{
	Return_Code = RC_READ_NON_EXISTING_PAGE;
	printf("\nPage requested wasn't found");
	}
	else
	{
	// Seek it to no of pages passed fseek(file pointer,PAGE_SIZE(no of pages-1),SEEK_SET))
	fseek (fHandle -> mgmtInfo , PAGE_SIZE * (pageNum - 1) , SEEK_SET);
	// fread (page handle,size of page,no of blocks to read,fp)
	printf("\nReading current block...");
	fread ( memPage, PAGE_SIZE , 1, fHandle -> mgmtInfo);
	// Set the position of page in a file
	fHandle->curPagePos=pageNum;
	Return_Code = RC_OK ;
	}
}
return Return_Code;
}

// Return current Block position
 
int getBlockPos (SM_FileHandle *fHandle)
{
int Block_Pos = 0;
// Check for file handle
if (fHandle != NULL)
// Return position of page in a file (fHandle -> current position)
Block_Pos=fHandle->curPagePos;
else
printf ("\nFile handle error");
return Block_Pos;
}

RC readFirstBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
RC Return_Code;
FILE *File_Pointer;
File_Pointer = fHandle -> mgmtInfo;
// Check for mgmt_Info and if its proper 
if ( fHandle -> mgmtInfo = NULL)
{
Return_Code = RC_FILE_HANDLE_NOT_INIT;
printf("\nHandle seeking error");
}
else
{
// read first block
// current postion of page to one
	// Seek to first block (fp,0,seek_set)
	fseek ( File_Pointer , 0 , SEEK_SET);
	// Send to first of file
	rewind (File_Pointer);
	// fread (page handle,size of page,no of blocks to read,fp)
	printf("\nReading first block...");
	fread ( memPage, PAGE_SIZE , 1, File_Pointer);
	// Set the position of page in a file
	fHandle->curPagePos=0;
	Return_Code = RC_OK ;
	// Set tht again for book keeping
	fHandle-> mgmtInfo = File_Pointer;
}
return Return_Code;
}

RC readPreviousBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
RC Return_Code;
// Check for mgmt_Info and if its proper 
if ( fHandle -> mgmtInfo = NULL)
{
Return_Code = RC_FILE_HANDLE_NOT_INIT;
printf("\nHandle seeking error");
}
else
{
	if (fHandle -> curPagePos == 1)
	{
	printf("\nPage requested doesnt exist as now we are in page 1");
	Return_Code = RC_READ_NON_EXISTING_PAGE;
	}
	else
	{
	// Seek to first block (fp,0,seek_set)
	fseek (fHandle -> mgmtInfo , ( fHandle->curPagePos - 2) , SEEK_SET);
	// fread (page handle,size of page,no of blocks to read,fp)
	printf("\nReading previous block...");
	fread ( memPage, PAGE_SIZE , 1, fHandle -> mgmtInfo);
	// Set the position of page in a file
	fHandle->curPagePos= fHandle->curPagePos-1 ;
	Return_Code = RC_OK ;
	}
}
return Return_Code;
}

RC readCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
RC Return_Code;
// Check for mgmt_Info and if its proper 
if ( fHandle -> mgmtInfo = NULL)
{
Return_Code = RC_FILE_HANDLE_NOT_INIT;
printf("\nHandle seeking error");
}
else
{
	if (fHandle -> curPagePos == fHandle -> totalNumPages)
	{
	printf("\nPage requested doesnt exist as now we are in Last page");
	Return_Code = RC_READ_NON_EXISTING_PAGE;
	}
	else
	{
	// Seek to next block (fp,0,seek_set)
	fseek (fHandle -> mgmtInfo , 0 , SEEK_CUR);
	// fread (page handle,size of page,no of blocks to read,fp)
	printf("\nReading current block...");
	fread ( memPage, PAGE_SIZE , 1, fHandle -> mgmtInfo);
	Return_Code = RC_OK ;
	}
}
return Return_Code;
}

RC readNextBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
RC Return_Code;
// Check for mgmt_Info and if its proper 
if ( fHandle -> mgmtInfo = NULL)
{
Return_Code = RC_FILE_HANDLE_NOT_INIT;
printf("\nHandle seeking error");
}
else
{
	if (fHandle -> curPagePos == fHandle -> totalNumPages)
	{
	printf("\nPage requested doesnt exist as now we are in last page");
	Return_Code = RC_READ_NON_EXISTING_PAGE;
	}
	else
	{
	// Seek to first block (fp,0,seek_set)
	fseek (fHandle -> mgmtInfo , ( fHandle->curPagePos ) , SEEK_SET);
	// fread (page handle,size of page,no of blocks to read,fp)
	printf("\nReading previous block...");
	fread ( memPage, PAGE_SIZE , 1, fHandle -> mgmtInfo);
	// Set the position of page in a file
	fHandle->curPagePos= fHandle->curPagePos+1 ;
	Return_Code = RC_OK ;
	}
}
return Return_Code;
}

RC readLastBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
RC Return_Code;
// Check for mgmt_Info and if its proper 
if ( fHandle -> mgmtInfo = NULL)
{
Return_Code = RC_FILE_HANDLE_NOT_INIT;
printf("\nHandle seeking error");
}
else
{
// read last block
// current position of page to no of pages
	// Seek to first block (fp,0,seek_set)
	fseek (fHandle -> mgmtInfo , PAGE_SIZE , SEEK_END);
	// fread (page handle,size of page,no of blocks to read,fp)
	printf("\nReading first block...");
	fread ( memPage, PAGE_SIZE , 1, fHandle -> mgmtInfo);
	// Set the position of page in a file
	fHandle->curPagePos= fHandle-> totalNumPages;
	Return_Code = RC_OK ;
}
return Return_Code;
}


/* writing blocks to a page file */

RC writeBlock (int pageNum, SM_FileHandle *fHandle, SM_PageHandle memPage)
{
RC Return_Code;
FILE *File_Pointer;
File_Pointer = fHandle->mgmtInfo;
printf ("File is:%s",fHandle->fileName);
// Check for mgmt_Info and if its proper 
if ( fHandle -> mgmtInfo = NULL)
{
Return_Code = RC_FILE_HANDLE_NOT_INIT;
printf("\nHandle seeking error");
}
else
{
		printf("\nPage exists");
		// Seek it to no of pages passed fseek(file pointer,PAGE_SIZE(no of pages),SEEK_SET))
		fseek ( File_Pointer ,(PAGE_SIZE * pageNum),SEEK_SET);
		// fwrite (page handle,PAGE_SIZE,1,fp)
		printf("\nReading current block...");
		if (fwrite ( memPage, PAGE_SIZE , 1, File_Pointer) != 0)
		{
		// Set the current position of page in a file
		fHandle->curPagePos=pageNum;
		Return_Code = RC_OK ;
		}
		else
		{
		// If failure then set RC_WRITE_FAILED 3 
		Return_Code = RC_WRITE_FAILED;
		RC_message = "\nNo Write access to file";	
		printf(RC_message);
		}
fHandle->mgmtInfo = File_Pointer;
}
return Return_Code;
}

RC writeCurrentBlock (SM_FileHandle *fHandle, SM_PageHandle memPage)
{
RC Return_Code;
FILE *fp;
// Check for mgmt_Info and if its proper 
if ( fHandle -> mgmtInfo = NULL)
{
Return_Code = RC_FILE_HANDLE_NOT_INIT;
printf("\nHandle seeking error");
}
else
{
	// check for no of pages (If more return RC_READ_NON_EXISTING_PAGE 4) then write block
	if(fHandle->curPagePos > fHandle-> totalNumPages )
	{
	Return_Code = RC_READ_NON_EXISTING_PAGE;
	printf("\nPage requested wasn't found");
	}
	else
	{
		//fp = 0; 
		printf("\nPage exists");
		// Seek it to no of pages passed fseek(file pointer,PAGE_SIZE(no of pages-1),SEEK_SET))
		fseek (fHandle -> mgmtInfo , 0 , SEEK_CUR);
		// fwrite (page handle,PAGE_SIZE,1,fp)
		printf("\nReading current block...");
		if (fwrite ( memPage, PAGE_SIZE , 1, fHandle -> mgmtInfo) != 0)
		{
		// Set the current position of page in a file
		Return_Code = RC_OK ;
		}
		else
		{
		// If failure then set RC_WRITE_FAILED 3 
		Return_Code = RC_WRITE_FAILED;
		RC_message = "\nNo Write access to file";	
		printf(RC_message);
		}
	}
}
return Return_Code;
}

RC appendEmptyBlock (SM_FileHandle *fHandle)
{
FILE *File_Pointer;
RC Return_Code;
int ctr;
// Check for mgmt_Info and if its proper 
if ( fHandle -> mgmtInfo = NULL)
{
Return_Code = RC_FILE_HANDLE_NOT_INIT;
printf("\nHandle seeking error");
}
else
{
// update no of pages = pages +1
fHandle -> totalNumPages ++;
// Seek it to last page fseek(file pointer,PAGE_SIZE,SEEK_END)
fseek ( fHandle -> mgmtInfo , PAGE_SIZE , SEEK_END );
// Fill it with \0 bytes for PAGE_SIZE bytes
for(ctr = 0 ; ctr<PAGE_SIZE; ctr++)
fprintf(File_Pointer,"%c",'\0');
// current page position to last page
fHandle -> curPagePos = fHandle-> totalNumPages;
Return_Code = RC_OK;
}
return Return_Code;
}

RC ensureCapacity (int numberOfPages, SM_FileHandle *fHandle)
{
RC Return_Code;
int ctr,pageBlocks,hold_size,set;
struct stat stat_instance;
// Check for mgmt_Info and if its proper 
if (stat(fHandle-> fileName, &stat_instance) == 0)
{
pageBlocks = (stat_instance.st_size) / PAGE_SIZE;
	if ((stat_instance.st_size) % PAGE_SIZE > 0)
	pageBlocks = pageBlocks+1;
	if(pageBlocks > numberOfPages)
	set = 0;
	else 
	set = 1;
	if(set == 1)
	{
	hold_size = (numberOfPages - pageBlocks) *PAGE_SIZE;
	for (ctr = 0 ; ctr < hold_size ; ctr ++)
	fprintf (fHandle->mgmtInfo, "%c", '\0');
	}
	else
	printf ("\nWe already ensured capacity");
Return_Code = RC_OK;
}
else
{
printf ("\nHandle error");
Return_Code = RC_FILE_HANDLE_NOT_INIT;
}
return Return_Code;
}
