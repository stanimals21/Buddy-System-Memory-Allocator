#ifndef _BuddyAllocator_h_                   // include file only once
#define _BuddyAllocator_h_
#include <iostream>
#include <vector>
using namespace std;
typedef unsigned int uint;

/* declare types as you need */

class BlockHeader{
public:
	// think about what else should be included as member variables
	int block_size;  // size of the block
	BlockHeader* next; // pointer to the next block
	bool isFree;
};

class LinkedList{
	// this is a special linked list that is made out of BlockHeader s. 
public:
	BlockHeader* head;		// you need a head of the list
	int listSize = 0;
public:
	void insert (BlockHeader* b) // adds a block to the list
	{	
		// if linked list is empty
		if(head==nullptr){
			head = b;
			listSize++;
		}
		// if linked list not empty (insert at front)
		else{
			b->next = head;
			head = b;
			listSize++;
		}
	}

	void remove (BlockHeader* b){  // removes a block from the list
		BlockHeader* it = head;

		// if b is the first blockheader
		if(it == head){
			BlockHeader* temp = head;
			
			// if list has more than one blockheader
			if(it->next != nullptr)
			{
				head = it->next;
				listSize--;
			}
			else // if list only has one blockheader
			{
				head = nullptr;
				listSize--;
			}
		}
		
		// if b is not first blockheader
		else if(it != head){
			it = head->next;
			BlockHeader* prevIt = head;

			while(it != b)
			{
				// check if b is late node
				if(it->next == nullptr)
				{
					prevIt->next = nullptr;
					listSize++;
					return;
				}
				it = it->next;
				prevIt = prevIt->next;
			}
			prevIt->next = it->next;
			listSize++;
		}
	}
};

class BuddyAllocator{
private:
	/* declare more member variables as necessary */
	vector<LinkedList> FreeList;
	int basic_block_size;
	int total_memory_size;
	char* total_mem_ptr;

private:
	/* private function you are required to implement
	 this will allow you and us to do unit test */
	
	BlockHeader* getbuddy (BlockHeader * addr); 
	// given a block address, this function returns the address of its buddy 
	
	bool arebuddies (BlockHeader* block1, BlockHeader* block2);
	// checks whether the two blocks are buddies are not

	BlockHeader* merge (BlockHeader* block1, BlockHeader* block2);
	// this function merges the two blocks returns the beginning address of the merged block
	// note that either block1 can be to the left of block2, or the other way around

	BlockHeader* split (BlockHeader* block);
	// splits the given block by putting a new header halfway through the block
	// also, the original header needs to be corrected


public:
	BuddyAllocator (int _basic_block_size, int _total_memory_length); 
	/* This initializes the memory allocator and makes a portion of 
	   ’_total_memory_length’ bytes available. The allocator uses a ’_basic_block_size’ as 
	   its minimal unit of allocation. The function returns the amount of 
	   memory made available to the allocator. If an error occurred, 
	   it returns 0. 
	*/ 

	~BuddyAllocator(); 
	/* Destructor that returns any allocated memory back to the operating system. 
	   There should not be any memory leakage (i.e., memory staying allocated).
	*/ 

	char* alloc(int _length); 
	/* Allocate _length number of bytes of free memory and returns the 
		address of the allocated portion. Returns 0 when out of memory. */ 

	int free(char* _a); 
	/* Frees the section of physical memory previously allocated 
	   using ’my_malloc’. Returns 0 if everything ok. */ 
   
	void printlist ();
	/* Mainly used for debugging purposes and running short test cases */
	/* This function should print how many free blocks of each size belong to the allocator
	at that point. The output format should be the following (assuming basic block size = 128 bytes):

	[0] (128): 5
	[1] (256): 0
	[2] (512): 3
	[3] (1024): 0
	....
	....
	 which means that at this point, the allocator has 5 128 byte blocks, 3 512 byte blocks and so on.*/
};

#endif 
