/**
 *  Memory Pool
 *  (c) James G Hurley, 2011.
 *
 **/
/**
 *
 *  Memory/Pool.h
 *  
 *  Inspired by boost::pool, except it allocates contiguous blocks for different
 *  game component types.  The first [sizeof(void*)] bytes of each block is a 
 *  pointer to the next free block - IF AND ONLY IF THE BLOCK IS FREE - if it
 *  isn't, the memory could be anything.
 **/

#include <Memory/Pool.h>
#include <string.h>
#include <string>


namespace helios
{
    
    void 
    Pool::allocate_pool(std::string type, size_t size)
    {
        
       
        // Allocate the pool for the component type and create the initial chunk
        // first pointer chain
        
        D_PRINT("\nAllocating pool for %s: %zu\n", type.c_str(), size);
        
        size_t chunk_size = size;
        ulong chunk_count = mPoolSize ;
        
        block[type] = (byte*)calloc(1,chunk_count*chunk_size*sizeof(byte));
        
        byte *base = block[type];
        for ( ulong i = 0 ; i < chunk_count ; ++i )
        {
            byte * _p = base + (chunk_size*i);
            
            ulong * p = (ulong*)_p;
            *p = (ulong)base+(chunk_size*(i+1));
        }
        firstPtr[type] = (void*)&block[type][0];
    }
    // Allocate a chunk by moving the firstPtr to the next available chunk
    //
    void *
    Pool::alloc(std::string type, size_t size)
    {
        mMutex.lock();
        
        size = ( size + 31 ) & ~31;   // Align to 32-bytes.
        
        if(block[type] == NULL) 
            allocate_pool(type,size); // Allocate a new pool if there isn't one for this component type
        
        ulong *p = 0;
          
        void *fp = firstPtr[type];
        
        if(fp)
        {
            p  = (ulong*)fp;
            fp = (ulong*)*p;
        }
        
        firstPtr[type] = fp;
        
        mMutex.unlock();
        
        return (void*)p;
    }
    // Reclaim the memory chunk by adding the chunk to the list of free
    // memory.
    void 
    Pool::release(void *ptr, std::string type)
    {
      //  boost::mutex::scoped_lock l(mMutex);
        mMutex.lock();
        ulong *p = (ulong*)ptr;

        void *fp = firstPtr[type];
        // If the pointer is before the first free chunk,
        // Simply point the first free chunk to this chunk
        // and create a pointer to the previous first free
        // chunk in the first sizeof(void*) bytes of this chunk
        if(fp && ptr < fp)
        {
            *p = (ulong)fp;
            fp = p;
            
        }
        // Otherwise, follow the chain and find where the chunk belongs.
        else if(fp) {
            
            ulong *v= (ulong*)fp, *prev;
            while(v && v < p)
            {
                prev = v;
                v = (ulong*)*v;
            }
            *prev = (ulong)ptr;
            *p    = (ulong)v;
        } 
        // If the memory chunk is completely used up, firstPtr == 0.
        // Free this chunk, and assign it to firstPtr. Assign the next ptr
        // to 0.
        else {
            fp = p;
            *p = 0UL;
        }
        firstPtr[type] = fp;
        
        mMutex.unlock();
    }
    Pool::~Pool()
    {
       
       // std::map<std::string, byte*>::iterator it, ite;
        auto it = block.begin();
        auto ite = block.end();
        
        // Free the pool memory
        for ( ; it != ite ; ++it)
        {            
            free((*it).second);
        }
    }
}
