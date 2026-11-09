#pragma once
#include <cmath>
namespace BlasterRules
{
inline bool CanFire(double Now,double Next,bool Alive,bool Enabled,int Ammo)
{
    return std::isfinite(Now) && std::isfinite(Next) && Now>=0.0 && Alive && Enabled && Ammo>0 && Now>=Next;
}
}
