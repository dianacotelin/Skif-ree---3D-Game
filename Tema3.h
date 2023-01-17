#pragma once

#include <string>
#include <unordered_map>

#include "components/simple_scene.h"
#include "components/transform.h"


namespace m1
{
    class Tema3 : public gfxc::SimpleScene
    {
    public:
        Tema3();
        ~Tema3();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1 = NULL, Texture2D* texture2 = NULL, const glm::vec3& color = glm::vec3(1));
        Texture2D* CreateRandomTexture(unsigned int width, unsigned int height);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        std::unordered_map<std::string, Texture2D*> mapTextures;
        int location;
        int Earth = 0;
        int myMouseX;
        int myMouseY;
        int boxX;
        int boxZ;
        std::vector <glm::vec2> coords;

        unsigned int materialShininess;
        float materialKd;
        float materialKs;
        float spotlight = 0;
        int count;

        glm::vec3 lightPosition;
        glm::vec3 lightDirection;

        glm::vec3 point_light_pos[28];
        glm::vec3 point_light_color[28];
        glm::vec3 spotlight_pos[28];
        int object[28];
        float TimeX;
        int gifts;
        int alive;
    };
}   // namespace m1
