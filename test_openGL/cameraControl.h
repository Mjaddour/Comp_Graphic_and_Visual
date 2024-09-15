#ifndef CAMERACONTROL_H
#define CAMERACONTROL_H

#include <GLFW/glfw3.h>

// Declare scroll callback function
void scroll_callback(GLFWwindow* window, double xpos, double ypos);

// Function to process keyboard input
void processInput(GLFWwindow* window);

// Declare mouse callback function
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// Declare external variables
extern float camX;    // Camera position X-coordinate
extern float camY;    // Camera position Y-coordinate
extern float camZ;    // Camera position Z-coordinate
extern float camSpeed; // Camera movement speed

#endif
