<div align="center">
  <h1>Tetris using SFML Multimedia Library</h1>
</div>

> [!NOTE]
>   Welcome to the repository of the Tetris project built with the SFML library. The game itself is not directly innovative, both in idea and implementation.

>  Sometimes with my pharisaical way of thinking, looking at the modern “IT” industry, one can't help but want to renounce it and go into a free, personal study of this science, delving deeper and comprehending every detail, every aspect of the subject. It is a pity that we have forgotten that a programmer used to be a scientist, and now every fifth or ninth person is thinking about going into IT. It's ridiculous, some will say, it's evolution and development that has brought us to this point. I have a counterargument to such grumbling: physics is also evolving, its origins go deep into the history of civilization, but a physicist is still someone who has studied many aspects of it, not just how to use Docker and frameworks. I hope you caught my point, and if not, it's not worth continuing to read the rest of the essay.  
> I want to, try to, learn the ins and outs of the language - C++, computer science and architecture, processor logic.
> I don't study programming by seven-mile steps, rather by steps, which is less than a versta. And sometimes it's not very pleasant, remembering the capitalist rule. Time should work for us.

> [!NOTE]
> This project is not just a collection of code lines, but a living embodiment of the path of knowledge, an open book, through the pages of which you can trace the stages of formation of a programmer in love with C++. It is a kind of odyssey, where each step is a new chapter filled with discoveries and overcoming.



Available Annotations:

- [English version] - the current version you are currently on
- [Russian version](./TetRu.md)
----


### Architecture and key components

### Global constants and basic settings

At the very beginning of the project, key parameters are defined: block sizes, playfield sizes, indents and window sizes. This makes it easy to adapt the game to different resolutions and ensures visual consistency:

- **Block and field sizes:**  
  Constants defining cell sizes and overall indents ensure that elements are harmoniously arranged on the screen.

- **Window settings:**  
  The window dimensions are calculated taking into account the playing field, the report area and the preview area of the next figure.

### Shapes definition and rotation function

Shapes (tetrominoes) are represented as arrays of strings, where the symbols `‘X’` and `‘.’` define filled and empty cells, respectively. The `rotate` function calculates the index of a block in a 4×4 grid after a given rotation:

- **Rotation Flexibility:**  
  Four rotation angles (_0°, 90°, 180°, and 270°_) are supported, allowing smooth and predictable animations.

- **Efficient coding:**  
  Using a compact representation of shapes allows you to easily scale and modify the set of available tetrominoes.

### Particle System for visual effects

The `ParticleSystem` class implements a nifty mechanism for creating visual effects:

- **Particle generation:**  
  The `addParticles` method creates multiple particles with individual parameters such as speed, direction and lifespan, giving a dynamic ‘comet tail’ effect.

- **Physics and animation:**  
  The natural effects of gravity and the gradual fading of particles give the impression of a realistic physical simulation, while keeping the whole process high performance.

### TetrisGame Class

The core of the project is the `TetrisGame` class, which is responsible for all the game logic:

- **Collision handling:**  
  The `doesPieceFit` method carefully checks whether a piece can be placed on the playing field, preventing ridiculous situations and ensuring correct behaviour of the pieces.

- **Integration of pieces into the field:**  
  The `lockPiece` method locks the figure into the grid, analyses filled lines and initiates visual effects using the particle system.

- **Spawn Mechanics:**  
  Applying prediction of the next piece through the preview mechanism allows the player to strategically plan their moves.

- **Input Processing:**  
  Intuitive controls (moving, turning and accelerated descent) make gameplay easy and fun.

- **Drawing and visualisation:**  
  The `draw` method is responsible for the comprehensive rendering of all game elements, including the dynamic grid, current figure, preview area and particle visual effects.

---

## interesting solutions

The project is full of interesting, in my humble opinion, solutions that not only improve the gameplay, but also make it visually exciting:

- **Dynamic time management:**  
  Using the `Clock` class in combination with the `dt` variable allows for smooth updating of game states, regardless of hardware characteristics.

- **Effective visual solutions:**  
  The particle system when removing filled lines not only enhances the aesthetic impression, but also emphasises the dynamism and liveliness of the gameplay.

- **Clear separation of logic and representation:**  
  The architectural decision to separate game logic processing and visual representation makes the code easy to read, maintainable and scalable for future developments.

---

## Conclusion

The **Tetris using SFML** project is not just an implementation of a classic game, it is a true work of digital art, where every line of code breathes innovation and passion for perfection. We are confident that this solution will be an inspirational base for developers looking to combine functionality, beauty and high performance in a single project.

Join us, explore the possibilities, experiment with the code and make your own improvements - because the art of development is born in every new solution!

---

*Author: [Schreiry (David Greve)]*  
* Date: 2025-02-14 *
