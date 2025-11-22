#include <NM-Config/Config.h>

#include <stdio.h>

#include "Source/Mesh-Processing.h"

int main(int argc, char **argv)
{
	mp_mesh_t mesh = mp_mesh_initialise();
	strcpy(mesh.name, "Test Mesh");
	if (argc > 1) { strcpy(mesh.path, argv[1]); }
	else { strcpy(mesh.path, "Meshes/stanford-bunny.obj"); }

	#ifdef MP_DEBUG
	mp_mesh_print(stdout, &mesh);
	#endif

	mp_mesh_free(&mesh);
	return 0;
}
