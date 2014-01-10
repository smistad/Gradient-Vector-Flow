#ifndef GVF_H
#define GVF_H
#include "SIPL/Core.hpp"

SIPL::Volume<SIPL::float3> * createVectorField(SIPL::Volume<float> * v);

float calculateMaxResidual(SIPL::Volume<SIPL::float3>* vectorField,
        SIPL::Volume<SIPL::float3>* initialVectorField,
        const float mu
);

SIPL::Volume<SIPL::float3> * GVF(
        SIPL::Volume<SIPL::float3> * initialVectorField,
        const int iterations,
        const float mu
);

#endif
