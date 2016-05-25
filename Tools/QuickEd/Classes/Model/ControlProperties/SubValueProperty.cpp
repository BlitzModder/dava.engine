#include "SubValueProperty.h"

#include "ValueProperty.h"

using namespace DAVA;

SubValueProperty::SubValueProperty(int32 anIndex, const DAVA::String& propName)
    : AbstractProperty()
    , index(anIndex)
    , name(propName)
{
}

SubValueProperty::~SubValueProperty()
{
}

uint32 SubValueProperty::GetCount() const
{
    return 0;
}

AbstractProperty* SubValueProperty::GetProperty(int index) const
{
    return NULL;
}

void SubValueProperty::Accept(PropertyVisitor* visitor)
{
    DVASSERT(false);
}

const DAVA::String& SubValueProperty::GetName() const
{
    return name;
}

SubValueProperty::ePropertyType SubValueProperty::GetType() const
{
    return TYPE_VARIANT;
}

DAVA::VariantType::eVariantType SubValueProperty::GetValueType() const
{
    return GetValueProperty()->GetSubValueType(index);
}

VariantType SubValueProperty::GetValue() const
{
    return GetValueProperty()->GetSubValue(index);
}

void SubValueProperty::SetValue(const DAVA::VariantType& newValue)
{
    GetValueProperty()->SetSubValue(index, newValue);
}

VariantType SubValueProperty::GetDefaultValue() const
{
    return GetValueProperty()->GetDefaultSubValue(index);
}

void SubValueProperty::SetDefaultValue(const DAVA::VariantType& newValue)
{
    GetValueProperty()->SetDefaultSubValue(index, newValue);
}

void SubValueProperty::ResetValue()
{
    GetValueProperty()->ResetValue();
}

bool SubValueProperty::IsOverriddenLocally() const
{
    return GetValueProperty()->IsOverriddenLocally();
}

ValueProperty* SubValueProperty::GetValueProperty() const
{
    return DynamicTypeCheck<ValueProperty*>(GetParent());
}
