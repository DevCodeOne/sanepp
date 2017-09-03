#pragma once

#include <iterator>
#include <mutex>

#include <sane/sane.h>

#include "sane_device.h"

class sane_instance final {
    public:
        static void create_instance(sane_authorization_callback callback = nullptr);
        ~sane_instance();

        sane_instance(const sane_instance &) = delete;
        sane_instance(sane_instance &&) = delete;

        sane_instance &operator=(const sane_instance &) = delete;
        sane_instance &operator=(sane_instance &&) = delete;

        static sane_instance *instance();
        sane_device_info_list get_devices(bool local_devices_only = false) const;

        SANE_Int version_code() const;
    private:
        sane_instance(sane_authorization_callback callback);

        bool m_initialized = false;
        SANE_Int m_version_code;

        static std::unique_ptr<sane_instance> _instance;
        static std::mutex sane_instance_mutex;
};
