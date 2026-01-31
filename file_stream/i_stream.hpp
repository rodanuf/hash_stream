#pragma once 

template <typename T>
class i_stream {
public:
    virtual ~i_stream() = default;

    virtual T read() = 0;

    virtual void write(const T &value) = 0;
    virtual void move_position(int position) = 0;
    virtual void reset() = 0;
    virtual void close() = 0;

    virtual int get_current_pos() const = 0;
};