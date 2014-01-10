#include "gradientVectorFlow.hpp"
#include <cmath>

float calculateMaxResidual(SIPL::Volume<SIPL::float3>* vectorField,
        SIPL::Volume<float> * v,
        float mu
    ) {
    float maxResidual = -1;

    // Calculate initial vector field first
    SIPL::Volume<SIPL::float3> * initialVectorField = new SIPL::Volume<SIPL::float3>(v->getSize());
    for(int x = 1; x < v->getWidth()-1; x++) {
    for(int y = 1; y < v->getHeight()-1; y++) {
    for(int z = 1; z < v->getDepth()-1; z++) {
        SIPL::float3 vector;
        vector.x = 0.5*(v->get(x+1,y,z)-v->get(x-1,y,z));
        vector.y = 0.5*(v->get(x,y+1,z)-v->get(x,y-1,z));
        vector.z = 0.5*(v->get(x,y,z+1)-v->get(x,y,z-1));
        initialVectorField->set(x,y,z,vector);
    }}}

    // Go through each voxel and calculate the residual
    for(int z = 1; z < vectorField->getDepth()-1; z++) {
    for(int y = 1; y < vectorField->getHeight()-1; y++) {
    for(int x = 1; x < vectorField->getWidth()-1; x++) {
        SIPL::int3 pos(x,y,z);
        SIPL::float3 v2 = vectorField->get(pos);
        SIPL::float3 f = initialVectorField->get(pos);
        SIPL::float3 laplacian = -6*v2
                + vectorField->get(pos+SIPL::int3(1,0,0))
                + vectorField->get(pos+SIPL::int3(0,1,0))
                + vectorField->get(pos+SIPL::int3(0,0,1))
                + vectorField->get(pos-SIPL::int3(1,0,0))
                + vectorField->get(pos-SIPL::int3(0,1,0))
                + vectorField->get(pos-SIPL::int3(0,0,1));
        SIPL::float3 residual = mu * laplacian - (f.x*f.x+f.y*f.y+f.z*f.z)*(v2-f);
        if(fabs(residual.x) > maxResidual) {
            maxResidual = fabs(residual.x);
        }
        if(fabs(residual.y) > maxResidual) {
            maxResidual = fabs(residual.y);
        }
        if(fabs(residual.z) > maxResidual) {
            maxResidual = fabs(residual.z);
        }
    }}}

    return maxResidual;
}
