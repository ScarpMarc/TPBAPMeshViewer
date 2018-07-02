# TPBAPMeshViewer
Mesh opener for Treasure Planet: Battle at Procyon 

This program can open and convert .mdb mesh files.

It has to be run via CMD or PowerShell.
1. Place the converter in your BD_Meshes folder, or copy some meshes into the converter's folder 
2. Open CMD and type in "cd <folder>" where folder is the location of the mesh viewer and hit ENTER.
3. Type ./TPInspector_<version> -all (just type ./TP and then hit TAB, it should do) and hit ENTER.

At the current state it only supports the -all command, which converts all of the meshes in the folder.
It creates two sub-folders: one with the mesh data and one with the texture name data.

The program can export to .OBJ, .MTL file generation is still pending because some reverse-engineering still has to be done.

More commands and documentation coming soon-ish.
