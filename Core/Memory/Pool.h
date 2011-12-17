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

#ifndef __Pool_h
#define __Pool_h
#include <stdlib.h>


#include <map>
#include <common.h>

#ifdef _LIBCPP_STABLE_APPLE_ABI
    #include <thread>
    #include <mutex>
#else
    #include <boost/utility.hpp>
    #include <boost/scoped_ptr.hpp>
    #include <boost/thread/thread.hpp>
    #include <boost/thread/once.hpp>
#endif



typedef unsigned char byte;
typedef unsigned long ulong;
namespace helios
{
       
    class Pool
    {
    private:
        std::map<std::string,void*> firstPtr;
        
        //boost::mutex       mMutex;
#ifdef _LIBCPP_STABLE_APPLE_ABI
        std::mutex mMutex;
#else
        boost::mutex       mMutex;
#endif
        
        std::map<std::string, byte*> block;
        size_t mChunkSize;
        size_t mPoolSize;
        
        void allocate_pool(std::string type,size_t size);
        
    public:
        Pool(size_t chunk_size, size_t pool_size) : mChunkSize(chunk_size), mPoolSize(pool_size) {};
        Pool(size_t pool_size) : mPoolSize(pool_size) {} ;
        ~Pool();
      
        void *alloc (std::string type,size_t size);
        void release(void* ptr, std::string type);
        
    };
};
#endif
