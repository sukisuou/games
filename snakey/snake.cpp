#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <random>

//variables
float step = 0.2f;
bool gameOver = false;
bool eaten = true;

//istg this guy always pops up in projects
static std::mt19937 rng(std::random_device{}());

//structure of each segment (square)
struct Square{
    float x1 = -0.2f, y1 = -0.2f, x2 = 0.0f, y2 = 0.0f;   //sides
    float blue = 0.75f;
    
    void draw(){
        glColor3f(0.0f, 0.35f, blue);
        glBegin(GL_QUADS);
            glVertex2f(x1, y1);
            glVertex2f(x2, y1);
            glVertex2f(x2, y2);
            glVertex2f(x1, y2);
        glEnd();
    } 
};

//structure of apple
struct Circle{
    float cx, cy;
    float r = 0.06f, pi = 3.14159f;
    int segment = 100;
    
    void draw(){
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);     //centre of circle
        for(int i = 0; i <= segment; ++i){
            float angle = 2.0f * pi * (float)i / (float)segment;
            float x = cx + r * std::cos(angle);
            float y = cy + r * std::sin(angle);
            glVertex2f(x, y);
        }
        glEnd();
    } 
};

//head part
Square head;
float dirx = 0.2f, diry = 0.0f;

//body segments
int length = 3;
Square* body = new Square[100];

//applessss
Circle apple;

//head movement functions
bool canTurn = true;    //turn lock
inline void move(){
    head.x1 += dirx;
    head.x2 += dirx;
    head.y1 += diry;
    head.y2 += diry;
    canTurn = true;
}
inline void turn(float dx, float dy){
    //avoid 180 degrees turn
    if(!canTurn) return;
    if((dirx != 0.0f && dx == -dirx) || (diry != 0.0f && dy == -diry)) return;

    //turns the snake
    dirx = dx;
    diry = dy;
    canTurn = false;
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(action == GLFW_PRESS){
        float dx = 0.0f, dy = 0.0f;
        switch(key){
            case GLFW_KEY_W : dy = 0.2f; break;
            case GLFW_KEY_A : dx = -0.2f; break;
            case GLFW_KEY_S : dy = -0.2f; break;
            case GLFW_KEY_D : dx = 0.2f; break;
        }

        turn(dx, dy);
    }
}

//body parts
void bodySegment(){
    //each segments follow previous segments
    for(int i = length - 1; i > 0; --i){
        body[i] = body[i - 1];
        body[i].blue = 0.75f + i * 0.002f;  //gradient
    }
    
    //first body == copy of head
    body[0] = head;
}

//wall collision
inline bool wcol(float min, float max){
    return (min < -1.0f || max > 1.0f);
}
//body collision
inline bool bcol(const Square& a, const Square& b){
    //center of head
    float hx = (a.x1 + a.x2) / 2.0f;
    float hy = (a.y1 + a.y2) / 2.0f;

    //check if center inside body segment
    return (hx >= b.x1 && hx <= b.x2 && hy >= b.y1 && hy <= b.y2);
}
//apple eat
inline bool eat(){
    Square temp;
    temp.x1 = apple.cx - apple.r;
    temp.x2 = apple.cx + apple.r;
    temp.y1 = apple.cy - apple.r;
    temp.y2 = apple.cy + apple.r;
    if(bcol(temp, head)) return true;
    else return false;
}

//apple random positioning
void placeApple(){
    while(true){
        std::uniform_int_distribution<int> intGen(-5, 4);
        //generate random position
        float x = intGen(rng) * step + 0.1f;
        float y = intGen(rng) * step + 0.1f;
        
        //check collision
        bool collision = false;
        Square temp;

        //check head
        temp.x1 = x - apple.r;
        temp.x2 = x + apple.r;
        temp.y1 = y - apple.r;
        temp.y2 = y + apple.r;
        if(bcol(temp, head)) collision = true;

        //check body
        for(int i = 0; i < length; ++i){
            if(bcol(temp, body[i])){
                collision = true;
                break;
            }
        }

        //if no collision, place apple
        if(!collision){
            apple.cx = x;
            apple.cy = y;
            eaten = false;
            break;
        }
    }
}


//main function
int main(){
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); //keeps it unresizable
    GLFWwindow* window = glfwCreateWindow(720, 720, "Snake Game! (use WASD to move)", NULL, NULL);

    //window and keyboard inputs
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  //enable vsync
    glfwSetKeyCallback(window, key_callback);

    //set up viewport and clearing color
    glViewport(0, 0, 720, 720);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //black background
    
    double lastMoveTime = 0.0;
    const double moveDelay = 0.15; //seconds between moves
    
    while(!glfwWindowShouldClose(window)){
        glClear(GL_COLOR_BUFFER_BIT);
        
        //checkered background
        for(int i = 0; i < 10; ++i){         //x-axis squares
            for(int j = 0; j < 10; ++j){     //y-axis squares
                if((i + j) % 2 == 0) glColor3f(0.0f, 1.0f, 0.0f); //
                else glColor3f(0.0f, 0.9f, 0.0f);
    
                float x = -1.0f + i * step;
                float y = -1.0f + j * step;
    
                glBegin(GL_QUADS);
                    glVertex2f(x, y);
                    glVertex2f(x + step, y);
                    glVertex2f(x + step, y + step);
                    glVertex2f(x, y + step);
                glEnd();
            }
        }

        //add apples
        if(eaten) placeApple();

        //interval between moves
        double currentTime = glfwGetTime();
        if(currentTime - lastMoveTime >= moveDelay && !gameOver){
            move(); //move snake
            bodySegment();  //add body segment
            lastMoveTime = currentTime;
        }

        //draw apple
        apple.draw();

        //draw head
        head.draw();
        
        //draw body segments
        for(int i = 1; i < length; ++i){
            body[i].draw();
        }

        //wall collision
        if(wcol(head.x1, head.x2) || wcol(head.y1, head.y2)) gameOver = true;     //fix later

        //body collision
        for(int i = 4; i < length; ++i){    //skip first 3 segments + head
            if(bcol(head, body[i])){
                gameOver = true;
                break;
            }
        }

        //eating sequence
        Square tail = body[length - 1]; //last position
        if(eat()){
            eaten = true;
            body[length] = tail;    //add segment at tail
            ++length;
        }

        //game over
        if(gameOver) break;

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete[] body;
    
    glfwDestroyWindow(window);
    glfwTerminate();
    
    return 0;
}