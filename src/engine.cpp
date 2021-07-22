#include "../include/engine.hpp"

GLFWwindow *window;
int screenWidth, screenHeight;
unsigned int VAO, VBO;
float positions[6]{
    -0.5f, -0.5f,
    0.5f, 0.5f,
    0.0f, -0.5f
};
unsigned int shader;

void inputHandler(GLFWwindow *window, int key, int scancode, int action, int mods){
    if (action == GLFW_PRESS){
        switch (key){
            case GLFW_KEY_ESCAPE:{
                glfwTerminate();
                std::exit(EXIT_SUCCESS);
                break;
            }
        }    
    } 
}

void errorHandler(int error, const char *description){
    std::cerr << "Error: " << description << "\n";
}

std::string readShader(std::string filePath){
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open()){
        std::cerr << "Unable to read file at `" << filePath << "`\n";
        std::exit(EXIT_FAILURE);
    }

    std::string line;
    while (!fileStream.eof()){
        std::getline(fileStream, line);
        content += line + "\n";
    }

    fileStream.close();
    return content;
}

static unsigned int compileShader(unsigned int type, const std::string &source){
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (!result){
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char log[length];
        glGetShaderInfoLog(id, length, &length, log);
        
        std::cerr << ((type == GL_VERTEX_SHADER) ? "Vertex" : "Fragment") << " shader compilation failed: " << log << "\n";
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int createShader(const std::string &vertexShader, const std::string &fragmentShader){
    unsigned int program = glCreateProgram();

    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    
    return program;
}

void init(int width, int height, const char *name){
    if (!glfwInit()){
        std::cerr << "Error: GLFW failed to initialize.\n";
        std::exit(EXIT_FAILURE);
    }

    screenHeight = height;
    screenWidth = width;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_RESIZABLE, false);
    
    window = glfwCreateWindow(screenWidth, screenHeight, name, nullptr, nullptr);
    
    glfwSetErrorCallback(errorHandler);
    glfwSetKeyCallback(window, inputHandler);
    
    if (!window){
        std::cerr << "Error: GLFW failed to create a window.\n";
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    
    if (glewInit() != GLEW_OK){
        std::cerr << "Error: GLEW failed to initialize.\n";
        std::exit(EXIT_FAILURE);
    }
    
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);  
    glViewport(0, 0, screenWidth, screenHeight);
    
    std::string vertexShader = readShader("./shaders/shader.vert");
    std::string fragmentShader = readShader("./shaders/shader.frag");
    
    shader = createShader(vertexShader, fragmentShader);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void update(){
    glfwPollEvents();
}

void render(){
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
}

void clean(){
    glfwDestroyWindow(window);
    glfwTerminate();
}
