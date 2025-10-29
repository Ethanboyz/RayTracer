# RayTracer
## What is this project??
Behold, a basic ray tracer I started to learn more about how graphics work, expose
myself to the math/theory behind raytracing, and test my linear algebra coursework in my free
time.

It uses simplex noise functions to generate randomized heightmaps, uses those to construct
triangle meshes that resemble terrain, throws some water on top of it, adds some sunlight,
and outputs renders of the resulting scene to a .ppm image file.

## How to run
Clone this repository in a new directory and run
```mkdir build && cd build && cmake --preset release .. && cd release && cmake --build . && cd bin```.
Running the executable will create a new image.ppm file with the
completed render.

`./RayTracer -s 123456789 -n 10000 -t 0.5`
 - -s: optional, specify a seed for the terrain generation (default: random seed)
 - -n: optional, specify the samples per pixel taken (default: 10, increase for less noise)
 - -t: optional, specify the length of each triangle (default: 0.5, decrease for smoother terrain)

## Progress
Below is a non-exhaustive list of small milestones I've implemented along with features I haven't
gotten to yet.

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
- [ ] Alternative acceleration structures (for height map traversal)
### Phase 3: Volumetrics
- [ ] Clouds
  - Separate ray marching for volumetrics? Or more path tracing?
- [ ] Realistic water?
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
  - [ ] Next event estimation
- [ ] Debug visualizations (wireframes)
- [ ] Spectral rendering