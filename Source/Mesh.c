#include "Mesh.h"

mp_mesh_t mp_mesh_initialise()
{
	mp_mesh_t mesh;
	memset(&mesh, 0, sizeof(mesh));

	strcpy(mesh.name, "");
	strcpy(mesh.path, "");

	mesh.vertices		= NULL;
	mesh.normals		= NULL;
	mesh.colours		= NULL;
	mesh.uv_coordinates	= NULL;

	mesh.num_lod_levels	= 1;
	for (int i = 0; i < NM_MAX_LOD_LEVELS; i++) { mesh.faces[i] = NULL; }

	return mesh;
}

int mp_mesh_allocate(mp_mesh_t *mesh, char error_message[NM_MAX_ERROR_LENGTH])
{
	mp_mesh_free(mesh);

	mesh->vertices = malloc(mesh->num_vertices * sizeof(mp_position_t));
	mesh->normals = malloc(mesh->num_normals * sizeof(mp_normal_t));
	mesh->colours = malloc(mesh->num_colours * sizeof(mp_colour_t));
	mesh->uv_coordinates = malloc(mesh->num_uv_coordinates * sizeof(mp_uv_t));
	mesh->faces[0] = malloc(mesh->num_faces[0] * sizeof(mp_face_t));
	if (!mesh->vertices || !mesh->normals || !mesh->colours ||
			!mesh->uv_coordinates || !mesh->faces[0])
	{
		snprintf(error_message, NM_MAX_ERROR_LENGTH,
			"Could not allocate memory for mesh \"%s\".", mesh->name);
		return -1;
	}

	memset(mesh->vertices, 0, mesh->num_vertices * sizeof(mp_position_t));
	memset(mesh->normals, 0, mesh->num_normals * sizeof(mp_normal_t));
	memset(mesh->colours, 0, mesh->num_colours * sizeof(mp_colour_t));
	memset(mesh->uv_coordinates, 0, mesh->num_uv_coordinates * sizeof(mp_uv_t));
	memset(mesh->faces[0], 0, mesh->num_faces[0] * sizeof(mp_face_t));

	// Temporarily assign additional LOD levels to pointer for first:
	for (int i = 1; i < NM_MAX_LOD_LEVELS; i++) { mesh->faces[i] = mesh->faces[0]; }

	return 0;
}

void mp_mesh_free(mp_mesh_t *mesh)
{
	if (mesh->vertices)
	{
		free(mesh->vertices);
		mesh->vertices = NULL;
	}

	if (mesh->normals)
	{
		free(mesh->normals);
		mesh->normals = NULL;
	}

	if (mesh->colours)
	{
		free(mesh->colours);
		mesh->colours = NULL;
	}

	if (mesh->uv_coordinates)
	{
		free(mesh->uv_coordinates);
		mesh->uv_coordinates = NULL;
	}

	for (uint8_t i = 0; i < mesh->num_lod_levels; i++)
	{
		if (mesh->faces[i])
		{
			free(mesh->faces[i]);
			mesh->faces[i] = NULL;
		}
	}
}

#ifdef MP_DEBUG
void mp_mesh_print(FILE *file, mp_mesh_t *mesh)
{
	fprintf(file, "*******************\n");
	fprintf(file, "* Mesh debug info *\n");
	fprintf(file, "*******************\n");

	fprintf(file, "Mesh name: %s\n", mesh->name);
	fprintf(file, "Mesh path: %s\n", mesh->path);

	fprintf(file, "\n");

	fprintf(file, "Number of vertices: %u\n", mesh->num_vertices);
	fprintf(file, "Number of normals: %u\n", mesh->num_normals);
	fprintf(file, "Number of colours: %u\n", mesh->num_colours);
	fprintf(file, "Number of UV coordinates: %u\n", mesh->num_uv_coordinates);
	fprintf(file, "Number of faces: ");
	for (int i = 0; i < (mesh->num_lod_levels - 1); i++)
	{
		fprintf(file, "%u, ", mesh->num_faces[i]);
	}
	fprintf(file, "%u\n", mesh->num_faces[mesh->num_lod_levels - 1]);

	if ((mesh->num_faces[0] > 0) && mesh->vertices && mesh->faces[0])
	{
		fprintf(file, "\n");

		fprintf(file, "Data for vertices in first face:\n");
		for (int i = 0; i < 3; i++)
		{
			fprintf(file, "Vertex %d:\n", i);
			fprintf(file, "--> Position %u: %f, %f, %f\n", mesh->faces[0][0].p[i],
				mesh->vertices[mesh->faces[0][0].p[i]].x,
				mesh->vertices[mesh->faces[0][0].p[i]].y,
				mesh->vertices[mesh->faces[0][0].p[i]].z);
			fprintf(file, "--> Normal %u: %d, %d, %d\n", mesh->faces[0][0].n[i],
				mesh->normals[mesh->faces[0][0].n[i]].x,
				mesh->normals[mesh->faces[0][0].n[i]].y,
				mesh->normals[mesh->faces[0][0].n[i]].z);
			fprintf(file, "--> Colour %u: %u, %u, %u, %u\n", mesh->faces[0][0].c[i],
				mesh->colours[mesh->faces[0][0].c[i]].r,
				mesh->colours[mesh->faces[0][0].c[i]].g,
				mesh->colours[mesh->faces[0][0].c[i]].b,
				mesh->colours[mesh->faces[0][0].c[i]].a);
			fprintf(file, "--> UV coordinates %u: %f, %f\n", mesh->faces[0][0].u[i],
				mesh->uv_coordinates[mesh->faces[0][0].u[i]].u,
				mesh->uv_coordinates[mesh->faces[0][0].u[i]].v);
		}
	}
}
#endif
