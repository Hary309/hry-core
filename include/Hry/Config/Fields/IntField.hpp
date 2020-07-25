#pragma once

#include "NumericFieldBase.hpp"

HRY_NS_BEGIN

class IntField : public NumericFieldBase<int>
{
public:
    void setInputType(int step = 1, int stepFast = 100)
    {
        _controlType = InputType{ step, stepFast };
    }

    void setDragType(int speed = 1, int min = 0, int max = 0, const std::string& format = "%d")
    {
        _controlType = DragType{ speed, min, max, format, 0 };
    }

    void setSliderType(int min, int max, const std::string& format = "%d")
    {
        _controlType = SliderType{ min, max, format, 0 };
    }

protected:
    virtual void imguiRender();
};

HRY_NS_END
