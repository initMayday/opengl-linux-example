#include "shader.h"
#include <spdlog/spdlog.h>

void parse_success(unsigned int shader_handle, std::string type) {
    int success;
    glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[1024];
        glGetShaderInfoLog(shader_handle, 1024, NULL, log);
        spdlog::error("Failed to compile shader - Type: {} Handle: {}, Log: {}", type, shader_handle, log);
        exit(EXIT_FAILURE);
    }
}

void compile_shader(unsigned int shader_handle) {
    glCompileShader(shader_handle);
    parse_success(shader_handle, "COMPILE");
}

void set_shader_source(unsigned int shader_handle, std::string* source) {
    const char* source_temp = source->c_str();
    glShaderSource(shader_handle, 1, &source_temp, NULL); 
    // Args: Handle, number of shaders, shader (we have just one, add more params if more), and then the length of each shader (its NULL terminated, so put NULL)
}

unsigned int link_shaders(unsigned int vertex_shader_handle, unsigned int fragment_shader_handle) {
    unsigned int shader_program_handle = glCreateProgram();
    glAttachShader(shader_program_handle, vertex_shader_handle);
    glAttachShader(shader_program_handle, fragment_shader_handle);
    glLinkProgram(shader_program_handle); // Linking the two shaders into one executable

    parse_success(shader_program_handle, "LINK");
    return shader_program_handle;
}


unsigned int create_shader(std::string* vertex_shader_src, std::string* fragment_shader_src) {
    // Compile shaders
    unsigned int vertex_shader_handle = glCreateShader(GL_VERTEX_SHADER);
    set_shader_source(vertex_shader_handle, vertex_shader_src);
    compile_shader(vertex_shader_handle);
    unsigned int fragment_shader_handle = glCreateShader(GL_FRAGMENT_SHADER);
    set_shader_source(fragment_shader_handle, fragment_shader_src);
    compile_shader(fragment_shader_handle);

    // Link shaders
    unsigned int shader_program_handle = link_shaders(vertex_shader_handle, fragment_shader_handle);
    glDeleteShader(vertex_shader_handle); glDeleteShader(fragment_shader_handle);
    return shader_program_handle;
}
