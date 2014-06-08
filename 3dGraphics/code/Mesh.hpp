#ifndef MESH_HEADER
#define MESH_HEADER

	#include <vector>
	#include <cassert>
	#include "Rasterizer.hpp"
	#include "cpp-toolkit\headers\Point.hpp"
	#include "cpp-toolkit\headers\Rotation.hpp"
	#include "cpp-toolkit\headers\Translation.hpp"
	#include "cpp-toolkit\headers\Scaling.hpp"
	#include "objloader\objLoader.h"
	#include "SFML/System/Vector3.hpp"

    namespace aut
    {
		template< class COLOR_BUFFER_TYPE >
        class Mesh
        {
			typedef COLOR_BUFFER_TYPE            Color_Buffer;
            typedef typename Color_Buffer::Color Color;
			typedef toolkit::Point4f			 Vertex;
			typedef std::vector< Vertex >        Vertex_Buffer;
            typedef std::vector< int    >        Index_Buffer;
			typedef std::vector< Color  >        Vertex_Colors;

			//Buffers
			Index_Buffer                      index_buffer;
			Vertex_Buffer                     original_vertex;
			Vertex_Buffer                     transformed_vertex;
			std::vector< toolkit::Point4i >   display_vertex;
			Vertex_Colors                     original_colors;

			int screen_width;
			int screen_height;

			//Matrices
			toolkit::Rotation3f    rotation_x;
			toolkit::Rotation3f    rotation_y;
			toolkit::Rotation3f    rotation_z;
			toolkit::Translation3f translation;
			toolkit::Scaling3f     scale;

			//World position, rotation, scale
			sf::Vector3<float> pos;
			sf::Vector3<float> rot;
			sf::Vector3<float> sc;

        public:

			//Constructor
			Mesh(Color_Buffer & target, int swidth, int sheight):
			  screen_width(swidth), screen_height(sheight) 
			{
				//Initialize matrices
				translation.set(0, 0, 0);
				rotation_x.set<toolkit::Rotation3f::AROUND_THE_X_AXIS>(0.0f);
				rotation_y.set<toolkit::Rotation3f::AROUND_THE_Y_AXIS>(0.0f);
				rotation_z.set<toolkit::Rotation3f::AROUND_THE_Z_AXIS>(0.0f);

				pos.x = 0;
				pos.y = 0;
				pos.z = 0;

				rot.x = 0;
				rot.y = 0;
				rot.z = 0;

				sc.x = 0;
				sc.y = 0;
				sc.z = 0;
			}

			//Loads a mesh from the given path
			void load(std::string name)
			{
				std::vector<std::vector<int>> mesh_vertex;
				std::vector<std::vector<int>> mesh_triangles;
				std::vector<std::vector<int>> mesh_colors;

				objLoader *objData = new objLoader();

				objData->load((char*)name.c_str());

				int vcounter = 0;

				mesh_vertex.resize   (objData->faceCount * 3);
				mesh_colors.resize   (objData->faceCount * 3);
				mesh_triangles.resize(objData->faceCount);

				//Read the mesh
				//faces
				for(size_t i = 0; i < mesh_triangles.size(); ++i)
				{
					size_t vertexinface = objData->faceList[i]->vertex_count;
					mesh_triangles[i].resize(vertexinface);

					//vertices in the face
					for(size_t j = 0; j < vertexinface; ++j)
					{
						mesh_triangles[i][j] = vcounter;

						mesh_vertex[vcounter].resize(4);
						mesh_colors[vcounter].resize(3);

						//Vertex and Color init: x, y, z
						for(size_t k = 0; k < 3; ++k)
						{
							mesh_vertex[vcounter][k] = int(objData->vertexList[objData->faceList[i]->vertex_index[j]]->e[k]);
							if(objData->faceList[i]->material_index == -1)
							{
								mesh_colors[vcounter][k] = 0;
							}
							else
							{
								mesh_colors[vcounter][k] = int(objData->materialList[objData->faceList[i]->material_index]->diff[k] * 255);
							}
						}
						mesh_vertex[vcounter][3] = 1;
						vcounter++;
					}
				}

				//Initialize the buffers
				size_t number_of_colors = calculate_vector2d_size(mesh_colors) / (sizeof(int) * 3);
				size_t vertex_count     = calculate_vector2d_size(mesh_vertex) / (sizeof(int) * 4);
				assert(number_of_colors == vertex_count);					// Same number of colors and vertices

				//Initializes the original vertex list
				original_vertex.resize       (vertex_count);
				transformed_vertex.resize    (vertex_count);
				display_vertex.resize        (vertex_count);
				original_colors.resize       (number_of_colors);

				for(size_t i = 0; i<vertex_count; ++i)
				{
					original_colors[i].set (mesh_colors[i][0], mesh_colors[i][1], mesh_colors[i][2]);

					int vertex_array[4];
					for(size_t j = 0; j<4; j++)
					{
						vertex_array[j] = mesh_vertex[i][j];
					}
					
					original_vertex[i] = Vertex(vertex_array);
				}

				//Initializes the index list
				size_t tri_count = calculate_vector2d_size(mesh_triangles) / (sizeof(int) * 3);

				index_buffer.resize(tri_count * 3);

				Index_Buffer::iterator index_i = index_buffer.begin();

				for(size_t i = 0; i<tri_count; ++i)
				{
					*index_i++ = mesh_triangles[i][0];
					*index_i++ = mesh_triangles[i][1];
					*index_i++ = mesh_triangles[i][2];
				}

				delete(objData);
			}

			//Sets the transformation buffer
			void update(toolkit::Transformation3f viewMatrix)
			{
				toolkit::Projection3f  projection  (5, 15, 20, float(screen_width) / float(screen_height));

				//apply transformation matrices
				toolkit::Transformation3f transformation = projection * viewMatrix * translation * rotation_x * rotation_y * rotation_z * scale;

				//check that the 4th number is still 1
				for (int index = 0, number_of_vertices = original_vertex.size (); index < number_of_vertices; index++)
				{
					Vertex & vertex = transformed_vertex[index] = toolkit::Matrix44f(transformation) * toolkit::Matrix41f(original_vertex[index]);
					float divisor = (1.f / vertex[3]);
					vertex[0] *= divisor;
					vertex[1] *= divisor;
					vertex[2] *= divisor;
					vertex[3]  = 1.f;
				}
			}

			//Draws the faces of the mesh on the screen
			template< class COLOR_BUFFER_TYPE >
            void paint (Rasterizer<COLOR_BUFFER_TYPE> *rasterizer)
			{
				//Transform to screen coordinates
				size_t vertex_number = transformed_vertex.size();

				toolkit::Scaling3f        scaling        = toolkit::Scaling3f    (float(screen_width / 2), float(screen_height / 2), 100000000.f);
				toolkit::Translation3f    translation    = toolkit::Translation3f(float(screen_width / 2), float(screen_height / 2), 0.f);
				toolkit::Transformation3f transformation = translation * scaling;

				for (size_t index = 0; index < vertex_number; index++)
				{
					display_vertex[index] = toolkit::Point4i( toolkit::Matrix44f(transformation) * toolkit::Matrix41f(transformed_vertex[index]) );
				}

				//Draw Triangles
				for(int * i = index_buffer.data(), * end = i + index_buffer.size(); i<end; i+=3)
				{
					if (is_frontface (transformed_vertex.data (), i))
					{
						rasterizer->set_color                    (original_colors[*i]);
						rasterizer->fill_convex_polygon_z_buffer (display_vertex.data (), i, i+3);
					}
				}
			}

			//Draws the wireframe of the mesh
			template< class COLOR_BUFFER_TYPE >
			void wire_paint(Rasterizer<COLOR_BUFFER_TYPE> *rasterizer)
			{
				//Transform to screen coordinates
				size_t vertex_number = transformed_vertex.size();

				std::vector<toolkit::Point2i> rounded_vertex_list(vertex_number);

				for(size_t i = 0; i<vertex_number; ++i)
				{
					toolkit::Point2i& r_vertex     = rounded_vertex_list[i];
					toolkit::Point4f& trans_vertex = transformed_vertex [i];

					r_vertex.coordinates()[0] = int( std::floorf(trans_vertex.coordinates()[0] * screen_width  / 2 + 0.5f) ) + screen_width  / 2;
					r_vertex.coordinates()[1] = int( std::floorf(trans_vertex.coordinates()[1] * screen_height / 2 + 0.5f) ) + screen_height / 2;
				}

				rasterizer->set_color(0,255,0);

				//Draw Triangles
				for(int i = 0, index_num = index_buffer.size(); i<index_num;)
				{
					toolkit::Point2i& rounded_vertex_0 = rounded_vertex_list[index_buffer[i++]];
					toolkit::Point2i& rounded_vertex_1 = rounded_vertex_list[index_buffer[i++]];
					toolkit::Point2i& rounded_vertex_2 = rounded_vertex_list[index_buffer[i++]];

					rasterizer->draw_triangle
					(
						rounded_vertex_0.coordinates ()[0], rounded_vertex_0.coordinates ()[1],
						rounded_vertex_1.coordinates ()[0], rounded_vertex_1.coordinates ()[1],
						rounded_vertex_2.coordinates ()[0], rounded_vertex_2.coordinates ()[1]
					);
				}
			}

			//Sets a new rotation for the mesh
			inline void setRotation(float _x, float _y, float _z)
			{
				rotation_x.set<toolkit::Rotation3f::AROUND_THE_X_AXIS>(_x);
				rotation_y.set<toolkit::Rotation3f::AROUND_THE_Y_AXIS>(_y);
				rotation_z.set<toolkit::Rotation3f::AROUND_THE_Z_AXIS>(_z);

				rot.x = _x;
				rot.y = _y;
				rot.z = _z;
			}

			//Sets a new position of the mesh in the world
			inline void setTranslation(float _x, float _y, float _z)
			{
				translation.set(_x, _y, _z);
				pos.x = _x;
				pos.y = _y;
				pos.z = _z;
			}

			//Sets a new scale of the mesh
			inline void setScale(float _x, float _y, float _z)
			{
				scale.set(_x, _y, _z);
				sc.x = _x;
				sc.y = _y;
				sc.z = _z;
			}

			//Sets a new scale of the mesh
			inline void setScale(float value)
			{
				scale.set(value, value, value);
				sc.x = value;
				sc.y = value;
				sc.z = value;
			}

			//return the position of the mesh in the world
			sf::Vector3<float> getTranslation()
			{
				return pos;
			}

			//return the rotation of the mesh in the world
			sf::Vector3<float> getRotation()
			{
				return rot;
			}

			//return the scale of the mesh
			sf::Vector3<float> getScale()
			{
				return sc;
			}

			//Moves the mesh a given offset
			void Move(float _x, float _y, float _z)
			{
				pos.x += _x;
				pos.y += _y;
				pos.z += _z;

				translation.set(pos.x, pos.y, pos.z);
			}

			//Rotates the mesh a given offset
			void Rotate(float _x, float _y, float _z)
			{
				rot.x += _x;
				rot.y += _y;
				rot.z += _z; 

				rotation_x.set<toolkit::Rotation3f::AROUND_THE_X_AXIS>(rot.x);
				rotation_y.set<toolkit::Rotation3f::AROUND_THE_Y_AXIS>(rot.y);
				rotation_z.set<toolkit::Rotation3f::AROUND_THE_Z_AXIS>(rot.z);
			}

			//Adds a scale to the mesh
			void sumScale(float value)
			{
				sc.x += value;
				sc.y += value;
				sc.z += value;
				scale.set(sc.x, sc.y, sc.z);
			}

			//Adds a scale to the mesh
			void sumScale(float _x, float _y, float _z)
			{
				sc.x += _x;
				sc.y += _y;
				sc.z += _z;
				scale.set(sc.X, sc.Y, sc.Z);
			}

		private:
			bool is_frontface (const Vertex * const projected_vertex, const int * const indexes)
			{
				const Vertex & v0 = projected_vertex[indexes[0]];
				const Vertex & v1 = projected_vertex[indexes[1]];
				const Vertex & v2 = projected_vertex[indexes[2]];

				// Se asumen coordenadas proyectadas y polígonos definidos en sentido horario.
				// Se comprueba a qué lado de la línea que pasa por v0 y v1 queda el punto v2:

				return ((v1[0] - v0[0]) * (v2[1] - v0[1]) - (v2[0] - v0[0]) * (v1[1] - v0[1]) > 0.f);
			}

			int inline calculate_vector2d_size(std::vector<std::vector<int>> v)
			{
				size_t s = 0;
				for(size_t i = 0; i < v.size(); ++i)
				{
					for (size_t j = 0; j < v[i].size(); ++j)
					{
						s += sizeof(v[i][j]);
					}
				}

				return s;
			}

        };

    }

#endif
