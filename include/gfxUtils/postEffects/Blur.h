//
//  EyeCandyLib library, Ben Jack's personal creative coding library.
//
//  Blur.h
//
//  Ben Jack 19/03/13
//
//--------------------------------------------------------------------
//
//    Blur effect
//
//--------------------------------------------------------------------

#ifndef __EYECANDY_BLUREFFECT_H_
#define __EYECANDY_BLUREFFECT_H_

#include "cinder/gl/GLSLProg.h"
#include "cinder/gl/Fbo.h"

#include "PostEffect.h"

namespace EyeCandy{
    namespace GfxUtils{
        
        
        class Blur: public PostEffect{
            
        public:
            
            Blur(int i_scale = 1, int i_blurPasses = 1)
            {
                
                _blurPasses = i_blurPasses; _scale = i_scale;
                _blurKernel[0] = 0.3125; _blurKernel[1] = 0.375; _blurKernel[2] = 0.3125;
                
                try{
                    
                    _blurShader = GlslProg( ci::app::loadAsset("shaders/passThru_vert.vs"), ci::app::loadAsset("shaders/bloom/row3f.fs"));
                    
                }catch( const std::exception &e ) {
                    cinder::app::console() << e.what() << std::endl;
                    return;
                }
                
            }
            
            virtual void setupFBOs(Area i_FboArea){
                
                _size = i_FboArea;
                _scaledSize = Area(0,0,_size.getWidth()/(_scale+0.0), _size.getHeight()/(_scale+0.0));
                
                Fbo::Format blitterFormat;
                blitterFormat.setSamples(0);
                blitterFormat.enableDepthBuffer(false);
               
                _pingFbo = boost::shared_ptr<Fbo>(new Fbo(i_FboArea.getWidth()/_scale, i_FboArea.getHeight()/_scale, blitterFormat));
                _pongFbo = boost::shared_ptr<Fbo>(new Fbo(i_FboArea.getWidth()/_scale, i_FboArea.getHeight()/_scale, blitterFormat));
             
                
                _cam = CameraPersp( i_FboArea.getWidth(), i_FboArea.getHeight(), 60.0f );
                _cam.setPerspective( 60, i_FboArea.getWidth()/(i_FboArea.getHeight()+0.0), 1, 1000 );
                
            }
            
            virtual void apply(boost::shared_ptr<Fbo> i_Fbo){
                
                gl::color(1,1,1,1);
                
                blit(i_Fbo, _pingFbo, true);
                
                gl::pushMatrices();
                gl::setMatrices(_cam);
                                
                for(int j = 0; j < _blurPasses; j++){
                    
                    _blurShader.bind();
                    _blurShader.uniform("source", 0);
                    _blurShader.uniform("coefficients", _blurKernel, 3);
                    _blurShader.uniform("offsetx", 1.2f/(_scaledSize.getWidth()+0.0f));
                    _blurShader.uniform("offsety", 0.0f);
                    
                    //HORISONTAL BLUR
                    _pongFbo->bindFramebuffer();
                    _pingFbo->bindTexture();
                    gl::clear(ColorA(0,0,0,0));
                    gl::drawSolidRect(Rectf(0,0,_scaledSize.getWidth(), _scaledSize.getHeight()));
                    _pingFbo->unbindTexture();
                    _pongFbo->unbindFramebuffer();
                    
                    _blurShader.uniform("offsetx", 0.0f);
                    _blurShader.uniform("offsety", 1.2f/(_scaledSize.getWidth()+0.0f));
                    
                    //VERTICAL BLUR
                    _pongFbo->bindTexture();
                    _pingFbo->bindFramebuffer();
                    gl::clear(ColorA(0,0,0,0));
                    gl::drawSolidRect(Rectf(0,0, _scaledSize.getWidth(), _scaledSize.getHeight()));
                    _pingFbo->unbindFramebuffer();
                    _pongFbo->unbindTexture();
                    
                    _blurShader.unbind();
                }
                
                gl::popMatrices();
                
                blit(_pingFbo, i_Fbo, true);
                
            }
            
        private:
            
            int _blurPasses, _scale;
            Area _size, _scaledSize;
            
            ci::gl::GlslProg _blurShader;
            
            boost::shared_ptr<Fbo> _pingFbo, _pongFbo;
            
            CameraPersp _cam;
            float _blurKernel[3];
        };
        
    }
}

#endif