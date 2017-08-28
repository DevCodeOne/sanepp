#pragma once

#include <iterator>

#include <sane/sane.h>

#include "sane_device.h"

using authorization_callback = SANE_Auth_Callback;

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

class sane_instance final {
    public:
        ~sane_instance();

        sane_instance(const sane_instance &) = delete;
        sane_instance(sane_instance &&) = delete;

        sane_instance &operator=(const sane_instance &) = delete;
        sane_instance &operator=(sane_instance &&) = delete;

        sane_device_info_list get_devices(bool local_devices_only = false) const;

        SANE_Int version_code() const;
    private:
        sane_instance(authorization_callback callback);

        bool m_initialized = false;
        SANE_Int m_version_code;

        friend void create_sane_instance(authorization_callback);
};

void create_sane_instance(authorization_callback callback = nullptr);
sane_instance *get_sane_instance();


