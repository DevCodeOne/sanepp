#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include <sane/sane.h>

#include "sane_options.h"

namespace sanepp {
    class Device final {
       public:
        Device(const Device &) = delete;
        Device(Device &&);
        ~Device();

        Device &operator=(const Device &) = delete;
        Device &operator=(Device &&);

        const std::vector<Option> &options() const;

        explicit operator bool() const;

       private:
        Device(const char *device_name);
        void load_options();

        SANE_Handle m_device_handle;
        SANE_Status m_device_status;

        std::vector<Option> m_options;

        friend class DeviceInfo;
    };

    class DeviceInfo final {
       public:
        std::string_view name() const;
        std::string_view vendor() const;
        std::string_view model() const;
        std::string_view type() const;
        std::optional<Device> open() const;

       private:
        DeviceInfo(const SANE_Device *device);

        const SANE_Device *m_device;

        friend bool operator==(const DeviceInfo &lhs, const DeviceInfo &rhs);

        friend class Sane;
    };

    bool operator==(const DeviceInfo &lhs, const DeviceInfo &rhs);
    bool operator!=(const DeviceInfo &lhs, const DeviceInfo &rhs);
}  // namespace sanepp
