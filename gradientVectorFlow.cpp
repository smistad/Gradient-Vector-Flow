#include "gradientVectorFlow.hpp"
#include <cmath>
#include <iostream>
using namespace SIPL;

Volume<float3> * GVF(
        Volume<float3> * initialVectorField,
        const int iterations,
        const float mu
        ) {
    Volume<float3> * vectorField = new Volume<float3>(initialVectorField->getSize());
    Volume<float3> * temp;
    // TODO neumann boundary cond.
    std::cout << "GVF iteration:";
    for(int i = 0; i < iterations; ++i) {
        if(i > 0)
            delete temp;
        Volume<float3> * newVectorField = new Volume<float3>(initialVectorField->getSize());
        for(int z = 0; z < vectorField->getDepth(); z++) {
        for(int y = 0; y < vectorField->getHeight(); y++) {
        for(int x = 0; x < vectorField->getWidth(); x++) {
            int3 pos(x,y,z);

            // Neumann boundary conditions (if a coordinate is 0, change it to 2.
            pos.x = x == 0 ? 2 : pos.x;
            pos.y = y == 0 ? 2 : pos.y;
            pos.z = z == 0 ? 2 : pos.z;

            float3 v = vectorField->get(pos);
            float3 f = initialVectorField->get(pos);
            float3 laplacian = -6*v
                    + vectorField->get(pos+int3(1,0,0))
                    + vectorField->get(pos+int3(0,1,0))
                    + vectorField->get(pos+int3(0,0,1))
                    + vectorField->get(pos-int3(1,0,0))
                    + vectorField->get(pos-int3(0,1,0))
                    + vectorField->get(pos-int3(0,0,1));

            newVectorField->set(pos, v + mu*laplacian - (f.x*f.x+f.y*f.y+f.z*f.z)*(v-f));
        }}}
        temp = vectorField;
        vectorField = newVectorField;
        if(i % 25 == 0)
            std::cout << std::endl;
        std::cout << i << " ";
    }

    return vectorField;
}

Volume<float3> * createVectorField(Volume<float> * v) {
    Volume<float3> * initialVectorField = new Volume<float3>(v->getSize());
    initialVectorField->fill(float3(0,0,0));
    for(int x = 1; x < v->getWidth()-1; x++) {
    for(int y = 1; y < v->getHeight()-1; y++) {
    for(int z = 1; z < v->getDepth()-1; z++) {
        float3 vector;
        vector.x = 0.5*(v->get(x+1,y,z)-v->get(x-1,y,z));
        vector.y = 0.5*(v->get(x,y+1,z)-v->get(x,y-1,z));
        vector.z = 0.5*(v->get(x,y,z+1)-v->get(x,y,z-1));
        initialVectorField->set(x,y,z,vector);
    }}}
    return initialVectorField;
}

float calculateMaxResidual(Volume<float3>* vectorField,
        Volume<float3> * initialVectorField,
        const float mu
    ) {
    float maxResidual = -1;


    // Go through each voxel and calculate the residual
    for(int z = 1; z < vectorField->getDepth()-1; z++) {
    for(int y = 1; y < vectorField->getHeight()-1; y++) {
    for(int x = 1; x < vectorField->getWidth()-1; x++) {
        int3 pos(x,y,z);
        float3 v2 = vectorField->get(pos);
        float3 f = initialVectorField->get(pos);
        float3 laplacian = -6*v2
                + vectorField->get(pos+int3(1,0,0))
                + vectorField->get(pos+int3(0,1,0))
                + vectorField->get(pos+int3(0,0,1))
                + vectorField->get(pos-int3(1,0,0))
                + vectorField->get(pos-int3(0,1,0))
                + vectorField->get(pos-int3(0,0,1));
        float3 residual = mu * laplacian - (f.x*f.x+f.y*f.y+f.z*f.z)*(v2-f);
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
