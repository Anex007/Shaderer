#include "Shader.h"

#include <iostream>

#include "FileIO.h"

/*
    TODO: Test out uniform data after you recompiled the fragment shader.
    NOTE: WE pass in the code instead of the fragmentPath into this constructor do it a better way!!
*/

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    std::string fragmentSrc;
    if (vertexPath == nullptr || fragmentPath == nullptr) {
        std::cerr << "Failed to open Shaders" << std::endl;
        return;
    }

    if (!IOManager::GetManager()->ReadFromFile(vertexPath, m_VertexSrc)) {
        std::cerr << "Failed to open vertex Shader" << std::endl;
        return;
    }

    if (!IOManager::GetManager()->ReadFromFile(fragmentPath, fragmentSrc)) {
        std::cerr << "Failed to open fragment Shader" << std::endl;
        return;
    }

    unsigned int vertexShader, fragmentShader;
    const char* vertexSrc_c = m_VertexSrc.c_str();
    const char* fragmentSrc_c = fragmentSrc.c_str();
    char infoLog[512];
    int success;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSrc_c, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Error compiling vertex shader: \n\n" << infoLog << std::endl;
        return;
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSrc_c, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "Error compiling fragment shader: \n\t" << infoLog << std::endl;
        return;
    }

    m_RendererId = glCreateProgram();
    glAttachShader(m_RendererId, vertexShader);
    glAttachShader(m_RendererId, fragmentShader);
    glLinkProgram(m_RendererId);
    glGetProgramiv(m_RendererId, GL_LINK_STATUS, &success);

    glDetachShader(m_RendererId, vertexShader);
    glDetachShader(m_RendererId, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if (!success) {
        glGetProgramInfoLog(m_RendererId, 512, NULL, infoLog);
        std::cerr << "Error Linking Shaders: \n\t" << infoLog << std::endl;
        return;
    }
}

Shader::~Shader()
{
    if (m_RendererId)
        glDeleteProgram(m_RendererId);
}

bool Shader::UpdateFragmentSrc(const std::string& fragmentSrc)
{
    int success;
    unsigned int vertexShader, fragmentShader;

    const char* vertexSrc_c = m_VertexSrc.c_str();
    const char* fragmentSrc_s = fragmentSrc.c_str();


    // Delete the old shader
    glDeleteProgram(m_RendererId);
    m_UniformLocations.clear();

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSrc_c, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, m_ErrorStr);
        std::cerr << "Error compiling vertex shader: \n\t" << m_ErrorStr << std::endl;
        return false;
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSrc_s, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, m_ErrorStr);
        std::cerr << "Error compiling fragment shader: \n\t" << m_ErrorStr << std::endl;
        return false;
    }

    m_RendererId = glCreateProgram();
    glAttachShader(m_RendererId, vertexShader);
    glAttachShader(m_RendererId, fragmentShader);
    glLinkProgram(m_RendererId);
    glGetProgramiv(m_RendererId, GL_LINK_STATUS, &success);

    glDetachShader(m_RendererId, vertexShader);
    glDetachShader(m_RendererId, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if (!success) {
        glGetProgramInfoLog(m_RendererId, 512, NULL, m_ErrorStr);
        std::cerr << "Error Linking Shaders: \n\t" << m_ErrorStr << std::endl;
        return false;
    }
    return true;
}


void Shader::ParseLastError(std::map<int, std::string>& Errs)
{
    // 0(16) : error C1068: too much data in type constructor
    // 0(16) : error C1503: undefined variable "glFragCoord"
    int _fileNum, lineNum;
    std::vector<std::pair<int, std::string>> errorList;
    
    std::stringstream ss(m_ErrorStr);
    std::string line;

    while (std::getline(ss, line, '\n')) {
        sscanf(m_ErrorStr, "%d(%d) : ", &_fileNum, &lineNum);
        std::size_t found = line.find(':');
        if (found != std::string::npos) {
            Errs.emplace(lineNum, line.substr(found + 2));
        }
    }

}

void Shader::Bind() const
{
    glUseProgram(m_RendererId);
}

void Shader::UploadUniform4f(const char* name, Vec4f& v)
{
    int loc = GetUniformLocation(name);
    if (loc == -1) return;
    glUniform4f(loc, v.x, v.y, v.z, v.w);
}

void Shader::UploadUniform3f(const char* name, Vec3f& v)
{
    int loc = GetUniformLocation(name);
    if (loc == -1) return;
    glUniform3f(loc, v.x, v.y, v.z);
}

void Shader::UploadUnifrom2f(const char* name, Vec2f& v)
{
    int loc = GetUniformLocation(name);
    if (loc == -1) return;
    glUniform2f(loc, v.x, v.y);
}

void Shader::UploadUnifrom2f(const char* name, float x, float y)
{
    int loc = GetUniformLocation(name);
    if (loc == -1) return;
    glUniform2f(loc, x, y);
}

void Shader::UploadUniform1f(const char* name, float v)
{
    int loc = GetUniformLocation(name);
    if (loc == -1) return;
    glUniform1f(loc, v);
}

void Shader::UploadUniform1i(const char* name, int v)
{
    int loc = GetUniformLocation(name);
    if (loc == -1) return;
    glUniform1i(loc, v);
}

void Shader::UploadUniform1ui(const char* name, unsigned int v)
{
    int loc = GetUniformLocation(name);
    if (loc == -1) return;
    glUniform1ui(loc, v);
}

int Shader::GetUniformLocation(const char* name)
{
    if (!m_UniformLocations.count(name)) {
        int loc = glGetUniformLocation(m_RendererId, name);
        //if (loc == -1) std::cout << "Cannot find location for " << name << std::endl;
        // We're storing the value of -1 even if its not valid
        m_UniformLocations[name] = loc;
        return loc;
    } else {
        return m_UniformLocations[name];
    }
}
