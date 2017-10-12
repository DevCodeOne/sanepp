#pragma once

#include <vector>
#include <functional>
#include <optional>
#include <mutex>

#include <sane/sane.h>

#include "sane_device.h"

using sane_authorization_callback = SANE_Auth_Callback;


/**
 * @brief The sane_version class
 * TODO Complete this class
 */
class sane_version final {
    public:
        sane_version(unsigned char major_version, unsigned char minor_version,
                unsigned short build_version); /**< Create sane_version to compare with logical Operators */
        unsigned char major_version(); /**< Observer to check the Major Version Code */

    private:
        explicit sane_version(SANE_Int version_code = 0); /**< Create sane_version with default 0 */

        unsigned char m_major_version; /**< 8 Bit Major Version */
        unsigned char m_minor_version; /**< 8 Bit Minor Version */
        unsigned short m_build_version; /**< 16 Bit Build Version */

        friend class sane; /**< friend declaration for using sane_version in sane*/
        friend bool operator>(const sane_version &lhs, const sane_version &rhs); /**< friend declaration for logical Operators */
        friend bool operator==(const sane_version &lhs, const sane_version &rhs); /**< friend declaration for compare equivalend and antivalent */
};

/// Logical Operators to check and compare the versioncode
bool operator<=(const sane_version &lhs, const sane_version &rhs);
bool operator<(const sane_version &lhs, const sane_version &rhs);
bool operator>(const sane_version &lhs, const sane_version &rhs);
bool operator>=(const sane_version &lhs, const sane_version &rhs);
bool operator==(const sane_version &lhs, const sane_version &rhs);
bool operator!=(const sane_version &lhs, const sane_version &rhs);

/**
 * @brief The sane class
 *
 * Class to initalize sane and ensure that the librarys are opened and closed correctly
 */
class sane final {
    public:

        using callback_type = void(const std::string &resource, std::string &username, std::string &password);

        sane();
        ~sane();

        sane(const sane &) = delete; /**< no copy constructable allowed */
        sane(sane &&) = delete; /**< no move constructable allowed */

        sane &operator=(const sane &) = delete; /**< no copy asignment */
        sane &operator=(sane &&) = delete; /**< no move asignment */

        static void authorization_callback(const std::function<callback_type> &callback);
        std::vector<sane_device_info> devices(bool local_devices_only = false) const; /**< All available devices (Default with NW)*/

        const sane_version &version() const; /**< Version of the Sane Backend whitch is initialized */
        explicit operator bool() const; /**< Condition operator for valid initialization */
    private:
        static void callback_wrapper(SANE_String_Const resource, SANE_Char *name, SANE_Char *password);


        static std::function<callback_type> _callback;
        static std::mutex sane_instance_mutex; /**< threadsafety */
        static sane_version _version; /**< Versionobject */
        static bool _initialized; /**< Variable for the condition operator */
        static unsigned int _instance_count; /**< Counter of construction and destruction */
};
