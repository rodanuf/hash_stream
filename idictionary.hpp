#pragma once 

template <typename t_key, typename t_value>
class i_dictionary 
{
public: 
    virtual ~i_dictionary() = default;

    virtual int get_count() const = 0;
    virtual int get_capacity() const = 0;

    virtual t_value get(const t_key& key) const = 0;

    virtual bool contains_key(const t_key& key) const = 0;
    virtual bool try_add(const t_key& key, const t_value& value) = 0;
    virtual bool try_remove(const t_key& key) = 0;

    virtual uniq_ptr<i_iterator<t_key>> get_keys_iterator() const = 0;
};