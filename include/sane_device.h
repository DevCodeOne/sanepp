#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <optional>

#include <sane/sane.h>

#include "sane_options.h"

// TODO maybe add invalid state
class sane_device final {
    public:

        sane_device(const sane_device &) = delete;
        sane_device(sane_device &&);
        ~sane_device();

        sane_device &operator=(const sane_device &) = delete;
        sane_device &operator=(sane_device &&);

        const std::vector<sane_option> &options() const;

        explicit operator bool() const;
    private:
        sane_device(const char *device_name);
        void load_options();

        SANE_Handle m_device_handle;
        SANE_Status m_device_status;

        std::vector<sane_option> m_options;

        friend class sane_device_info;
};

class sane_device_info final {
    public:
        std::string_view name() const;
        std::string_view vendor() const;
        std::string_view model() const;
        std::string_view type() const;
        std::optional<sane_device> open() const;
    private:
        sane_device_info(const SANE_Device *device);

        const SANE_Device *m_device;

        friend bool operator==(const sane_device_info &lhs, const sane_device_info &rhs);

        friend class sane;
};

bool operator==(const sane_device_info &lhs, const sane_device_info &rhs);
bool operator!=(const sane_device_info &lhs, const sane_device_info &rhs);
