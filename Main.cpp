#include "Ackerman.h"
#include "BuddyAllocator.h"
#include "math.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void easytest(BuddyAllocator* ba){
  // be creative here
  // know what to expect after every allocation/deallocation cycle

  // here are a few examples
  ba->printlist();
  // allocating a byte
  char * mem = ba->alloc(1);
  
  // now print again, how should the list look now
  ba->printlist();

  ba->free (mem); // give back the memory you just allocated
  ba->printlist(); // shouldn't the list now look like as in the beginning
}

int main(int argc, char ** argv) {

  int basic_block_size = 128, memory_length = 1024*1024*128;

  // source: https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html#Example-of-Getopt
  int opt;

  while((opt = getopt(argc, argv, "b:m:")) != -1)  
  {  
    if((char)opt == 'b'){
      basic_block_size = stoi(optarg);
    }
    else if ((char)opt == 'm'){
      memory_length = stoi(optarg); 
    }
    else{
      break;
    }
  }  

  // input checks
  if(ceil(log2(basic_block_size)) != floor(log2(basic_block_size)))
  {
    cout << "Invalid input: basic block size must be of the form 2^i, where i is an integer." << endl;
    return 0;
  }
  else if(memory_length % basic_block_size != 0)
  {
    cout << "Invalid input: memory length must be a multiple of bbs" << endl;
    return 0;
  }

  // create memory manager
  BuddyAllocator * allocator = new BuddyAllocator(basic_block_size, memory_length);

  // the following won't print anything until you start using FreeList and replace the "new" with your own implementation
  easytest (allocator);
  
  // stress-test the memory manager, do this only after you are done with small test cases
  Ackerman* am = new Ackerman ();
  am->test(allocator); // this is the full-fledged test. 
  
  // destroy memory manager
  delete allocator;
}
