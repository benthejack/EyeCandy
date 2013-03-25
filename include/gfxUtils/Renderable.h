//
//  EyeCandyLib library, Ben Jack's personal creative coding library.
//
//  Renderable.h
//
//  Ben Jack 19/03/13
//
//--------------------------------------------------------------------
//
//    An interface for renderable items
//
//--------------------------------------------------------------------


#ifndef __EYECANDY_RENDERABLE_H_
#define __EYECANDY_RENDERABLE_H_

#include "boost/shared_ptr.hpp"
#include "cinder/gl/GLSLProg.h"


namespace EyeCandy{ namespace GfxUtils{
    
        class Renderable{
            
        public:
         
            virtual void render(){};
            
        };
    
        typedef boost::shared_ptr<GfxUtils::Renderable> Renderable_ptr;
        typedef boost::shared_ptr<ci::gl::GlslProg> Shader_ptr;
    
    }
}

#endif