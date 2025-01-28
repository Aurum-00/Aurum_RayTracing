////#include <glew.h>
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
////#include <freeglut.h>
//
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
//#include "LightandObject.h"
//#include "BVH.h"
////#include "GLFWmodel.h"
//
//#include "hdrloader.h"
//
//#include "imgui.h"
//#include "imgui_impl_glfw.h"
//#include "imgui_impl_opengl3.h"
//
//#include <iostream>
//#include <algorithm>
//#include <ctime>
//#include <iomanip>
//
//#define INF 114514.0
////#define SCR_WIDTH 1024;
////#define SCR_HEIGHT 1024;
//
//// settings
//const unsigned int SCR_WIDTH = 1024;
//const unsigned int SCR_HEIGHT = 1024;
//unsigned int frameCounter = 0;
//
//unsigned int trianglesTextureBuffer;
//unsigned int nodesTextureBuffer;
//unsigned int lastFrame;
//unsigned int hdrMap;
////unsigned int frameCounter = 0;
//
//class RenderPass {
//public:
//    GLuint FBO = 0;
//    GLuint vao, vbo;
//    //
//    std::vector<GLuint> colorAttachments;
//    GLuint program;
//    int width = 1024;
//    int height = 1024;
//    void bindData(bool finalPass = false) {
//        if (!finalPass) glGenFramebuffers(1, &FBO);
//        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
//
//        glGenBuffers(1, &vbo);
//        glBindBuffer(GL_ARRAY_BUFFER, vbo);
//        //为了使图像渲染到一个VBO 为帧缓冲做准备
//        std::vector<vec3> square = { vec3(-1, -1, 0), vec3(1, -1, 0), vec3(-1, 1, 0), vec3(1, 1, 0), vec3(-1, 1, 0), vec3(1, -1, 0) };
//        glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * square.size(), NULL, GL_STATIC_DRAW);
//        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec3) * square.size(), &square[0]);
//
//        glGenVertexArrays(1, &vao);
//        glBindVertexArray(vao);
//        glEnableVertexAttribArray(0);   // layout (location = 0) 
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
//        // 不是 finalPass 则生成帧缓冲的颜色附件,绑到FBO上
//        if (!finalPass) {
//            std::vector<GLuint> attachments;
//            for (int i = 0; i < colorAttachments.size(); i++) {
//                glBindTexture(GL_TEXTURE_2D, colorAttachments[i]);
//                // 将它附加到当前绑定的帧缓冲对象
//                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorAttachments[i], 0);// 将颜色纹理绑定到 i 号颜色附件
//                attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
//            }
//            glDrawBuffers(attachments.size(), &attachments[0]);
//        }
//
//        glBindFramebuffer(GL_FRAMEBUFFER, 0);
//    }
//    void draw(std::vector<GLuint> texPassArray = {}) {
//        glUseProgram(program);
//        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
//        glBindVertexArray(vao);
//        // 传上一帧的帧缓冲颜色附件
//        for (int i = 0; i < texPassArray.size(); i++) {
//            glActiveTexture(GL_TEXTURE0 + i);
//            glBindTexture(GL_TEXTURE_2D, texPassArray[i]);
//            //std::string uName = "texPass" + std::to_string(i);
//            glUniform1i(glGetUniformLocation(program, "texPass0"), i);
//        }
//        glViewport(0, 0, width, height);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        glDrawArrays(GL_TRIANGLES, 0, 6);
//
//        glBindVertexArray(0);
//        glBindFramebuffer(GL_FRAMEBUFFER, 0);
//        glUseProgram(0);
//    }
//};
//
//void readObj(std::string filepath, std::vector<Triangle>& triangles, Material material, mat4 trans, bool smoothNormal) {
//
//    // 顶点位置，索引
//    std::vector<vec3> vertices;
//    std::vector<GLuint> indices;
//
//    // 打开文件流
//    std::ifstream fin(filepath);
//    std::string line;
//    if (!fin.is_open()) {
//        std::cout << "文件 " << filepath << " 打开失败" << std::endl;
//        exit(-1);
//    }
//
//    // 计算 AABB 盒，归一化模型大小
//    float maxx = -11451419.19;
//    float maxy = -11451419.19;
//    float maxz = -11451419.19;
//    float minx = 11451419.19;
//    float miny = 11451419.19;
//    float minz = 11451419.19;
//
//    // 按行读取
//    while (std::getline(fin, line)) {
//        std::istringstream sin(line);   // 以一行的数据作为 string stream 解析并且读取
//        std::string type;
//        GLfloat x, y, z;
//        int v0, v1, v2;
//        int vn0, vn1, vn2;
//        int vt0, vt1, vt2;
//        char slash;
//
//        // 统计斜杆数目，用不同格式读取
//        int slashCnt = 0;
//        for (int i = 0; i < line.length(); i++) {
//            if (line[i] == '/') slashCnt++;
//        }
//
//        // 读取obj文件
//        sin >> type;
//        if (type == "v") {
//            sin >> x >> y >> z;
//            vertices.push_back(vec3(x, y, z));
//            maxx = max(maxx, x); maxy = max(maxx, y); maxz = max(maxx, z);
//            minx = min(minx, x); miny = min(minx, y); minz = min(minx, z);
//        }
//        if (type == "f") {
//            if (slashCnt == 6) {
//                sin >> v0 >> slash >> vt0 >> slash >> vn0;
//                sin >> v1 >> slash >> vt1 >> slash >> vn1;
//                sin >> v2 >> slash >> vt2 >> slash >> vn2;
//            }
//            else if (slashCnt == 3) {
//                sin >> v0 >> slash >> vt0;
//                sin >> v1 >> slash >> vt1;
//                sin >> v2 >> slash >> vt2;
//            }
//            else {
//                sin >> v0 >> v1 >> v2;
//            }
//            indices.push_back(v0 - 1);
//            indices.push_back(v1 - 1);
//            indices.push_back(v2 - 1);
//        }
//    }
//
//    //// 模型大小归一化
//    //float lenx = maxx - minx;
//    //float leny = maxy - miny;
//    //float lenz = maxz - minz;
//    //float maxaxis = max(lenx, max(leny, lenz));
//    //for (auto& v : vertices) {
//    //    v.x /= maxaxis;
//    //    v.y /= maxaxis;
//    //    v.z /= maxaxis;
//    //}
//
//    // 通过矩阵进行坐标变换
//    for (auto& v : vertices) {
//        vec4 vv = vec4(v.x, v.y, v.z, 1);
//        vv = trans * vv;
//        v = vec3(vv.x, vv.y, vv.z);
//    }
//
//    // 生成法线
//    std::vector<vec3> normals(vertices.size(), vec3(0, 0, 0));
//    for (int i = 0; i < indices.size(); i += 3) {
//        vec3 p1 = vertices[indices[i]];
//        vec3 p2 = vertices[indices[i + 1]];
//        vec3 p3 = vertices[indices[i + 2]];
//        vec3 n = normalize(cross(p2 - p1, p3 - p1));
//        normals[indices[i]] += n;
//        normals[indices[i + 1]] += n;
//        normals[indices[i + 2]] += n;
//    }
//
//    // 构建 Triangle 对象数组
//    int offset = triangles.size();  // 增量更新
//    triangles.resize(offset + indices.size() / 3);
//    for (int i = 0; i < indices.size(); i += 3) {
//        Triangle& t = triangles[offset + i / 3];
//        // 传顶点属性
//        t.p1 = vertices[indices[i]];
//        t.p2 = vertices[indices[i + 1]];
//        t.p3 = vertices[indices[i + 2]];
//        if (!smoothNormal) {
//            vec3 n = normalize(cross(t.p2 - t.p1, t.p3 - t.p1));
//            t.n1 = n; t.n2 = n; t.n3 = n;
//        }
//        else {
//            t.n1 = normalize(normals[indices[i]]);
//            t.n2 = normalize(normals[indices[i + 1]]);
//            t.n3 = normalize(normals[indices[i + 2]]);
//        }
//
//        // 传材质
//        t.material = material;
//    }
//}
//
//mat4 getTransformMatrix(vec3 rotateCtrl, vec3 translateCtrl, vec3 scaleCtrl) 
//{
//    glm::mat4 unit(    // 单位矩阵
//        glm::vec4(1, 0, 0, 0),
//        glm::vec4(0, 1, 0, 0),
//        glm::vec4(0, 0, 1, 0),
//        glm::vec4(0, 0, 0, 1)
//    );
//    mat4 scale = glm::scale(unit, scaleCtrl);
//    mat4 translate = glm::translate(unit, translateCtrl);
//    mat4 rotate = unit;
//    rotate = glm::rotate(rotate, glm::radians(rotateCtrl.x), glm::vec3(1, 0, 0));
//    rotate = glm::rotate(rotate, glm::radians(rotateCtrl.y), glm::vec3(0, 1, 0));
//    rotate = glm::rotate(rotate, glm::radians(rotateCtrl.z), glm::vec3(0, 0, 1));
//
//    mat4 model = translate * rotate * scale;
//    return model;
//}
//
//
////// 获取着色器对象
////GLuint getShaderProgram(std::string fshader, std::string vshader) {
////    // 读取shader源文件
////    std::string vSource = readShaderFile(vshader);
////    std::string fSource = readShaderFile(fshader);
////    const char* vpointer = vSource.c_str();
////    const char* fpointer = fSource.c_str();
////
////    // 容错
////    GLint success;
////    GLchar infoLog[512];
////
////    // 创建并编译顶点着色器
////    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
////    glShaderSource(vertexShader, 1, (const GLchar**)(&vpointer), NULL);
////    glCompileShader(vertexShader);
////    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);   // 错误检测
////    if (!success)
////    {
////        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
////        std::cout << "顶点着色器编译错误\n" << infoLog << std::endl;
////        exit(-1);
////    }
////
////    // 创建并且编译片段着色器
////    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
////    glShaderSource(fragmentShader, 1, (const GLchar**)(&fpointer), NULL);
////    glCompileShader(fragmentShader);
////    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);   // 错误检测
////    if (!success)
////    {
////        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
////        std::cout << "片段着色器编译错误\n" << infoLog << std::endl;
////        exit(-1);
////    }
////
////    // 链接两个着色器到program对象
////    GLuint shaderProgram = glCreateProgram();
////    glAttachShader(shaderProgram, vertexShader);
////    glAttachShader(shaderProgram, fragmentShader);
////    glLinkProgram(shaderProgram);
////
////    // 删除着色器对象
////    glDeleteShader(vertexShader);
////    glDeleteShader(fragmentShader);
////
////    return shaderProgram;
////}
//
////    // 绘制
////    clock_t t1, t2;
////    double dt, fps;
////    unsigned int frameCounter = 0;
////    void display() {
////
////        // 帧计时
////        t2 = clock();
////        dt = (double)(t2 - t1) / CLOCKS_PER_SEC;
////        fps = 1.0 / dt;
////        std::cout << "\r";
////        std::cout << std::fixed << std::setprecision(2) << "FPS : " << fps << "    迭代次数: " << frameCounter;
////        t1 = t2;
////
////        // 相机参数
////        //vec3 eye = camera.Front;
////        //eye.x *= r; eye.y *= r; eye.z *= r;
////        //mat4 cameraRotate = lookAt(eye, vec3(0, 0, 0), vec3(0, 1, 0));  // 相机注视着原点
////        //cameraRotate = inverse(cameraRotate);   // lookat 的逆矩阵将光线方向进行转换
////
////        vec3 eye = camera.Front;
////        mat4 cameraRotate = camera.GetViewMatrix();
////        cameraRotate = inverse(cameraRotate);
////
////        // 传 uniform 给 pass1
////        glUseProgram(pass1.program);
////        glUniform3fv(glGetUniformLocation(pass1.program, "eye"), 1, value_ptr(eye));
////        glUniformMatrix4fv(glGetUniformLocation(pass1.program, "cameraRotate"), 1, GL_FALSE, value_ptr(cameraRotate));
////        glUniform1ui(glGetUniformLocation(pass1.program, "frameCounter"), frameCounter++);// 传计数器用作随机种子
////
////        glActiveTexture(GL_TEXTURE0);
////        glBindTexture(GL_TEXTURE_BUFFER, trianglesTextureBuffer);
////        glUniform1i(glGetUniformLocation(pass1.program, "triangles"), 0);
////
////        glActiveTexture(GL_TEXTURE1);
////        glBindTexture(GL_TEXTURE_BUFFER, nodesTextureBuffer);
////        glUniform1i(glGetUniformLocation(pass1.program, "nodes"), 1);
////
////        glActiveTexture(GL_TEXTURE2);
////        glBindTexture(GL_TEXTURE_2D, lastFrame);
////        glUniform1i(glGetUniformLocation(pass1.program, "lastFrame"), 2);
////
////        glActiveTexture(GL_TEXTURE3);
////        glBindTexture(GL_TEXTURE_2D, hdrMap);
////        glUniform1i(glGetUniformLocation(pass1.program, "hdrMap"), 3);
////
////        // 绘制
////        pass1.draw();
////        pass2.draw(pass1.colorAttachments);
////        pass3.draw(pass2.colorAttachments);
////
////        //glutSwapBuffers();
////    }
////
////    void draw(std::vector<unsigned int> texPassArray = {}) 
////    {
////        glUseProgram(program);
////        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
////        glBindVertexArray(vao);
////        // 传上一帧的帧缓冲颜色附件
////        for (int i = 0; i < texPassArray.size(); i++) {
////            glActiveTexture(GL_TEXTURE0 + i);
////            glBindTexture(GL_TEXTURE_2D, texPassArray[i]);
////            std::string uName = "texPass" + std::to_string(i);
////            glUniform1i(glGetUniformLocation(program, uName.c_str()), i);
////        }
////        glViewport(0, 0, width, height);
////        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
////        glDrawArrays(GL_TRIANGLES, 0, 6);
////
////        glBindVertexArray(0);
////        glBindFramebuffer(GL_FRAMEBUFFER, 0);
////        glUseProgram(0);
////    }
////};
//
////GLuint trianglesTextureBuffer;
////GLuint nodesTextureBuffer;
////GLuint lastFrame;
////GLuint hdrMap;
//
////使用pass
//RenderPass pass1;
//RenderPass pass2;
//RenderPass pass3;
//
//
////测试中心函数
//vec3 test(vec3 a, vec3 b, vec3 c)
//{
//    vec3 center;
//    center = (a + b + c) / vec3(3, 3, 3);
//    return center;
//}
//
////纹理函数
//GLuint getTextureRGB32F(int width, int height) {
//    GLuint tex;
//    glGenTextures(1, &tex);
//    glBindTexture(GL_TEXTURE_2D, tex);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    return tex;
//}
//
////键盘鼠标回调函数
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void processInput(GLFWwindow* window);
//
//glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
//
//// 创建相机类对象camera
//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//float lastX = SCR_WIDTH / 2.0f;
//float lastY = SCR_HEIGHT / 2.0f;
//bool firstMouse = true;
//
////// timing
//float deltaTime = 0.0f;
//float ClastFrame = 0.0f;
//
//int main()
//{
//    // glfw: initialize and configure
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//#ifdef __APPLE__
//    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif
//
//
//    // glfw window creation
//    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
//    if (window == NULL)
//    {
//        std::cout << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return -1;
//    }
//    glfwSwapInterval(1); // Enable vsync
//
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
//
//    // glad: load all OpenGL function pointers
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
//
//    //glEnable(GL_DEPTH_TEST);
//
//    // scene config
//    std::vector<Triangle> triangles;
//    vector<float> Trivertices;
//
//    Material m;
//    //m.baseColor = vec3(1, 1, 1);
//    ////Import("Stanford Bunny.obj", Trivertices, triangles, m, true);
//    //readObj("cubeup.obj", triangles, m, getTransformMatrix(vec3(0, 0, 0), vec3(0.3, -1.6, 0), vec3(1.5, 1.5, 1.5)),true);
//    //readObj("models/room.obj", triangles, m, getTransformMatrix(vec3(0, 0, 0), vec3(0.0, -2.5, 0), vec3(10, 10, 10)), true);
//
//    //m.baseColor = vec3(0.725, 0.71, 0.68);
//    //m.baseColor = vec3(0.725, 0.71, 0.68);
//    ////Import("quad.obj", Trivertices, triangles, m, false);
//    //readObj("quad.obj", triangles, m, getTransformMatrix(vec3(0, 0, 0), vec3(0, -1.4, 0), vec3(18.83, 0.01, 18.83)), false);
//    //
//    //m.baseColor = vec3(1, 1, 1);
//    //m.emissive = vec3(30, 20, 10);
//    ////readObj("models/quad.obj", triangles, m, getTransformMatrix(vec3(0, 0, 0), vec3(0.0, 1.38, -0.0), vec3(0.7, 0.01, 0.7)), false);
//    ////Import("sphere.obj", Trivertices, triangles, m, false);
//    //readObj("sphere.obj", triangles, m, getTransformMatrix(vec3(0, 0, 0), vec3(0.0, 0.9, -0.0), vec3(1, 1, 1)), false);
//
//    m.baseColor = vec3(0.77, 0.235, 0.01);
//    readObj("cubeleft.obj", triangles, m, getTransformMatrix(vec3(0, 0, 0), vec3(0, -0.4, 0), vec3(1, 1, 1)), false);
//    m.baseColor = vec3(0.14, 0.54, 0.168);
//    readObj("cuberight.obj", triangles, m, getTransformMatrix(vec3(0, 0, 0), vec3(0, -0.4, 0), vec3(1, 1, 1)), false);
//    m.baseColor = vec3(1, 1, 1);
//    readObj("cubeup.obj", triangles, m, getTransformMatrix(vec3(0, 0, 0), vec3(0, -0.4, 0), vec3(1, 1, 1)), false);
//    m.baseColor = vec3(1, 1, 1);
//    readObj("cubedown.obj", triangles, m, getTransformMatrix(vec3(0, 0, 0), vec3(0, -0.4, 0), vec3(1, 1, 1)), false);
//    m.baseColor = vec3(1, 1, 1);
//    readObj("cubeback.obj", triangles, m, getTransformMatrix(vec3(0, 0, 0), vec3(0, -0.4, 0), vec3(1, 1, 1)), false);
//
//    m.baseColor = vec3(1, 1, 1);
//    //m.metallic = -1.0;
//    readObj("ball.obj", triangles, m, getTransformMatrix(vec3(0, 0, 0), vec3(0.05, -0.4, 0.02), vec3(1, 1, 1)), false);
//    //m.metallic = 0.0;
//    //m.baseColor = vec3(1, 1, 1);
//    //m.metallic = -1.0;
//    ////m.roughness = 0.0;
//    //readObj("sphere.obj", triangles, m, getTransformMatrix(vec3(0, 0, 0), vec3(0, -0.47, 0.1), vec3(1.1, 1.1, 1.1)), true);
//    //m.metallic = 0.0;
//    //m.roughness = 0.0;
//
//    m.baseColor = vec3(0.196, 0.972, 1);
//    readObj("cubecolor.obj", triangles, m, getTransformMatrix(vec3(0, 0, 0), vec3(0.05, -0.4, 0.02), vec3(1, 1, 1)), false);
//    m.baseColor = vec3(1, 1, 1);
//    //m.roughness = 1.0f;
//
//    //m.roughness = 0.0;
//    readObj("Stanford Bunny.obj", triangles, m, getTransformMatrix(vec3(0, 27.0, 0), vec3(0.14, -0.5, 0.1), vec3(2.2, 2.2, 2.2)), true);
//
//    //m.roughness = 0.0;
//    m.baseColor = vec3(1, 1, 1);
//    m.emissive = vec3(30, 20, 10);
//    readObj("light.obj", triangles, m, getTransformMatrix(vec3(0, 0, 0), vec3(0, -0.4, 0), vec3(1, 1, 1)), false); 
//
//
//
//    int nTriangles = triangles.size();
//    std::cout << "模型读取完成: 共 " << nTriangles << " 个三角形" << std::endl;
//
//    // 建立 bvh
//    BVHNode testNode;
//    testNode.left = 255;
//    testNode.right = 128;
//    testNode.n = 30;
//    testNode.AA = vec3(1, 1, 0);
//    testNode.BB = vec3(0, 1, 0);
//    std::vector<BVHNode> nodes{ testNode };
//    //buildBVH(triangles, nodes, 0, triangles.size() - 1, 8);
//    buildBVH(triangles, nodes, 0, triangles.size() - 1, 8);
//    int nNodes = nodes.size();
//    std::cout << "BVH 建立完成: 共 " << nNodes << " 个节点" << std::endl;
//
//    // 编码 三角形, 材质
//    std::vector<Triangle_encoded> triangles_encoded(nTriangles);
//    for (int i = 0; i < nTriangles; i++) {
//        Triangle& t = triangles[i];
//        Material& m = t.material;
//        // 顶点位置
//        triangles_encoded[i].p1 = t.p1;
//        triangles_encoded[i].p2 = t.p2;
//        triangles_encoded[i].p3 = t.p3;
//        // 顶点法线
//        triangles_encoded[i].n1 = t.n1;
//        triangles_encoded[i].n2 = t.n2;
//        triangles_encoded[i].n3 = t.n3;
//        // 材质
//        triangles_encoded[i].emissive = m.emissive;
//        triangles_encoded[i].baseColor = m.baseColor;
//        triangles_encoded[i].param1 = vec3(m.subsurface, m.metallic, m.specular);
//        triangles_encoded[i].param2 = vec3(m.specularTint, m.roughness, m.anisotropic);
//        triangles_encoded[i].param3 = vec3(m.sheen, m.sheenTint, m.clearcoat);
//        triangles_encoded[i].param4 = vec3(m.clearcoatGloss, m.IOR, m.transmission);
//    }
//
//    // 编码 BVHNode, aabb
//    std::vector<BVHNode_encoded> nodes_encoded(nNodes);
//    for (int i = 0; i < nNodes; i++) {
//        nodes_encoded[i].childs = vec3(nodes[i].left, nodes[i].right, 0);
//        nodes_encoded[i].leafInfo = vec3(nodes[i].n, nodes[i].index, 0);
//        nodes_encoded[i].AA = nodes[i].AA;
//        nodes_encoded[i].BB = nodes[i].BB;
//    }
//
//
//
//
//
//    // ----------------------------------------------------------------------------- //
//
//    // 生成纹理
//
//    // 三角形数组
//    GLuint tbo0;
//    glGenBuffers(1, &tbo0);
//    glBindBuffer(GL_TEXTURE_BUFFER, tbo0);
//    glBufferData(GL_TEXTURE_BUFFER, triangles_encoded.size() * sizeof(Triangle_encoded), &triangles_encoded[0], GL_STATIC_DRAW);
//    glGenTextures(1, &trianglesTextureBuffer);
//    glBindTexture(GL_TEXTURE_BUFFER, trianglesTextureBuffer);
//    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, tbo0);
//
//    // BVHNode 数组
//    GLuint tbo1;
//    glGenBuffers(1, &tbo1);
//    glBindBuffer(GL_TEXTURE_BUFFER, tbo1);
//    glBufferData(GL_TEXTURE_BUFFER, nodes_encoded.size() * sizeof(BVHNode_encoded), &nodes_encoded[0], GL_STATIC_DRAW);
//    glGenTextures(1, &nodesTextureBuffer);
//    glBindTexture(GL_TEXTURE_BUFFER, nodesTextureBuffer);
//    glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, tbo1);
//
//
//
//
//
//    // hdr 全景图
//    HDRLoaderResult hdrRes;
//    bool r = HDRLoader::load("11circus_arena_4k.hdr", hdrRes);
//    hdrMap = getTextureRGB32F(hdrRes.width, hdrRes.height);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, hdrRes.width, hdrRes.height, 0, GL_RGB, GL_FLOAT, hdrRes.cols);
//
//    // ----------------------------------------------------------------------------- //
//
//    // 管线配置
//    Shader Pass1Shader("./content/shader/pass1VS.glsl", "./content/shader/pass1FS.glsl");
//    pass1.program = Pass1Shader.ID;
//    //pass1.width = pass1.height = 256;
//    pass1.colorAttachments.push_back(getTextureRGB32F(pass1.width, pass1.height));
//    //pass1.colorAttachments.push_back(getTextureRGB32F(pass1.width, pass1.height));
//    //pass1.colorAttachments.push_back(getTextureRGB32F(pass1.width, pass1.height));
//    pass1.bindData();
//
//    glUseProgram(pass1.program);
//    glUniform1i(glGetUniformLocation(pass1.program, "nTriangles"), triangles.size());
//    glUniform1i(glGetUniformLocation(pass1.program, "nNodes"), nodes.size());
//    glUniform1i(glGetUniformLocation(pass1.program, "width"), pass1.width);
//    glUniform1i(glGetUniformLocation(pass1.program, "height"), pass1.height);
//    glUseProgram(0);
//
//    Shader Pass2Shader("./content/shader/pass1VS.glsl", "pass2.fsh");
//    pass2.program = Pass2Shader.ID;
//    lastFrame = getTextureRGB32F(pass2.width, pass2.height);
//    pass2.colorAttachments.push_back(lastFrame);
//    pass2.bindData();
//
//    Shader Pass3Shader("./content/shader/pass1VS.glsl", "pass3.fsh");
//    pass3.program = Pass3Shader.ID;
//    pass3.bindData(true);
//    // ----------------------------------------------------------------------------- //
//
//    //std::cout << "开始..." << std::endl << std::endl;
//
//    //clock_t t1, t2;
//    //double dt, fps;
//    //unsigned int frameCounter = 0;
//    // render loop
//    while (!glfwWindowShouldClose(window))
//    {
//        // input
//        // -----
//        processInput(window);
//
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        float currentFrame = static_cast<float>(glfwGetTime());
//        deltaTime = currentFrame - lastFrame;
//        ClastFrame = currentFrame;
//
//        ImGui_ImplOpenGL3_NewFrame();
//        ImGui_ImplGlfw_NewFrame();
//        ImGui::NewFrame();
//        ImGui::Begin("Element");
//        ImGui::Text("Samples = %d", frameCounter);
//        ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
// 
//        ImGui::End();
//
//        // 绘制
//
//        //// 帧计时
//        //t2 = clock();
//        //dt = (double)(t2 - t1) / CLOCKS_PER_SEC);
//        //fps = 1.0 / dt;
//        //std::cout << "\r";
//        //std::cout << std::fixed << std::setprecision(2) << "FPS : " << fps << "    迭代次数: " << frameCounter;
//        //t1 = t2;
//        // 相机参数
//        
//        //vec3 eye = camera.Front;
//        vec3 eye = camera.Front;
//        eye.x *= camera.Zoom; eye.y *= camera.Zoom; eye.z *= camera.Zoom;
//        mat4 cameraRotate = lookAt(eye, vec3(0, 0, 0), vec3(0, 1, 0));
//        //mat4 cameraRotate = camera.GetViewMatrix();
//        cameraRotate = inverse(cameraRotate);   // lookat 的逆矩阵将光线方向进行转换
//        // 传 uniform 给 pass1
//        glUseProgram(pass1.program);
//        glUniform3fv(glGetUniformLocation(pass1.program, "eye"), 1, value_ptr(eye));
//        glUniformMatrix4fv(glGetUniformLocation(pass1.program, "cameraRotate"), 1, GL_FALSE, value_ptr(cameraRotate));
//        glUniform1ui(glGetUniformLocation(pass1.program, "frameCounter"), frameCounter++);// 传计数器用作随机种子
//
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_BUFFER, trianglesTextureBuffer);
//        glUniform1i(glGetUniformLocation(pass1.program, "triangles"), 0);
//
//        glActiveTexture(GL_TEXTURE1);
//        glBindTexture(GL_TEXTURE_BUFFER, nodesTextureBuffer);
//        glUniform1i(glGetUniformLocation(pass1.program, "nodes"), 1);
//
//        glActiveTexture(GL_TEXTURE2);
//        glBindTexture(GL_TEXTURE_2D, lastFrame);
//        glUniform1i(glGetUniformLocation(pass1.program, "lastFrame"), 2);
//
//        glActiveTexture(GL_TEXTURE3);
//        glBindTexture(GL_TEXTURE_2D, hdrMap);
//        glUniform1i(glGetUniformLocation(pass1.program, "hdrMap"), 3);
//
//        // 绘制
//
//        //Bunny.Draw(Pass1Shader);
//        //    unsigned int VBO, cubeVAO;
////    glGenVertexArrays(1, &cubeVAO);
////    glGenBuffers(1, &VBO);
////
////    glBindBuffer(GL_ARRAY_BUFFER, VBO);
////    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
////    // position attribute
////    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
////    glEnableVertexAttribArray(0);
//        pass1.draw();
//        pass2.draw(pass1.colorAttachments);
//        pass3.draw(pass2.colorAttachments);
//
//        //提交imgui渲染
//        ImGui::Render();
//        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//
//    // glfw: terminate, clearing all previously allocated GLFW resources.
//    glfwTerminate();
//    return 0;
//}
//
//
//// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
//// ---------------------------------------------------------------------------------------------------------
//void processInput(GLFWwindow* window)
//{
//    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//    {
//        camera.ProcessKeyboard(FORWARD, deltaTime);
//        frameCounter = 0;
//    }
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//    {
//        camera.ProcessKeyboard(BACKWARD, deltaTime);
//        frameCounter = 0;
//    }
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//    {
//        camera.ProcessKeyboard(LEFT, deltaTime);
//        frameCounter = 0;
//    }
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//    {
//        camera.ProcessKeyboard(RIGHT, deltaTime);
//        frameCounter = 0;
//    }
//    //frameCounter = 0;
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
//        frameCounter = 0;
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
//        //frameCounter = 0;
//    }
//}
//
//// glfw: whenever the mouse scroll wheel scrolls, this callback is called
//// ----------------------------------------------------------------------
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//    camera.ProcessMouseScroll(static_cast<float>(yoffset));
//    frameCounter = 0;
//}

