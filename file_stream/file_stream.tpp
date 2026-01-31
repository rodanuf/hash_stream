#include "file_stream.hpp"
#include <stdexcept>

template<typename T>
file_stream<T>::file_stream(const std::string &path)
    : file_path(path), position(0), is_open(false)
{
    open_file();
}

template<typename T>
file_stream<T>::~file_stream()
{
    if (is_open)
    {
        file.close();
    }
}

template<typename T>
T file_stream<T>::read()
{
    if (file.eof())
    {
        throw std::out_of_range("End of stream reached");
    }

    if (!is_open)
    {
        throw std::runtime_error("Cannot open file: " + file_path);
    }

    T value;
    file.read(reinterpret_cast<char *>(&value), sizeof(T));
    if (file.gcount() != sizeof(T))
    {
        throw std::runtime_error("Incomplete read");
    }
    ++position;
    return value;
}

template<typename T>
void file_stream<T>::write(const T &item)
{
    if (!is_open)
    {
        throw std::runtime_error("Cannot open file: " + file_path);
    }

    file.write(reinterpret_cast<const char *>(&item), sizeof(T));
    ++position;
    if (!file.good())
    {
        throw std::runtime_error("Write error");
    }
}

template<typename T>
void file_stream<T>::move_position(int pos)
{
    open_file();
    file.seekg(0, std::ios::end);
    int file_size_elements = static_cast<int>(file.tellg()) / sizeof(T);

    if (pos < 0 || pos > file_size_elements)
    {
        throw std::out_of_range("Position out of bounds");
    }
    move_position_in_bytes(pos);
}

template<typename T>
int file_stream<T>::get_current_pos() const
{
    return position;
}

template<typename T>
void file_stream<T>::reset()
{
    if (is_open)
    {
        file.flush();
    }
}

template<typename T>
void file_stream<T>::close()
{
    if (is_open)
    {
        file.close();
        is_open = false;
    }
}

template<typename T>
uniq_ptr<array_sequence<T>> file_stream<T>::to_sequence()
{
    uniq_ptr<array_sequence<T>> result(new array_sequence<T>());
    reset();
    while (file.peek() != EOF)
    {
        try
        {
            result->append_element(read());
        }
        catch (...)
        {
            break;
        }
    }
    return result;
}

template<typename T>
void file_stream<T>::from_sequence(const array_sequence<T> &seq)
{
    reset();
    for (int i = 0; i < seq.get_length(); ++i)
    {
        write(seq.get(i));
    }
}

template<typename T>
void file_stream<T>::open_file()
{
    if (!is_open)
    {
        file.open(file_path, std::ios::binary | std::ios::in | std::ios::out);
        if (!file)
        {
            file.open(file_path, std::ios::binary | std::ios::out);
            file.close();
            file.open(file_path, std::ios::binary | std::ios::in | std::ios::out);
        }
        position = 0;
        is_open = true;
    }
    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file: " + file_path);
    }
}

template<typename T>
void file_stream<T>::move_position_in_bytes(int element_pos)
{
    file.seekg(element_pos * sizeof(T), std::ios::beg);
    file.seekp(element_pos * sizeof(T), std::ios::beg);
    position = element_pos;
}