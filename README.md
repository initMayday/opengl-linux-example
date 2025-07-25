# opengl-linux-example
An opengl example using meson, based upon learning opengl. Mainly here to show you how to build opengl on linux. This is easy to do (and hence I considered not posting this), but I remember a few years back when I initially tried to learn OpenGL, I struggled with this and hence here is a ladder for anyone who was like me a few years ago, just incase it helps anyone.

First run
meson setup build

Then you can run ./run.sh. It runs meson compile -C build, and runs the resulting binary.

glad and stb_image are under their own respective licenses - they are only included here for the sake of completeness
