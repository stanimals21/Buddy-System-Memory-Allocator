#include "BuddyAllocator.h"
#include "math.h"
#include <iostream>
using namespace std;

BuddyAllocator::BuddyAllocator (int _basic_block_size, int _total_memory_length){
  // ensure that bbs and total_mem are multiples of 2
  int bbs = pow(2,ceil(log2(_basic_block_size))); // ceil function to make sure that X in a 2^X size is an integer
  int total_mem_value = pow(2,ceil(log(_total_memory_length)));

  // initialize address pointing to head of memory chunk
  total_mem_ptr = new char(_total_memory_length);

  // create FreeList
  BlockHeader* fullBlock = (BlockHeader*) total_mem_ptr; // initial block
  fullBlock->block_size = total_mem_value;
  fullBlock->isFree = true;
  fullBlock->next = nullptr;

  // calculate number of lists in FreeList
  
}

BuddyAllocator::~BuddyAllocator (){
	
}

char* BuddyAllocator::alloc(int _length) {
  /* This preliminary implementation simply hands the call over the 
     the C standard library! 
     Of course this needs to be replaced by your implementation.
  */
  return new char [_length];
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
      count ++;
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

