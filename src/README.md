# ALPHA 1 - RAYTRACING ASSIGNMENT - SAMUEL PALTI - 40126607

The current solution will store the json passed with the following argument :

`./raytracing ../assets/relative_path.json`

It will store the raw data in the following arrays :
geometry data -> raw_geometry
light data -> raw_light
output data -> raw_output

Then a PPM file will be created in the build directory using the `"filename"` attribute from the output data.
