# RayTracer
## What is this project??
Behold, a tiny monte carlo path tracer I started to learn more about how graphics work, expose
myself to the math/theory behind raytracing, and test my linear algebra coursework in my free
time.

It uses simplex noise functions to generate randomized heightmaps, uses those to construct
triangle meshes that resemble terrain, throws some water on top of it, adds some sunlight,
and outputs renders of the resulting scene to a .ppm image file.

The result? A full HD sunset over low-poly islands scene with different terrain each iteration.

## How to run
Clone this repository in a new directory and run
```mkdir build && cd build && cmake --preset release .. && cd release && cmake --build . && cd bin```.
Running the executable will create a new image.ppm file with the
completed render.

`./RayTracer -s 123456789 -n 10000 -t 0.5`
 - -s: optional, specify a seed for the terrain generation (default: random seed)
 - -n: optional, specify the samples per pixel taken (default: 10, increase for less noise)
 - -t: optional, specify the length of each triangle (default: 0.5, decrease for smoother terrain)

## Images
<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/fdde00bf-d750-491c-a66a-4280490173f3" />
Seed: 3753314839, spp: 100000

#### Test Scenes
Some test scenes I hardcoded during development that kind of look sort of nice:
<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/ad923c28-4533-42c3-9bef-c8d6b7e8c7b9" />
<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/1cc6c780-fc8b-4c37-9084-1514a78cb39c" />
<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/5710509e-345c-4338-b78c-e279aeca5450" />
<img width="1920" height="1080" alt="image" src="https://github.com/user-attachments/assets/61893a1c-a586-458f-a263-b89435a8b4f7" />

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
