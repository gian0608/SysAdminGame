#include <../includes/glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader_s.h"

#include <cmath>
#include <iostream>
#include <vector>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);

// Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Camera
glm::vec3 cameraPos =
    glm::vec3(0.0f, 15.0f, 15.0f); // Top-down isometric-ish (Closer view)
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Player
glm::vec3 playerPos =
    glm::vec3(0.0f, 0.5f, 0.0f); // Y=0.5 because cube is 1x1x1, center at 0.5
float playerSpeed = 5.0f;

// Servers
struct Server {
  glm::vec3 position;
  bool isBroken;
  float animationTime; // For broken animation
};

std::vector<Server> servers;

// Interaction
bool spacePressed = false;
const float INTERACTION_DISTANCE = 2.0f;

int main() {
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw window creation
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

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // build and compile our shader zprogram
  // ------------------------------------
  Shader ourShader("shaders/vertex_shader.glsl",
                   "shaders/fragment_shader.glsl");

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float vertices[] = {
      // positions          // texture coords
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

  // Floor vertices (large plane)
  float floorVertices[] = {
      // positions          // texture coords
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

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // texture coord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Floor VAO/VBO
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

  // Load Textures
  // -------------------------
  stbi_set_flip_vertically_on_load(true); // Fix texture orientation
  unsigned int texture1 = loadTexture("assets/container.jpg"); // Dummy texture
  unsigned int textureFloor = loadTexture("assets/floor.jpg"); // Dummy texture
  unsigned int textureWall = loadTexture("assets/wall.jpg");
  unsigned int textureHead = loadTexture("assets/char_head.jpg");
  unsigned int textureHair = loadTexture("assets/char_hair.jpg");
  unsigned int textureBody = loadTexture("assets/char_body.jpg");
  unsigned int textureArm = loadTexture("assets/char_arm.jpg");
  unsigned int textureLeg = loadTexture("assets/char_leg.jpg");
  unsigned int textureServer = loadTexture("assets/server.jpg");
  unsigned int textureMonitor = loadTexture("assets/monitor.jpg");
  unsigned int textureKeyboard = loadTexture("assets/keyboard.jpg");

  ourShader.use();
  ourShader.setInt("texture1", 0);

  // Initialize Servers (Grid Layout)
  // Grid from -6 to 6 with step 4 (positions: -6, -2, 2, 6) -> 4x4 grid
  for (float x = -6.0f; x <= 6.0f; x += 4.0f) {
    for (float z = -6.0f; z <= 6.0f; z += 4.0f) {
      // Randomly decide if broken (50% chance)
      bool broken = (rand() % 2) == 0;
      servers.push_back({glm::vec3(x, 0.5f, z), broken, 0.0f});
    }
  }

  glEnable(GL_DEPTH_TEST);

  // render loop
  // -----------
  while (!glfwWindowShouldClose(window)) {
    // per-frame time logic
    // --------------------
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // input
    // -----
    processInput(window);

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // activate shader
    ourShader.use();

    // camera/view transformation
    glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
    ourShader.setMat4("view", view);

    // projection transformation
    glm::mat4 projection =
        glm::perspective(glm::radians(45.0f),
                         (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);

    // Draw Floor
    glBindVertexArray(floorVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureFloor);
    glm::mat4 model = glm::mat4(1.0f);
    ourShader.setMat4("model", model);
    ourShader.setVec3("objectColor",
                      glm::vec3(1.0f, 1.0f, 1.0f)); // White tint (normal)
    // Default UVs for floor/walls
    ourShader.setVec2("uvOffset", glm::vec2(0.0f, 0.0f));
    ourShader.setVec2("uvScale", glm::vec2(1.0f, 1.0f));
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Draw Walls
    glBindVertexArray(VAO);                    // Switch back to cube VAO
    glBindTexture(GL_TEXTURE_2D, textureWall); // Use wall texture
    // North
    model = glm::mat4(1.0f);
    model = glm::translate(
        model, glm::vec3(0.0f, 1.5f, -10.0f)); // Height 3.0 -> Center Y 1.5
    model = glm::scale(model, glm::vec3(20.0f, 3.0f, 0.5f));
    ourShader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // South
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 1.5f, 10.0f));
    model = glm::scale(model, glm::vec3(20.0f, 3.0f, 0.5f));
    ourShader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // East
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(10.0f, 1.5f, 0.0f));
    model = glm::scale(model, glm::vec3(0.5f, 3.0f, 20.0f));
    ourShader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // West
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-10.0f, 1.5f, 0.0f));
    model = glm::scale(model, glm::vec3(0.5f, 3.0f, 20.0f));
    ourShader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Draw Player (Character)
    glBindVertexArray(VAO);
    // Remove global texture bind, we bind per part
    // glBindTexture(GL_TEXTURE_2D, texture1);
    ourShader.setVec3("objectColor",
                      glm::vec3(1.0f, 1.0f, 1.0f)); // Full texture color

    // Animation State
    bool isWalking = false;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
      isWalking = true;
    }

    static float walkTime = 0.0f;
    if (isWalking) {
      walkTime += deltaTime * 10.0f; // Speed of animation
    } else {
      walkTime = 0.0f; // Reset when stopping
    }

    float limbAngle = sin(walkTime) * glm::radians(45.0f);

    // 1. Body
    glBindTexture(GL_TEXTURE_2D, textureBody);
    model = glm::mat4(1.0f);
    model = glm::translate(model, playerPos);
    model = glm::scale(model, glm::vec3(0.6f, 0.8f, 0.4f));
    ourShader.setMat4("model", model);
    ourShader.setVec2("uvOffset", glm::vec2(0.0f, 0.0f)); // Full texture
    ourShader.setVec2("uvScale", glm::vec2(1.0f, 1.0f));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 2. Head
    model = glm::mat4(1.0f);
    model = glm::translate(model, playerPos + glm::vec3(0.0f, 0.6f, 0.0f));
    model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
    ourShader.setMat4("model", model);
    ourShader.setVec2("uvOffset", glm::vec2(0.0f, 0.0f));
    ourShader.setVec2("uvScale", glm::vec2(1.0f, 1.0f));

    // Draw Hair (Back, Top, Bottom, Left, Right)
    // Indices: 0-6 (Back), 12-36 (Top, Bottom, Right, Left)
    glBindTexture(GL_TEXTURE_2D, textureHair);
    glDrawArrays(GL_TRIANGLES, 0, 6);   // Back
    glDrawArrays(GL_TRIANGLES, 12, 24); // Top, Bottom, Right, Left

    // Draw Face (Front)
    // Indices: 6-12 (Front)
    glBindTexture(GL_TEXTURE_2D, textureHead);
    glDrawArrays(GL_TRIANGLES, 6, 6); // Front

    // 3. Right Arm
    glBindTexture(GL_TEXTURE_2D, textureArm);
    model = glm::mat4(1.0f);
    // Pivot point at shoulder (top of arm)
    glm::vec3 rightArmPos = playerPos + glm::vec3(0.45f, 0.1f, 0.0f);
    model = glm::translate(model, rightArmPos);
    model =
        glm::translate(model, glm::vec3(0.0f, 0.3f, 0.0f)); // Move pivot to top
    model = glm::rotate(model, limbAngle, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, glm::vec3(0.0f, -0.3f, 0.0f)); // Move back
    model = glm::scale(model, glm::vec3(0.2f, 0.6f, 0.2f));
    ourShader.setMat4("model", model);
    ourShader.setVec2("uvOffset", glm::vec2(0.0f, 0.0f)); // Full texture
    ourShader.setVec2("uvScale", glm::vec2(1.0f, 1.0f));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 4. Left Arm
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

    // 5. Right Leg
    glBindTexture(GL_TEXTURE_2D, textureLeg);
    model = glm::mat4(1.0f);
    glm::vec3 rightLegPos = playerPos + glm::vec3(0.15f, -0.6f, 0.0f);
    model = glm::translate(model, rightLegPos);
    model = glm::translate(model, glm::vec3(0.0f, 0.3f, 0.0f)); // Pivot at hip
    model = glm::rotate(model, -limbAngle, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, glm::vec3(0.0f, -0.3f, 0.0f));
    model = glm::scale(model, glm::vec3(0.25f, 0.6f, 0.25f));
    ourShader.setMat4("model", model);
    ourShader.setVec2("uvOffset", glm::vec2(0.0f, 0.0f)); // Full texture
    ourShader.setVec2("uvScale", glm::vec2(1.0f, 1.0f));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 6. Left Leg
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

    // Draw Servers (PC Workstations)
    // Draw Servers (PC Cubes)
    for (const auto &server : servers) {
      model = glm::mat4(1.0f);
      model = glm::translate(model, server.position);

      // Animation Logic
      if (server.isBroken) {
        float time = glfwGetTime();
        float offset = sin(time * 10.0f) * 0.05f;
        model = glm::translate(model, glm::vec3(offset, 0.0f, 0.0f));
        ourShader.setVec3("objectColor",
                          glm::vec3(1.0f, 0.5f, 0.5f)); // Red tint
      } else {
        ourShader.setVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f)); // Normal
      }

      ourShader.setMat4("model", model);

      // Draw Case (Back, Top, Bottom, Left, Right)
      glBindTexture(GL_TEXTURE_2D, textureServer);
      glDrawArrays(GL_TRIANGLES, 0, 6);   // Back
      glDrawArrays(GL_TRIANGLES, 12, 24); // Top, Bottom, Right, Left

      // Draw Screen (Front)
      glBindTexture(GL_TEXTURE_2D, textureMonitor);
      glDrawArrays(GL_TRIANGLES, 6, 6); // Front
    }

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved
    // etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(1, &floorVAO);
  glDeleteBuffers(1, &floorVBO);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  float velocity = playerSpeed * deltaTime;
  glm::vec3 nextPos = playerPos;

  // Helper lambda for collision
  auto checkCollision = [&](glm::vec3 targetPos) -> bool {
    // 1. Map Boundaries (Floor is -10 to 10)
    // Player width ~0.6 (radius 0.3), depth ~0.4 (radius 0.2)
    if (targetPos.x < -9.5f || targetPos.x > 9.5f)
      return true;
    if (targetPos.z < -9.5f || targetPos.z > 9.5f)
      return true;

    // 2. Server Collision (AABB)
    for (const auto &server : servers) {
      // Collision x-axis?
      bool collisionX = std::abs(targetPos.x - server.position.x) <
                        (0.3f + 0.5f); // Player half-width + Server half-width
      // Collision z-axis?
      bool collisionZ = std::abs(targetPos.z - server.position.z) <
                        (0.2f + 0.5f); // Player half-depth + Server half-depth

      if (collisionX && collisionZ)
        return true;
    }
    return false;
  };

  // Try Z movement
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    nextPos.z -= velocity;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    nextPos.z += velocity;

  if (!checkCollision(nextPos)) {
    playerPos.z = nextPos.z;
  } else {
    nextPos.z = playerPos.z; // Reset for X check
  }

  // Try X movement
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    nextPos.x -= velocity;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    nextPos.x += velocity;

  if (!checkCollision(nextPos)) {
    playerPos.x = nextPos.x;
  }

  // Camera follows player (simple)
  cameraTarget = playerPos;
  cameraPos = playerPos + glm::vec3(0.0f, 15.0f, 15.0f);

  // Interaction
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    if (!spacePressed) // Debounce
    {
      spacePressed = true;
      for (auto &server : servers) {
        float dist = glm::distance(playerPos, server.position);
        if (dist < INTERACTION_DISTANCE && server.isBroken) {
          server.isBroken = false;
          // Trigger "Pop" animation logic here if needed (e.g. set a timer)
          std::cout << "Server Fixed!" << std::endl;
        }
      }
    }
  } else {
    spacePressed = false;
  }
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

// utility function for loading a 2D texture from file
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
                    GL_NEAREST); // Pixel art look
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(data);
  } else {
    std::cout << "Texture failed to load at path: " << path << std::endl;
    stbi_image_free(data);
  }

  return textureID;
}
