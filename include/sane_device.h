#pragma once

#include <string>
#include <experimental/optional>

#include <sane/sane.h>

class sane_device final {
    public:
        sane_device(const sane_device &) = delete;
        sane_device(sane_device &&);
        ~sane_device();

        sane_device &operator=(const sane_device &) = delete;
        sane_device &operator=(sane_device &&);
        explicit operator bool() const;
    private:
        sane_device(const char *device_name);

        SANE_Handle m_device_handle;
        SANE_Status m_device_status;

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
        std::experimental::optional<sane_device> open_device() const;
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


