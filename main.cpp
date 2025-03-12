
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include <iostream>
#include<glm/gtc/matrix_transform.hpp>
#include<gl/GL.h>
#include"object.h"
#include "camera.h"
#include <random>
#include <thread>
#include <queue>
using namespace glm;
using namespace std;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

unsigned int SCR_WIDTH = 1600;
unsigned int SCR_HEIGHT = 900;
unsigned int OFF_WIDTH = 0;
unsigned int OFF_HEIGHT = 0;
camera cam;
float resolution = 1.0f;
bool MouseLocked = false;
bool ELocked = false;
bool erere = false;
int adee = 1;
float adeee = 0;
float ArmL = 0.27f;
float ArmL2 = 0.0f;
float ArmL3 = 1.25;
float deltaTime = 1.0f;
float UTime = 0.0f;
float sceeee = 1.0f;

// Define the random number generator and distribution
std::random_device rd;  // Seed generator
std::mt19937 gen(rd()); // Mersenne Twister engine
std::uniform_real_distribution<float> Rand(0.0f, 1.0f); // Range [0, 1]
GLFWwindow* window;

int main()
{
    #pragma region Start


    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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

    GLuint lyrs[6] = { 0,0,0,0,0,0 };
    unsigned int shaderProgram = initShader("shaders/def.vert", "shaders/def.geom", "shaders/def.frag");
    createShadowFramebufferCube shBF = createShadowFramebufferCube(500);

    cout << "size" << endl;
    int c = 20;
    
    while (c <= 1 || c > 50)
    {
        if(c>50)
        {
            cout << "too large" << endl;
        }
        cout << "size" << endl;
        
        float cc;
        cin >> cc;
        c = int(cc);
    } 
    
    
    cam = camera(glm::vec3(0.0f, 0.2f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f)); 
    
    
    float vertices[] = {
        0.0f, -0.01f, 0.0f,
        0.0f, 0.01f, 0.0f,
         -0.01f,0.0f, 0.0f,
         0.01f, 0.0f,0.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


    float sensitivity = 0.001f;
    double preTime = 0.0;
    double curTime = 0.0;
    double timeDif;
    unsigned int counter = 0;
    cam.updateSize(vec2(SCR_WIDTH, SCR_HEIGHT) * resolution);
    #pragma endregion
    #pragma region Update
    glDepthFunc(GL_LEQUAL);
    glfwSwapInterval(1);
    SpObj testObj = SpObj(vec3(0), vec3(0), glm::vec3(0.09f), initCubeBuffer({9}), 27, 0);
    
    while (!glfwWindowShouldClose(window))
    {
       
        curTime = glfwGetTime();
        timeDif = curTime - preTime;
        counter++;
        if (timeDif >= 0.1) {
            std::string FPS = std::to_string((int)((1.0 / timeDif) * counter));
            std::string ms = std::to_string((int)((timeDif / counter) * 1000));
            std::string title = "EPQ:  " + FPS + "FPS  " + ms + "ms " + std::to_string(curTime) + "curTime";
            glfwSetWindowTitle(window, title.c_str());
            preTime = curTime;
            deltaTime = (float)((1.0 / counter) * timeDif);

            counter = 0;
        }
        if (SCR_WIDTH > 0 && SCR_HEIGHT > 0 && timeDif < 1.0) {


            if (MouseLocked)
            {

                // Stores the coordinates of the cursor
                double mouseX;
                double mouseY;
                // Fetches the coordinates of the cursor
                glfwGetCursorPos(window, &mouseX, &mouseY);

                // Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
                // and then "transforms" them into degrees 
                cam.rot.x += sensitivity * (float)((SCR_HEIGHT / 2) + OFF_HEIGHT - mouseY);
                cam.rot.y += sensitivity * (float)((SCR_WIDTH / 2) + OFF_WIDTH - mouseX);
                // cout << sensitivity * (float)((SCR_HEIGHT / 2) - mouseY) / SCR_HEIGHT << " " << sensitivity * (float)((SCR_WIDTH / 2) - mouseX) / SCR_WIDTH << endl;
                

                // Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
                glfwSetCursorPos(window, (SCR_WIDTH / 2) + OFF_WIDTH, (SCR_HEIGHT / 2) + OFF_HEIGHT);


                processInput(window);
                
                
                glDisable(GL_BLEND);


                glViewport(0, 0, 500, 500);

                if (UTime > 10) UTime = 0;
                float cpt = 0.1f;
                


                UTime += deltaTime;

                cam.GFB.bind(true);
                glUseProgram(shaderProgram);
                mat4 ma = cam.matrix(float(SCR_WIDTH) / float(SCR_HEIGHT));
                glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "uProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(ma));
                glUniform3f(glGetUniformLocation(shaderProgram, "camPos"), cam.pos.x, cam.pos.y, cam.pos.z);
                glEnable(GL_CULL_FACE);
                glCullFace(GL_FRONT);

                glDisable(GL_CULL_FACE);

                //draw


                glViewport(OFF_WIDTH, OFF_HEIGHT, SCR_WIDTH, SCR_HEIGHT);


                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                glEnable(GL_DEPTH_TEST);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



                glUseProgram(cam.ScreenShader);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, cam.PFB1.ColTex);
                glUniform4f(glGetUniformLocation(cam.ScreenShader, "textureMatrix"), 0, 0, 1, 1);
                cam.drawScreen();
                glUseProgram(cam.MapShader);
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, cam.MFB.ColTex);
                glUniform1i(glGetUniformLocation(cam.MapShader, "ColT2"), 0);
                glUniform4f(glGetUniformLocation(cam.MapShader, "textureMatrix"), 0, 0, 1, 1);
                glUniform2f(glGetUniformLocation(cam.MapShader, "screen"), SCR_WIDTH, SCR_HEIGHT);
                cam.drawScreen();

            }
            else {

                // Stores the coordinates of the cursor
                double mouseX;
                double mouseY;
                // Fetches the coordinates of the cursor
                glfwGetCursorPos(window, &mouseX, &mouseY);
                vec2 mousep = vec2((mouseX-OFF_WIDTH - (SCR_WIDTH- SCR_HEIGHT)/2)/ SCR_HEIGHT - 0.5,0.5- (mouseY-OFF_HEIGHT) / SCR_HEIGHT)*2.0f;

                if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && (abs(mousep.x) > 1 || abs(mousep.y) > 1))
                {
                    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
                    glfwSetCursorPos(window, (SCR_WIDTH / 2) + OFF_WIDTH, (SCR_HEIGHT / 2) + OFF_HEIGHT);
                    MouseLocked = true;
                }

            }
        }
        


        glfwPollEvents();
        glfwSwapBuffers(window);
    }

#pragma endregion
    #pragma region End


    glDeleteProgram(shaderProgram);

    glfwTerminate();
   
    return 0;

#pragma endregion


}

void processInput(GLFWwindow* window)
{

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
   if (width * 9 > height * 16)
   {
       SCR_WIDTH = height * (16.0/ 9.0);
       OFF_HEIGHT = 0;
       OFF_WIDTH = (width -SCR_WIDTH)/2;
       SCR_HEIGHT = height;
   }
   else
    {
        SCR_WIDTH = width;
        OFF_WIDTH = 0;
        OFF_HEIGHT = (height - SCR_HEIGHT)/2;
        SCR_HEIGHT = width * (9.0/16.0);
    }
     
    cam.updateSize(vec2(SCR_WIDTH, SCR_HEIGHT) * resolution);
}