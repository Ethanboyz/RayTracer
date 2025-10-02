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
```mkdir build && cd build && cmake .. && cmake --build .```
Running the executable in ./build/bin will create a new image.ppm file with the
completed render.

## Progress
Below is a non-exhaustive list of small milestones I've implemented along with features I plan
on cooking up.

### Phase 1: Baseline Ray Tracing
- [X] Ray generation (backwards ray tracing)
- [X] Sphere objects
    - [X] Surface normals
- [X] Materials:
    - [X] Colors
    - [X] Diffuse reflection
    - [X] Specular reflection
    - [ ] Refraction
- [ ] Emissive objects
- [ ] Directional/environmental light
- [ ] BVH to replace lists
- [X] Better lights
  - [X] Shadows
  - [X] Color attenuation
  - [ ] Refractions + reflections
- [X] Antialiasing (sampling)
- [ ] Multithreaded primary ray generation
- [ ] Camera space
  - [ ] Proper camera coord system (+world space transformations)
  - [ ] Possible camera panning?
- [ ] Texture mapping?
- [ ] More...
### Phase 2: Procedural Generation
- [ ] Heightmaps & triangle meshes
- [ ] Noise functions
### Phase 3: Volumetrics
- [ ] Clouds
- [ ] Water
### Phase 4: GPU Acceleration
- [ ] CUDA/OpenMP port
  - [ ] Sweet, sweet parallelism
  - [ ] Performance benchmarking
### Phase 5: Polish??

## Sources:
- https://gabrielgambetta.com/computer-graphics-from-scratch
- https://raytracing.github.io/books/RayTracingInOneWeekend.html
- https://scratchapixel.com/