# A ring buffer library

A ring buffer library with strategy selection for buffers of arbitrary sizes.

## Buffer creation

Include the header.

```cpp
#include <ringbuffer/RingBuffer.hpp>
using namespace ringbuffer;
```

### Dynamic buffer creation

After that, in order to dynamically create a buffer, call the static `RingBuffer::create(size)`
method with any buffer size greater than `0`:

```cpp
RingBuffer *buffer = RingBuffer::create(64);
```

By doing so, `buffer` takes the ownership of the `new`'d memory
and will delete it upon destruction.

### Buffer reuse

If an already existing memory location is to be used, you can
pass the pointer to it to `RingBuffer::create(ptr, size)`.

```cpp
#define BUFFER_SIZE (64)
rbdata_t storage[BUFFER_SIZE];

RingBuffer *buffer = RingBuffer::create(storage, BUFFER_SIZE);
```

In that case, ownership of the memory location is not transferred unless
you pass `true` as the third parameter (`free_on_delete`) to `create()`.

### Optimization strategies

An optimized (branchless) strategy for the ring buffer will be used if the
given size is a power of two.

## Reading from and writing to the buffer

In order to read and write from the buffer, the `tryRead()`, `readSync()`, 
`tryWrite()` and `writeSync()` methods exist.

Both sync methods will block if the buffer is either full (when writing) or
empty (when reading)

```cpp
// Read example: block if the buffer is empty
rbdata_t rdata;
data = buffer->readSync();

// Write example: block if the buffer is full
rbdata_t wdata = 0x42;
buffer->writeSync(wdata);
```

If non-blocking access is required, the `try...()` methods can be used. 
Both return a `bool` signaling if the read or write operation succeeded.

```cpp
// Read example: return false if the buffer is empty
rbdata_t rdata;
bool dataRead = buffer->tryRead(&rdata);

// Write example: return false if the buffer is full
rbdata_t wdata = 0x42;
bool dataWritten = buffer->tryWrite(wdata);
```

Note that neither read and write operations are atomic.

In order to flush the buffer, the `reset()` method can be used which
resets the internal capacity counter. To retrieve the current fill level,
`buffer->getFillLevel()` can be called; The remaining free space can
be obtained by `buffer->getCapacity()`.