#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "shader.h"

std::string vertex_shader_src = R"(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColour;
layout (location = 2) in vec2 aTexCoord;
out vec3 DefinedColour;
out vec2 Texcoord;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    DefinedColour = aColour;
    Texcoord = aTexCoord;
})";

std::string fragment_shader_src = R"(#version 330 core
out vec4 FragColour;
in vec3 DefinedColour;
in vec2 Texcoord;

uniform sampler2D UniTex;

void main()
{
    FragColour = texture(UniTex, Texcoord);
})";

void process_input(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// CALLBACK:GLFW, whenever window size changes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


int main()
{
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Get window handle
    GLFWwindow* window = glfwCreateWindow(1000, 1000, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        spdlog::error("Failed to create GLFW window");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Load GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        spdlog::debug("Failed to initialize GLAD");
        exit(EXIT_FAILURE);
    }    

    unsigned int shader_program_handle = create_shader(&vertex_shader_src, &fragment_shader_src);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        spdlog::error("Failed to load texture");
    }
    stbi_image_free(data);

    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    }; 
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,
        1, 2, 3
    };

    unsigned int VBO, VAO, EBO; // VBO is the vertex raw data, VAO is metadata about it (such as every 9 numbers it resets pattern, eg every 3 is position, normal, colour), EBO is the index buffer and specifies the order of verticies
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO to VBO, to specify this VAO is the metadata for the VBO just after (VAO starts 'recording')
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Loading vertices raw data into the VBO

    // Loading the order of the vertices into the EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // THIS IS NOT GLOBAL - THIS BINDS IT TO THE VAO, NOT TO OPENGL GENERALLY! HENCE WHY WE DO NOT UNBIND IT LATER - IT IS SPECIFIC TO THE VAO. THE OTHER
    // TWO ARE GLOBAL, AND HENCE WE UNBIND THEM AFTER WE ARE FINISHED WITH THEM, BUT THIS ISN'T
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // This configures the VAO, this is the metadata
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // Unbind the VBO. It is common practice to do this before the VAO. The VAO does not care as it has already recorded which VBO it tracks.
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // Unbind the VAO to stop 'recording'
    glBindVertexArray(0); 

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe view

    // Event loop
    while (!glfwWindowShouldClose(window))
    {
        process_input(window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw triangle
        glUseProgram(shader_program_handle); // Set the 2 shaders we're using
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0); // Args, type to draw, number of indices to draw from EBO, data type of index, offset from start of EBO in bytes
        // glBindVertexArray(0); // no need to unbind it every time 
 
        // Swap front and back buffers, and poll events
        glfwSwapBuffers(window); // We implictly render to the off-screen buffer, done automatically by Opengl and GLFW
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


