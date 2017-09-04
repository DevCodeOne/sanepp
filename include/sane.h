#pragma once

#include <iterator>
#include <vector>

#include <sane/sane.h>

#include "sane_device.h"

using sane_authorization_callback = SANE_Auth_Callback;

class sane final {
    public:
        static void create_instance(sane_authorization_callback callback = nullptr);
        ~sane();

        sane(const sane &) = delete;
        sane(sane &&) = delete;

        sane &operator=(const sane &) = delete;
        sane &operator=(sane &&) = delete;

        static const sane &instance();
        std::vector<sane_device_info> devices(bool local_devices_only = false) const;

        SANE_Int version() const;
        explicit operator bool() const;
    private:
        sane(sane_authorization_callback callback);

        bool m_initialized = false;
        SANE_Int m_version_code;

        static std::unique_ptr<sane> _instance;
};
