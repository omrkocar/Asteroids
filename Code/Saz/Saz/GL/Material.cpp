#include "SazPCH.h"

#include "Material.h"

namespace Saz {

Material::Material(ShaderProgram* pShaderProgram, Texture* pTexture, Color color, vec2 uvScale, vec2 uvOffset)
{
    m_pShaderProgram = pShaderProgram;
    m_pTexture = pTexture;
    m_Color = color;
    m_UVScale = uvScale;
    m_UVOffset = uvOffset;
}

Material::Material(ShaderProgram* pShaderProgram, Color color)
{
    m_pShaderProgram = pShaderProgram;
    m_Color = color;
}

Material::~Material()
{
}

} // namespace fw
