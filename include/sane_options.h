#pragma once

#include <memory>

#include <sane/sane.h>

// TODO maybe use cast operators
class sane_option_value {
    public:
        sane_option_value(SANE_Value_Type type);
        virtual ~sane_option_value() = default;

        virtual sane_option_value *copy() const = 0;

        SANE_Value_Type value_type() const;
    private:
        const SANE_Value_Type m_value_type;
};

class sane_option_value_bool final : public sane_option_value {
    public:
        sane_option_value_bool(SANE_Bool value);
        virtual ~sane_option_value_bool() = default;

        virtual void value(const sane_option_value_bool &value);
        virtual SANE_Bool value() const;
        virtual sane_option_value *copy() const;
    private:
        SANE_Bool m_value;
};

class sane_option_value_int final : public sane_option_value {
    public:
        sane_option_value_int(SANE_Int value);
        virtual ~sane_option_value_int() = default;

        virtual void value(const sane_option_value_int &value);
        virtual SANE_Int value() const;
        virtual sane_option_value *copy() const;
   private:
        SANE_Int m_value;
};

class sane_option_value_fixed final : public sane_option_value {
    public:
        sane_option_value_fixed(SANE_Fixed value);
        virtual ~sane_option_value_fixed() = default;

        virtual void value(const sane_option_value_fixed &value);
        virtual SANE_Fixed value() const;
        virtual sane_option_value *copy() const;
   private:
        SANE_Fixed m_value;
};

class sane_option_value_string final : public sane_option_value {
    public:
        sane_option_value_string(SANE_String value);
        virtual ~sane_option_value_string() = default;

        virtual void value(const sane_option_value_string &value);
        virtual SANE_String value() const;
        virtual sane_option_value *copy() const;
   private:
        SANE_String m_value;
};

// Has no value
class sane_option_value_button final : public sane_option_value {
    public:
        sane_option_value_button();
        virtual ~sane_option_value_button() = default;

        virtual void value(const sane_option_value_button &value);
        virtual sane_option_value *copy() const;
};

// Has no value
class sane_option_value_group final : public sane_option_value {
    public:
        sane_option_value_group();
        virtual ~sane_option_value_group() = default;

        virtual void value(const sane_option_value_group &value);
        virtual sane_option_value *copy() const;
};

// TODO Maybe replace SANE_Int with custom_data_type ?
class option_description final {
    public:
        option_description(SANE_Int id);
        option_description &name(const std::string &name);
        option_description &title(const std::string &title);
        option_description &description(const std::string &description);

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
// Discuss if it is a good idea to use references instead of pointers
// so that wrong dynamic_cast will throw exceptions instead of just returning
// nullptr
class option final {
    public:
        option(SANE_Handle device_handle,
                const sane_option_value *value,
                const option_description &description);
        const sane_option_value *value() const;
        const option_description &description() const;
    private:
        SANE_Handle m_device_handle;
        std::unique_ptr<sane_option_value> m_value;
        option_description m_option_description;
};


