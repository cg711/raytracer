# TODO

## Mandatory

- [x] A camera with configurable position, orientation, and field of view
- [x] Anti-aliasing
- [x] Ray/sphere intersections
- [x] Ray/triangle intersections
- [x] The ability to load textures (file format(s) of your choice; may use third-party libraries)
- [x] Textured spheres and triangles
  - [x] Spheres
  - [x] Triangles
- [x] The ability to load and render triangle meshes (file format(s) of your choice; may use third-party libraries for loading)
  - [x] Image loading
  - [x] OBJ Files
- [x] A spatial subdivision acceleration structure of your choice
  - [x] BVH
- [x] Specular, diffuse, and dielectric materials (per first volume of Ray Tracing in One Weekend series)
- [x] Emissive materials (lights)

### Extra Needed (25 base, 75 for A)
- [x] Motion blur (10)
- [x] Volume rendering (smoke, clouds, etc.) (10)
- [x] Perlin noise (10)
- [x] Quads (10)
- [x] Parralelization (10)

### Compile

```
g++ -std=c++11 main.cpp -o main
```