#pragma once

#include <string>
#include <vector>
#include <memory>
#include <experimental/optional>

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

        const std::vector<std::unique_ptr<option>> &options() const;

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
        std::experimental::optional<sane_device> open() const;
    private:
        sane_device_info(const SANE_Device *device);

        const SANE_Device *m_device;

        friend bool operator==(const sane_device_info &lhs, const sane_device_info &rhs);

        friend class sane;
};

bool operator==(const sane_device_info &lhs, const sane_device_info &rhs);
bool operator!=(const sane_device_info &lhs, const sane_device_info &rhs);
