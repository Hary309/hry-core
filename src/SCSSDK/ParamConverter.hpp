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

    virtual void process(const scs_named_value_t& param, void* data) = 0;
};

template<typename>
struct ParamConverterCreator
{
};

template<typename T>
auto CreateConverter()
{
    return ParamConverterCreator<T>::create();
}

template<typename ClassType>
struct ParamConverter
{
private:
    std::vector<std::shared_ptr<FieldBase>> _fields;

public:
    template<typename ValueType>
    void bind(std::string_view id, ValueType ClassType::*member)
    {
        addSimple(id, member, CreateBasicAdapter<ValueType>());
    }

    template<typename ValueType, typename InnerValueType>
    void bind(
        std::string_view id, ValueType ClassType::*member, InnerValueType ValueType::*innerMember)
    {
        addComplex(id, member, innerMember, CreateBasicAdapter<InnerValueType>());
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
            addIndexedSize(countID.value(), member, CreateBasicAdapter<uint32_t>());
        }

        addSimpleIndexed(fieldID, member, CreateBasicAdapter<ValueType>());
    }

    template<typename ValueType>
    void bindIndexed(
        std::optional<std::string_view> countID, std::vector<ValueType> ClassType::*member)
    {
        static_assert(!HasAdapter_v<ValueType>, "This type isn't complex!");

        if (countID.has_value())
        {
            addIndexedSize(countID.value(), member, CreateBasicAdapter<uint32_t>());
        }

        auto converter = CreateConverter<ValueType>();

        for (const auto& field : converter.getFields())
        {
            addComplexIndexed(member, field);
        }
    }

    ClassType process(const scs_named_value_t* params) const
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
    template<typename MemberPtr, typename AdapterType>
    struct SimpleField : public FieldBase
    {
        MemberPtr member;
        AdapterType adapter;

        SimpleField(std::string_view id, MemberPtr member, AdapterType&& adapter)
            : FieldBase(id), member(member), adapter(std::move(adapter))
        {
        }

        void process(const scs_named_value_t& param, void* data) override
        {
            auto& typedData = *static_cast<ClassType*>(data);
            typedData.*member = adapter.get(param.value);
        }
    };

    template<typename MemberPtr, typename InnerMemberPtr, typename AdapterType>
    struct ComplexField : public FieldBase
    {
        MemberPtr member;
        InnerMemberPtr innerMember;
        AdapterType adapter;

        ComplexField(
            std::string_view id,
            MemberPtr member,
            InnerMemberPtr innerMember,
            AdapterType&& adapter)
            : FieldBase(id), member(member), innerMember(innerMember), adapter(std::move(adapter))
        {
        }

        void process(const scs_named_value_t& param, void* data) override
        {
            auto& typedData = *static_cast<ClassType*>(data);
            auto& innerClass = typedData.*member;
            innerClass.*innerMember = adapter.get(param.value);
        }
    };

    template<typename MemberPtr, typename AdapterType>
    struct IndexedSizeField : public FieldBase
    {
        MemberPtr member;
        AdapterType adapter;

        IndexedSizeField(std::string_view id, MemberPtr member, AdapterType&& adapter)
            : FieldBase(id), member(member), adapter(std::move(adapter))
        {
        }

        void process(const scs_named_value_t& param, void* data) override
        {
            auto& typedData = *static_cast<ClassType*>(data);
            auto& vector = typedData.*member;

            auto value = adapter.get(param.value);

            printf("Resize to: %d\n", value);

            vector.resize(value);
        }
    };

    template<typename VectorPtr, typename AdapterType>
    struct SimpleIndexedField : public FieldBase
    {
        VectorPtr vector;
        AdapterType adapter;

        SimpleIndexedField(std::string_view id, VectorPtr vector, AdapterType&& adapter)
            : FieldBase(id), vector(vector), adapter(std::move(adapter))
        {
            isIndexed = true;
        }

        void process(const scs_named_value_t& param, void* data) override
        {
            auto& typedData = *static_cast<ClassType*>(data);
            auto& field = typedData.*vector;

            auto value = adapter.get(param.value);

            if (field.size() <= param.index)
            {
                field.resize(param.index);
            }

            field.insert(field.begin() + param.index, value);
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

        void process(const scs_named_value_t& param, void* data) override
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
    template<typename ValueType, typename AdapterType>
    void addSimple(std::string_view id, ValueType ClassType::*member, AdapterType&& adapter)
    {
        using MemberPtr_t = ValueType(ClassType::*);
        using Field_t = SimpleField<MemberPtr_t, typename std::decay_t<AdapterType>>;

        _fields.push_back(std::shared_ptr<FieldBase>(
            new Field_t(id, member, std::forward<AdapterType>(adapter))));
    }

    template<typename ValueType, typename InnerValueType, typename AdapterType>
    void addComplex(
        std::string_view id,
        ValueType ClassType::*member,
        InnerValueType ValueType::*innerMember,
        AdapterType&& adapter)
    {
        using MemberPtr_t = ValueType(ClassType::*);
        using InnerMemberPtr_t = InnerValueType(ValueType::*);
        using Field_t =
            ComplexField<MemberPtr_t, InnerMemberPtr_t, typename std::decay_t<AdapterType>>;

        _fields.push_back(std::shared_ptr<FieldBase>(
            new Field_t(id, member, innerMember, std::forward<AdapterType>(adapter))));
    }

    template<typename ValueType, typename AdapterType>
    void addIndexedSize(std::string_view id, ValueType ClassType::*member, AdapterType&& adapter)
    {
        using VectorPtr_t = ValueType(ClassType::*);
        using Field_t = IndexedSizeField<VectorPtr_t, typename std::decay_t<AdapterType>>;

        _fields.push_back(std::shared_ptr<FieldBase>(
            new Field_t(id, member, std::forward<AdapterType>(adapter))));
    }

    template<typename ValueType, typename AdapterType>
    void addSimpleIndexed(std::string_view id, ValueType ClassType::*member, AdapterType&& adapter)
    {
        using VectorPtr_t = ValueType(ClassType::*);
        using Field_t = SimpleIndexedField<VectorPtr_t, typename std::decay_t<AdapterType>>;

        _fields.push_back(std::shared_ptr<FieldBase>(
            new Field_t(id, member, std::forward<AdapterType>(adapter))));
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
HRY_NS_END
