#include "main.h"
#include "timer.h"
#include "plane.h"
#include "ground.h"
#include "object.h"
#include "fan.h"
#include "missile.h"
#include "altMeter.h"
#include "indicator.h"
#include "ring.h"
#include "bomb.h"
#include "fuel.h"
#include "checkpoint.h"
#include "enemy.h"
#include "bullet.h"
#include "volcano.h"
#include "score.h"
#include "para.h"
#include "arrow.h"
using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/
Arrow arrow;
Plane plane;
Ground ground;
AltMeter altMeter1, altMeter2, altMeter3, altMeter4;
Indicator indi1, indi2, indi3, indi4; 
vector < Object > objects;
vector < Missile > missiles;
vector < Bomb > bombs;
vector < Ring > rings;
vector < Fuel > fuels;
vector < Bullet > bullets;
vector < Volcano > volcanos;
vector < Score > score;
vector < Para > para;

Checkpoint checkpoint;
Enemy enemy;

int cur_cp = 0;
bool isMissile = false;
bool isBomb = false;
bool isBullet = false;
int missileTick = 0;
int bulletTick = 0;

double prev_x = 0;
double prev_y = 0;
// Fan fan;
float heli_cam_radius1 = 10;
float heli_cam_radius2 = 10;

int scoreTick = 0;

float cam_a = 0, cam_b = 4, cam_c = 10;
float tar_a, tar_b, tar_c;
int cam_pos = 0;
bool cam = true;
int cam_tick = 0;
int total_cam_pos = 5;
float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;

float heli_rotate_angle = 0;
float heli_ele_angle = 0;


Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */


void draw() {
	
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);


    glm::vec3 eye1 ( 0, 0, 10);

    glm::vec3 target1 (0, 0, 0);
    glm::vec3 up1 (0, 1, 0);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye1, target1, up1 );
    glm::mat4 VP2 = Matrices.projection * Matrices.view;
    altMeter1.draw(VP2);
    altMeter2.draw(VP2);
    altMeter3.draw(VP2);
    altMeter4.draw(VP2);
    indi1.draw(VP2);
    indi2.draw(VP2);
    indi3.draw(VP2);
    indi4.draw(VP2);
    arrow.draw(VP2);
    for (int i = 0; i < score.size(); i ++){
   		score[i].draw(VP2);
   	} 
    // Eye - Location of camera. Don't change unless you are sure!!
    glm::vec3 eye ( cam_a, cam_b, cam_c );

    // eye = rotate * eye;
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (tar_a, tar_b, tar_c);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up );


  
     // Rotating Camera for 3D
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
    checkpoint.draw(VP);
    enemy.draw(VP);
    // fan.draw(VP);
    for (int i = 0; i < objects.size(); i ++){
    	objects[i].draw(VP);	
    }
    for (int i = 0; i < missiles.size(); i ++){
    	missiles[i].draw(VP);
    }
    for (int i = 0; i < rings.size(); i ++){
    	rings[i].draw(VP);
    }
    for (int i = 0; i < bombs.size(); i++) {
    	bombs[i].draw(VP);
    }
    for (int i = 0; i < fuels.size(); i++) {
    	fuels[i].draw(VP);
    }
    for (int i = 0; i < bullets.size(); i++) {
    	bullets[i].draw(VP);
    }
    for (int i = 0; i < volcanos.size(); i++) {
    	volcanos[i].draw(VP);
    }
    for (int i = 0; i < para.size(); i++) {
    	para[i].draw(VP);
    }
    
}
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (cam_pos == 4){
		if ( xpos > prev_x ) { // right
			heli_rotate_angle += 1;

		}
		else { // left
			heli_rotate_angle -= 1;
		}
		if ( ypos < prev_y ) { // up
			heli_ele_angle += 1;
		}
		else {
			heli_ele_angle -= 1;
		}
		heli_cam_radius2 = heli_cam_radius1*cos(heli_ele_angle * M_PI / 180.0f);
		cam_a = plane.position.x + heli_cam_radius2*cos(heli_rotate_angle * M_PI / 180.0f);		
		cam_b = plane.position.y + heli_cam_radius1*sin(heli_ele_angle * M_PI / 180.0f);		
		cam_c = plane.position.z + heli_cam_radius2*sin(heli_rotate_angle * M_PI / 180.0f);		
		prev_x = xpos;
		prev_y = ypos;
	}
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS && !isBomb) {
    	Bomb bomb = Bomb(plane.position.x, plane.position.y - 1, plane.position.z, COLOR_BOMB);
    	bomb.rotationX = plane.rotationX;
    	bomb.rotationY = plane.rotationY;
    	bomb.rotationZ = plane.rotationZ;
    	bombs.push_back(bomb);
    	isBomb = true;
        // popup_menu();
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !isMissile) {
    	Missile missile = Missile(plane.position.x, plane.position.y, plane.position.z, COLOR_YELLOW);
    	missile.rotationX = plane.rotationX;
    	missile.rotationY = plane.rotationY;
    	missile.rotationZ = plane.rotationZ;
    	missiles.push_back(missile);
    	isMissile = true;
        // popup_menu();
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (cam_pos == 4){
		if (yoffset == 1) {
			if (heli_cam_radius1 > 1){
				heli_cam_radius1 --;
			}
		}
		else if (yoffset == -1) {
			if (heli_cam_radius1 < 20) {
				heli_cam_radius1 ++;
			}
		}
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
    	if ( c == 4) {
    		// cam_a = plane.position.x + heli_cam_radius;
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
    	// fan.tiltL();
    }
    if (d) {
    	plane.tiltR();
    	// fan.tiltR();
    }
    if (q) {
    	plane.rotateL();
    	// fan.rotateL();
    }
    if (e) {
    	plane.rotateR();
    	// fan.rotateR();
    }
    if (space) {
    	if (plane.position.y <= 30){
    		cam_b += 0.1f;
    		plane.boost();
    	}
    	if (indi2.position.x + 0.0084f <= -0.5f){
    		indi2.position.x += 0.0084f;
    	}
    	// fan.boost();
    }
    if (w) {
    	if (indi3.position.x < -0.5) {
    		indi3.position.x += 0.02;
    	}
    	plane.forward();
    	// fan.forward();
    }

    // glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
}



void tick_elements() {



	float zdif1 = -1*cos(plane.rotationY * M_PI / 180.0f);
	float xdif1 = -1*sin(plane.rotationY * M_PI / 180.0f);

	float zdif2 = objects[cur_cp].position.z;
	float xdif2 = objects[cur_cp].position.x;

	arrow.rotationZ = acos( (xdif2*xdif1 + zdif2*zdif1)/( sqrt( zdif1*zdif1 + xdif1*xdif1 )*sqrt( zdif2*zdif2 + xdif2*xdif2 ) ) );  


	char str[200];
	sprintf(str, "Checkpints Completed : %d",cur_cp);
    glfwSetWindowTitle(window, str);
	//gravity
	cam_b -= 0.05f;

	if (plane.health < 0) {
		printf("Killed\n");
		quit(window);
	}
	//enemy bullets
	if (!isBullet) {
		isBullet = true;
		Bullet bullet = Bullet(enemy.position.x, enemy.position.y, enemy.position.z, plane.position.x, plane.position.y, plane.position.z, COLOR_BULLET);

		bullet.angle1 = enemy.position.x - plane.position.x;
		bullet.angle2 = enemy.position.y - plane.position.y;
		bullet.angle3 = enemy.position.z - plane.position.z;
		bullets.push_back(bullet);
	}
	
	indi2.position.x -= 0.00416f;
	indi1.position.x -= 0.00416f;
	if (indi3.position.x >= -5.5) {
		indi3.position.x -= 0.01;
	}
	if ( indi1.position.x <= -5.5) {
		// printf("Out of Fuel\n");
		// quit(window);
	}
	if (plane.position.y <= -30) {
		// printf("Crashed\n");
		// quit(window);
	}
    plane.tick();
    switch(cam_pos){
    	case 0: //TPP
    		tar_a = plane.position.x;
    		tar_b = plane.position.y + 2;
    		tar_c = plane.position.z;
    		cam_a = plane.position.x + 10*sin(plane.rotationY * M_PI / 180.0f);
    		cam_c = plane.position.z + 10*cos(plane.rotationY * M_PI / 180.0f); 
    		cam_b = plane.position.y + 4;
    		break;
    	case 1: //Top
    		tar_a = plane.position.x;
    		tar_b = plane.position.y;
    		tar_c = plane.position.z;
   			cam_a = plane.position.x + 10*sin(plane.rotationY * M_PI / 180.0f);
  			cam_c = plane.position.z + 10*cos(plane.rotationY * M_PI / 180.0f); 
  			cam_b = plane.position.y + 20;
   			break;
   		case 2: //FPP
   			tar_a =  plane.position.x - 20*sin(plane.rotationY * M_PI / 180.0f);;
    		tar_b = plane.position.y + 4;
    		tar_c = plane.position.z - 20*cos(plane.rotationY * M_PI / 180.0f);

   			cam_a = plane.position.x;
  			cam_c = plane.position.z;
   			cam_b = plane.position.y + 4;
   			break;
   		case 3: //TOWER	

   			tar_a = plane.position.x;
    		tar_c = plane.position.z;
    		tar_b = plane.position.y;

   			cam_a = 10;
   			cam_b = 10;
  			cam_c = 10;
  			break;

  		case 4: // Heli - Cam
  			tar_a = plane.position.x;
    		tar_c = plane.position.z;
    		tar_b = plane.position.y;

    		heli_cam_radius2 = heli_cam_radius1*cos(heli_ele_angle * M_PI / 180.0f);
			cam_a = plane.position.x + heli_cam_radius2*cos(heli_rotate_angle * M_PI / 180.0f);		
			cam_b = plane.position.y + heli_cam_radius1*sin(heli_ele_angle * M_PI / 180.0f);		
			cam_c = plane.position.z + heli_cam_radius2*sin(heli_rotate_angle * M_PI / 180.0f);		
  			glfwSetCursorPosCallback(window, cursor_pos_callback);
  			glfwSetScrollCallback(window, scroll_callback);
  			break;
    }

	

	cam_tick ++;
	if (cam_tick >= 50) {
		cam_tick = 0;
		cam = true;
	}
	missileTick ++;
	if (missileTick >= 50) {
		missileTick = 0;
		isMissile = false;
		isBomb = false;
	}
	bulletTick ++;
	if (bulletTick >= 100) {
		isBullet = false;
		bulletTick = 0;
	}
	scoreTick ++;
	if (scoreTick >= 200) {
		plane.score += 1;
		scoreTick = 0;
	}
    for (int i = 0; i < missiles.size(); i++) {
    	missiles[i].forward();
    	if ( (pow( (missiles[i].position.x - objects[cur_cp].position.x), 2 ) + pow( (missiles[i].position.z - objects[cur_cp].position.z), 2 ) <= 45 ) &&  abs(objects[cur_cp].position.y - missiles[i].position.y) <=  3 ) {
    		objects.erase(objects.begin() + cur_cp);
    		cur_cp ++;
    		plane.score += 5;
    		if (cur_cp == 5) {
    			printf("YOU WIN!!!\n");
    			quit(window);
    		}
    		missiles.erase(missiles.begin() + i);	
    	}
    	else if (missiles[i].position.z > 1000 || missiles[i].position.z < -1000) {
    		missiles.erase(missiles.begin() + i);	
    	}
    	else {
    		for (int j = 0; j < para.size(); j ++){
		    	if ( (pow( (missiles[i].position.x - para[j].position.x), 2 ) + pow( (missiles[i].position.z - para[j].position.z), 2 ) <= 45 ) &&  abs(para[j].position.y - missiles[i].position.y) <=  3 ) {
    				missiles.erase(missiles.begin() + i);	
		    		para.erase(para.begin() + j);	
		    		plane.score += 3;
		    	}

    		}
    	}
    }
    for (int i = 0; i < bombs.size(); i++) {
    	bombs[i].forward();
    	if ( (pow( (bombs[i].position.x - objects[cur_cp].position.x), 2 ) + pow( (bombs[i].position.z - objects[cur_cp].position.z), 2 ) <= 100 ) &&  abs(objects[cur_cp].position.y - bombs[i].position.y) <= 1.0) {
    		objects.erase(objects.begin() + cur_cp);
    		cur_cp ++;
    		plane.score += 5;
    		if (cur_cp == 5) {
    			printf("YOU WIN!!!\n");
    			quit(window);
    		}
    		bombs.erase(bombs.begin() + i);
    	}
    	else if (bombs[i].position.y < -30) {
    		bombs.erase(bombs.begin() + i);
    	}
    }

    for (int i = 0; i < rings.size(); i++) {
    	if ( (pow( (rings[i].position.x - plane.position.x), 2 ) + pow( (rings[i].position.y - plane.position.y), 2 ) <= 25 ) &&  abs(plane.position.z - rings[i].position.z) <= 0.5 ) {
    		// printf("ring\n");
    		rings.erase(rings.begin() + i);
    		plane.score += 2;
    	}
    }

    for (int i = 0; i < fuels.size(); i++) {
    	if ( (pow( (fuels[i].position.x - plane.position.x), 2 ) + pow( (fuels[i].position.y - plane.position.y), 2 ) <= 25 ) &&  abs(plane.position.z - fuels[i].position.z) <= 0.5 ) {
    		// printf("fuel\n");
    		indi1.position.x = -0.5f;
    		fuels.erase(fuels.begin() + i);
    	}
    }

    for (int i = 0; i < bullets.size(); i++) {
    	bullets[i].forward();
    	if ( pow( (bullets[i].position.x - plane.position.x) , 2) + pow((bullets[i].position.z - plane.position.z) , 2) + pow((bullets[i].position.z - plane.position.z) , 2) <= 0.5 ) {
    		bullets.erase(bullets.begin() + i);
    		indi4.position.x -= 0.5;
    		plane.health --;
    	}
    	else if (bullets[i].position.z > 1000 || bullets[i].position.z < -1000 || bullets[i].position.y > 30) {
    		bullets.erase(bullets.begin() + i);	
    	}
    }
    for (int i = 0; i < volcanos.size(); i ++) {
    	if (abs(plane.position.x - volcanos[i].position.x) <= 5 && abs(plane.position.z - volcanos[i].position.z) <= 5 ) {
    		// plane.health --;
    		// indi4.position.x -= 0.5;
    		// volcanos[i].collided = true;
    		printf("Killed by Volcano\n");
    		quit(window);
    		break;
    	}
    }
    for (int i = 0; i < para.size(); i++) {
    	para[i].tick();
    }

    checkpoint.position.x = objects[cur_cp].position.x;
	checkpoint.position.y = objects[cur_cp].position.y + 8;
	checkpoint.position.z = objects[cur_cp].position.z;
	checkpoint.tick();

	enemy.position.x = objects[cur_cp].position.x;
	enemy.position.y = objects[cur_cp].position.y + 0.5;
	enemy.position.z = objects[cur_cp].position.z;
	

	int c = plane.score;
	writeScore(c%10, (c/10)%10, (c/100)%10);

    camera_rotation_angle += 1;
}
void writeScore(int unit, int dec, int hund ){
	score.clear();
	Score s;
	int numb[] = {hund, dec, unit};
	for (int i = 0; i < 3; i++){
		if (numb[i] != 5 && numb[i] != 6){
			s = Score(6 + (i+1)*1.5 , 1 + 9, -10, COLOR_RED); // right top
	    	score.push_back(s);
		}
		if ( numb[i] != 2){
			Score s = Score(6 + (i+1)*1.5, 0 + 9, -10, COLOR_RED); //right bottom
	    	score.push_back(s);
		}

		if ( numb[i] != 1 && numb[i] != 2 && numb[i] != 7  && numb[i] != 3){
			s = Score(5 + (i+1)*1.5, 1 + 9, -10, COLOR_RED); // left top
	    	score.push_back(s);
		}
	    if ( numb[i] == 2 || numb[i] == 8 || numb[i] == 6 || numb[i] == 0 ){
			s = Score(5 + (i+1)*1.5, 0 + 9, -10, COLOR_RED); // left bottom
	    	score.push_back(s);
		}
	    if ( numb[i] != 1 && numb[i] != 7 && numb[i] != 0){
			s = Score(5.5 + (i+1)*1.5, 0.5f + 9, -10, COLOR_RED); //middle
	    	s.rotation = 90;
	   	 	score.push_back(s);
		}
		if ( numb[i] != 1 && numb[i] != 4 ){
			s = Score(5.5 + (i+1)*1.5, 1.5f + 9, -10, COLOR_RED); // top
	    	s.rotation = 90;
	    	score.push_back(s);
		}
	    if ( numb[i] != 1 && numb[i] != 7  && numb[i] != 4){
			s = Score(5.5 + (i+1)*1.5, -0.5f + 9, -10, COLOR_RED); //bottom
	    	s.rotation = 90;
	    	score.push_back(s);
		}    
	    	        
	}
}
/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models
	tar_a = plane.position.x;
    tar_b = plane.position.y;
    tar_c = plane.position.z;
    plane = Plane(0, 0, 0, COLOR_RED);
    ground = Ground(0, 0, COLOR_GROUND);
    altMeter1 = AltMeter(-5.5f, 5.5f, 0, COLOR_RED);
    altMeter2 = AltMeter(-5.5f, 5, 0, COLOR_GREEN);
    altMeter3 = AltMeter(-5.5f, 4.5f, 0, COLOR_YELLOW);
    altMeter4 = AltMeter(-5.5f, 4, 0, COLOR_RING);

    indi1 = Indicator(-0.5f, 5.4f, 0, COLOR_BLACK); //fuel
    indi2 = Indicator(-3, 4.9f, 0, COLOR_BLACK); //alt
    indi3 = Indicator(-5.5f, 4.4f, 0, COLOR_BLACK); //speed
    indi4 = Indicator(-0.5f, 3.9f, 0, COLOR_BLACK); //health

    arrow = Arrow(4, -3, 0, COLOR_BLACK);

    for (int i = 0; i < 20; i ++){
    	objects.push_back(Object( rand()%1000 - 500, -30,rand()%1000 - 500, COLOR_ISLAND));
    }

    for (int i = 0 ; i < 50; i ++) {
    	rings.push_back( Ring( rand()%1000 - 500, rand()%20 - 10, rand()%1000 - 500, rand()%90 - 45, COLOR_RING));
    }

    for (int i = 0 ; i < 50; i ++) {
    	fuels.push_back( Fuel( rand()%1000 - 500, rand()%20 - 10, rand()%1000 - 500, COLOR_FUEL));
    }
    for (int i = 0; i < 5; i ++) {
    	volcanos.push_back( Volcano(rand()%1000 - 500, -30, rand()%1000 - 500, COLOR_ISLAND) );
    }

    for (int i = 0 ; i < 20; i ++) {
    	para.push_back(Para( rand()%1000 - 500, rand()%20 - 10, rand()%1000 - 500, COLOR_PARA ));
	}

  	checkpoint = Checkpoint(objects[cur_cp].position.x, objects[cur_cp].position.y + 8, objects[cur_cp].position.z, COLOR_RING);
  	enemy = Enemy(objects[cur_cp].position.x, objects[cur_cp].position.y + 0.5, objects[cur_cp].position.z, COLOR_RING);
	
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
