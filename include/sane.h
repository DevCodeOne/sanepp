#pragma once

#include <vector>
#include <functional>
#include <optional>
#include <mutex>

#include <sane/sane.h>

#include "sane_device.h"

using sane_authorization_callback = SANE_Auth_Callback;

// TODO Complete this class
class sane_version final {
    public:
        unsigned short major_version();

    private:
        sane_version(SANE_Int version_code = 0);
        sane_version(unsigned short maj, unsigned short min, unsigned short bld);

        unsigned short m_major_version;
        unsigned short m_minor_version;
        unsigned short m_built_version;

        friend class sane;
        friend bool operator<=(const sane_version &lhs, const sane_version &rhs);
        friend bool operator==(const sane_version &lhs, const sane_version &rhs);
};
bool operator<=(const sane_version &lhs, const sane_version &rhs);
bool operator>(const sane_version &lhs, const sane_version &rhs);
bool operator==(const sane_version &lhs, const sane_version &rhs);
bool operator!=(const sane_version &lhs, const sane_version &rhs);


// TODO rewrite to monostate otherwise it's difficult to delete objects prematurely
// (to restart sane in case a new scanner gets connected)
class sane final {
    public:

        using callback_type = void(const std::string &resource, std::string &username, std::string &password);

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

        static std::function<callback_type> _callback;
        static std::mutex sane_instance_mutex;
};
