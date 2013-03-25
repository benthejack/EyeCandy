//
//  EyeCandyLib library, Ben Jack's personal creative coding library.
//
//  BillboardShader.h
//
//  Ben Jack 21/03/13
//
//--------------------------------------------------------------------
//
//    For rendering fragmentshader quads
//
//--------------------------------------------------------------------





#ifndef __EYECANDY_BILLBOARDSHADER_H_
#define __EYECANDY_BILLBOARDSHADER_H_

#include <vector>
#include "boost/Function.hpp"
#include "cinder/gl/GLSLProg.h"
#include "GeometryRenderer.h"
#include "Renderable.h"

namespace EyeCandy{
    namespace ShaderUtils{
        
        using namespace ci;
        using namespace ci::gl;
        
        /**********************************************************/
        /*    Billboard Shader for rendering a fragshaded quad    */
        /**********************************************************/

        class BillboardShader: public Renderable{
            
        public:
            
            BillboardShader(Vec2i i_size, Shader_ptr i_shader);
            ~BillboardShader(){}
            
            void setShaderSetterFunc(boost::function<void(Shader_ptr)> i_shaderFunc){ _geomRend->setShaderFunc(i_shaderFunc); };
            virtual void render(){ _geomRend->render(); };
            
        private:
            Vec2i _size;
            GeomUtils::GeomRenderer_ptr _geomRend;
            
        };

        

    }
}

#endif