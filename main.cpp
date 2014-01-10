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
    if(argc != 4) {
        std::cout << "Usage: " << argv[0] << " filename.mhd mu iterations" << std::endl;
        return 0;
    }

    const float mu = atof(argv[2]);
    const int iterations = atoi(argv[3]);
    INIT_TIMER

    // Load MHD volume specified in arguments using SIPL
    SIPL::Volume<float> * volume = new SIPL::Volume<float>(argv[1], SIPL::IntensityTransformation(SIPL::NORMALIZED));
    SIPL::int3 size = volume->getSize();

    // Create initial vector field
    SIPL::Volume<SIPL::float3> * initialVectorField = createVectorField(volume);

    // Call the GVF method
    START_TIMER
    SIPL::Volume<SIPL::float3> * result = GVF(initialVectorField, iterations, mu);
    STOP_TIMER("GVF")
    
    // Calculate the error
    std::cout << "Maximum magnitude of residuals (error) was: " << calculateMaxResidual(result, initialVectorField, mu) << std::endl;
    delete volume;

    result->display(0.0, 0.1);
    
    // Create magnitude image and display it
    SIPL::Volume<float> * magnitude = new SIPL::Volume<float>(size);
    for(int i = 0; i < magnitude->getTotalSize(); i++)
        magnitude->set(i, result->get(i).length());
    magnitude->display();

    // TODO: create 2D version?
}

