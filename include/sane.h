#pragma once

#include <functional>
#include <mutex>
#include <optional>
#include <vector>

#include <sane/sane.h>

#include "sane_device.h"

namespace sanepp {
    using sane_authorization_callback = SANE_Auth_Callback;

    /**
     * @brief The sane_version class
     */
    class Version final {
       public:
        Version(unsigned char major_version, unsigned char minor_version,
                unsigned short build_version); /**< Create version to compare with logical Operators */
        unsigned char major_version();         /**< Observer to check the Major Version Code */

       private:
        explicit Version(SANE_Int version_code = 0); /**< Create version with default 0 */

        unsigned char m_major_version;  /**< 8 Bit Major Version */
        unsigned char m_minor_version;  /**< 8 Bit Minor Version */
        unsigned short m_build_version; /**< 16 Bit Build Version */

        friend class Sane; /**< friend declaration for using version in sane*/
        friend bool operator>(const Version &lhs, const Version &rhs); /**< friend declaration for logical Operators */
        friend bool operator==(const Version &lhs,
                               const Version &rhs); /**< friend declaration for compare equivalend and antivalent */
    };

    /// Logical Operators to compare the versioncode
    bool operator<=(const Version &lhs, const Version &rhs);
    bool operator<(const Version &lhs, const Version &rhs);
    bool operator>(const Version &lhs, const Version &rhs);
    bool operator>=(const Version &lhs, const Version &rhs);
    bool operator==(const Version &lhs, const Version &rhs);
    bool operator!=(const Version &lhs, const Version &rhs);

    /**
     * @brief The sane class
     *
     * Class to initalize sane and ensure that the librarys are opened and closed correctly
     */
    class Sane final {
       public:
        using callback_type = void(const std::string &resource, std::string &username, std::string &password);

        Sane();
        ~Sane();

        Sane(const Sane &) = delete; /**< no copy constructable allowed */
        Sane(Sane &&) = delete;      /**< no move constructable allowed */

        Sane &operator=(const Sane &) = delete; /**< no copy asignment */
        Sane &operator=(Sane &&) = delete;      /**< no move asignment */

        static void authorization_callback(const std::function<callback_type> &callback);
        std::vector<DeviceInfo> devices(
            bool local_devices_only = false) const; /**< All available devices (Default with NW)*/

        const Version &version() const; /**< Version of the Sane Backend whitch is initialized */
        explicit operator bool() const; /**< Condition operator for valid initialization */
       private:
        static void callback_wrapper(SANE_String_Const resource, SANE_Char *name, SANE_Char *password);

        static std::function<callback_type> _callback;
        static std::mutex sane_instance_mutex; /**< threadsafety */
        static Version _version;               /**< Versionobject */
        static bool _initialized;              /**< Variable for the condition operator */
        static unsigned int _instance_count;   /**< Counter of construction and destruction */
    };
}  // namespace sanepp
