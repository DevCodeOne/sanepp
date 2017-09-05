#pragma once

#include <vector>
#include <functional>
#include <optional>

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

        using callback_type = void(SANE_String_Const, SANE_Char *, SANE_Char *);

        ~sane();

        sane(const sane &) = delete;
        sane(sane &&) = delete;

        sane &operator=(const sane &) = delete;
        sane &operator=(sane &&) = delete;

        static const sane &instance();
        static void authorization_callback(const std::function<callback_type> &callback);
        std::vector<sane_device_info> devices(bool local_devices_only = false) const;

        const sane_version &version() const;
        explicit operator bool() const;
    private:
        sane(sane_authorization_callback callback);
        static void callback_wrapper(SANE_String_Const resource, SANE_Char *name, SANE_Char *password);

        sane_version m_version;
        bool m_initialized;

        static std::function<void(SANE_String_Const, SANE_Char *, SANE_Char *)> _callback;
};