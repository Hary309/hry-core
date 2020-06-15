#pragma once

namespace hry
{

class Core
{
public:
    Core();
    Core(Core &&) = default;
    Core(const Core &) = default;
    Core &operator=(Core &&) = default;
    Core &operator=(const Core &) = default;
    ~Core();

private:
    
};

}
