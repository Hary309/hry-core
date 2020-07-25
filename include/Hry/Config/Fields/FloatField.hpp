#pragma once

#include "NumericFieldBase.hpp"

HRY_NS_BEGIN

class FloatField : public NumericFieldBase<float>
{
public:
    void setInputType(float step = 1, float stepFast = 100)
    {
        _controlType = InputType{ step, stepFast };
    }

    void setDragType(
        float speed = 1.f,
        float min = 0.f,
        float max = 0.f,
        const std::string& format = "%.3f",
        float power = 1.f)
    {
        _controlType = DragType{ speed, min, max, format, power };
    }

    void setSliderType(float min, float max, const std::string& format = "%.3f", float power = 1.f)
    {
        _controlType = SliderType{ min, max, format, power };
    }

protected:
    virtual void imguiRender();
};

HRY_NS_END
