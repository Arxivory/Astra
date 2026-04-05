# ASTRA Space: 3D Orbital Simulation

<img src="./Assets/Astra Preview.png" alt="Astra Preview" width="400">


ASTRA Space is a high-performance, GPU-accelerated 3D orbital simulation built with a custom C++ and OpenGL engine. It provides a real-time, interactive visualization of the solar system, utilizing physics-informed calculations to simulate celestial dynamics with high-fidelity rendering.

---

## Software Description

ASTRA Space serves as a technical bridge between scientific simulation and interactive computer graphics. Unlike simple linear animations, this engine treats every planet, moon, and star as a dynamic entity within a shared gravitational field.

The simulation is rendered using a **camera-relative coordinate system** to maintain floating-point precision across the vast distances of the solar system (from the Sun to Saturn and beyond). It features a custom GLSL-based rendering pipeline including:

- **Normal Mapping & TBN Space**: Accurate surface relief that reacts dynamically to light.  
- **Atmospheric Scattering**: Rim lighting effects that simulate the appearance of planetary atmospheres.  
- **Alpha-Blended Ring Systems**: Specialized transparency handling for Saturn's rings.  

---

## N-Body Physics & Euler Integration

At the core of ASTRA is a physics engine that calculates the movement of celestial bodies based on **Newtonian Gravity**.

To solve the differential equations of motion, the simulation employs **Euler’s Integration**. At every frame, the engine:

1. **Calculates Acceleration**: Uses the gravitational constant and the mass of central bodies to determine the force acting on an object.  
2. **Updates Velocity**: Adjusts the current velocity vector by adding the product of acceleration and the time step (\(\Delta t\)).  
3. **Updates Position**: Moves the object in 3D space based on its new velocity.  

This approach allows for emergent behaviors, such as the Moon accurately orbiting the Earth while both simultaneously orbit the Sun, all calculated in real-time.

---

## Controls & Navigation

The simulation offers a "Fly-through" camera and a specialized UI for system-wide navigation.

| Action        | Input                                                   |
|---------------|---------------------------------------------------------|
| Movement      | W, A, S, D (Forward, Left, Backward, Right)             |
| Orientation   | Mouse movement to look and steer                        |
| Selection     | Left-Click on any celestial body to view its data        |
| Speed Control | Camera Speed Slider (Adjusts flight velocity)           |
| Target Lock   | Focus Button in the System Navigator to follow a planet |

---

## Key Features

### Real-time Telemetry
Select any object to view its live data, including its name, precise 3D coordinates, and current orbital velocity in km/s.

### Dynamic System Navigator
An interactive sidebar allows you to instantly track every major body in the simulation. You can "Focus" the camera on a planet to watch its rotation and orbit from a fixed relative perspective.

### Visualization Tools
- **Orbital Trajectories**: Toggleable path lines that visualize the history and future heading of each planet.  
- **Orbital Grid**: A spatial reference plane (ecliptic) to help understand the orientation of the solar system.  
- **Time Manipulation**: A "Time Factor" slider that allows you to accelerate time to watch years of orbital movement in seconds or slow it down to observe subtle rotations.  

### Advanced Rendering
The engine utilizes custom vertex and fragment shaders to handle complex lighting scenarios, ensuring that the dark side of planets remains realistically occluded while the sunward side exhibits high-detail specular and diffuse highlights.

