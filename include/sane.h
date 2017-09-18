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
        sane_version(unsigned char major_version, unsigned char minor_version,
                unsigned short build_version);
        unsigned char major_version();

    private:
        explicit sane_version(SANE_Int version_code = 0);

        unsigned char m_major_version;
        unsigned char m_minor_version;
        unsigned short m_build_version;

        friend class sane;
        friend bool operator>(const sane_version &lhs, const sane_version &rhs);
        friend bool operator==(const sane_version &lhs, const sane_version &rhs);
};

bool operator<=(const sane_version &lhs, const sane_version &rhs);
bool operator<(const sane_version &lhs, const sane_version &rhs);
bool operator>(const sane_version &lhs, const sane_version &rhs);
bool operator>=(const sane_version &lhs, const sane_version &rhs);
bool operator==(const sane_version &lhs, const sane_version &rhs);
bool operator!=(const sane_version &lhs, const sane_version &rhs);


class sane final {
    public:

        using callback_type = void(const std::string &resource, std::string &username, std::string &password);

        sane();
        ~sane();

        sane(const sane &) = delete;
        sane(sane &&) = delete;

        sane &operator=(const sane &) = delete;
        sane &operator=(sane &&) = delete;

        static void authorization_callback(const std::function<callback_type> &callback);
        std::vector<sane_device_info> devices(bool local_devices_only = false) const;

        const sane_version &version() const;
        explicit operator bool() const;
    private:
        static void callback_wrapper(SANE_String_Const resource, SANE_Char *name, SANE_Char *password);


        static std::function<callback_type> _callback;
        static std::mutex sane_instance_mutex;
        static sane_version _version;
        static bool _initialized;
        static unsigned int _instance_count;
};
