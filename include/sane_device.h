#pragma once

#include <string>
#include <vector>
#include <memory>
#include <experimental/optional>

#include <sane/sane.h>

#include "sane_options.h"

using sane_authorization_callback = SANE_Auth_Callback;

// TODO maybe add invalid state
class sane_device final {
    public:

        typedef std::vector<option>::iterator iterator;
        typedef std::vector<option>::const_iterator const_iterator;

        sane_device(const sane_device &) = delete;
        sane_device(sane_device &&);
        ~sane_device();

        sane_device &operator=(const sane_device &) = delete;
        sane_device &operator=(sane_device &&);

        iterator begin();
        const_iterator cbegin() const;
        iterator end();
        const_iterator cend() const;

        explicit operator bool() const;
    private:
        sane_device(const char *device_name);
        void load_options();

        SANE_Handle m_device_handle;
        SANE_Status m_device_status;

        std::vector<option> m_options;

        friend class sane_device_info;
};

// TODO Maybe replace c-style strings with std::string_view ?
class sane_device_info final {
    public:
        const char *name() const;
        const char *vendor() const;
        const char *model() const;
        const char *type() const;
        std::experimental::optional<sane_device> open_this_device() const;
    private:
        sane_device_info(const SANE_Device **device);

        const SANE_Device **m_device;

        friend bool operator==(const sane_device_info &lhs, const sane_device_info &rhs);
        friend class sane_device_info_list_iterator;
};

bool operator==(const sane_device_info &lhs, const sane_device_info &rhs);
bool operator!=(const sane_device_info &lhs, const sane_device_info &rhs);

class sane_device_info_list_iterator final
    : public std::iterator_traits<sane_device_info> {
    public:
        sane_device_info_list_iterator();

        sane_device_info_list_iterator &operator=(sane_device_info_list_iterator);
        sane_device_info_list_iterator &operator++();
        sane_device_info_list_iterator operator++(int);
        sane_device_info &operator*();
        sane_device_info *operator->();
        void swap(sane_device_info_list_iterator &);

        const sane_device_info &operator*() const;
        const sane_device_info *operator->() const;
    private:
        sane_device_info_list_iterator(const SANE_Device **device_ptr);

        sane_device_info m_device_info;

        friend class sane_device_info_list;
};

bool operator==(const sane_device_info_list_iterator &lhs,
        const sane_device_info_list_iterator &rhs);
bool operator!=(const sane_device_info_list_iterator &lhs,
        const sane_device_info_list_iterator &rhs);
void swap(sane_device_info_list_iterator &lhs, sane_device_info_list_iterator &rhs);

class sane_device_info_list final {
    public:
        sane_device_info_list_iterator begin();
        sane_device_info_list_iterator end();
    private:
        sane_device_info_list(const SANE_Device **device_list);
        sane_device_info_list();

        const SANE_Device **m_device_list;

        friend class sane_instance;
};
