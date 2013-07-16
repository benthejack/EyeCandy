//
//  EyeCandyLib library, Ben Jack's personal creative coding library.
//
//  SSAO.h
//
//  Ben Jack 6/05/13
//
//--------------------------------------------------------------------
//
//    Screen space ambient occlusion effect
//
//--------------------------------------------------------------------



#ifndef __EYECANDY_SSAOEFFECT_H_
#define __EYECANDY_SSAOEFFECT_H_

#include "cinder/gl/GLSLProg.h"
#include "cinder/gl/Fbo.h"

#include "PostEffect.h"

namespace EyeCandy{
    namespace GfxUtils{

        class SSAO : public PostEffect{
          
        public:
            
            SSAO(){
                
            }
            
            ~SSAO{};
            
            
            virtual void setupFBOs(Area i_FboArea){
            
            };
            
            
            virtual void apply(boost::shared_ptr<Fbo> i_Fbo){
            
            };
            
            
        private:
            
        };

    }
}