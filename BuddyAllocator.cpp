#include "BuddyAllocator.h"
#include "math.h"
#include <iostream>
using namespace std;

BuddyAllocator::BuddyAllocator (int _basic_block_size, int _total_memory_length){
  basic_block_size = _basic_block_size;
  total_memory_size = _total_memory_length;
  
  // ensure that bbs and total_mem are multiples of 2
  int bbs = pow(2,log2(_basic_block_size));
  int total_mem_value = pow(2,log2(_total_memory_length));

  // initialize address pointing to head of memory chunk
  total_mem_ptr = new char[_total_memory_length];

  // create FreeList
  BlockHeader* fullBlock = (BlockHeader*) total_mem_ptr; // initial block
  fullBlock->block_size = total_mem_value;
  fullBlock->isFree = true;
  fullBlock->next = nullptr;

  // calculate number of lists in FreeList and pushback that many freelists
  int listSize = log2(total_mem_value)-log2(bbs)+1;
  for(int i =0; i<listSize; i++){
    FreeList.push_back(LinkedList());
    FreeList.at(i).head = NULL;
  }

  // add fullBlock to FreeList
  FreeList[listSize-1].insert(fullBlock); // smallest(index 0) to largest(index x)
}

BuddyAllocator::~BuddyAllocator (){
	// for(int i=0; i<FreeList.size(); i++)
  // {
  //   while(FreeList[i].head != NULL){
  //     delete FreeList;
  //   }
  // }
}

// private BuddyAllocator Functions

BlockHeader* BuddyAllocator::split(BlockHeader* block) {
  char* rightAddr = (char*) block + block->block_size/2;
  char* leftAddr = (char*) block;

  // create two new blocks
  BlockHeader* rightBlock_addr = (BlockHeader*) rightAddr;
  BlockHeader* leftBlock_addr = (BlockHeader*) leftAddr;

  rightBlock_addr->block_size = block->block_size/2;
  leftBlock_addr->block_size = block->block_size/2;
  
  freeList_loc = int(log2(ceil((double)(rightBlock_addr->block_size)/(double)basic_block_size)));

  // delete old node
  FreeList[freeList_loc+1].remove(block);

  // insert new nodes (at front)
  FreeList[freeList_loc].insert(rightBlock_addr);
  FreeList[freeList_loc].insert(leftBlock_addr);
  
  // return one of the old block's locations
  return rightBlock_addr;

};

// To  ---------------------------------------------------

BlockHeader* BuddyAllocator::getbuddy (BlockHeader * addr){
  
}

bool BuddyAllocator::arebuddies (BlockHeader* block1, BlockHeader* block2){
  // checks to see if buddies are free
  if(block1->isFree == true && block2->isFree == true) {return true;}
  else {return false;}

}

BlockHeader* BuddyAllocator::merge (BlockHeader* block1, BlockHeader* block2){
  // get locations of blocks
  char* left = (char*) block1;
  char* right = (char*) block2;

  // reverse pointers if in wrong order
  if(block1 > block2)
  {
    char* temp = left;
    left = right;
    right = temp;
  }

  // find location of FreeList to merge into
  int merge_loc = log2(block1->block_size/basic_block_size) + 1; // gives index to merge to
  int del_loc = merge_loc-1; // gives index to delete to

  // delete right and left from freelist (not sure to pass in casted left/right or block1/block2)
  FreeList[del_loc].remove((BlockHeader*)left);
  FreeList[del_loc].remove((BlockHeader*)right);

  // double the size of right and add it to bigger memory freelist (is making a new BlockHeader* bigBlock necessary?)
  BlockHeader* bigBlock = (BlockHeader*) left;
  FreeList[merge_loc].insert((BlockHeader*)bigBlock);
  bigBlock->block_size = 2 * block1->block_size; // remember blocks are free by defualt
  
  return bigBlock;

}

// -----------------------------------------------------

char* BuddyAllocator::alloc(int _length) {
  /* This preliminary implementation simply hands the call over the 
     the C standard library! 
     Of course this needs to be replaced by your implementation.
  */
  
 // iterate through FreeList to find smallest BlockHeader to fit our memory length (start at index 0)
  BlockHeader* space_addr;
  for(int i = 0; i < FreeList.size(); i++){
    if(FreeList[i].head == nullptr && i == FreeList.size()-1)
    {
      return nullptr;
    }

    if(FreeList[i].head == nullptr)
    {
      // skip iteration if true
    }
    else if(FreeList[i].head->block_size >= _length + sizeof(BlockHeader))
    {
      space_addr = FreeList[i].head;
      break;
    }
  }
  
  BlockHeader* track = space_addr;
  while((track->block_size/2 >= (_length + sizeof(BlockHeader))) && (track->block_size/2 >= basic_block_size)) {
    track = split(track);
  }
  
  // assign attributes to new block (allocating BlockHeader)
  track->block_size = _length;
  track->isFree = false;

  // remove last element
  FreeList[freeList_loc].remove(track);

  // return removed element
  return (char*) (track+1);
}

int BuddyAllocator::free(char* _a) {
  /* Same here! */
  delete _a;
  return 0;
}

void BuddyAllocator::printlist (){
  cout << "Printing the Freelist in the format \"[index] (block size) : # of blocks\"" << endl;
  for (int i=0; i<FreeList.size(); i++){
    cout << "[" << i <<"] (" << ((1<<i) * basic_block_size) << ") : ";  // block size at index should always be 2^i * bbs
    int count = 0;
    BlockHeader* b = FreeList [i].head;
    // go through the list from head to tail and count
    while (b){
      count++;
      // block size at index should always be 2^i * bbs
      // checking to make sure that the block is not out of place
      if (b->block_size != (1<<i) * basic_block_size){
        cerr << "ERROR:: Block is in a wrong list" << endl;
        exit (-1);
      }
      b = b->next;
    }
    cout << count << endl;  
  }
}

