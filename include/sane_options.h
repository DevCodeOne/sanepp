#pragma once

#include <memory>

#include <sane/sane.h>

class sane_option_value {
    public:
        sane_option_value(SANE_Value_Type type);
        virtual ~sane_option_value() = default;

        virtual void set_value(sane_option_value *value) = 0;

        virtual SANE_Bool bool_value() const = 0;
        virtual SANE_Int int_value() const = 0;
        virtual SANE_Fixed fixed_value() const = 0;
        virtual SANE_String string_value() const = 0;

        virtual sane_option_value *copy() const = 0;

        SANE_Value_Type value_type() const;

    private:
        const SANE_Value_Type m_value_type;
};

class sane_option_value_bool final : public sane_option_value {
    public:
        sane_option_value_bool(SANE_Bool value);
        virtual ~sane_option_value_bool() = default;

        virtual void set_value(sane_option_value *value);
        virtual SANE_Bool bool_value() const;
        virtual sane_option_value *copy() const;
    private:
        virtual SANE_Int int_value() const;
        virtual SANE_Fixed fixed_value() const;
        virtual SANE_String string_value() const;

        SANE_Bool m_value;
};

class sane_option_value_int final : public sane_option_value {
    public:
        sane_option_value_int(SANE_Int value);
        virtual ~sane_option_value_int() = default;

        virtual void set_value(sane_option_value *value);
        virtual SANE_Int int_value() const;
        virtual sane_option_value *copy() const;
   private:
        virtual SANE_Bool bool_value() const;
        virtual SANE_Fixed fixed_value() const;
        virtual SANE_String string_value() const;

        SANE_Int m_value;
};

class sane_option_value_fixed final : public sane_option_value {
    public:
        sane_option_value_fixed(SANE_Fixed value);
        virtual ~sane_option_value_fixed() = default;

        virtual void set_value(sane_option_value *value);
        virtual SANE_Fixed fixed_value() const;
        virtual sane_option_value *copy() const;
   private:
        virtual SANE_Bool bool_value() const;
        virtual SANE_Int int_value() const;
        virtual SANE_String string_value() const;

        SANE_Fixed m_value;
};

class sane_option_value_string final : public sane_option_value {
    public:
        sane_option_value_string(SANE_String value);
        virtual ~sane_option_value_string() = default;

        virtual void set_value(sane_option_value *value);
        virtual SANE_String string_value() const;
        virtual sane_option_value *copy() const;
   private:
        virtual SANE_Bool bool_value() const;
        virtual SANE_Int int_value() const;
        virtual SANE_Fixed fixed_value() const;

        SANE_String m_value;
};

// Has no value
class sane_option_value_button final : public sane_option_value {
    public:
        sane_option_value_button();
        virtual ~sane_option_value_button() = default;

        virtual void set_value(sane_option_value *value);
        virtual sane_option_value *copy() const;
    private:
        virtual SANE_Bool bool_value() const;
        virtual SANE_Int int_value() const;
        virtual SANE_Fixed fixed_value() const;
        virtual SANE_String string_value() const;
};

// Has no value
class sane_option_value_group final : public sane_option_value {
    public:
        sane_option_value_group();
        virtual ~sane_option_value_group() = default;

        virtual void set_value(sane_option_value *value);
        virtual sane_option_value *copy() const;
    private:
        virtual SANE_Bool bool_value() const;
        virtual SANE_Int int_value() const;
        virtual SANE_Fixed fixed_value() const;
        virtual SANE_String string_value() const;
};

class option_description final {
    public:
        option_description &name(const std::string &name);
        option_description &title(const std::string &title);
        option_description &description(const std::string &description);

        const std::string &name() const;
        const std::string &title() const;
        const std::string &description() const;
    private:
        std::string m_name = "";
        std::string m_title = "";
        std::string m_description = "";
};

// TODO Maybe add more characteristics
class option final {
    public:
        option(const sane_option_value *value,
                const option_description &description);
        const option_description &description() const;
    private:
        std::unique_ptr<sane_option_value> m_value;
        option_description m_option_description;
};


