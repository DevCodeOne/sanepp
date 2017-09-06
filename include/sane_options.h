#pragma once

#include <optional>
#include <string>
#include <variant>
#include <iostream>

#include <sane/sane.h>

class sane_int {
public:
    sane_int(SANE_Int value = 0);
    SANE_Int value() const;
private:
    SANE_Int m_value;
};

class sane_fixed {
public:
    sane_fixed(SANE_Fixed value = 0);
    SANE_Fixed value() const;
private:
    SANE_Fixed m_value;
};

class sane_bool {
public:
    sane_bool(SANE_Bool value = false);
    SANE_Bool value() const;
private:
    SANE_Bool m_value;
};

// TODO complete these
class sane_string {
public:
sane_string(SANE_String value = "");
SANE_String value() const;
private:
SANE_String m_value
};

class sane_button {

};

class sane_group {

};

std::ostream &operator<<(std::ostream &os, const sane_int &value);
std::ostream &operator<<(std::ostream &os, const sane_fixed &value);
std::ostream &operator<<(std::ostream &os, const sane_bool &value);
std::ostream &operator<<(std::ostream &os, const sane_string &value);

class sane_option_description final {
    public:
        sane_option_description(SANE_Int id);
        sane_option_description &name(const std::string &name);
        sane_option_description &title(const std::string &title);
        sane_option_description &description(const std::string &description);

        const std::string &name() const;
        const std::string &title() const;
        const std::string &description() const;
        SANE_Int id() const;
    private:
        const SANE_Int m_id;
        std::string m_name = "";
        std::string m_title = "";
        std::string m_description = "";
};

// TODO Maybe add more characteristics
class sane_option final {
    public:
        typedef std::variant<sane_bool, sane_int, sane_fixed, sane_string, sane_button, sane_group> value_type;

        template<typename T>
        sane_option(SANE_Handle device_handle,
                const T &value,
                const sane_option_description &description);
        const value_type &value() const;
        const sane_option_description &description() const;
    private:
        SANE_Handle m_device_handle;
        sane_option_description m_option_description;
        mutable value_type m_value;
};

template<typename T>
sane_option::sane_option(SANE_Handle device_handle,
        const T &value,
        const sane_option_description &description)
    : m_device_handle(device_handle), m_option_description(description),
    m_value(value) {
}
