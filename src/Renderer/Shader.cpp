/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#include "Shader.h"
#include <iostream>

#include <GL/glew.h>


Shader::Shader(unsigned int programId,
               const std::string& vertexFileName,
               const std::string& fragmentFileName) :
    _program(programId)
    , _vertexFileName(vertexFileName)
    , _fragmentFileName(fragmentFileName)
{

}

Shader::~Shader()
{
}

void Shader::setProgramId(unsigned int newId)
{
    _program = newId;

}

void Shader::deleteProgram()
{
    glDeleteProgram(_program);
    _program = 0;

}

std::string Shader::getVertexFileName()
{
    return _vertexFileName;
}

std::string Shader::getFragmentFileName()
{
    return _fragmentFileName;
}

void Shader::bind()
{
    // Enable program
    glUseProgram(_program);
    // Bind all the texture(s)
    _bindTextures();

}

void Shader::unbind()
{
    glUseProgram(0);


}

void Shader::linkTextureNameToTexturePosition(const std::string& textureName,
        unsigned int position)
{
    _textureLinks.insert(textureName, position);

    std::cout << "Shader \"" << "\", " << " texture \"" << textureName << "\" linked with buffer n" << position;
    std::cout << std::endl;


}

void Shader::linkVector3F(const std::string& variableName,
                          const Vector3F& value)
{
    int location = glGetUniformLocation (_program, variableName.c_str());
    if(location >= 0)
    {
        glUniform3f(location, value.x, value.y, value.z);

    }
    else
    {
        std::cout << "Error in Shader " << "Failed to link the vec3 variable"<< std::endl;
    }
}


void Shader::linkVector3FArray(const std::string& variableName,
                               unsigned int size,
                               Vector3F* value)
{
    int location = glGetUniformLocation (_program, variableName.c_str());
    if(location >= 0)
    {
        glUniform3fv(location, size*3, reinterpret_cast<float*>(value));

    }
    else
    {
        std::cout << "Error in Shader " << "Failed to link the vec3 array"<< std::endl;
    }
}

void Shader::linkFloat(const std::string& variableName,
                       float value)
{
    int location = glGetUniformLocation (_program, variableName.c_str());
    if(location >= 0)
    {
        glUniform1f(location, value);

    }
    else
    {
        std::cout << "Error in Shader " << "Failed to link the float variable"<< std::endl;
    }
}


void Shader::linkFloatArray(const std::string& variableName,
                            unsigned int size,
                            float* value)
{
    int location = glGetUniformLocation (_program, variableName.c_str());
    if(location >= 0)
    {
        glUniform1fv(location, size, value);

    }
    else
    {
        std::cout << "Error in Shader " << "Failed to link the float array variable"<< std::endl;
    }
}

void Shader::linkInt(const std::string& variableName,
                     int value)
{
    int location = glGetUniformLocation (_program, variableName.c_str());
    if(location >= 0)
    {
        glUniform1i(location, value);

    }
    else
    {
        //std::cout << "Error in Shader " << "Failed to link the int  variable"<< std::endl;
    }
}


void Shader::linkIntArray(const std::string& variableName,
                          unsigned int size,
                          int* value)
{
    int location = glGetUniformLocation (_program, variableName.c_str());
    if(location >= 0)
    {
        glUniform1iv(location, size, value);

    }
    else
    {
        std::cout << "Error in Shader " << "Failed to link the int array variable"<< std::endl;
    }
}


unsigned int Shader::getTexturePosition(const std::string& textureName)
{
    if( _textureLinks.contains(textureName) )
    {
        return _textureLinks.find(textureName).getValue();
    }
    else
    {
        std::cout << "Error in Shader " << " impossible to fin the position of the texture" << std::endl;
        return 0;
    }
}

void Shader::_bindTextures()
{
    int location;

    HashTable<const std::string, unsigned int>::HashTableIterator it(&_textureLinks);
    while(it.hasNext())
    {
        it.next();
        location = glGetUniformLocation( _program, it.value().getKey().c_str());

        if(location >= 0)
        {
            glUniform1i(location, it.value().getValue());
        }
    }

}

