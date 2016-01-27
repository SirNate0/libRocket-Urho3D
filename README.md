# libRocket-Urho3D

A derivitive of [Mike's libRocket-Urho3D port](https://github.com/realrunner/urho3d-librocket).

This is incomplete. A sample is included, and can be run by calling the following (there is a main file that does this in the root class):

    Root::instance()->Run();

There are currently some limitations:

- Can only create one 3D document at a time
- Incomplete input support
- Some texture wrapping issues with 3D documents, and the material(?) leaves it looking faded
- Possible memory leaks, especially with the Document components, which do not release the libRocket contexts properly, I think.
- Events only propagate from libRocket to Urho, but you cannot send events from Urho to libRocket (except through the libRocket code).
- The `<select>` element does not display.

This will be improved as time goes on.

## Directions

1. Merge the UrhoSource directory with the UrhoRepo/Source directory (the change in the Source/CMakeLists.txt file is the addition of the following to before "add_subdirectory (Urho3D)":

    if (URHO3D_ROCKET)
        add_subdirectory (ThirdParty/libRocket)
    endif ()

2. Rebuild the Urho3D library with this change. Install the LibRocket headers where they can be included with '<Rocket/Core/File.h>', etc.
3. Add the needed ./CMake folder from the Urho repository, and run the project scripts. You will likely have to copy the Data and CoreData folders into the ./bin directory, at which point you should merge the ./Data folder with the other ./bin/Data folder.
