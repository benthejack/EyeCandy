//
//  EyeCandyLib library, Ben Jack's personal creative coding library.
//
//  PostEffect.h
//
//  Ben Jack 19/03/13
//
//--------------------------------------------------------------------
//
//    Post effect superclass
//
//--------------------------------------------------------------------

#ifndef __EYECANDY_POSTEFFECT_H_
#define __EYECANDY_POSTEFFECT_H_

#include "cinder/app/App.h"
#include "cinder/Camera.h"


namespace EyeCandy{
    namespace GfxUtils{
        
        enum PostEffectType{ BLUR = 0, BLOOM };
        
        using namespace ci;
        using namespace ci::gl;

        class PostEffect{
        
        public:
            
            PostEffect(){};
            ~PostEffect(){};
            
            

            virtual void setupFBOs(Area i_FboArea){};
            virtual void apply(boost::shared_ptr<Fbo> i_Fbo){};
            
            void blit(boost::shared_ptr<Fbo> i_FboFrom, boost::shared_ptr<Fbo> i_FboTo, bool flipTexture = false){
                
                if(!_blitter)
                    setupBlitShader();
                
                boost::shared_ptr<Fbo> bigger = i_FboFrom->getWidth() >  i_FboTo->getWidth() ? i_FboFrom : i_FboTo;
                
                CameraPersp cam( bigger->getWidth(), bigger->getHeight(), 60.0f );
                cam.setPerspective( 60, bigger->getAspectRatio(), 1, 1000 );
                
                gl::pushMatrices();
                gl::setMatrices(cam);
                
                i_FboTo->bindFramebuffer();
                i_FboFrom->bindTexture();
                gl::clear(ColorA(0,0,0,0));
                _blitter.bind();
                _blitter.uniform("pass0", 0);
                _blitter.uniform("flipTexture", flipTexture);
                gl::drawSolidRect(Rectf(i_FboTo->getBounds()));
                i_FboTo->unbindFramebuffer();
                _blitter.unbind();
                
                gl::popMatrices();
            }
            
        private:
            
            void setupBlitShader(){
                try{
                    _blitter = GlslProg( ci::app::loadAsset("shaders/passThru_vert.vs"), ci::app::loadAsset("shaders/bloom/blit.fs"));
                }catch( const std::exception &e ) {
                    cinder::app::console() << "PostEffect constructor failed to load: "<< e.what() << std::endl;
                    return;
                }
            }
            
            GlslProg _blitter;
            
            
        };
        
    }
}

#endif