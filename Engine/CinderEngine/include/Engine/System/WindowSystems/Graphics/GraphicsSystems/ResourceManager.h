#ifndef _RESOURCE_MANAGER_H
#define _RESOURCE_MANAGER_H


#include "Resources.h"
#include "SpriteSheet.h"
#include "ComputeShader.h"
#include <unordered_map>

namespace Framework
{


  class Resources
  {
  public:
    Resources();
    ~Resources ();

    void Load_Resources ();

    string TextureResourcePath = "../../Resources/Textures/";
    string SpriteSheetResourcePath = "../../Resources/SpriteSheets/";
    string ShaderResourcePath = "../../Resources/Shaders/";
    string AudioResourcePath = "../../Resources/Audio/";

    Texture* Get_Texture (string textureName);
    SpriteSheet* Get_SpriteSheet (string spriteSheetName);
    Shader* Get_Shader (string shaderName);
    Sound* Get_Sound(string soundName);
    ComputeShader* Get_ComputeShader (string shaderName);

    static Resources* RS;

  private:
    std::unordered_map <string, Texture*> textures;
    std::unordered_map <string, SpriteSheet*> spriteSheets;
    std::unordered_map <string, Shader*> shaders;
    std::unordered_map<string, ComputeShader*> computeShaders;
    std::unordered_map <string, Sound*> sounds;
    void Load_Textures ();
    void Load_SpriteSheets ();
    void Load_Shaders ();
    void Load_Sounds ();
  };


}

#endif // !_RESOURCE_MANAGER_H

