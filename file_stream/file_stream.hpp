#pragma once

#include "i_stream.hpp"
#include "../lab3_2ndsem/headers/array_sequence.hpp"
#include "../pointers/uniq_ptr.hpp"
#include <fstream>
#include <string>

template <typename T>
class file_stream : public i_stream<T>
{
private:
    std::string file_path;
    std::fstream file;
    int position;
    bool is_open;

public:
    explicit file_stream(const std::string &path);
    ~file_stream() override;

    T read() override;

    void write(const T& item) override;
    void move_position(int position) override;
    void reset() override;
    void close() override;
    void from_sequence(const array_sequence<T> &seq);

    int get_current_pos() const override;

    uniq_ptr<array_sequence<T>> to_sequence();


private:
    void open_file();
    void move_position_in_bytes(int element_pos);

};

#include "file_stream.tpp"