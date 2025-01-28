//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtc/quaternion.hpp>
//#include <glm/gtx/quaternion.hpp>
//
//#include "Shader.h"
//#include "Camera.h"
//#include "Model.h"
//#include "Texture.h"
//
//#include "imgui.h"
//#include "imgui_impl_glfw.h"
//#include "imgui_impl_opengl3.h"
//
//#include <iostream>
//
//#define INF 114514.0
//
//using namespace glm;
//using namespace std;
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void processInput(GLFWwindow* window);
//glm::vec3 lightPos(2.4f, 2.0f, 4.0f);
//
//// settings
//const unsigned int SCR_WIDTH = 1920;
//const unsigned int SCR_HEIGHT = 1080;
//
//// camera
//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//float lastX = SCR_WIDTH / 2.0f;
//float lastY = SCR_HEIGHT / 2.0f;
//bool firstMouse = true;
//
//// timing
//float deltaTime = 0.0f;
//float lastFrame = 0.0f;
//
//// 物体表面材质定义
//struct Material {
//    vec3 emissive = vec3(0, 0, 0);  // 作为光源时的发光颜色
//    vec3 baseColor = vec3(1, 1, 1);
//    float subsurface = 0.0;
//    float metallic = 0.0;
//    float specular = 0.0;
//    float specularTint = 0.0;
//    float roughness = 0.0;
//    float anisotropic = 0.0;
//    float sheen = 0.0;
//    float sheenTint = 0.0;
//    float clearcoat = 0.0;
//    float clearcoatGloss = 0.0;
//    float IOR = 1.0;
//    float transmission = 0.0;
//};
//
//// 三角形定义
//struct Triangle {
//    vec3 p1, p2, p3;    // 顶点坐标
//    vec3 n1, n2, n3;    // 顶点法线
//    Material material;  // 材质
//};
//
//// BVH 树节点
//struct BVHNode {
//    int left, right;    // 左右子树索引
//    int n, index;       // 叶子节点信息               
//    vec3 AA, BB;        // 碰撞盒
//};
//
////三角形编码
//struct Triangle_encoded {
//    vec3 p1, p2, p3;    // 顶点坐标
//    vec3 n1, n2, n3;    // 顶点法线
//    vec3 emissive;      // 自发光参数
//    vec3 baseColor;     // 颜色
//    vec3 param1;        // (subsurface, metallic, specular)
//    vec3 param2;        // (specularTint, roughness, anisotropic)
//    vec3 param3;        // (sheen, sheenTint, clearcoat)
//    vec3 param4;        // (clearcoatGloss, IOR, transmission)
//};
//
////BVH编码
//struct BVHNode_encoded {
//    vec3 childs;        // (left, right, 保留)
//    vec3 leafInfo;      // (n, index, 保留)
//    vec3 AA, BB;
//};
//
//// 按照三角形中心排序 -- 比较函数
//bool cmpx(const Triangle& t1, const Triangle& t2) {
//    vec3 center1 = (t1.p1 + t1.p2 + t1.p3) / vec3(3, 3, 3);
//    vec3 center2 = (t2.p1 + t2.p2 + t2.p3) / vec3(3, 3, 3);
//    return center1.x < center2.x;
//}
//bool cmpy(const Triangle& t1, const Triangle& t2) {
//    vec3 center1 = (t1.p1 + t1.p2 + t1.p3) / vec3(3, 3, 3);
//    vec3 center2 = (t2.p1 + t2.p2 + t2.p3) / vec3(3, 3, 3);
//    return center1.y < center2.y;
//}
//bool cmpz(const Triangle& t1, const Triangle& t2) {
//    vec3 center1 = (t1.p1 + t1.p2 + t1.p3) / vec3(3, 3, 3);
//    vec3 center2 = (t2.p1 + t2.p2 + t2.p3) / vec3(3, 3, 3);
//    return center1.z < center2.z;
//}
//
////// 计算 AABB 盒，归一化模型大小
////float maxx = -1919810.00;
////float maxy = -1919810.00;
////float maxz = -1919810.00;
////float minx = 1919810.00;
////float miny = 1919810.00;
////float minz = 1919810.00;
//
//
//
//int main()
//{
//    // glfw: initialize and configure
//    // ------------------------------
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//#ifdef __APPLE__
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif
//
//    // glfw window creation
//    // --------------------
//    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
//    if (window == NULL)
//    {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwMakeContextCurrent(window);
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//    glfwSetCursorPosCallback(window, mouse_callback);
//    glfwSetScrollCallback(window, scroll_callback);
//
//    // Setup Dear ImGui context
//    IMGUI_CHECKVERSION();
//    ImGui::CreateContext();
//    ImGuiIO& io = ImGui::GetIO(); (void)io;
//    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
//    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
//
//    // Setup Dear ImGui style
//    ImGui::StyleColorsDark();
//    //ImGui::StyleColorsLight();
//
//    // Setup Platform/Renderer backends
//    ImGui_ImplGlfw_InitForOpenGL(window, true);
//    ImGui_ImplOpenGL3_Init("#version 330");
//
//    // tell GLFW to capture our mouse
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//
//    // glad: load all OpenGL function pointers
//    // ---------------------------------------
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
//    //开深度测试
//    glEnable(GL_DEPTH_TEST);
//
//   //设置shader
//    Shader ourShader("./content/shader/modelshader.vs", "./content/shader/Modelshader.fs");
//    Shader lightCubeShader("./content/shader/Texturevertexshader.vs", "./content/shader/Texturefragmentshader.fs");
//    //要在模型同目录内有MTL文件且指定好纹理assimp库才会去读取纹理图片
//    Model ourModel("./content/model/hydrant/hydrant.obj");
//
//    float vertices[] = {
//      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//       0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//       0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//       0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//      -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//
//      -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
//       0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
//       0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
//       0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
//      -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
//      -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
//
//      -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//      -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//      -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//      -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//      -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//      -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//
//       0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//       0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//       0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//       0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//       0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//       0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//
//      -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//       0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//       0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//       0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//      -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//      -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//
//      -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
//       0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
//       0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//       0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//      -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//      -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
//    };
//    // first, configure the cube's VAO (and VBO)
//    unsigned int VBO, cubeVAO;
//    glGenVertexArrays(1, &cubeVAO);
//    glGenBuffers(1, &VBO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//    // position attribute
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//    // normal attribute
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//    glEnableVertexAttribArray(1);
//
//
//    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
//    unsigned int lightCubeVAO;
//    glGenVertexArrays(1, &lightCubeVAO);
//    glBindVertexArray(lightCubeVAO);
//
//    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    //imgui 可修改元素
//    glm::vec3 model_translate = glm::vec3(0.0, -0.8, 0.0);
//    glm::vec3 model_scale = glm::vec3(0.01, 0.01, 0.01);
//    float radians = 45.0f;
//
//    // draw in wireframe
//    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//
//    // render loop
//    // -----------
//    while (!glfwWindowShouldClose(window))
//    {
//        // per-frame time logic
//        float currentFrame = static_cast<float>(glfwGetTime());
//        deltaTime = currentFrame - lastFrame;
//        lastFrame = currentFrame;
//
//        // input
//        processInput(window);
//
//        // render
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        ImGui_ImplOpenGL3_NewFrame();
//        ImGui_ImplGlfw_NewFrame();
//        ImGui::NewFrame();
//
//        //Imgui 需渲染元素
//        ImGui::Begin("Element");
//        ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//        if (ImGui::BeginTabBar("Dir", 0))
//        {
//            if (ImGui::BeginTabItem("Hydrant"))
//            {
//                ImGui::Text("Hydrant:");
//                ImGui::Text("translate");
//                ImGui::SliderFloat("x", &model_translate.x, -10.0, 10.0);
//                ImGui::SliderFloat("y", &model_translate.y, -10.0, 10.0);
//                ImGui::SliderFloat("z", &model_translate.z, -10.0, 10.0);
//                ImGui::Text("rotation");
//                ImGui::SliderFloat("rad", &radians, 22.5, 100.0);
//                ImGui::Text("scale");
//                ImGui::SliderFloat("x_s", &model_scale.x, -0.5, 0.5);
//                ImGui::SliderFloat("y_s", &model_scale.y, -0.5, 0.5);
//                ImGui::SliderFloat("z_s", &model_scale.z, -0.5, 0.5);
//
//                ImGui::EndTabItem();
//            }
//            ImGui::EndTabBar();
//        }
//        ImGui::End();
//        //结束Imgui
//
//        // view/projection transformations
//        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//        glm::mat4 view = camera.GetViewMatrix();
//        // render the cube
//        glBindVertexArray(cubeVAO);
//        glDrawArrays(GL_TRIANGLES, 0, 36);
//        // also draw the lamp object
//        lightCubeShader.use();
//        lightCubeShader.setMat4("projection", projection);
//        lightCubeShader.setMat4("view", view);
//        glm::mat4 modelC = glm::mat4(1.0f);
//        modelC = glm::translate(modelC, lightPos);
//        modelC = glm::scale(modelC, glm::vec3(0.2f)); // a smaller cube
//        lightCubeShader.setMat4("model", modelC);
//
//        glBindVertexArray(lightCubeVAO);
//        glDrawArrays(GL_TRIANGLES, 0, 36);
//
//
//        // don't forget to enable shader before setting uniforms
//        ourShader.use();
//        ourShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
//        ourShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
//        ourShader.setVec3("lightPos", lightPos);
//        ourShader.setVec3("viewPos", camera.Position);
//        ourShader.setInt("texture_diffuse1", 0);
//
//        // render the loaded model
//        glm::mat4 model = glm::mat4(1.0f);
//        model = glm::translate(model, model_translate); // translate it down so it's at the center of the scene
//        model = glm::scale(model, model_scale);	// it's a bit too big for our scene, so scale it down
//
//        model = glm::rotate(model, glm::radians(radians), glm::vec3(0.0, 1.0, 0.0));
//
//        ourShader.setMat4("model", model);
//        ourShader.setMat4("projection", projection);
//        ourShader.setMat4("view", view);
//        ourModel.Draw(ourShader);
//
//        //提交imgui渲染
//        ImGui::Render();
//        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
//        // -------------------------------------------------------------------------------
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    // glfw: terminate, clearing all previously allocated GLFW resources.
//    // ------------------------------------------------------------------
//    glfwTerminate();
//    return 0;
//}
//
//// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
//// ---------------------------------------------------------------------------------------------------------
//void processInput(GLFWwindow* window)
//{
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//        camera.ProcessKeyboard(FORWARD, deltaTime);
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//        camera.ProcessKeyboard(BACKWARD, deltaTime);
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//        camera.ProcessKeyboard(LEFT, deltaTime);
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//        camera.ProcessKeyboard(RIGHT, deltaTime);
//}
//
//// glfw: whenever the window size changed (by OS or user resize) this callback function executes
//// ---------------------------------------------------------------------------------------------
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//    // make sure the viewport matches the new window dimensions; note that width and 
//    // height will be significantly larger than specified on retina displays.
//    glViewport(0, 0, width, height);
//}
//
//void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
//{
//    //double xpos_c, ypos_c;
//
//    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
//    {
//        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//
//        float xpos = static_cast<float>(xposIn);
//        float ypos = static_cast<float>(yposIn);
//        float xoffset = xpos - lastX;
//        float yoffset = lastY - ypos; 
//
//        lastX = xpos;
//        lastY = ypos;
//
//        camera.ProcessMouseMovement(xoffset, yoffset);
//
//    }
//    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
//    {
//
//        double xpos, ypos;
//        glfwGetCursorPos(window, &xpos, &ypos);
//        lastX = xpos;
//        lastY = ypos;
//        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
//    }
//}
//
//// glfw: whenever the mouse scroll wheel scrolls, this callback is called
//// ----------------------------------------------------------------------
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//    camera.ProcessMouseScroll(static_cast<float>(yoffset));
//}