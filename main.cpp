#include "SIPL/Core.hpp"
#include "gradientVectorFlow.hpp"

#ifndef KERNELS_DIR
#define KERNELS_DIR ""
#endif
#include <chrono>

#define TIMING

#ifdef TIMING
#define INIT_TIMER auto start = std::chrono::high_resolution_clock::now();
#define START_TIMER  start = std::chrono::high_resolution_clock::now();
#define STOP_TIMER(name)  std::cout << "RUNTIME of " << name << ": " << \
    std::chrono::duration_cast<std::chrono::milliseconds>( \
            std::chrono::high_resolution_clock::now()-start \
    ).count() << " ms " << std::endl;
#else
#define INIT_TIMER
#define START_TIMER
#define STOP_TIMER(name)
#endif

int main(int argc, char ** argv) {
    const float mu = 0.2f;
    INIT_TIMER

    // Load MHD volume specified in arguments using SIPL
    SIPL::Volume<float> * volume = new SIPL::Volume<float>(argv[1], SIPL::IntensityTransformation(SIPL::NORMALIZED));
    SIPL::int3 size = volume->getSize();

    // Create vector field on the GPU

    // Call the runFMGGVF method
    START_TIMER

    STOP_TIMER("FMG GVF")
    
    SIPL::Volume<SIPL::float3> * result = new SIPL::Volume<SIPL::float3>(size);
    result->setData(data);

    std::cout << "Maximum magnitude of residuals was: " << calculateMaxResidual(result, volume, mu) << std::endl;
    delete volume;

    result->display(0.0, 0.1);
    
    // Create magnitude image and display it
    SIPL::Volume<float> * magnitude = new SIPL::Volume<float>(size);
    for(int i = 0; i < magnitude->getTotalSize(); i++)
        magnitude->set(i, result->get(i).length());
    magnitude->display();

    // TODO: create 2D version?
}

