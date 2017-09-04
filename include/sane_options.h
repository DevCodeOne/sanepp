#pragma once

#include <memory>
#include <optional>

#include <sane/sane.h>

class sane_option_value {
    public:
        sane_option_value(SANE_Value_Type type);
        virtual ~sane_option_value() = default;

        virtual sane_option_value *copy() const = 0;
        SANE_Value_Type value_type() const;

        // No template function sind template functions can't be virtual,
        // because of that we would have to cast again.
        virtual void value_int(SANE_Int value) = 0;
        virtual void value_bool(SANE_Bool value) = 0;
        virtual void value_fixed(SANE_Fixed value) = 0;
        virtual void value_string(SANE_String value) = 0;

        virtual std::optional<SANE_Int> as_int() const = 0;
        virtual std::optional<SANE_Bool> as_bool() const = 0;
        virtual std::optional<SANE_Fixed> as_fixed() const = 0;
        virtual std::optional<SANE_String> as_string() const = 0;
    private:
        const SANE_Value_Type m_value_type;
};

class sane_option_value_bool final : public sane_option_value {
    public:
        sane_option_value_bool(SANE_Bool value);
        virtual ~sane_option_value_bool() = default;

        virtual void value(const sane_option_value_bool &value);
        virtual sane_option_value *copy() const override;

        inline virtual void value_int(SANE_Int) override { }
        inline virtual void value_bool(SANE_Bool value) override { m_value = value; }
        inline virtual void value_fixed(SANE_Fixed) override { }
        inline virtual void value_string(SANE_String) override { }

        inline virtual std::optional<SANE_Int> as_int() const override { return {}; }
        inline virtual std::optional<SANE_Bool> as_bool() const override { return m_value; }
        inline virtual std::optional<SANE_Fixed> as_fixed() const override { return {}; }
        inline virtual std::optional<SANE_String> as_string() const override { return {}; }

    private:
        SANE_Bool m_value;
};

class sane_option_value_int final : public sane_option_value {
    public:
        sane_option_value_int(SANE_Int value);
        virtual ~sane_option_value_int() = default;

        virtual void value(const sane_option_value_int &value);
        virtual sane_option_value *copy() const override;

        inline virtual void value_int(SANE_Int value) override { m_value = value; }
        inline virtual void value_bool(SANE_Bool) override { }
        inline virtual void value_fixed(SANE_Fixed) override { }
        inline virtual void value_string(SANE_String) override { }

        inline virtual std::optional<SANE_Int> as_int() const override { return m_value; }
        inline virtual std::optional<SANE_Bool> as_bool() const override { return {}; }
        inline virtual std::optional<SANE_Fixed> as_fixed() const override { return {}; }
        inline virtual std::optional<SANE_String> as_string() const override { return {}; }

   private:
        SANE_Int m_value;
};

class sane_option_value_fixed final : public sane_option_value {
    public:
        sane_option_value_fixed(SANE_Fixed value);
        virtual ~sane_option_value_fixed() = default;

        virtual void value(const sane_option_value_fixed &value);
        virtual sane_option_value *copy() const override;

        inline virtual void value_int(SANE_Int) override { }
        inline virtual void value_bool(SANE_Bool) override { }
        inline virtual void value_fixed(SANE_Fixed value) override { m_value = value; }
        inline virtual void value_string(SANE_String) override { }

        inline virtual std::optional<SANE_Int> as_int() const override { return {}; }
        inline virtual std::optional<SANE_Bool> as_bool() const override { return {}; }
        inline virtual std::optional<SANE_Fixed> as_fixed() const override { return m_value; }
        inline virtual std::optional<SANE_String> as_string() const override { return {}; }

   private:
        SANE_Fixed m_value;
};

class sane_option_value_string final : public sane_option_value {
    public:
        sane_option_value_string(SANE_String value);
        virtual ~sane_option_value_string() = default;

        virtual void value(const sane_option_value_string &value);
        virtual sane_option_value *copy() const override;

        inline virtual void value_int(SANE_Int) override { }
        inline virtual void value_bool(SANE_Bool) override { }
        inline virtual void value_fixed(SANE_Fixed) override { }
        inline virtual void value_string(SANE_String value) override { m_value = value; }

        inline virtual std::optional<SANE_Int> as_int() const override { return {}; }
        inline virtual std::optional<SANE_Bool> as_bool() const override { return {}; }
        inline virtual std::optional<SANE_Fixed> as_fixed() const override { return {}; }
        inline virtual std::optional<SANE_String> as_string() const override { return m_value; }

  private:
        SANE_String m_value;
};

// Has no value
class sane_option_value_button final : public sane_option_value {
    public:
        sane_option_value_button();
        virtual ~sane_option_value_button() = default;

        virtual sane_option_value *copy() const override;

        inline virtual void value_int(SANE_Int) override { }
        inline virtual void value_bool(SANE_Bool) override { }
        inline virtual void value_fixed(SANE_Fixed) override { }
        inline virtual void value_string(SANE_String) override { }

        inline virtual std::optional<SANE_Int> as_int() const override { return {}; }
        inline virtual std::optional<SANE_Bool> as_bool() const override { return {}; }
        inline virtual std::optional<SANE_Fixed> as_fixed() const override { return {}; }
        inline virtual std::optional<SANE_String> as_string() const override { return {}; }
};

// Has no value
class sane_option_value_group final : public sane_option_value {
    public:
        sane_option_value_group();
        virtual ~sane_option_value_group() = default;

        virtual sane_option_value *copy() const override;

        inline virtual void value_int(SANE_Int) override { }
        inline virtual void value_bool(SANE_Bool) override { }
        inline virtual void value_fixed(SANE_Fixed) override { }
        inline virtual void value_string(SANE_String) override { }

        inline virtual std::optional<SANE_Int> as_int() const override { return {}; }
        inline virtual std::optional<SANE_Bool> as_bool() const override { return {}; }
        inline virtual std::optional<SANE_Fixed> as_fixed() const override { return {}; }
        inline virtual std::optional<SANE_String> as_string() const override { return {}; }
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
        const sane_option_value &value() const;
        const option_description &description() const;
    private:
        SANE_Handle m_device_handle;
        std::unique_ptr<sane_option_value> m_value;
        option_description m_option_description;
};


