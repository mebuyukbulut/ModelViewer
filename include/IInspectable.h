#pragma once
class IInspectable
{
public:
    virtual ~IInspectable() = default;
    virtual void onInspect() = 0;  // Pure virtual
};