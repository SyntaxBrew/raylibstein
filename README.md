
![alt text](<thumbnail.png>)

# Raylibstein

A Wolfenstein-inspired 3D raycaster built from scratch using C++ and Raylib.<br>
[▶️ Play in Browser](https://syntaxbrew.github.io/raylibstein/)

## Features
- Textured walls and shading
- Player movement, rotation, and collision detection
- DDA-based raycasting (Digital Differential Analysis)
- FOV and perspective correction (fish-eye, flat-screen projection)
- Real-time Minimap with ray visualization

## Controls
| Key | Action |
|-----|--------|
| W | Move forward |
| S | Move backward |
| A | Strafe left |
| D | Strafe right |
| Mouse | Look |
| Mouse Wheel | Zoom |
| ~ | Toggle Noclip |
| Arrow Keys | Turn |

## How it Works
The renderer casts a ray for each vertical column of the screen, and uses DDA to determine which grid cell the ray intersects first. The perpendicular distance to the wall is then used to calculate its projected height as a vertical stripe on the screen. All these vertical stripes are drawn together to construct a 3D-like perspective.

For drawing textures, the renderer determines the ray's hit position along a wall, and samples a specific column from the texture. The texture column is then scaled to the projected wall height and is drawn to the screen.

Camera → Ray Sweep → DDA → Wall Hit → Perpendicular Distance → Projection → Texture Sampling → Screen

## Resources Used
- https://lodev.org/cgtutor/raycasting.html