# RayTracer
## Please note
This project is very young and under heavy development. There will be lots of changes
and generally a lot of incompleteness.

## What is this project??
Nothing more than a small undertaking to learn more about how GPUs work, expose myself to the
math/theory behind raytracing, and test my linear algebra coursework in my free time.
Right now, it simply outputs simple raytraced renders to a .ppm image file. More exciting
stuff to come in the near future...

## How to run
Clone this repository in a new directory and run
```mkdir build && cd build && cmake .. && cmake --build .```
Running the executable in ./build/bin will create a new image.ppm file with the
completed render.

## Progress
#### Features Implemented:
- Ray generation (backwards ray tracing)
- Sphere objects
  - Surface normals
- Materials:
  - Colors
- Light sources:
  - Ambient

#### WIP:
- BVH to replace list
- Lights (directional, point lights)
  - Diffuse
  - Specular
  - Shadows
  - Refractions + reflections
- Antialiasing
- Possible camera panning?
  - Proper camera coord system (+world space transformations)
- More...

#### Broader Roadmap:
- [ ] Baseline raytracing (WIP)
- [ ] Procedural generation
  - Heightmaps, noise functions
- [ ] Volumetrics
  - Clouds
  - Water
- [ ] CUDA/OpenMP port
  - Sweet, sweet parallelism
  - Performance benchmarking
- [ ] Polish??

#### Helpful Resources:
- https://raytracing.github.io/books/RayTracingInOneWeekend.html
- https://scratchapixel.com/
- https://gabrielgambetta.com/computer-graphics-from-scratch