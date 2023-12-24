#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include<vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"shaderClass.h"
#include"Camera.h"
#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

unsigned int VBO, VAO;
const float range = 3.0f;

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


// Attempt to build linear surface
// 
//void render(double a, double b, double lam) {
//    std::vector<float> vertices;
//
//    glLineWidth(1.0f);
//  
//    for (float x = -10.0f; x <= 10.0f; x += 0.2f) {
//        for (float y = -10.0f; y <= 10.0f; y += 0.2f) {
//            float equationOfLinearGenerator = x * x - (lam * lam * b * b / (a * a)) * y * y;
//
//            float z = x * x / (a * a) - y * y / (b * b);
//
//            bool isPointBelong = equationOfLinearGenerator == z * z;
//            
//            if (isPointBelong) {
//                vertices.push_back(x);
//                vertices.push_back(y);
//                vertices.push_back(z);
//
//                /*vertices.push_back(x + 0.2f);
//                vertices.push_back((x + 0.2f) * (x + 0.2f) / (a * a) - (y * y) / (b * b));
//                vertices.push_back(y);*/
//            }
//        }
//    }
//
//    GLuint VAO, VBO;
//    // Generate a VAO and bind it
//    glGenVertexArrays(1, &VAO);
//    glBindVertexArray(VAO);
//
//    // Generate a VBO and bind it
//    glGenBuffers(1, &VBO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//
//    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
//
//    // Enable the vertex attribute
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    
//    glBindVertexArray(VAO);
//    //GL_LINES GL_LINE_STRIP
//    glDrawArrays(GL_LINES, 0, vertices.size() / 3);
//    
//    glBindVertexArray(0);
//    glDeleteBuffers(1, &VBO);
//    glDeleteVertexArrays(1, &VAO);
//}


//Method works
GLuint vao, vbo;
void render() {
     //Set the line thickness
    glLineWidth(1.0f);

    // Start the ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Create a window with sliders for x and y
    ImGui::Begin("Control Window");

    static float constA = 4.6f;
    static float constB = 4.2f;

    ImGui::SliderFloat("const a", &constA, -10.0f, 10.0f);
    ImGui::SliderFloat("const b", &constB, -10.0f, 10.0f);

    ImGui::End();

    std::vector<float> vertices;

     //Generate vertices for the linear wireframe
    for (float x = -10.0f; x <= 10.0f; x += 0.2f) {
        for (float y = -10.0f; y <= 10.0f; y += 0.2f) {
             //Compute z based on the equation z = (x * x) / 2.0f - (y * y) / 4.0f
            float z = (x * x) / (constA* constA) - (y * y) / (constB*constB);

            // Store the two adjacent vertices of each line segment
            vertices.push_back(x);
            vertices.push_back(z);
            vertices.push_back(y);

            vertices.push_back(x + 0.2f);
            vertices.push_back((x + 0.2f) * (x + 0.2f) / (constA * constA) - (y * y) / (constB * constB));
            vertices.push_back(y);
        }
    }
   

    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    GLenum mode = GL_LINES;
    GLsizei count = vertices.size() / 3;
    glDrawArrays(mode, 0, count);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glBindVertexArray(0);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader ourShader("shader.v", "fragment.f");
    
    ourShader.use();

    Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 0.0f, 3.0f));

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    float size = 1.0f;
    ourShader.use();
    glUniform1f(glGetUniformLocation(ourShader.ID, "size"), size);
  ;
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

        // Handles camera inputs
        camera.Inputs(window);
        // Updates and exports the camera matrix to the Vertex Shader
        camera.Matrix(90.0f, 0.1f, 100.0f, ourShader, "camMatrix");

        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();



    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
