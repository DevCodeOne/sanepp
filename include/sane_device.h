#pragma once

#include <string>
#include <map>
#include <memory>
#include <experimental/optional>

#include <sane/sane.h>

#include "sane_options.h"

class sane_device final {
    typedef std::map<SANE_Int, std::unique_ptr<option>> map_type;
    public:

        typedef map_type::iterator iterator;
        typedef map_type::const_iterator const_iterator;

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

        std::map<SANE_Int, std::unique_ptr<option>> m_options;

        friend class sane_device_info;
};

// TODO Maybe replace c-style strings with std::strings
// but that will cost performance and more memory
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

        friend class sane_device_info_list_iterator;
        friend bool operator==(const sane_device_info &lhs,
                const sane_device_info &rhs);
};

bool operator==(const sane_device_info &lhs,
        const sane_device_info &rhs);
bool operator!=(const sane_device_info &lhs,
        const sane_device_info &rhs);


