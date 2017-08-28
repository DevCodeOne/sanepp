#include "sane_options.h"

// Implementation of sane_option_value
sane_option_value::sane_option_value(SANE_Value_Type type)
    : m_value_type(type) {
}

SANE_Value_Type sane_option_value::value_type() const {
    return m_value_type;
}

sane_option_value *sane_option_value_bool::copy() const {
    return new sane_option_value_bool(m_value);
}

// Implementation of sane_option_value_bool
sane_option_value_bool::sane_option_value_bool(SANE_Bool value)
    : sane_option_value(SANE_TYPE_BOOL), m_value(value) {
}

void sane_option_value_bool::set_value(sane_option_value *value) {
    if (value_type() == value->value_type()) {
        m_value = ((sane_option_value_bool *) value)->m_value;
    }
}

SANE_Bool sane_option_value_bool::bool_value() const {
    return m_value;
}

// TODO maybe throw exceptions so the user knows he's doing something
// wrong
SANE_Int sane_option_value_bool::int_value() const {
    return -1;
}

SANE_Fixed sane_option_value_bool::fixed_value() const {
    return -1;
}

SANE_String sane_option_value_bool::string_value() const {
    return nullptr;
}

// Implementation of sane_operation_type_int
sane_option_value_int::sane_option_value_int(SANE_Int value)
    : sane_option_value(SANE_TYPE_INT), m_value(value) {
}

void sane_option_value_int::set_value(sane_option_value *value) {
    if (value_type() == value->value_type()) {
        m_value = ((sane_option_value_int *) value)->m_value;
    }
}

sane_option_value *sane_option_value_int::copy() const {
    return new sane_option_value_int(m_value);
}

// TODO maybe throw exceptions so the user knows he's doing something
// wrong
SANE_Bool sane_option_value_int::bool_value() const {
    return false;
}

SANE_Int sane_option_value_int::int_value() const {
    return m_value;
}

SANE_Fixed sane_option_value_int::fixed_value() const {
    return -1;
}

SANE_String sane_option_value_int::string_value() const {
    return nullptr;
}

// Implementation of sane_option_value_fixed
sane_option_value_fixed::sane_option_value_fixed(SANE_Fixed value)
    : sane_option_value(SANE_TYPE_FIXED), m_value(value) {
}

void sane_option_value_fixed::set_value(sane_option_value *value) {
    if (value_type() == value->value_type()) {
        m_value = ((sane_option_value_fixed *) value)->m_value;
    }
}

sane_option_value *sane_option_value_fixed::copy() const {
    return new sane_option_value_fixed(m_value);
}

// TODO maybe throw exceptions so the user knows he's doing something
// wrong
SANE_Bool sane_option_value_fixed::bool_value() const {
    return false;
}

SANE_Int sane_option_value_fixed::int_value() const {
    return -1;
}

SANE_Fixed sane_option_value_fixed::fixed_value() const {
    return m_value;
}

SANE_String sane_option_value_fixed::string_value() const {
    return nullptr;
}

// Implementation of sane_option_value_string
sane_option_value_string::sane_option_value_string(SANE_String value)
    : sane_option_value(SANE_TYPE_STRING), m_value(value) {
}

void sane_option_value_string::set_value(sane_option_value *value) {
    if (value_type() == value->value_type()) {
        m_value = ((sane_option_value_string *) value)->m_value;
    }
}

sane_option_value *sane_option_value_string::copy() const {
    return new sane_option_value_string(m_value);
}

// TODO maybe throw exceptions so the user knows he's doing something
// wrong
SANE_Bool sane_option_value_string::bool_value() const {
    return false;
}

SANE_Int sane_option_value_string::int_value() const {
    return -1;
}

SANE_Fixed sane_option_value_string::fixed_value() const {
    return -1;
}

SANE_String sane_option_value_string::string_value() const {
    return m_value;
}

// Implementation of sane_option_value_button
sane_option_value_button::sane_option_value_button()
    : sane_option_value(SANE_TYPE_BUTTON){
}

void sane_option_value_button::set_value(sane_option_value *) {
}

sane_option_value *sane_option_value_button::copy() const {
    return new sane_option_value_button();
}

// TODO maybe throw exceptions so the user knows he's doing something
// wrong
SANE_Bool sane_option_value_button::bool_value() const {
    return false;
}

SANE_Int sane_option_value_button::int_value() const {
    return -1;
}

SANE_Fixed sane_option_value_button::fixed_value() const {
    return -1;
}

SANE_String sane_option_value_button::string_value() const {
    return nullptr;
}

// Implementation of sane_option_value_group
sane_option_value_group::sane_option_value_group()
    : sane_option_value(SANE_TYPE_GROUP){
}

void sane_option_value_group::set_value(sane_option_value *) {
}

sane_option_value *sane_option_value_group::copy() const {
    return new sane_option_value_button();
}

// TODO maybe throw exceptions so the user knows he's doing something
// wrong
SANE_Bool sane_option_value_group::bool_value() const {
    return false;
}

SANE_Int sane_option_value_group::int_value() const {
    return -1;
}

SANE_Fixed sane_option_value_group::fixed_value() const {
    return -1;
}

SANE_String sane_option_value_group::string_value() const {
    return nullptr;
}

// Implementation of option_description
option_description &option_description::name(const std::string &name) {
    m_name = name;

    return *this;
}

// Implementation of option_description
option_description &option_description::title(const std::string &title) {
    m_title = title;

    return *this;
}

// Implementation of option_description
option_description &option_description::description(const std::string &desc) {
    m_description = desc;

    return *this;
}

// Implementation of option
option::option(const sane_option_value &value, option_description &description)
    : m_value(value.copy()), m_option_description(description) {
}

const option_description &option::description() const {
    return m_option_description;
}
