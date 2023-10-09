﻿#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "Model.h"
#include "Loader.h"
#include "Render.h"
// #include "ObjLoader.h"
#include "OBJFileLoader.h"
#include "Texture.h"
#include "TexturedModel.h"
#include "stb_image.h"
#include "MasterRender.h"
#include "TerrainTexturePack.h"
#include <vector>
#include <iostream>

#define random(a, b) (rand() % (b - a + 1) + a)

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void scrollFunc(GLFWwindow* window, double offsetx, double offsety);

//窗口设置
const unsigned int SCR_WIDTH = 1080;
const unsigned int SCR_HEIGHT = 720;
const unsigned int FPS_CAP = 120;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//Camera初始位置
double fov = 20.0;

int main()
{
    //glfw初始化和配置
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //glfw创建窗口
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Silent Forest", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //glad加载所有openGL函数指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    
    //Render
    Loader loader;
    //Load Texture of Terrain
    TerrainTexture backgroundTexture = TerrainTexture(loader.loadTexture("../texture/grass.png"));
    TerrainTexture rTexture = TerrainTexture(loader.loadTexture("../texture/mud.png"));
    TerrainTexture gTexture = TerrainTexture(loader.loadTexture("../texture/mud.png"));
    TerrainTexture bTexture = TerrainTexture(loader.loadTexture("../texture/gound.png"));

    TerrainTexturePack texturePack(backgroundTexture, rTexture, gTexture, bTexture);
    TerrainTexture blendMap = TerrainTexture(loader.loadTexture("../texture/blendMap.png"));

    vector<Entity> entities;
    Terrain terrain1(1, 1, loader, texturePack, blendMap, "../texture/heightmap.png");
    
    //地面随即加草
    ObjLoader fernloader = ObjLoader();
    ModelData ferndata = fernloader.loadObj("../object/fern.obj");
    Model fernmodel = loader.LoadToV(ferndata.vertices, 3*ferndata.numOfVertices, ferndata.textureCoords, 2*ferndata.numOfVertices, ferndata.indices, ferndata.numOfIndices, ferndata.normals, 3*ferndata.numOfVertices);
    Texture fernTextureAtlas = Texture(loader.loadTexture("../texture/fern.png"));
    fernTextureAtlas.numberOfRows = 2;
    TexturedModel fern = TexturedModel(fernmodel, fernTextureAtlas);
    fern.texture.setHasTransparency(1);
    fern.texture.setUseFakeLighting(1);

    //地面的树
    ObjLoader treeLoader = ObjLoader();
    ModelData treeData = treeLoader.loadObj("../object/pine.obj");
    Model treeModel = loader.LoadToV(treeData.vertices, 3*treeData.numOfVertices, treeData.textureCoords, 2*treeData.numOfVertices, treeData.indices, treeData.numOfIndices, treeData.normals, 3*treeData.numOfVertices);
    Texture treeTextureAtlas = Texture(loader.loadTexture("../texture/pine.png"));
    TexturedModel tree = TexturedModel(treeModel, treeTextureAtlas);
    tree.texture.setHasTransparency(1);
    tree.texture.setUseFakeLighting(1);

    vector<glm::vec3> treePos;

    for(int i = 0; i < 1600; i++){
        if(i % 2 == 0){
            float x = random(0, 800);
            float z = random(0, 800);
            float y = terrain1.getHeightOfTerrain(x, z);
            entities.push_back(Entity(fern, random(1, 4), glm::vec3(x, y, z), 0, 0, 0, 0.3f));
        }
        if(i % 3 == 0){
            float x = random(0, 800);
            float z = random(0, 800);
            float y = terrain1.getHeightOfTerrain(x, z);
            entities.push_back(Entity(tree, 1, glm::vec3(x, y, z), 0, 0, 0, 1.0f));
            treePos.push_back(glm::vec3(x, y, z));
        }
    }

    Light light = Light(glm::vec3(1000,1000,-1000), glm::vec3(1.0,1.0,1.0), glm::vec3(1.8,0,0), glm::vec3(1,0,0), 10000.0);
    vector<Light> lights;
    lights.push_back(light);
    
    // Terrain terrain2(1, 0, loader, texturePack, blendMap, "texture/heightmap.png");

    // Player try
    ObjLoader objloader2 = ObjLoader();
    ModelData pdata = objloader2.loadObj("../object/car.obj");
    Model Pmodel = loader.LoadToV(pdata.vertices, 3*pdata.numOfVertices, pdata.textureCoords, 2*pdata.numOfVertices, pdata.indices, pdata.numOfIndices, pdata.normals, 3*pdata.numOfVertices);
    Texture Prawtexture=Texture(loader.loadTexture("../texture/Car.png"));
    TexturedModel Ptexturedmodel=TexturedModel(Pmodel, Prawtexture);
    //TexturedModel Ptexturedmodel=TexturedModel(Pmodel, Texture()/*Prawtexture*/);
    Ptexturedmodel.texture.setHasTransparency(0);
    Ptexturedmodel.texture.setUseFakeLighting(0);
    Texture Ptexture = Ptexturedmodel.texture;
    Ptexture.shineDamper = 10;
    Ptexture.reflectivity = 1;
    Player player = Player(Ptexturedmodel, 1, glm::vec3(400, 0, 400), 0, 0, 0, 1.5);
    player.addWindow(window);
    Light carLight = Light(glm::vec3(400, 0, 400), glm::vec3(1,1,1), glm::vec3(1, 0.1, 0.02), glm::vec3(1,0,0), -0.9);
    player.setLight(&carLight);
    player.setCollideObject(treePos);
    Camera camera= Camera(window, &player);
    glfwSetScrollCallback(window, scrollFunc);

    lights.push_back(carLight);

    // render loop
    MasterRender renderer = MasterRender(loader, camera);
    renderer.prepareShadowFBO();
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        player.move(deltaTime, terrain1);
        lights[1]=*player.light;
        // cout<<lights[0].cutOff<<endl;
        camera.distanceFromPlayer = fov;
        camera.move(terrain1);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer.processEntity(player);
        // renderer.processTerrain(terrain2);
        for(int i=0;i<entities.size(); i++){
            renderer.processEntity(entities[i]);
        }
        renderer.renderShadowMap(lights[0]);

        terrain1.bindShadowMap(renderer.depthMap);
        renderer.processEntity(player);
        renderer.processTerrain(terrain1);
        // renderer.processTerrain(terrain2);
        for(int i=0;i<entities.size(); i++){
            renderer.processEntity(entities[i]);
        }
        // lights[0].pos.x = player.position.x;
        // lights[0].pos.z = player.position.z;
        // lights[0].pos.y = 2;
        // cout<<lights[1].pos.x<<" "<<lights[1].pos.z<<endl;
        renderer.render(lights,camera);
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    renderer.Clear();
    loader.Clean();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

float getCurrentTime() {
    return glfwGetTime();
}

void scrollFunc(GLFWwindow* window, double xoffset, double yoffset){
    if (fov >= 1.0f && fov <= 90.0f) {
        fov -= yoffset;
    }

    fov = fov <= 1.0f ? 1.0f : fov;
    fov = fov >= 90.0f ? 90.0f : fov;
}