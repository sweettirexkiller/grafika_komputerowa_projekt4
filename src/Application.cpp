#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"


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
        float positions[] = {
		    -0.5f, -0.5f,
		     0.5f, -0.5f,
		     0.5f, 0.5f, 
             -0.5f, 0.5f
	    };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        // Vertex Array Object
        unsigned int vao;
        GlCall(glGenVertexArrays(1, &vao));
        GlCall(glBindVertexArray(vao));

        // Vertex Buffer Object
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));

        GlCall(glEnableVertexAttribArray(0));
        // thil links the buffer to the vao
        GlCall(glVertexAttribPointer(0,  2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

        // Index Buffer Object
        IndexBuffer ib(indices, 6);

        // parcing shader code
        ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

        // creating shader
        unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
        GlCall(glUseProgram(shader));

        // addint uniform to shader, this is how we can change the color
        // location is the location of the uniform value in the shader
        GlCall( int location = glGetUniformLocation(shader, "u_Color"));
        ASSERT(location != -1);
        GlCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));
    
    
        //unbind everything
        GlCall(glBindVertexArray(0));
        GlCall(glUseProgram(0));
        GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window)){

            processInput(window);

            GlCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

            GlCall(glClear(GL_COLOR_BUFFER_BIT));

            // bind shader
            GlCall(glUseProgram(shader));
            // change color
            GlCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

            // bind vao and ibo
            GlCall(glBindVertexArray(vao));
            ib.Bind();
     

            GlCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            // change color
            if (r > 1.0f)
            {
			    increment = -0.05f;
		    }
            else if (r < 0.0f)
            {
			    increment = 0.05f;
		    }
            r += increment;

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