#include "lab_m1/Tema3/Tema3.h"
#include "lab_m1/lab4/transform3D.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema3::Tema3()
{
}


Tema3::~Tema3()
{
}


void Tema3::Init()
{
    const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "textures");

    // Load textures
    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "snow.jpg").c_str(), GL_REPEAT);
        mapTextures["snow"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "crate.jpg").c_str(), GL_REPEAT);
        mapTextures["crate"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "pine.jpg").c_str(), GL_REPEAT);
        mapTextures["pine"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "wood.png").c_str(), GL_REPEAT);
        mapTextures["wood"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "gift.jpg").c_str(), GL_REPEAT);
        mapTextures["gift"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "marble.jpg").c_str(), GL_REPEAT);
        mapTextures["marble"] = texture;
    }

    {
        Texture2D* texture = new Texture2D();
        texture->Load2D(PATH_JOIN(sourceTextureDir, "rock.jpg").c_str(), GL_REPEAT);
        mapTextures["rock"] = texture;
    }





    {
        mapTextures["random"] = CreateRandomTexture(25, 25);
    }

    // Load meshes
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("cone");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "cone"), "cone.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    

    // Create a simple quad
    {
        vector<glm::vec3> vertices
        {
            glm::vec3(-5.0f,   0.0f, 5.0f),    // top right
            glm::vec3(5.0f,  0.0f, 5.0f),    // bottom right
            glm::vec3(5.0f, 0.0f, -5.0f),    // bottom left
            glm::vec3(-5.0f,  0.0f, -5.0f),    // top left
        };

        vector<glm::vec3> normals
        {
            glm::vec3(0, 1, 1),
            glm::vec3(1, 0, 1),
            glm::vec3(1, 0, 0),
            glm::vec3(0, 1, 0)
        };

        vector<glm::vec2> textureCoords
        {
            // TODO(student): Complete texture coordinates for the square

            glm::vec2(0.0f, 0.0f),
            glm::vec2(0.0f, 1.0f),
            glm::vec2(1.0f, 1.0f),
            glm::vec2(1.0f, 0.0f)

        };

        vector<unsigned int> indices =
        {
            0, 1, 3,
            1, 2, 3
        };

        Mesh* mesh = new Mesh("square");
        mesh->InitFromData(vertices, normals, textureCoords, indices);
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader* shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("LabShader2");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "VertexShader2.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FragmentShader2.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Light & material properties
    {
        
        lightDirection = glm::vec3(0, -1, 0);
        
        materialShininess = 30;
        materialKd = 0.5;
        materialKs = 0.5;
        spotlight = 0;
    }

    {
        count = 1;
        point_light_pos[0] = glm::vec3(0, 0.3f, -2.3f);
        point_light_color[0] = glm::vec3(1, 1, 0.2);
        for (int i = -200; i < 200; i+= 15) {
            float y = rand() % 400;
            float x = i;
            coords.push_back(glm::vec2(x * 0.1f, (y - 100.0f) * 0.1f));
            point_light_pos[count] = glm::vec3(x * 0.1f, 1.0f, (y - 100.0f) * 0.1f);
            
            object[count] = rand() % 4;
            if (object[count] == 0) {
                point_light_color[count] = glm::vec3(0.4, 1, 0.3);
                
            }
            else if (object[count] == 1) {
                point_light_color[count] = glm::vec3(1, 0.8, 1);
            } else if (object[count] == 2) {
                point_light_color[count] = glm::vec3(0, 0.3, 1);
            } else {
                point_light_color[count] = glm::vec3(1, 0.3, 1);
            }
            
            count++;
        }
        



        float x = point_light_pos[0].x;
        float y = point_light_pos[0].y;
        float z = point_light_pos[0].z;
        float y2 = y * cos(RADIANS(40.0f)) - z * sin(RADIANS(20.0f));
        float z2 = y * sin(RADIANS(40.0f)) + z * cos(RADIANS(20.0f));
        point_light_pos[0] = glm::vec3(x, y2, z2);
        

    }
    Earth = 0;
    boxX = 0;
    boxZ = -2.0f;
    TimeX = 0;
    gifts = 0;
    alive = 1;
    
    
}


void Tema3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.4, 0.7, 1, 0.5);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema3::Update(float deltaTimeSeconds)
{
    // TODO(student): Choose an object and add a second texture to it.
    // For example, for the sphere, you can have the "earth" texture
    // and the "random" texture, and you will use the `mix` function
    // in the fragment shader to mix these two textures.
    //
    // However, you may have the unpleasant surprise that the "random"
    // texture now appears onto all objects in the scene, even though
    // you are only passing the second texture for a single object!
    // Why does this happen? How can you solve it?

    if (alive == 1) {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(boxX, 1.0f, boxZ));
        
        float angle = 0;
        glm::ivec2 resolution = window->GetResolution();
        modelMatrix = glm::rotate(modelMatrix, RADIANS(22.0f), glm::vec3(1, 0, 0));
        
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));

        if ((myMouseY < resolution.y / 2)  && (resolution.x/2 - myMouseX != 0)) {
            angle = atan((resolution.y / 2 -0.5 - myMouseY) / (resolution.x / 2 - myMouseX));
            modelMatrix = glm::rotate(modelMatrix, angle + RADIANS(90), glm::vec3(0, 1, 0));
        }
        
        if (myMouseX > resolution.x / 2) {
            TimeX += 0.05 * -cos(angle);
        }
        else if (myMouseX < resolution.x / 2) {
            TimeX += 0.05 * cos(angle);
        }

        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["crate"], mapTextures["crate"], glm::vec3(0.25, 0.75, 0.75));

        modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.2, -0.2, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2, 0, 3));
        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["wood"], mapTextures["wood"], glm::vec3(0.25, 0.75, 0.75));
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(boxX, 1.0f, boxZ));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(22.0f), glm::vec3(1, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
        if ((myMouseY < resolution.y / 2) && (resolution.x / 2 - myMouseX != 0)) {
            angle = atan((resolution.y / 2 -0.5 - myMouseY) / (resolution.x / 2 - myMouseX));
            modelMatrix = glm::rotate(modelMatrix, angle + RADIANS(90), glm::vec3(0, 1, 0));
        }
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0.2, -0.2, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2, 0, 3));

        RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["wood"], mapTextures["wood"], glm::vec3(0.25, 0.75, 0.75));

        
    


    
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::rotate(modelMatrix, RADIANS(22.0f), glm::vec3(1, 0, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(4.0f));
        //modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.5f, 0.0f));
        
        Earth = 1;
        RenderSimpleMesh(meshes["square"], shaders["LabShader"], modelMatrix, mapTextures["snow"], mapTextures["snow"], glm::vec3(0.25, 0.75, 0.75));
        Earth = 0;
    
        for (int i = 0; i < 27; i++) {
            
            modelMatrix = glm::mat4(1);
            float scaleFact;
            float translateFact;
            int random = object[i + 1];
            if (random == 0) {
                scaleFact = 0.75;
                translateFact = 0.8f;
            }
            else {
                scaleFact = 0.3;
                translateFact = 0.2f;
            }
            coords[i].y -= deltaTimeSeconds * 1.5 *(1- cos(angle));
            if (myMouseX > resolution.x / 2) {
                coords[i].x -= cos(angle) * 0.05;
            }
            else if (myMouseX < resolution.x / 2) {
                coords[i].x += cos(angle) * 0.05;
            }
            if (random == 2) {

            }
            if (coords[i].y < -16) {
                coords[i].y += 22;
                object[i+1] = rand() % 4;
                if (object[i+1] == 0) {
                    point_light_color[i+1] = glm::vec3(0.4, 1, 0.3);
                }
                else if(object[i+1] == 1) {
                    point_light_color[i+1] = glm::vec3(1, 0.8, 1);
                }
                else  if (object[i + 1] == 2) {
                    point_light_color[i+1] = glm::vec3(0, 0.3, 1);
                }
                else {
                    point_light_color[i + 1] = glm::vec3(1, 0.3, 1);
                }
                
            }
            if (random == 1) {
                float posX = point_light_pos[0].x;
                float posZ = point_light_pos[0].z;
                if ((-0.5 <= coords[i].x)&& (coords[i].x <= 0.5) && (-2.5 <= coords[i].y)&& (coords[i].y <= -1)) {
                    coords[i].y = -16;
                    gifts++;
                }

                
            } else if ((-0.5 <= coords[i].x) && (coords[i].x <= 0.5) && (-2.5 <= coords[i].y) && (coords[i].y <= -1)) {
                cout << endl << "Scorul tau este:" << gifts << endl;
                alive = 0;

            }


            float x = coords[i].x;
            float y = 1.0f;
            float z = coords[i].y;

            float y2 = y * cos(RADIANS(22.0f)) - z * sin(RADIANS(22.0f));
            float z2 = y * sin(RADIANS(22.0f)) + z * cos(RADIANS(22.0f));
            point_light_pos[i + 1] = glm::vec3(x, y2, z2);
            modelMatrix *= transform3D::RotateOX(RADIANS(22.0f));
            

            
            if (random == 0) {
                
                modelMatrix *= transform3D::Translate(coords[i].x, translateFact + 0.4f, coords[i].y);
                modelMatrix *= transform3D::Scale(scaleFact, scaleFact, scaleFact);
                RenderSimpleMesh(meshes["cone"], shaders["LabShader"], modelMatrix, mapTextures["pine"], mapTextures["pine"], glm::vec3(0.25, 0.75, 0.75));
                modelMatrix *= transform3D::Translate(0, translateFact - 2.4f , 0);
                modelMatrix *= transform3D::Scale(0.5, 1, 0.5);
                RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["wood"], mapTextures["wood"], glm::vec3(0.25, 0.75, 0.75));
            }
            else if (random == 1) {
                modelMatrix *= transform3D::Translate(coords[i].x, translateFact, coords[i].y);
                modelMatrix *= transform3D::Scale(scaleFact, scaleFact, scaleFact);
                RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["gift"], mapTextures["gift"], glm::vec3(0.25, 0.75, 0.75));
            }
            else if (random == 2) {
                point_light_pos[i + 1] = glm::vec3(x + 0.4f, y2, z2);
                modelMatrix *= transform3D::Translate(coords[i].x, translateFact, coords[i].y);
                modelMatrix *= transform3D::Scale(scaleFact, scaleFact + 1.5f, scaleFact);
                RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["marble"], mapTextures["marble"], glm::vec3(0.25, 0.75, 0.75));
                modelMatrix *= transform3D::Translate(0, 0.5f, 0);
                modelMatrix *= transform3D::Scale(3, 0.2, 1);
                RenderSimpleMesh(meshes["box"], shaders["LabShader"], modelMatrix, mapTextures["marble"], mapTextures["marble"], glm::vec3(0.25, 0.75, 0.75));

            }
            else {
                modelMatrix *= transform3D::Translate(coords[i].x, translateFact, coords[i].y);
                modelMatrix *= transform3D::Scale(scaleFact, scaleFact, scaleFact);
                RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, mapTextures["rock"], mapTextures["rock"], glm::vec3(0.25, 0.75, 0.75));
                modelMatrix *= transform3D::Translate(0.5f, 0, 0);
                modelMatrix *= transform3D::Scale(1, 1.2, 1);
                RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], modelMatrix, mapTextures["rock"], mapTextures["rock"], glm::vec3(0.25, 0.75, 0.75));
            }
        }

        
        
    }


    
}


void Tema3::FrameEnd()
{
    DrawCoordinateSystem();
}


void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1, Texture2D* texture2, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set shader uniforms for light properties
    GLuint light_position = glGetUniformLocation(shader->program, "light_position");
    glUniform3fv(light_position, 28, glm::value_ptr(point_light_pos[0]));

    GLuint light_color = glGetUniformLocation(shader->program, "light_color");
    glUniform3fv(light_color, 28, glm::value_ptr(point_light_color[0]));

    


    int light_direction = glGetUniformLocation(shader->program, "light_direction");
    glUniform3f(light_direction, lightDirection.x, lightDirection.y, lightDirection.z);



    // Set eye position (camera position) uniform
    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    // Set material property uniforms (shininess, kd, ks, object color) 
    int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess, materialShininess);

    int spotlight1 = glGetUniformLocation(shader->program, "spotlight");
    glUniform1i(spotlight1, spotlight);

    int material_kd = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd, materialKd);

    int material_ks = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks, materialKs);

    int object_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3f(object_color, color.r, color.g, color.b);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // TODO(student): Set any other shader uniforms that you need

    if (texture1)
    {
        // TODO(student): Do these:
        // - activate texture location 0
        // - bind the texture1 ID
        // - send theuniform value
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);


    }

    if (texture2)
    {
        // TODO(student): Do these:
        // - activate texture location 1
        // - bind the texture2 ID
        // - send the uniform value

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);

    }

    location = glGetUniformLocation(shader->program, "Time");

    glUniform1f(location, Engine::GetElapsedTime());

    int location2 = glGetUniformLocation(shader->program, "Earth");
    glUniform1i(location2, Earth);

    location2 = glGetUniformLocation(shader->program, "Time2");
    glUniform1f(location2, TimeX);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


Texture2D* Tema3::CreateRandomTexture(unsigned int width, unsigned int height)
{
    GLuint textureID = 0;
    unsigned int channels = 3;
    unsigned int size = width * height * channels;
    unsigned char* data = new unsigned char[size];

    // TODO(student): Generate random texture data
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    for (int i = 0; i < size; i++) {
        data[i] = rand() % 256;
    }

    // TODO(student): Generate and bind the new texture ID


    if (GLEW_EXT_texture_filter_anisotropic) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
    }
    // TODO(student): Set the texture parameters (MIN_FILTER, MAG_FILTER and WRAPPING MODE) using glTexParameteri

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);




    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    CheckOpenGLError();

    // Use glTexImage2D to set the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    // TODO(student): Generate texture mip-maps
    glGenerateMipmap(GL_TEXTURE_2D);

    CheckOpenGLError();

    // Save the texture into a wrapper Texture2D class for using easier later during rendering phase
    Texture2D* texture = new Texture2D();
    texture->Init(textureID, width, height, channels);

    SAFE_FREE_ARRAY(data);
    return texture;
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema3::OnInputUpdate(float deltaTime, int mods)
{
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 right = GetSceneCamera()->m_transform->GetLocalOXVector();
        glm::vec3 forward = GetSceneCamera()->m_transform->GetLocalOZVector();
        forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));
    }
}


void Tema3::OnKeyPress(int key, int mods)
{
    if ((key == GLFW_KEY_R) && (alive == 0)) {
        alive = 1;
        gifts = 0;

        count = 1;
        for (int i = -200; i < 200; i += 15) {
            float y = rand() % 400;
            float x = i;
            coords.push_back(glm::vec2(x * 0.1f, (y - 100.0f) * 0.1f));
            point_light_pos[count] = glm::vec3(x * 0.1f, 1.0f, (y - 100.0f) * 0.1f);

            object[count] = rand() % 4;
            if (object[count] == 0) {
                point_light_color[count] = glm::vec3(0.4, 1, 0.3);

            }
            else if (object[count] == 1) {
                point_light_color[count] = glm::vec3(1, 0.8, 1);
            }
            else if (object[count] == 2) {
                point_light_color[count] = glm::vec3(0, 0.3, 1);
            }
            else {
                point_light_color[count] = glm::vec3(1, 0.3, 1);
            }

            count++;
        }
    }
    // Add key press event
}


void Tema3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    glm::ivec2 resolution = window->GetResolution();
    myMouseX = mouseX;
    myMouseY = resolution.y - mouseY;
    // Add mouse move event
}


void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema3::OnWindowResize(int width, int height)
{
}
