#include "InkShapeRender.h"

InkShapeRender::InkShapeRender(std::shared_ptr<Shader> pShader) : mFramebuffer(std::make_unique<Framebuffer>()), mShader(pShader)
{
}