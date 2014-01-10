#ifndef GVF_H
#define GVF_H
#include "SIPL/Core.hpp"

float calculateMaxResidual(SIPL::Volume<SIPL::float3>* vectorField,
        SIPL::Volume<float>* volume,
        float mu);
#endif
