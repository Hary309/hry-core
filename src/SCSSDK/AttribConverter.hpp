#pragma once

#include <limits>
#include <optional>
#include <string>
#include <vector>

#include <scssdk_value.h>

#include "Hry/Namespace.hpp"

#include "Adapters.hpp"

#undef max

HRY_NS_BEGIN

struct FieldBase
{
    std::string_view id;

    bool isIndexed = false;
    bool used = false;

    explicit FieldBase(std::string_view id) : id(id) {}
    virtual ~FieldBase() = default;

    virtual void process(const scs_named_value_t& param, void* data) noexcept = 0;
};

template<typename EnumType>
struct EnumDeserializer
{
private:
    std::vector<std::pair<std::string_view, EnumType>> _fields;

public:
    void add(std::string_view id, EnumType value) { _fields.emplace_back(std::pair{ id, value }); }

    EnumType getValue(std::string_view id) const noexcept
    {
        auto field = std::find_if(
            _fields.begin(), _fields.end(), [&id](const auto& v) { return v.first == id; });

        if (field != _fields.end())
        {
            return field->second;
        }

        return EnumType();
    }
};

template<typename>
struct EnumDeserializerCreator;

template<typename, typename = std::void_t<>>
struct HasEnumDeserializer : std::false_type
{
};

template<typename T>
struct HasEnumDeserializer<T, std::void_t<decltype(EnumDeserializerCreator<T>::create())>>
    : std::true_type
{
};

template<typename T>
inline constexpr auto HasEnumDeserializer_v = HasEnumDeserializer<T>::value;

template<typename T>
auto CreateEnumDeserializer()
{
    return EnumDeserializerCreator<T>::create();
}

template<typename ClassType>
struct AttribConverter
{
private:
    std::vector<std::shared_ptr<FieldBase>> _fields;

public:
    template<typename ValueType>
    void bind(std::string_view id, ValueType ClassType::*member)
    {
        if constexpr (!HasEnumDeserializer_v<ValueType>)
        {
            addSimple(id, member, CreateAdapter<ValueType>());
        }
        else
        {
            addEnum(id, member, CreateEnumDeserializer<ValueType>());
        }
    }

    template<typename ValueType, typename InnerValueType>
    void bind(
        std::string_view id, ValueType ClassType::*member, InnerValueType ValueType::*innerMember)
    {
        addComplex(id, member, innerMember, CreateAdapter<InnerValueType>());
    }

    template<typename ValueType>
    void bindIndexed(
        std::string_view fieldID,
        std::optional<std::string_view> countID,
        std::vector<ValueType> ClassType::*member)
    {
        static_assert(HasAdapter_v<ValueType>, "This type isn't simple!");

        if (countID.has_value())
        {
            addIndexedSize(countID.value(), member, CreateAdapter<uint32_t>());
        }

        addSimpleIndexed(fieldID, member, CreateAdapter<ValueType>());
    }

    template<typename ValueType>
    void bindIndexed(
        std::optional<std::string_view> countID, std::vector<ValueType> ClassType::*member)
    {
        static_assert(!HasAdapter_v<ValueType>, "This type isn't complex!");

        if (countID.has_value())
        {
            addIndexedSize(countID.value(), member, CreateAdapter<uint32_t>());
        }

        auto converter = CreateConverter<ValueType>();

        for (const auto& field : converter.getFields())
        {
            addComplexIndexed(member, field);
        }
    }

    ClassType process(const scs_named_value_t* params) const noexcept
    {
        for (const auto& field : _fields)
        {
            field->used = false;
        }

        ClassType type;

        const auto* it = params;

        while (it->name != nullptr)
        {
            for (const auto& field : _fields)
            {
                const auto isParamIndexed = it->index != std::numeric_limits<uint32_t>::max();

                if (field->isIndexed != isParamIndexed)
                {
                    continue;
                }

                if (!field->isIndexed && field->used)
                {
                    continue;
                }

                if (field->id == it->name)
                {
                    field->used = true;

                    field->process(*it, &type);
                    break;
                }
            }

            it++;
        }

        return type;
    }

    const auto& getFields() const { return _fields; }

private:
    template<typename MemberPtr, typename AdapterPtr>
    struct SimpleField : public FieldBase
    {
        MemberPtr member;
        AdapterPtr adapter;

        SimpleField(std::string_view id, MemberPtr member, AdapterPtr adapter)
            : FieldBase(id), member(member), adapter(std::move(adapter))
        {
        }

        void process(const scs_named_value_t& param, void* data) noexcept override
        {
            auto& typedData = *static_cast<ClassType*>(data);
            typedData.*member = adapter(param.value);
        }
    };

    template<typename MemberPtr, typename EnumDeserializer>
    struct EnumField : public FieldBase
    {
        MemberPtr member;
        EnumDeserializer deserializer;
        ValueAdapter_t<std::string> adapter{ &valueAdapter<std::string> };

        EnumField(std::string_view id, MemberPtr member, EnumDeserializer&& deserializer)
            : FieldBase(id), member(member), deserializer(std::move(deserializer))
        {
        }

        void process(const scs_named_value_t& param, void* data) noexcept override
        {
            auto& typedData = *static_cast<ClassType*>(data);
            auto str = adapter(param.value);

            typedData.*member = deserializer.getValue(str);
        }
    };

    template<typename MemberPtr, typename InnerMemberPtr, typename AdapterPtr>
    struct ComplexField : public FieldBase
    {
        MemberPtr member;
        InnerMemberPtr innerMember;
        AdapterPtr adapter;

        ComplexField(
            std::string_view id, MemberPtr member, InnerMemberPtr innerMember, AdapterPtr adapter)
            : FieldBase(id), member(member), innerMember(innerMember), adapter(std::move(adapter))
        {
        }

        void process(const scs_named_value_t& param, void* data) noexcept override
        {
            auto& typedData = *static_cast<ClassType*>(data);
            auto& innerClass = typedData.*member;
            innerClass.*innerMember = adapter(param.value);
        }
    };

    template<typename MemberPtr, typename AdapterPtr>
    struct IndexedSizeField : public FieldBase
    {
        MemberPtr member;
        AdapterPtr adapter;

        IndexedSizeField(std::string_view id, MemberPtr member, AdapterPtr adapter)
            : FieldBase(id), member(member), adapter(std::move(adapter))
        {
        }

        void process(const scs_named_value_t& param, void* data) noexcept override
        {
            auto& typedData = *static_cast<ClassType*>(data);
            auto& vector = typedData.*member;

            auto value = adapter(param.value);

            vector.resize(value);
        }
    };

    template<typename VectorPtr, typename AdapterPtr>
    struct SimpleIndexedField : public FieldBase
    {
        VectorPtr vector;
        AdapterPtr adapter;

        SimpleIndexedField(std::string_view id, VectorPtr vector, AdapterPtr adapter)
            : FieldBase(id), vector(vector), adapter(std::move(adapter))
        {
            isIndexed = true;
        }

        void process(const scs_named_value_t& param, void* data) noexcept override
        {
            auto& typedData = *static_cast<ClassType*>(data);
            auto& field = typedData.*vector;

            auto value = adapter(param.value);

            if (field.size() <= param.index)
            {
                field.resize(param.index + 1);
            }

            field[param.index] = value;
        }
    };

    template<typename VectorPtr>
    struct ComplexIndexedField : public FieldBase
    {
        VectorPtr vector;
        std::shared_ptr<FieldBase> converter;

        ComplexIndexedField(
            std::string_view id, VectorPtr vector, std::shared_ptr<FieldBase> converter)
            : FieldBase(id), vector(vector), converter(std::move(converter))
        {
            isIndexed = true;
        }

        void process(const scs_named_value_t& param, void* data) noexcept override
        {
            auto& typedData = *static_cast<ClassType*>(data);
            auto& field = typedData.*vector;

            if (field.size() <= param.index)
            {
                field.resize(param.index + 1);
            }

            converter->process(param, &field[param.index]);
        }
    };

private:
    template<typename ValueType, typename AdapterPtr>
    void addSimple(std::string_view id, ValueType ClassType::*member, AdapterPtr adapter)
    {
        using MemberPtr_t = ValueType(ClassType::*);
        using Field_t = SimpleField<MemberPtr_t, AdapterPtr>;

        _fields.push_back(std::shared_ptr<FieldBase>(new Field_t(id, member, adapter)));
    }

    template<typename ValueType, typename EnumDeserializerType>
    void addEnum(
        std::string_view id, ValueType ClassType::*member, EnumDeserializerType&& deserializer)
    {
        using MemberPtr_t = ValueType(ClassType::*);
        using Field_t = EnumField<MemberPtr_t, typename std::decay_t<EnumDeserializerType>>;

        _fields.push_back(std::shared_ptr<FieldBase>(
            new Field_t(id, member, std::forward<EnumDeserializerType>(deserializer))));
    }

    template<typename ValueType, typename InnerValueType, typename AdapterPtr>
    void addComplex(
        std::string_view id,
        ValueType ClassType::*member,
        InnerValueType ValueType::*innerMember,
        AdapterPtr adapter)
    {
        using MemberPtr_t = ValueType(ClassType::*);
        using InnerMemberPtr_t = InnerValueType(ValueType::*);
        using Field_t = ComplexField<MemberPtr_t, InnerMemberPtr_t, AdapterPtr>;

        _fields.push_back(
            std::shared_ptr<FieldBase>(new Field_t(id, member, innerMember, adapter)));
    }

    template<typename ValueType, typename AdapterPtr>
    void addIndexedSize(std::string_view id, ValueType ClassType::*member, AdapterPtr adapter)
    {
        using VectorPtr_t = ValueType(ClassType::*);
        using Field_t = IndexedSizeField<VectorPtr_t, AdapterPtr>;

        _fields.push_back(std::shared_ptr<FieldBase>(new Field_t(id, member, adapter)));
    }

    template<typename ValueType, typename AdapterPtr>
    void addSimpleIndexed(std::string_view id, ValueType ClassType::*member, AdapterPtr adapter)
    {
        using VectorPtr_t = ValueType(ClassType::*);
        using Field_t = SimpleIndexedField<VectorPtr_t, AdapterPtr>;

        _fields.push_back(std::shared_ptr<FieldBase>(new Field_t(id, member, adapter)));
    }

    template<typename ValueType>
    void addComplexIndexed(
        ValueType ClassType::*member, const std::shared_ptr<FieldBase>& converter)
    {
        using VectorPtr_t = ValueType(ClassType::*);
        using Field_t = ComplexIndexedField<VectorPtr_t>;

        _fields.push_back(
            std::shared_ptr<FieldBase>(new Field_t(converter->id, member, converter)));
    }
};

template<typename>
struct AttribConverterCreator;

template<typename T>
auto CreateConverter()
{
    return AttribConverterCreator<T>::create();
}

HRY_NS_END
