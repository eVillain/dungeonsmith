//
//  ChunkPool.h
//  DungeonSmith
//
//  Created by The Drudgerist on 01/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef __DungeonSmith__ChunkPool__
#define __DungeonSmith__ChunkPool__

#include "Chunk.h"
#include <stack>

class ChunkPool
{
public:
    ChunkPool(){}
    
    void Add(Chunk* chunk) {
        _pool.push(chunk);
    }
    
    Chunk* Acquire() {
        assert(!_pool.empty());
        Chunk* tmp(_pool.top());
        _pool.pop();
        return tmp;
    }
    
    bool Empty() const {
        return _pool.empty();
    }
    
    size_t Available() {
        return _pool.size();
    }
    
private:
    std::stack<Chunk*> _pool;
};
#endif /* defined(__DungeonSmith__ChunkPool__) */
