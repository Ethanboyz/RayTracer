# RayTracer
## Please note
This project is very young and under heavy development. There will be lots of changes
and generally a lot of incompleteness.

## What is this project??
Nothing more than a small PBR-style ray/path tracer undertaking to learn more about how GPUs work, expose myself to the
math/theory behind raytracing, and test my linear algebra coursework in my free time.
Right now, it simply outputs simple raytraced renders to a .ppm image file. More exciting
stuff to come in the near future...

## How to run
Clone this repository in a new directory and run
```mkdir build && cd build && cmake --preset release .. && cd release && cmake --build . && cd bin```.
Running the executable will create a new image.ppm file with the
completed render.

## Progress
Below is a non-exhaustive list of small milestones I've implemented along with features I plan
on cooking up.

### Phase 1: Baseline Ray Tracing
- [X] Ray generation (backwards ray tracing)
- [X] Sphere objects
 - [X] Surface normals
- [X] Materials:
  - [X] Color attenuation
    - [X] Diffuse reflection
    - [X] Specular reflection
    - [X] Refraction + total internal reflections + Schlick's
- [X] Shadows (through global illumination)
- [X] Emissive objects
- [ ] Better directional/environmental light
- [X] BVH to replace lists
  - [ ] SAH object splitting
- [X] Antialiasing (sampling)
- [X] Multithreaded pixel processing
- [X] Camera coordinate frame
- [X] Defocus blur
- [ ] Texture mapping?
- [ ] More...
### Phase 2: Procedural Generation
- [X] Triangles
- [X] Base terrain heightmaps
  - [X] Triangle mesh construction
  - [X] Noise functions
    - [X] Opensimplex
  - [ ] FBM
- [ ] Alternative acceleration structure (for height map traversal)
### Phase 3: Volumetrics
- [ ] Clouds
  - Separate ray marching for volumetrics? Or more path tracing?
- [ ] Water?
### Phase 4: GPU Acceleration
- [ ] CUDA port
    - [ ] GPU-based intersections system
    - [ ] Improved BVH constructor
    - [ ] Improved height map constructor
    - [ ] Performance benchmarking
### Phase 5: Polish??
- [ ] Fancy(er) atmospherics
- [ ] Config GUI
- [ ] Denoising
- [ ] Debug visualizations
- [ ] Spectral rendering