#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


// struct for shader source code
struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filePath) {
    std::ifstream stream(filePath);
	std::string line;
    static std::stringstream ss[2];
    enum class ShaderType
    {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
			if(line.find("vertex") != std::string::npos)
	        {
				// set mode to vertex
                type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				// set mode to fragment
                type = ShaderType::FRAGMENT;
			}
		}
        else
        {
			ss[(int)type] << line << '\n';
		}
	}

    return { ss[0].str(), ss[1].str() };

}

static unsigned int CompileShader(const std::string& source, unsigned int type)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	GlCall(glShaderSource(id, 1, &src, nullptr));
	GlCall(glCompileShader(id));

	// Error handling
	int result;
	GlCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
		int length;
		GlCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		// alloca is a stack memory allocation
		char* message = (char*)alloca(length * sizeof(char));
		GlCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to compile shader!" << std::endl;
		std::cout << message << std::endl;
		GlCall(glDeleteShader(id));
		return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GlCall(unsigned int program = glCreateProgram());
    GlCall(unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER));
    GlCall(unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER));
	GlCall(glAttachShader(program, vs));
    GlCall(glAttachShader(program, fs));
    GlCall(glLinkProgram(program));
    GlCall(glValidateProgram(program));
    // Delete the intermediate shader objects
    GlCall(glDeleteShader(vs));
    GlCall(glDeleteShader(fs));
	return program;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

   
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) 
    {
        glfwTerminate();
        return -1;
    }

    //create context with core profile 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //no backwards compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
     
    glfwSwapInterval(1);


    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error!" << std::endl;
    }

    /* print opengl version and which driver is used */
    std::cout << glGetString(GL_VERSION) << std::endl;
    
    {
        float vertices[] = {
            // positions          // colors           // texture coords
             0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
             0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
        };

        unsigned int indices[] = {
            0, 1, 2,
            1,2,3
        };

      

        // Vertex Array Object
        unsigned int vao;
        GlCall(glGenVertexArrays(1, &vao));
        GlCall(glBindVertexArray(vao));

        // Vertex Buffer Object
        VertexBuffer vb(vertices, 4 * 8 * sizeof(float));

        GlCall(glEnableVertexAttribArray(0));
        // position attribute
        GlCall(glVertexAttribPointer(0,  3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0));
        GlCall(glEnableVertexAttribArray(0));
        GlCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const void*)(3 * sizeof(float))));
        GlCall(glEnableVertexAttribArray(1));
        GlCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const void*)(6 * sizeof(float))));
        GlCall(glEnableVertexAttribArray(2));

        // Index Buffer Object
        IndexBuffer ib(indices, 6);

        // parcing shader code
        ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

        // creating shader
        unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
        Shader myShader = Shader(shader);
        GlCall(myShader.use());

        // addint uniform to shader, this is how we can change the color
        // location is the location of the uniform value in the shader
       /* GlCall( int location = glGetUniformLocation(shader, "u_Color"));
        ASSERT(location != -1);
        GlCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));
    */
    
        //unbind everything
        GlCall(glBindVertexArray(0));
        GlCall(glUseProgram(0));
        GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));



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
        unsigned char* data = stbi_load("res/textures/container.jpg", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)){

            processInput(window);

            GlCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

            GlCall(glClear(GL_COLOR_BUFFER_BIT));

            // bind shader
            GlCall(glUseProgram(shader));

            // bind vao and ibo
          
            glBindTexture(GL_TEXTURE_2D, texture);
            glBindVertexArray(vao);

            GlCall(glBindVertexArray(vao));
            ib.Bind();
     

            GlCall(glDrawElements(GL_TRIANGLES, 6*2, GL_UNSIGNED_INT, nullptr));

            /* Swap front and back buffers */
            GlCall(glfwSwapBuffers(window));
            /* Poll for and process events */
            GlCall(glfwPollEvents());
        }

        GlCall(glDeleteProgram(shader));

    }

    glfwTerminate();
    return 0;
}  