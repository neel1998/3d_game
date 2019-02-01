#include "main.h"
#include "timer.h"
#include "plane.h"
#include "ground.h"
#include "object.h"
using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/
Plane plane;
Ground ground;
vector < Object > objects;


float cam_a = 0, cam_b = 4, cam_c = 10;
int cam_pos = 0;
bool cam = true;
int cam_tick = 0;
int total_cam_pos = 4;
float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);


    // Eye - Location of camera. Don't change unless you are sure!!
    glm::vec3 eye ( cam_a, cam_b, cam_c );

    // eye = rotate * eye;
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (plane.position.x, plane.position.y, plane.position.z);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    plane.draw(VP);
    ground.draw(VP);
    for (int i = 0; i < objects.size(); i ++){
    	objects[i].draw(VP);
    }
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int a = glfwGetKey(window, GLFW_KEY_A);
    int d = glfwGetKey(window, GLFW_KEY_D);
    int q = glfwGetKey(window, GLFW_KEY_Q);
    int e = glfwGetKey(window, GLFW_KEY_E);
    int w = glfwGetKey(window, GLFW_KEY_W);
    int space = glfwGetKey(window, GLFW_KEY_SPACE);
    int c = glfwGetKey(window, GLFW_KEY_C);
    if (c && cam) {
    	cam = false;
    	cam_pos = (cam_pos + 1)%total_cam_pos;
    	switch(cam_pos){
    		case 0:
    			cam_a = (plane.position.x + 10)*sin(plane.rotationY * M_PI / 180.0f);
    			cam_c = (plane.position.z + 10)*cos(plane.rotationY * M_PI / 180.0f); 
    			cam_b = 4;
    			break;
    		case 1:
    			cam_a = 10*sin(plane.rotationY * M_PI / 180.0f);
    			cam_c = 10*cos(plane.rotationY * M_PI / 180.0f);
    			cam_b = 10; 
    			break;
    		case 2: 
    			cam_a = 10; cam_b = 0; cam_c = 0;
    			break;
    		case 3: 
    			cam_a = 1; cam_b = 20; cam_c = 0;
    			break;
    	}
    }
    if (left) {
      // a = 0; b = 2; c = 5;
        // Do something
    }
    if (right) {
      // a = 5; b= 0; c = 0;
    }
    if (a) {
    	plane.tiltL();
    }
    if (d) {
    	plane.tiltR();
    }
    if (q) {
    	plane.rotateL();
    	cam_a = (plane.position.x + 10)*sin(plane.rotationY * M_PI / 180.0f);
    	cam_c = (plane.position.z + 10)*cos(plane.rotationY * M_PI / 180.0f);
    }
    if (e) {
    	cam_a = (plane.position.x + 10)*sin(plane.rotationY * M_PI / 180.0f);
    	cam_c = (plane.position.z + 10)*cos(plane.rotationY * M_PI / 180.0f);
    	plane.rotateR();
    	
    }
    if (space) {
    	cam_b += 0.1f;
    	plane.boost();
    }
    if (w) {
    	// plane.forward();
    	// cam_c += 0.1f;
    }
}
void tick_elements() {
	cam_b -= 0.05f;
	cam_tick ++;
	if (cam_tick >= 50) {
		cam_tick = 0;
		cam = true;
	}
	// cam_b -= 0.01f;
    plane.tick();
    camera_rotation_angle += 1;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    plane = Plane(0, 0, 0, COLOR_RED);
    ground = Ground(0, 0, COLOR_GROUND);

    for (int i = 0; i < 500; i ++){
    	objects.push_back(Object( 0, 0, COLOR_RED));
    }	
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 600;
    int height = 600;

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() {
    float top    = screen_center_y + 10 / screen_zoom;
    float bottom = screen_center_y - 10 / screen_zoom;
    float left   = screen_center_x - 10 / screen_zoom;
    float right  = screen_center_x + 10 / screen_zoom;
    Matrices.projection = glm::perspective( (float)M_PI*(1.0f/3.0f), 1.0f, 0.1f, 10000.0f);
}
