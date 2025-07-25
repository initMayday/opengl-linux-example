#pragma once
#include <glad/glad.h>
#include <string>

unsigned int create_shader(std::string* vertex_shader_src, std::string* fragment_shader_src);
