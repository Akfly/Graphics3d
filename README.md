Graphics3D
==========

A 3D Graphic engine without using OpenGL or any other visual library.

Controls
--------

- Use the numbers 1, 2 and 3 to select an element (1 is Camera, 2 First Mesh, 3 Second Mesh).

- With the Left Mouse Button we move the element along the X axis.
- With the Right Mouse Button we move the element along the Z axis.
- With the Mouse Wheel we increase or reduce the mesh size (it does not work with the camera).

- Left Control plus Left Mouse Button rotates around the X axis.
- Left Control plus Middle Mouse Button rotates around the Y axis.
- Left Control plus Right Mouse Button rotates around the Z axis.

- Pressing the L button switches between Wireframe Mode and Normal Mode.
 
Classes
-------

**Application**

This class has some window properties. This will call the SFML method to open a window. There are two additional methods, one used to draw text and another one to call SFML events (and handle input). Here are some OpenGL calls, but those are to configure the window and text drawing, and has nothing to do about rendering.

**Camera**

This class represents the scene camera, it has position and rotation matrices and two vectors to represent this transformations outside this class. Its methods are only to get the ViewMatrix and set/get Position and Rotation.

**Mesh**

It has position, rotation and scale matrices (and their vectors) and neccessary buffers (initial vertices, transformed vertices, colors and indices). To load a mesh, we call the Load method, that uses the objLoader library. Here we load the buffers as 

    std::vector<std::vector<int>>

And then we pass that info to the class buffers.

In the Update method we just calculate the projection matrix (in -1, 1 coordinates) using the camera matrix and the mesh matrices.

We have two paint methods, one called normally and another one called when the wireframe is activated. In both we switch the coordinates to "Device Coordinates", but when drawing we call different methods from the rasterizer.

**View**

In this class we have the meshes and camera elements, and it calls their respective methods when neccessary (and transform them depending on the user input).

SetActiveMesh changes the focus of each element, and then we call MoveActive, RotateActive, etc to transform that element.

Executable
----------

You can download the executable version of this project from here: https://www.dropbox.com/s/ta7u24tbfotcehu/Graphics.rar

Credits
-------
- PixelBuffer classes as well as the Rasterizer class are written by Ángel Rodriguez.
- This project uses a library called cpp-toolkit written by Ángel Rodriguez. This library gives vectors and matrices. You can download it from here: https://bitbucket.org/angel-esne/cpp-toolkit
- This project uses a library to load the data from a .obj file. This library is the objLoader, you can downloa it from here: http://www.kixor.net/dev/objloader/




