Phase 1
- Create Map class         (2D grid, walls)
- Create Player class      (pos, dir, plane, speed, radius, methods)
- Create cast_ray method      (DDA raycasting)
- Implement 2D map rendering 
- Update player angle from mouse position    (from 2D perspective)
- Implement player movement in main game loop   (parallel and perpendicular movement)
- Implement accurate wall collisions    (sliding, corner detection)

Phase 2
- Add a pixel buffer (Raylib Image object)
- Implement core raycasting algorithm (ray sweep across the player's FOV)
- Render each ray collision as a vertical stripe based on its distance (update pixel buffer)
- Implement 3D player turning and mouse sensitivity 
- Fix fish-eye distortion bug
- Fix uneven ray angle distribution, calculate proper angle offsets
- Add face based shading and fog
- Add more player controls (FOV/speed adjustments)

Phase 3
- Implement Raylib wall texture loading before game loop starts
- Implement wall texturing logic, measure vertical and horizontal progress along a wall, update pixel buffer
- Implement solid roof and floor colors
- Implement roof and floor texturing logic