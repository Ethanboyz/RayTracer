# RayTracer
## Please note:
This project is very young and under heavy development. There will be lots of changes
and generally a lot of incompleteness.

## What is this project??
Nothing more than a small undertaking to learn more about how GPUs, expose myself to the
math/theory behind raytracing, and test my linear algebra coursework in my free time.
Right now, it simply outputs simple raytraced renders to a .ppm image file. More exciting
stuff to come in the near future...

## How to run
Clone this repository in a new directory and run
```mkdir build && cd build && cmake .. && cmake --build .```
Running the executable in ./build/bin will output a new image.ppm file with the
completed render.

#### Features Implemented:
- Ray generation (reverse ray tracing)
- Sphere objects
  - Surface normals

#### WIP:
- Lights (directional, point lights)
  - Diffuse
  - Specular
- Hittable/intersectable
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