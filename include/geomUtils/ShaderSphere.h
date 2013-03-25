//
//  ShaderSphere.h
//  EyeCandyLib
//
//  Created by Ben Jack on 16/03/13.
//  Copyright (c) 2013 Ben Jack. All rights reserved.
//

#ifndef __EyeCandyLib__ShaderSphere__
#define __EyeCandyLib__ShaderSphere__


#include "StripGenerator.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/Vbo.h"

namespace EyeCandy{
    namespace GeomUtils{
        
        /****************************************************************/
        /*   A smoothly lit sphere using only billboards with shaders   */
        /****************************************************************/
        class ShaderSphere{
            
        public:
            
            ShaderSphere(float i_radius);
            
            ~ShaderSphere(){
            }
            
            virtual void draw();
            
            void toggleDrawMode(){
                _drawSphere = !_drawSphere;
            }
            
            void setLightDir(cinder::Vec3f i_lightDir){
                _lightDir = i_lightDir;
            }
            
        private:
            
            bool _drawSphere;
            
            cinder::Vec3f _lightDir;
            
            ci::gl::GlslProg _shader;
            boost::shared_ptr<ci::gl::VboMesh> _mesh;
            float _time;
            
        };
        
        
        
    }
}

#endif /* defined(__EyeCandyLib__ShaderSphere__) */
