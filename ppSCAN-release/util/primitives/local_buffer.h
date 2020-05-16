#pragma once

#include <cstring>

// T requires to be a trivially copyable type,
// I is the global_buffer_size type.
// LocalWriteBuffer is trivially copyable, behaving like a view of pointers and sizes
template<typename T, typename I>
class LocalWriteBuffer {
    T *buffer_;     // allocated outside, can be either stack or heap memory
    I buffer_size_;   // consider the cache line, multiple of 64 bytes on CPUs.
    I buffer_cap_;

    // global data.
    T *global_data_;
    volatile I *global_buffer_size_ptr_;

    void submit() {
        I tempIdx = __sync_fetch_and_add(global_buffer_size_ptr_, buffer_size_);
        memcpy(global_data_ + tempIdx, buffer_, sizeof(T) * buffer_size_);
        buffer_size_ = 0;
    }

public:
    LocalWriteBuffer() = delete;

    LocalWriteBuffer(T *buffer, I buffer_cap, T *global_data, volatile I *global_buffer_size) :
            buffer_(buffer), buffer_size_(0), buffer_cap_(buffer_cap),
            global_data_(global_data), global_buffer_size_ptr_(global_buffer_size) {}

    void push(T data) {
        buffer_[buffer_size_++] = data;
        if (buffer_size_ >= buffer_cap_) {
            submit();
        }
    }

    void submit_if_possible() {
        if (buffer_size_ != 0) {
            submit();
        }
    }
};