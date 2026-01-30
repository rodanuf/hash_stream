#pragma once

template <typename t_element>
class i_iterator
{
public:
    virtual ~i_iterator() = default;

    virtual bool has_next() const = 0;
    virtual bool next() = 0;
    virtual bool try_get_current(t_element &element) = 0;
    
    virtual t_element get_current() const = 0;
};