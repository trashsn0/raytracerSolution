# ALPHA 2 - RAYTRACING ASSIGNMENT - SAMUEL PALTI - 40126607

The current solution will store the json passed with the following argument :

`./raytracing ../assets/relative_path.json`

It will store the raw data in the following arrays :
geometry data -> raw_geometry
    Object -> Sphere/Rectangle class
light data -> raw_light
    Lighting -> Point/Area class
output data -> raw_output
    Camera attributes

Using simple raycasting it will generate semi-broken pixel per pixel PPM output.
Then PPM file(s) will be created in the build directory using the `"filename"` attribute from the output data.
