#include <../includes/glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.hpp"

#include <cmath>
#include <iostream>
#include <vector>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);

// Impostazioni
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Telecamera
glm::vec3 cameraPos = glm::vec3(
    0.0f, 15.0f, 15.0f); // Vista isometrica dall'alto (vista ravvicinata)
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// Temporizzazione
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Giocatore
glm::vec3 playerPos =
    glm::vec3(0.0f, 0.5f, 0.0f); // Y=0.5 perché il cubo è 1x1x1, centro a 0.5
float playerSpeed = 5.0f;

// Server
struct Server {
  glm::vec3 position;
  bool isBroken;
  float animationTime; // Per l'animazione di rottura
};

std::vector<Server> servers;

// Interazione
bool spacePressed = false;
const float INTERACTION_DISTANCE = 2.0f;

// Stati del Gioco
enum GameState { MENU, GAME };
GameState currentState = MENU;

// Input Mouse (per evitare click multipli)
bool mousePressed = false;

int main() {
  // glfw: inizializza e configura
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // creazione della finestra glfw
  // --------------------
  GLFWwindow *window =
      glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "SysAdmin Game", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetWindowAspectRatio(window, SCR_WIDTH, SCR_HEIGHT);

  // glad: carica tutti i puntatori alle funzioni OpenGL
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // costruisci e compila il nostro programma shader
  // ------------------------------------
  Shader ourShader("shaders/vertex_shader.glsl",
                   "shaders/fragment_shader.glsl");

  // imposta i dati dei vertici (e i buffer) e configura gli attributi dei
  // vertici
  // ------------------------------------------------------------------
  float vertices[] = {
      // posizioni          // coordinate texture
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

  // Vertici del pavimento (grande piano)
  float floorVertices[] = {
      // posizioni          // coordinate texture
      10.0f, -0.5f, 10.0f,  10.0f, 0.0f,   -10.0f, -0.5f, 10.0f,
      0.0f,  0.0f,  -10.0f, -0.5f, -10.0f, 0.0f,   10.0f,

      10.0f, -0.5f, 10.0f,  10.0f, 0.0f,   -10.0f, -0.5f, -10.0f,
      0.0f,  10.0f, 10.0f,  -0.5f, -10.0f, 10.0f,  10.0f};

  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // attributo posizione
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // attributo coordinate texture
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // VAO/VBO del pavimento
  unsigned int floorVBO, floorVAO;
  glGenVertexArrays(1, &floorVAO);
  glGenBuffers(1, &floorVBO);
  glBindVertexArray(floorVAO);
  glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Quad VAO/VBO per UI (Menu)
  float quadVertices[] = {// posizioni        // texture Coords
                          -0.5f, 0.5f, 0.0f, 0.0f,  1.0f, -0.5f, -0.5f, 0.0f,
                          0.0f,  0.0f, 0.5f, 0.5f,  0.0f, 1.0f,  1.0f,

                          0.5f,  0.5f, 0.0f, 1.0f,  1.0f, -0.5f, -0.5f, 0.0f,
                          0.0f,  0.0f, 0.5f, -0.5f, 0.0f, 1.0f,  0.0f};

  unsigned int quadVAO, quadVBO;
  glGenVertexArrays(1, &quadVAO);
  glGenBuffers(1, &quadVBO);
  glBindVertexArray(quadVAO);
  glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Carica Texture
  // -------------------------
  stbi_set_flip_vertically_on_load(
      true); // Correggi l'orientamento della texture
  // unsigned int texture1 = loadTexture("assets/container.jpg"); // Texture
  // fittizia (Rossa)
  unsigned int textureFloor =
      loadTexture("assets/textures/environment/floor.jpg"); // Texture fittizia
  unsigned int textureWall =
      loadTexture("assets/textures/environment/wall.jpg");
  unsigned int textureHead =
      loadTexture("assets/textures/character/char_head.jpg");
  unsigned int textureHair =
      loadTexture("assets/textures/character/char_hair.jpg");
  unsigned int textureBody =
      loadTexture("assets/textures/character/char_body.jpg");
  unsigned int textureArm =
      loadTexture("assets/textures/character/char_arm.jpg");
  unsigned int textureLeg =
      loadTexture("assets/textures/character/char_leg.jpg");
  unsigned int textureServer =
      loadTexture("assets/textures/computer/server.jpg");
  unsigned int textureMonitor =
      loadTexture("assets/textures/computer/monitor.jpg");
  unsigned int textureKeyboard =
      loadTexture("assets/textures/computer/keyboard.jpg");
  unsigned int texturePlay = loadTexture("assets/images/play_button.png");
  unsigned int textureExit = loadTexture("assets/images/exit_button.png");

  ourShader.use();
  ourShader.setInt("texture1", 0);

  // Inizializza Server (Layout a Griglia)
  // Griglia da -6 a 6 con passo 4 (posizioni: -6, -2, 2, 6) -> griglia 4x4
  for (float x = -6.0f; x <= 6.0f; x += 4.0f) {
    for (float z = -6.0f; z <= 6.0f; z += 4.0f) {
      // Decidi casualmente se rotto (50% di probabilità)
      bool broken = (rand() % 2) == 0;
      servers.push_back({glm::vec3(x, 0.5f, z), broken, 0.0f});
    }
  }

  glEnable(GL_DEPTH_TEST);

  // ciclo di rendering
  // -----------
  while (!glfwWindowShouldClose(window)) {
    // logica temporale per frame
    // --------------------
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // input
    // -----
    if (currentState == GAME) {
      processInput(window);
    } else {
      // Input Menu
      if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

      // Gestione Mouse
      double xpos, ypos;
      glfwGetCursorPos(window, &xpos, &ypos);
      // Inverti Y perché OpenGL (0,0) è in basso a sinistra, ma glfw è in alto
      // a sinistra Tuttavia, useremo una proiezione ortografica con (0,0) in
      // basso a sinistra per coerenza Quindi dobbiamo convertire la Y del
      // mouse:
      ypos = SCR_HEIGHT - ypos;

      bool mouseDown =
          glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

      // Pulsante Gioca: Centro (400, 350), Dim (200, 80)
      // Bounds: x [300, 500], y [310, 390]
      if (xpos >= 300 && xpos <= 500 && ypos >= 310 && ypos <= 390) {
        if (mouseDown && !mousePressed) {
          currentState = GAME;
        }
      }

      // Pulsante Esci: Centro (400, 250), Dim (200, 80)
      // Bounds: x [300, 500], y [210, 290]
      if (xpos >= 300 && xpos <= 500 && ypos >= 210 && ypos <= 290) {
        if (mouseDown && !mousePressed) {
          glfwSetWindowShouldClose(window, true);
        }
      }

      mousePressed = mouseDown;
    }

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // attiva shader
    ourShader.use();

    if (currentState == MENU) {
      // Render Menu
      // Proiezione Ortografica 2D
      glm::mat4 projection =
          glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT);
      ourShader.setMat4("projection", projection);
      ourShader.setMat4("view", glm::mat4(1.0f)); // Nessuna view matrix

      glDisable(GL_DEPTH_TEST); // Disabilita depth test per UI 2D

      glBindVertexArray(quadVAO);
      ourShader.setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
      ourShader.setVec2("uvOffset", glm::vec2(0.0f, 0.0f));
      ourShader.setVec2("uvScale", glm::vec2(1.0f, 1.0f));

      // Pulsante Gioca
      glBindTexture(GL_TEXTURE_2D, texturePlay);
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(400.0f, 350.0f, 0.0f));
      model = glm::scale(model, glm::vec3(200.0f, 80.0f, 1.0f));
      ourShader.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 6);

      // Pulsante Esci
      glBindTexture(GL_TEXTURE_2D, textureExit);
      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(400.0f, 250.0f, 0.0f));
      model = glm::scale(model, glm::vec3(200.0f, 80.0f, 1.0f));
      ourShader.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 6);

    } else {
      // Render Gioco
      glEnable(GL_DEPTH_TEST); // Riabilita depth test

      // trasformazione telecamera/vista
      glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
      ourShader.setMat4("view", view);

      // trasformazione proiezione
      glm::mat4 projection =
          glm::perspective(glm::radians(45.0f),
                           (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
      ourShader.setMat4("projection", projection);

      // Disegna Pavimento
      glBindVertexArray(floorVAO);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, textureFloor);
      glm::mat4 model = glm::mat4(1.0f);
      ourShader.setMat4("model", model);
      ourShader.setVec3("objectColor",
                        glm::vec3(1.0f, 1.0f, 1.0f)); // Tinta bianca (normale)
      // UV predefiniti per pavimento/muri
      ourShader.setVec2("uvOffset", glm::vec2(0.0f, 0.0f));
      ourShader.setVec2("uvScale", glm::vec2(1.0f, 1.0f));
      glDrawArrays(GL_TRIANGLES, 0, 6);

      // Disegna Muri
      glBindVertexArray(VAO);                    // Torna al VAO del cubo
      glBindTexture(GL_TEXTURE_2D, textureWall); // Usa texture del muro
      // Nord
      model = glm::mat4(1.0f);
      model = glm::translate(
          model, glm::vec3(0.0f, 1.5f, -10.0f)); // Altezza 3.0 -> Centro Y 1.5
      model = glm::scale(model, glm::vec3(20.0f, 3.0f, 0.5f));
      ourShader.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      // Sud
      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(0.0f, 1.5f, 10.0f));
      model = glm::scale(model, glm::vec3(20.0f, 3.0f, 0.5f));
      ourShader.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      // Est
      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(10.0f, 1.5f, 0.0f));
      model = glm::scale(model, glm::vec3(0.5f, 3.0f, 20.0f));
      ourShader.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      // Ovest
      model = glm::mat4(1.0f);
      model = glm::translate(model, glm::vec3(-10.0f, 1.5f, 0.0f));
      model = glm::scale(model, glm::vec3(0.5f, 3.0f, 20.0f));
      ourShader.setMat4("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      // Disegna Giocatore (Personaggio)
      glBindVertexArray(VAO);
      // Rimuovi binding texture globale, leghiamo per parte
      // glBindTexture(GL_TEXTURE_2D, texture1);
      ourShader.setVec3("objectColor",
                        glm::vec3(1.0f, 1.0f, 1.0f)); // Colore texture completo

      // Stato Animazione
      bool isWalking = false;
      if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ||
          glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ||
          glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
          glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        isWalking = true;
      }

      static float walkTime = 0.0f;
      if (isWalking) {
        walkTime += deltaTime * 10.0f; // Velocità dell'animazione
      } else {
        walkTime = 0.0f; // Resetta quando si ferma
      }

      float limbAngle = sin(walkTime) * glm::radians(45.0f);

      // 1. Corpo
      glBindTexture(GL_TEXTURE_2D, textureBody);
      model = glm::mat4(1.0f);
      model = glm::translate(model, playerPos);
      model = glm::scale(model, glm::vec3(0.6f, 0.8f, 0.4f));
      ourShader.setMat4("model", model);
      ourShader.setVec2("uvOffset", glm::vec2(0.0f, 0.0f)); // Full texture
      ourShader.setVec2("uvScale", glm::vec2(1.0f, 1.0f));
      glDrawArrays(GL_TRIANGLES, 0, 36);

      // 2. Testa
      model = glm::mat4(1.0f);
      model = glm::translate(model, playerPos + glm::vec3(0.0f, 0.6f, 0.0f));
      model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
      ourShader.setMat4("model", model);
      ourShader.setVec2("uvOffset", glm::vec2(0.0f, 0.0f));
      ourShader.setVec2("uvScale", glm::vec2(1.0f, 1.0f));

      // Disegna Capelli (Dietro, Sopra, Sotto, Sinistra, Destra)
      // Indici: 0-6 (Dietro), 12-36 (Sopra, Sotto, Destra, Sinistra)
      glBindTexture(GL_TEXTURE_2D, textureHair);
      glDrawArrays(GL_TRIANGLES, 0, 6);   // Back
      glDrawArrays(GL_TRIANGLES, 12, 24); // Top, Bottom, Right, Left

      // Disegna Faccia (Fronte)
      // Indici: 6-12 (Fronte)
      glBindTexture(GL_TEXTURE_2D, textureHead);
      glDrawArrays(GL_TRIANGLES, 6, 6); // Front

      // 3. Braccio Destro
      glBindTexture(GL_TEXTURE_2D, textureArm);
      model = glm::mat4(1.0f);
      // Punto perno sulla spalla (cima del braccio)
      glm::vec3 rightArmPos = playerPos + glm::vec3(0.45f, 0.1f, 0.0f);
      model = glm::translate(model, rightArmPos);
      model = glm::translate(
          model, glm::vec3(0.0f, 0.3f, 0.0f)); // Sposta perno in cima
      model = glm::rotate(model, limbAngle, glm::vec3(1.0f, 0.0f, 0.0f));
      model = glm::translate(model,
                             glm::vec3(0.0f, -0.3f, 0.0f)); // Sposta indietro
      model = glm::scale(model, glm::vec3(0.2f, 0.6f, 0.2f));
      ourShader.setMat4("model", model);
      ourShader.setVec2("uvOffset", glm::vec2(0.0f, 0.0f)); // Full texture
      ourShader.setVec2("uvScale", glm::vec2(1.0f, 1.0f));
      glDrawArrays(GL_TRIANGLES, 0, 36);

      // 4. Braccio Sinistro
      glBindTexture(GL_TEXTURE_2D, textureArm);
      model = glm::mat4(1.0f);
      glm::vec3 leftArmPos = playerPos + glm::vec3(-0.45f, 0.1f, 0.0f);
      model = glm::translate(model, leftArmPos);
      model = glm::translate(model, glm::vec3(0.0f, 0.3f, 0.0f));
      model = glm::rotate(model, -limbAngle, glm::vec3(1.0f, 0.0f, 0.0f));
      model = glm::translate(model, glm::vec3(0.0f, -0.3f, 0.0f));
      model = glm::scale(model, glm::vec3(0.2f, 0.6f, 0.2f));
      ourShader.setMat4("model", model);
      ourShader.setVec2("uvOffset", glm::vec2(0.0f, 0.0f)); // Full texture
      ourShader.setVec2("uvScale", glm::vec2(1.0f, 1.0f));
      glDrawArrays(GL_TRIANGLES, 0, 36);

      // 5. Gamba Destra
      glBindTexture(GL_TEXTURE_2D, textureLeg);
      model = glm::mat4(1.0f);
      glm::vec3 rightLegPos = playerPos + glm::vec3(0.15f, -0.6f, 0.0f);
      model = glm::translate(model, rightLegPos);
      model =
          glm::translate(model, glm::vec3(0.0f, 0.3f, 0.0f)); // Perno sull'anca
      model = glm::rotate(model, -limbAngle, glm::vec3(1.0f, 0.0f, 0.0f));
      model = glm::translate(model, glm::vec3(0.0f, -0.3f, 0.0f));
      model = glm::scale(model, glm::vec3(0.25f, 0.6f, 0.25f));
      ourShader.setMat4("model", model);
      ourShader.setVec2("uvOffset", glm::vec2(0.0f, 0.0f)); // Full texture
      ourShader.setVec2("uvScale", glm::vec2(1.0f, 1.0f));
      glDrawArrays(GL_TRIANGLES, 0, 36);

      // 6. Gamba Sinistra
      glBindTexture(GL_TEXTURE_2D, textureLeg);
      model = glm::mat4(1.0f);
      glm::vec3 leftLegPos = playerPos + glm::vec3(-0.15f, -0.6f, 0.0f);
      model = glm::translate(model, leftLegPos);
      model = glm::translate(model, glm::vec3(0.0f, 0.3f, 0.0f));
      model = glm::rotate(model, limbAngle, glm::vec3(1.0f, 0.0f, 0.0f));
      model = glm::translate(model, glm::vec3(0.0f, -0.3f, 0.0f));
      model = glm::scale(model, glm::vec3(0.25f, 0.6f, 0.25f));
      ourShader.setMat4("model", model);
      ourShader.setVec2("uvOffset", glm::vec2(0.0f, 0.0f)); // Full texture
      ourShader.setVec2("uvScale", glm::vec2(1.0f, 1.0f));
      glDrawArrays(GL_TRIANGLES, 0, 36);

      // Disegna Server (Postazioni PC)
      // Disegna Server (Cubi PC)
      for (const auto &server : servers) {
        model = glm::mat4(1.0f);
        model = glm::translate(model, server.position);

        // Logica Animazione
        if (server.isBroken) {
          float time = glfwGetTime();
          float offset = sin(time * 10.0f) * 0.05f;
          model = glm::translate(model, glm::vec3(offset, 0.0f, 0.0f));
          ourShader.setVec3("objectColor",
                            glm::vec3(1.0f, 0.5f, 0.5f)); // Tinta rossa
        } else {
          ourShader.setVec3("objectColor",
                            glm::vec3(1.0f, 1.0f, 1.0f)); // Normale
        }

        ourShader.setMat4("model", model);

        // Disegna Case (Dietro, Sopra, Sotto, Sinistra, Destra)
        glBindTexture(GL_TEXTURE_2D, textureServer);
        glDrawArrays(GL_TRIANGLES, 0, 6);   // Dietro
        glDrawArrays(GL_TRIANGLES, 12, 24); // Sopra, Sotto, Destra, Sinistra

        // Disegna Schermo (Fronte)
        glBindTexture(GL_TEXTURE_2D, textureMonitor);
        glDrawArrays(GL_TRIANGLES, 6, 6); // Fronte
      }
    } // Fine else (GAME)

    // glfw: scambia buffer e controlla eventi IO (tasti premuti/rilasciati,
    // mouse mosso ecc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // opzionale: dealloca tutte le risorse una volta che non servono più:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(1, &floorVAO);
  glDeleteBuffers(1, &floorVBO);
  glDeleteVertexArrays(1, &quadVAO);
  glDeleteBuffers(1, &quadVBO);

  // glfw: termina, pulendo tutte le risorse GLFW precedentemente allocate.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}

// elabora tutti gli input: chiedi a GLFW se i tasti rilevanti sono stati
// premuti/rilasciati in questo frame e reagisci di conseguenza
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  float velocity = playerSpeed * deltaTime;
  glm::vec3 nextPos = playerPos;

  // Lambda di supporto per le collisioni
  auto checkCollision = [&](glm::vec3 targetPos) -> bool {
    // 1. Confini della Mappa (Il pavimento è da -10 a 10)
    // Larghezza giocatore ~0.6 (raggio 0.3), profondità ~0.4 (raggio 0.2)
    if (targetPos.x < -9.5f || targetPos.x > 9.5f)
      return true;
    if (targetPos.z < -9.5f || targetPos.z > 9.5f)
      return true;

    // 2. Collisione Server (AABB)
    for (const auto &server : servers) {
      // Collisione asse x?
      bool collisionX =
          std::abs(targetPos.x - server.position.x) <
          (0.3f + 0.5f); // Mezza larghezza giocatore + Mezza larghezza server
      // Collisione asse z?
      bool collisionZ =
          std::abs(targetPos.z - server.position.z) <
          (0.2f + 0.5f); // Mezza profondità giocatore + Mezza profondità server

      if (collisionX && collisionZ)
        return true;
    }
    return false;
  };

  // Prova movimento Z
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    nextPos.z -= velocity;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    nextPos.z += velocity;

  if (!checkCollision(nextPos)) {
    playerPos.z = nextPos.z;
  } else {
    nextPos.z = playerPos.z; // Resetta per controllo X
  }

  // Prova movimento X
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    nextPos.x -= velocity;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    nextPos.x += velocity;

  if (!checkCollision(nextPos)) {
    playerPos.x = nextPos.x;
  }

  // La telecamera segue il giocatore (semplice)
  cameraTarget = playerPos;
  cameraPos = playerPos + glm::vec3(0.0f, 15.0f, 15.0f);

  // Interazione
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    if (!spacePressed) // Antirimbalzo (Debounce)
    {
      spacePressed = true;
      for (auto &server : servers) {
        float dist = glm::distance(playerPos, server.position);
        if (dist < INTERACTION_DISTANCE && server.isBroken) {
          server.isBroken = false;
          // Attiva logica animazione "Pop" qui se necessario (es. imposta un
          // timer)
          std::cout << "Server Riparato!" << std::endl;
        }
      }
    }
  } else {
    spacePressed = false;
  }
}

// glfw: ogni volta che la dimensione della finestra cambia (da OS o
// ridimensionamento utente) questa funzione di callback viene eseguita
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

// funzione di utilità per caricare una texture 2D da file
// ---------------------------------------------------
unsigned int loadTexture(const char *path) {
  unsigned int textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
  if (data) {
    GLenum format;
    if (nrComponents == 1)
      format = GL_RED;
    else if (nrComponents == 3)
      format = GL_RGB;
    else if (nrComponents == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST); // Aspetto pixel art
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(data);
  } else {
    std::cout << "Caricamento texture fallito al percorso: " << path
              << std::endl;
    stbi_image_free(data);
  }

  return textureID;
}
