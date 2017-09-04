#pragma once

#include <iterator>
#include <vector>

#include <sane/sane.h>

#include "sane_device.h"

using sane_authorization_callback = SANE_Auth_Callback;

// TODO Complete this class
class sane_version final {
    public:
    private:
        sane_version(SANE_Int version_code = 0);
        void version_code(SANE_Int version_code);

        SANE_Int m_version_code;

        friend class sane;
};

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

        const sane_version &version() const;
        explicit operator bool() const;
    private:
        sane(sane_authorization_callback callback);

        bool m_initialized = false;
        sane_version m_version;

        static std::unique_ptr<sane> _instance;
};
