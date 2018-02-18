#include "main.h"
#include "timer.h"
#include "ball.h"
#include "ground.h"
#include "packman.h"
#include "pond.h"
#include "porcupine.h"
#include "magnet.h"
#include "tree.h"
#include "plank.h"
// #include "trampoline.h"

#define BITS 8

// function definition
void packman_initialize();
void packman_reinitialize(Packman packman[], int j);
int  packman_collision(Packman packman[], int j);
void player_flying();
void trampoline_jump();
int  pond_inside(const bounding_box_t & pond);
float pond_angle(const bounding_box_t & pond);
int pond_boundary(const bounding_box_t & pond);
int  porcupine_collision_detect1();
int  porcupine_collision_detect2();
void pond_move_right();
void pond_move_left();
void work_in_pond();
void jump_pond();
void Screen_Panning(GLFWwindow *window);
void Mouse_drag();

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;
color_t color_arr[7] = {COLOR_YELLOW, COLOR_GREEN, COLOR_WHITE, COLOR_PINK , COLOR_BLUE, COLOR_TAN, COLOR_VIOLET};
/**************************
* Customizable functions *
**************************/
int col_num = 7, duration = 600, start_time = 800, in_pond = 0, time_flight = 40;
Ball ball1, ball2;
Ground ground1, ground2, trampoline_pole1, trampoline_pole2;
Packman packman[100];
Pond pond, trampoline;
Porcupine porcu1, porcu2;
Magnet magnet_a, magnet_b;
Trees tree[10];
Plank plank[50];



mpg123_handle *mh;
unsigned char *buffer;
size_t buffer_size;
size_t done;
int err;

int driver;
ao_device *dev;

ao_sample_format format;
int channels, encoding;
long rate;


float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
double theta = (3.14159)/((double)6.00);

pid_t pid = 0;

Timer t60(1.0 / 60);
int jump = 0, drop_down = 0, tramp_jump = 0, level = 1, voice = 1, plank_ref = 0, plank_orient;
int packnum = 10, GAME_TIME = 3600, prev_level = 1, TIME_LEFT = 60, COUNT_TIME = 0; 
int LEFT_KEY, DOWN_KEY, RIGHT_KEY, UP_KEY, MOUSE_CLICk;
extern double drag_oldx, drag_oldy;
/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    // glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    // glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    // Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    ground1.draw(VP);
    ground2.draw(VP);


    if(level >=2)
    {
        if(magnet_a.num == 1)
        {
            magnet_a.draw(VP);
        }
        else if(magnet_b.num ==1)
        {
            magnet_b.draw(VP);
        }
    }
    

    for(int j=0;j< packnum + (level)*4 ;j++)
    {
        if(packman[j].life ==1)
        {
            packman[j].draw(VP);
            if(j%7 == 0)
            {
                plank[j].draw(VP);
            }
        }
    }   

    for(int j=0;j<6;j++)
    {
        tree[j].draw(VP);
    }

    pond.draw(VP);
    trampoline_pole1.draw(VP);
    trampoline_pole2.draw(VP);    
    trampoline.draw(VP);

    if(level >=2)
    {
        porcu1.draw(VP); 
    }
    if(level >= 3)
    {
        porcu2.draw(VP);
    }
    ball1.draw(VP);    
}

void tick_input(GLFWwindow *window) {
    int key_a  = glfwGetKey(window, GLFW_KEY_A);
    int key_d = glfwGetKey(window, GLFW_KEY_D);
    int space = glfwGetKey(window, GLFW_KEY_SPACE);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);
    bounding_box_t pond_tp = pond.bounding_box();
    Screen_Panning(window);
    Mouse_drag();

    if(pond_boundary(pond.bounding_box()))
    {
        if(key_a)
        {
            ball1.position.x -= 0.18;            
            pond_move_left();
        }
        if(key_d)
        {
            ball1.position.x += 0.26;
            pond_move_right();
        }
    }
    if(pond_inside(pond.bounding_box()))
    {
        //printf("%lf %lf\n",ball1.position.x , ball1.position.y);
        if(space && in_pond == 0 && ball1.position.x >= -1.35 && ball1.position.x <= -1.05)
        {
            in_pond = 1;
        }
    }
    else if(!pond_inside(pond.bounding_box())){
        in_pond = 0;
    }
    if (key_a && plank_ref == 0) 
    {
        if(ball1.position.x > -3.6)
        {
            ball1.position.x -= 0.06;
            plank_orient = 1;
        }
    }

    if(key_d && plank_ref == 0)
    {
        if(ball1.position.x < 3.7)
        {
            ball1.position.x += 0.06;
            plank_orient = 2;
        }
    }

    if(space && jump == 0)
    {
        ball1.speed = 0.22;
        jump = 1;
            
    }

}

void tick_elements() 
{
    if(plank_ref)
    {
        time_flight = time_flight - 1;
        if(time_flight <=0)
        {
            plank_ref = 0;
            time_flight = 40;
            ball1.accel_x = 0;
        }
        else
        {           
            if(plank_orient == 1)
            {
                if(ball1.position.x < 3.6)
                {
                    ball1.speed_x = 0;
                }
                ball1.speed_x -= ball1.accel_x;
                ball1.position.x -= ball1.speed_x;
                
            }
            else if(plank_orient == 2)
            {
                if(ball1.position.x > 3.7)
                {
                    ball1.speed_x = 0;
                }
                ball1.speed_x -= ball1.accel_x;
                ball1.position.x += ball1.speed_x;
            }
        }
    }
    if(level >= 2)
    { 
        porcu1.tick();
    }

    if(level >= 3)
    {
        porcu2.tick();
    }

    if(level >= 2)
    {
        if(magnet_a.num ==1)
        {
            duration  = duration - 1;
            if(ball1.position.y >= -1.7  && ball1.position.y <= 2.6 && ball1.position.x >= -2.6)
            {
                ball1.position.x -= 0.015;
            }
            if(duration ==0)
            {
                duration = 600;
                magnet_a.num = 0;
            }
        }
        else if(magnet_b.num ==1)
        {
            duration  = duration - 1;
            if(ball1.position.y >= -1.7  && ball1.position.y <= 2.6 && ball1.position.x <= 3.5)
            {
                ball1.position.x += 0.015;
            }
            if(duration ==0)
            {
                duration = 600;
                magnet_b.num = 0;
            }
        }
        else
        {
            start_time = start_time -1;
            if(start_time <=0)
            {
                start_time = 800 - (level)*40;
                int randi = rand()%2;
                if(randi == 0)
                {
                    magnet_a.num = 1;
                }
                else 
                {
                    magnet_b.num = 1;
                }
            }
        }
    }

    if(jump == 1 && !pond_inside(pond.bounding_box()))
    {
        ball1.accel_y = 0.008;
        ball1.accelearate();     
    }

    if(ball1.position.y <= -1.68 && !pond_inside(pond.bounding_box()))
    {
        ball1.speed = 0;
        jump =0;
        ball1.position.y = -1.69;
    }
    if(jump ==1 && ball1.position.y > -1.7 && ball1.position.y <= -1.45 && ball1.position.x > -2.25 && ball1.position.x <= 0.05)
    {
        ball1.position.x -= 0.001;
        ball1.position.y -= 0.2;
    }

    if(in_pond==1)
    {
        ball1.speed = 0.10;
        ball1.accel_y = 0.009;
        jump_pond();
    }

    if(pond_inside(pond.bounding_box()))
    {
    
        if(ball1.position.y < -2.9)
        {
            ball1.position.y += 0.05;
        }
        if(pond_boundary(pond.bounding_box()))
        {
            ball1.speed = 0;
            work_in_pond();
        }
        else
        {
            ball1.position.y -= 0.004;
            if(pond_boundary(pond.bounding_box()))
            {
                ball1.speed = 0;
                work_in_pond();
            }
        }
    }
    for(int j=0;j< packnum + (level)*4 ;j++)
    {
        if(packman[j].position.x >= 5 || packman[j].life == 0)
        {
           packman_reinitialize(packman,j);
        }
        if(ball1.speed<=0 && packman_collision(packman, j))
        {
            if(j%7 == 0)
            {
                plank_ref = 1;
                ball1.speed = -(ball1.speed) + 0.05;
                ball1.accel_x = 0.0005;
                ball1.score += 40;
            }
            else
            {
                ball1.score += 25;
            }
            ball1.speed = 0.17;
            jump = 1; 
            packman[j].life = 0;  
        }
        if(packman[j].life == 1)
        {
            packman[j].tick();
            if(j%7 ==0)
                plank[j].tick();
        }
           
    } 

    trampoline_jump();

    if(level >= 2)
    {
        if(porcupine_collision_detect1())
        {
            ball1.lives = ball1.lives -1;
            ball1.position.x = 1.0;
            ball1.position.y = -1.68;
        }  
    }
    if(level >= 3)
    {
        if(porcupine_collision_detect2())
        {
            ball1.lives = ball1.lives -1;
            ball1.position.x = 1.0;
            ball1.position.y = -1.68;
        }  
    }
    
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    //ball2.speed = -ball2.speed;

    ground1 = Ground(0,-0.3,COLOR_DARKGREEN, -2,0.3, 10, -10);
    ground2 = Ground(0,-0.5, COLOR_DARKBROWN, -6, -3.9 , 10.0,-10.0) ;

    magnet_a = Magnet(-2.7,2,COLOR_RED, -90);
    magnet_b = Magnet(2.7,2,COLOR_CRIMSON, 90);

    packman_initialize();  
    
    tree[0] = Trees(-3.8,-1.4, 0.0,0.0);
    tree[1] = Trees(-3.2,-1.4, 0.0,0.0);
    tree[2] = Trees(0.4,-1.4, 0.0,0.0);
    tree[3] = Trees(2.0,-1.4, 0.0,0.0);
    tree[4] = Trees(2.9,-1.4, 0.0,0.0);
    tree[5] = Trees(3.7,-1.4, 0.0,0.0);
    
    pond = Pond(-1.2,-2,COLOR_DARKBLUE,1.1, 360);

    trampoline_pole1 = Ground(1.07,-2,COLOR_CRIMSON,0.5,0.5,1.2,1.32);
    trampoline_pole2 = Ground(1.68,-2,COLOR_CRIMSON,0.5,0.5,1.63,1.75);
    trampoline = Pond(2.87,-1,COLOR_CRIMSON,0.55,361);
    ball1 = Ball(1, -1.68, COLOR_RED);

    
    porcu1 = Porcupine(-3.5,-1.4,0.012,-3.4,-5.5, COLOR_GOLD);
    porcu2 = Porcupine(4.2,-1.4,0.016,5.5,-4,COLOR_SILVER);

    
    
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    //glClearColor (0.0f, 0.0f, 0.0f, 0.0f);
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int ar
, char **argv) {
    srand(time(0));
    int width  = 800;
    int height = 900;

    window = initGLFW(width, height);

    initGL (window, width, height);

    pid = fork();
    if(pid == 0)
    {
        while(voice)
        {
            audio_init();
            audio_play();
            audio_close();
        }
        _exit(0);
    }

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            if(prev_level < level)
            {
                GAME_TIME = 3600 - (level)*300;
                COUNT_TIME = 0;
                prev_level = level;
            }
            if(GAME_TIME <=0)
            {
                cout << "Time Out" << "\n";
                quit(window);
            }
            else
            {
                TIME_LEFT = GAME_TIME/60;
                GAME_TIME--;
            }
    
            level = ball1.score/800 + 1;

            if(level > 5)
            {
                cout << "Game finished! You Win...."<< "\n";                
                quit(window);
            }
            if (ball1.lives == 0)
            {
                cout << "You loose" << "\n";
                quit(window);
            }

            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);
            char str1[30];
            sprintf(str1,"%d", ball1.score);
            char final_str[120] = "Score : ";
            strcat(final_str, str1);
            
            char str2[10];
            sprintf(str2,"%d", ball1.lives);
            strcat(final_str,"      Lives : ");
            strcat(final_str, str2);
            
            char str3[10];
            sprintf(str3,"%d", level);
            strcat(final_str,"      Level : ");
            strcat(final_str, str3);


            char str4[10];
            sprintf(str4,"%d", TIME_LEFT);
            strcat(final_str,"      Time Left : ");
            strcat(final_str, str4);

            glfwSetWindowTitle(window, final_str);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}


void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}


void packman_initialize()
{
    for(int j = 0 ; j< (packnum + 25);j++)
    {
        double rand_num1 = (double)((double)(rand()%10)  - (double)16.5 + (double)(rand()/RAND_MAX));
        double rand_num2 = (double)((double)(rand()%3) - (double)(rand()/RAND_MAX));
        double rand_num3 = rand_num2 + (double)(rand()/RAND_MAX) - (double)0.5;
        double radius = ((float)(rand() % 10) / 100) + 0.26;
        int clr = rand() % col_num;
        packman[j] = Packman(rand_num1, rand_num2, color_arr[clr], radius);
        packman[j].speed_x = (rand()%50)/((double)1000 + rand()/RAND_MAX)+ 0.006 + ((float)(level))/120.0;
        if(packman[j].speed_x < 0.03)
        {
            packman[j].speed_x += (0.012 + ((float)(level))/300.0);
        }
        packman[j].speed_x = -packman[j].speed_x;
        if(j%7==0)
        {
            float x_cord = rand_num1 ;//- packman[j].radius_pack*sin(M_PI/4.0);
            float y_cord = rand_num2 ;//+ packman[j].radius_pack*cos(M_PI/4.0);
            plank[j] = Plank(x_cord - 1.0*packman[j].radius_pack, y_cord + (1.42)*packman[j].radius_pack ,packman[j].speed_x,40);
        }
    }
}

void packman_reinitialize(Packman packman[], int j)
{
    double rand_num1 = (double)((double)(rand()%10)  - (double)16.5 + (double)(rand()/RAND_MAX));
    double rand_num2 = (double)((double)(rand()%3) - (double)(rand()/RAND_MAX));
    double rand_num3 = rand_num2 + (double)(rand()/RAND_MAX) - (double)0.5;
    packman[j].position.x = rand_num1;
    packman[j].position.y = rand_num2;
    packman[j].life = 1;
    packman[j].speed_x = (rand()%50)/((double)1000 + rand()/RAND_MAX) + ((float)(level))/120.0 + 0.005;
    if(packman[j].speed_x < 0.03)
    {
        packman[j].speed_x += (0.011 + ((float)(level))/300.0);
    }
    packman[j].speed_x = - packman[j].speed_x;
    if(j%7==0)
    {
        float x_cord = packman[j].position.x ;//- packman[j].radius_pack*sin(M_PI/4.0);
        float y_cord = packman[j].position.y ;//+ packman[j].radius_pack*cos(M_PI/4.0);
        plank[j] = Plank(x_cord - 1.0*packman[j].radius_pack ,y_cord + (1.42)*packman[j].radius_pack ,packman[j].speed_x, 40);
    }
    
}

int  packman_collision(Packman packman[], int j)
{
    double cord_x = packman[j].position.x - ball1.position.x;
    double cord_y = packman[j].position.y - ball1.position.y;
    double distance = sqrt(cord_x*cord_x + cord_y*cord_y);
    double radii = ball1.radius_ball + packman[j].radius_pack;
    if(distance <= radii && ball1.speed <= 0)
    {
        double chord_x  = (packman[j].radius_pack*ball1.position.x + packman[j].position.x*(distance - packman[j].radius_pack))/distance;
        double chord_y  = (packman[j].radius_pack*ball1.position.y + packman[j].position.y*(distance - packman[j].radius_pack))/distance;
        double check_max_x = packman[j].position.x + packman[j].radius_pack*sin(theta);
        double check_min_x  = packman[j].position.x - packman[j].radius_pack*sin(theta);
        if(chord_x <= check_max_x && chord_x >= check_min_x && chord_y >= packman[j].position.y)
        {
            return 1;
        }
    }

    return 0;
}

void player_flying()
{
    if(ball1.speed <= 0)
    {
        drop_down = 1;
    }
    ball1.accelearate(); 
    if(ball1.position.y <= -1.68)
    {
        ball1.position.y = -1.68;
        jump = 0;
        drop_down = 0;
        ball1.speed = 0.05;
    }
}

void trampoline_jump()
{
    double x_cord1 = ball1.position.x + ball1.radius_ball;
    double x_cord2 = ball1.position.x - ball1.radius_ball;
    if(ball1.position.x <= 3.45 && ball1.position.x >= 2.2)
    {
        if(ball1.position.y <= -0.7 && jump ==1)
        {
            ball1.position.y = -0.7;
            ball1.speed = 0.26;
            jump = 1;
        }   
    }
    else if(x_cord1 >= 2.2 && x_cord1 < 3.45 && jump == 0)
    {
        ball1.position.x =  2.2 - ball1.radius_ball;
    }
    else if(x_cord2 <= 3.45 && x_cord2 > 2.2 && jump == 0)
    {
        ball1.position.x = 3.45 + ball1.radius_ball;
    }
}

int  pond_inside(const bounding_box_t & pond)
{
    if((ball1.position.x >= (pond.x - pond.radius)) && (ball1.position.x <= (pond.x + pond.radius)) && ((ball1.position.y - ball1.radius_ball - 0.3) <= pond.y))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

float pond_angle(const bounding_box_t & pond)
{
    float dis = sqrt((ball1.position.y - pond.y)*(ball1.position.y - pond.y) + (ball1.position.x - pond.x)*(ball1.position.x - pond.x));
    float base = ball1.position.x - pond.x;
    float angle = acos(base/dis);
    return angle;
}

int pond_boundary(const bounding_box_t & pond)
{
    if(!pond_inside(pond))
    {
        return 0;
    }

    double dist = (ball1.position.y - pond.y)*(ball1.position.y - pond.y) + (ball1.position.x - pond.x)*(ball1.position.x - pond.x);
    double rad = (pond.radius - ball1.radius_ball)*(pond.radius - ball1.radius_ball);
    if(dist >= rad)
    {
        return 1;
    }
    return 0;
}

int  porcupine_collision_detect1()
{
    double min_x1 = porcu1.position.x;
    double max_x1 = porcu1.position.x + 0.9;
    double lft_x = ball1.position.x - ball1.radius_ball;
    double rgt_x = ball1.position.x + ball1.radius_ball;
    if (ball1.position.y <= -1.43 && ((lft_x <= max_x1 && lft_x >= min_x1) || (rgt_x >= min_x1 && rgt_x <= max_x1)))
    {
        return 1;
    }
    return 0;
}

int porcupine_collision_detect2()
{
    double min_x2 = porcu2.position.x;
    double max_x2 = porcu2.position.x + 0.9;
    double lft_x = ball1.position.x - ball1.radius_ball;
    double rgt_x = ball1.position.x + ball1.radius_ball;
    if(ball1.position.y <= -1.43 && ((lft_x <= max_x2 && lft_x >= min_x2) || (rgt_x >= min_x2 && rgt_x <= max_x2)))
    {
        return 1;
    }
    return 0;
}


void pond_move_left()
{
    float angle = pond_angle(pond.bounding_box());
    float epsilon = (float)M_PI/22;
    angle += (float)M_PI/35;
    if(angle < (float)M_PI - epsilon)
    {
        float dff = pond.radius_pond - ball1.radius_ball;
        ball1.position.x = pond.position.x + dff*cos(angle);
        ball1.position.y = pond.position.y - dff*sin(angle);
    }
    else
    {
        ball1.position.x = pond.position.x - pond.radius_pond;
        //ball1.position.y = pond.position.y + ball1.radius_ball;
        ball1.position.y = -1.7;
    }
}

void pond_move_right()
{
    float angle = pond_angle(pond.bounding_box());
    angle -= (float)M_PI/35;
    if(angle > 0.005f)
    {
        float dff = pond.radius_pond - ball1.radius_ball;
        ball1.position.x = pond.position.x + dff*cos(angle);
        ball1.position.y = pond.position.y - dff*sin(angle);
    }
    else
    {
        ball1.position.x = pond.position.x + pond.radius_pond;
        // ball1.position.y = pond_tp.y + ball1.radius_ball;
        ball1.position.y = -1.68;
    }
}

void work_in_pond()
{
    float angle = pond_angle(pond.bounding_box());
    if(ball1.position.x >= -1.26 && ball1.position.x <= -1.185 && ball1.position.y >= -2.785 && ball1.position.y <= -2.755)
    {
        ball1.position.x = -1.22;
        ball1.position.y = -2.77;
    }
    else
    {
        if((float)M_PI/2.0 - angle > 0.0f)
        {
            pond_move_left();
        }
        if(angle - (float)M_PI/2.0 >=0.0f)
        {          
                pond_move_right();
        }
    }
}

void jump_pond()
{
    ball1.speed -= ball1.accel_y;
    ball1.position.y += ball1.speed - ball1.accel_y;
    if(ball1.position.y >= -1.85 && ball1.position.y<= -1.6 && ball1.speed >=0)
    {
        ball1.position.y += 0.3;
    }
}

void Screen_Panning(GLFWwindow *window)
{
    LEFT_KEY = glfwGetKey(window, GLFW_KEY_LEFT);
    RIGHT_KEY = glfwGetKey(window, GLFW_KEY_RIGHT);
    UP_KEY = glfwGetKey(window, GLFW_KEY_UP);
    DOWN_KEY = glfwGetKey(window, GLFW_KEY_DOWN);

    if (LEFT_KEY && screen_center_x - 4/screen_zoom > -4)
    {
        screen_center_x -= 0.065;
        reset_screen();
    }
    if (DOWN_KEY && screen_center_y - 4/screen_zoom > -4) 
    {
        screen_center_y -= 0.065;
        reset_screen();
    }
    if (RIGHT_KEY && screen_center_x + 4/screen_zoom < 4) 
    {
        screen_center_x += 0.065;
        reset_screen();
    }
    if (UP_KEY && screen_center_y + 4/screen_zoom < 4) 
    {
        screen_center_y += 0.065;
        reset_screen();
    }
}

void Mouse_drag()
{
    MOUSE_CLICk = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if(MOUSE_CLICk)
    {
        if(drag_oldx == -1 && drag_oldy == -1)
        {
            glfwGetCursorPos(window, &drag_oldx, &drag_oldy);
        }
        else
        {
            int w, h;
            double new_x, new_y;
            glfwGetCursorPos(window, &new_x, &new_y);
            glfwGetWindowSize(window, &w, &h);

            screen_center_x -= 6.5 * (new_x - drag_oldx) / (w* screen_zoom);
            screen_center_y += 6.5 * (new_y - drag_oldy) / (h * screen_zoom);
            if (screen_center_x - 4 / screen_zoom < -4)
                screen_center_x = -4 + 4 / screen_zoom;
            if (screen_center_y - 4 / screen_zoom < -4)
                screen_center_y = -4 + 4 / screen_zoom;
            if (screen_center_x + 4 / screen_zoom > 4)
                screen_center_x = 4 - 4 / screen_zoom;
            if (screen_center_y + 4 / screen_zoom > 4)
                screen_center_y = 4 - 4 / screen_zoom;
            drag_oldx = new_x;
            drag_oldy = new_y;
            reset_screen();


            // float pos_x, pos_y;
            // pos_x = 6* (new_x - drag_oldx) / (w * screen_zoom);  /*for dragging ball*/
            // ball1.position.x = pos_x;
        }
    }
}

void audio_init()
{
    
    ao_initialize();
    driver = ao_default_driver_id();
    mpg123_init();
    mh = mpg123_new(NULL, &err);
    buffer_size = mpg123_outblock(mh);
    buffer = (unsigned char *)malloc(buffer_size * sizeof(unsigned char));

   
    mpg123_open(mh, "supermario.mp3");
    mpg123_getformat(mh, &rate, &channels, &encoding);

   
    format.bits = mpg123_encsize(encoding) * BITS;
    format.rate = rate;
    format.channels = channels;
    format.byte_format = AO_FMT_NATIVE;
    format.matrix = 0;
    dev = ao_open_live(driver, &format, NULL);
}

void audio_play()
{
    
    while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK)
    {
        ao_play(dev, (char *)buffer, done);
    }
    
}

void audio_close()
{
    free(buffer);
    ao_close(dev);
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
    ao_shutdown();
}
