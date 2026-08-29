Direction Vector
- Normalized (x, y) coordinates
- Length is usually fixed at 1.0
- Represents the physical 2D distance from player to the screen

Camera Plane
- Vector pointing from the center to one edge of the screen
- Always perpendicular to direction vector
- Forms a line representing HALF of the screen
- Negate to form the other half of the screen

FOV
- Angle between the left-most ray and the right-most ray 
- Represents the total angular width of your screen
- Raycasting breaks down at anything >= 180 degrees
	- However some games fixed this by using angular/curved raycasting! (distortions near edges of the screen)

<br>


Digital Differential Analyzer (DDA Algorithm)
- Used in the cast_ray method to allow rays to  traverse through the 2D grid more efficiently
- Collisions will only happen at the boundaries of cells, so rays can be made to only jump between horizontal and vertical grid lines (rather than tiny, fixed step sizes)
- Calculates the offset necessary to travel between each boundary at a given angle, then advances each ray at a constant rate using that offset
- Eliminates unnecessary checks in-between cells
- Avoids floating-point errors, and fixed, arbitrary step sizes that have a chance of missing a collision completely

Pixel Buffer
- Block of memory (1D array) used to represent a 2D grid of pixels
- Instead of rendering each shape separately, an entire block is sent directly to the GPU per frame, reducing overhead costs
- Each element in the 1D array holds the color data (represented by an int, since each piece of RGBA is 1 byte of data)
- Must convert 2D coordinates into the correct 1D index
    - index = row * width + cols
- This is done using the Raylib texture structure, and calling the UpdateTexture and DrawTexture methods in the main drawing loop