#ifndef SHADER_H
#define SHADER_H

/*!*****************************************************************************
*
*
*       \author     :      Arnaud Emilien
*       \version    :      0.1
*
*******************************************************************************/

#include "Texture.h"
#include "../Utils/HashTable.h"
#include "../Maths/Vector.h"

#include <iostream>
#include <string>
#include <map>

class Shader
{

private:
    /** Program Id */
    unsigned int _program;

    std::string _vertexFileName;
    std::string _fragmentFileName;

    HashTable<const std::string, unsigned int> _textureLinks;

    void _bindTextures();

public:
    Shader(unsigned int programId,
           const std::string& vertexFileName,
           const std::string& fragmentFileName);
    ~Shader();

    /** Load the shader */
    void bind();

    /** Unload the shader */
    void unbind();

    /** Get the program Id */
    unsigned int getProgramId() const
    {
        return _program;
    }

    /** Set the shader program Id. */
    void setProgramId(unsigned int newId);

    /** Delete the associated program of the graphic card memory
    and set the _program to 0. */
    void deleteProgram();

    /** Return the name of the Vertex Shader File. */
    std::string getVertexFileName();

    /** Return the name of the Fragment Shader File. */
    std::string getFragmentFileName();

    /** Link a texture with a texture position. */
    void linkTextureNameToTexturePosition(const std::string& textureName, unsigned int position);
    unsigned int getTexturePosition(const std::string& textureName);

    /** Link a Vector3F to the shader. */
    void linkVector3F(const std::string& variableName, const Vector3F& value);
    void linkVector3FArray(const std::string& variableName, unsigned int size, Vector3F* values);

    /** Link a Integer to the shader. */
    void linkInt(const std::string& variableName, int value);
    void linkIntArray(const std::string& variableName, unsigned int size, int* values);

    /** Link a float to the shader */
    void linkFloat(const std::string& variableName, float value);
    void linkFloatArray(const std::string& variableName, unsigned int size, float* values);

}; // Shader

#endif // SHADER_H
