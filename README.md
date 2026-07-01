# The Legend of Nora

This is a mini-game written in C++ and [raylib](https://github.com/raysan5/raylib). It follows our protagonist, Nora, who rides a motorcycle through the Congo jungle collecting books. To survive she must avoid chimps, and makes Ape friends along the way. 

If you haven't read [hitchhiker's guide to the galaxy](https://www.goodreads.com/book/show/11.The_Hitchhiker_s_Guide_to_the_Galaxy), then the references won't make any sense and I apologize.

## Building

The project requires a C++ compiler, as well as raylib libraries.

```
mkdir build
cd build/
cmake ../ # or with ninja-build
make -j
```

And then you may run the generated binary like so

```
./LegendOfNora
```
