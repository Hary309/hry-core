#pragma once

#include "NumericFieldBase.hpp"

HRY_NS_BEGIN

class DoubleField : public NumericFieldBase<double>
{
public:
    void setInputType(double step = 1, double stepFast = 100)
    {
        _controlType = InputType{ step, stepFast };
    }

    void setDragType(
        double speed = 1.0,
        double min = 0.0,
        double max = 0.0,
        const std::string& format = "%.6f",
        double power = 1.0)
    {
        _controlType = DragType{ speed, min, max, format, power };
    }

    void setSliderType(
        double min, double max, const std::string& format = "%.6f", double power = 1.0)
    {
        _controlType = SliderType{ min, max, format, power };
    }

protected:
    virtual void imguiRender();
};

HRY_NS_END
