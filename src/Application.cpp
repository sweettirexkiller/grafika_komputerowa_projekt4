#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<iostream>


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

    /* Make the window's context current */
    glfwMakeContextCurrent(window);


    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error!" << std::endl;
    }

    /* print opengl version and which driver is used */
    std::cout << glGetString(GL_VERSION) << std::endl;

    unsigned int buffer;
    float positions[6] = {
		-0.5f, -0.5f,
		 0.0f,  0.5f,
		 0.5f, -0.5f
	};
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        // this is legacy opengl code

        //glBegin(GL_TRIANGLES);
        //glVertex2f(-0.5f, -0.5f);
        //glVertex2f( 0.0f,  0.5f);
        //glVertex2f( 0.5f, -0.5f);
        //glEnd();

        // this is modern opengl code
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);



        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}