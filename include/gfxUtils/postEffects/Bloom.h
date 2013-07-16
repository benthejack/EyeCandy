//
//  EyeCandyLib library, Ben Jack's personal creative coding library.
//
//  Bloom.h
//
//  Ben Jack 19/03/13
//
//--------------------------------------------------------------------
//
//    Bloom effect
//
//--------------------------------------------------------------------

#ifndef __EYECANDY_BLOOMEFFECT_H_
#define __EYECANDY_BLOOMEFFECT_H_

#include "cinder/gl/GLSLProg.h"
#include "cinder/gl/Fbo.h"

#include "PostEffect.h"

namespace EyeCandy{
    namespace GfxUtils{

        
        class Bloom: public PostEffect{
            
        public:
            
            Bloom(int i_scale = 1, int i_blurPasses = 1, int i_layers = 1)
            {
                
                _blurPasses = i_blurPasses; _scale = i_scale; _layers = constrain(i_layers, 1, 7);
                _blurKernel[0] = 0.3125; _blurKernel[1] = 0.375; _blurKernel[2] = 0.3125;
                                
                try{
                    _blurShader = GlslProg( ci::app::loadAsset("shaders/passThru_vert.vs"), ci::app::loadAsset("shaders/bloom/row3f.fs"));
                    
                    std::stringstream s;
                    s<<"shaders/bloom/combine"<<(_layers+1)<<"f.fs";
                    std::cout<<"bloom loading: "<<s.str()<<" shader."<<std::endl;
                    _combiner   = GlslProg( ci::app::loadAsset("shaders/passThru_vert.vs"), ci::app::loadAsset(s.str()));
                }catch( const std::exception &e ) {
                    cinder::app::console() << e.what() << std::endl;
                    return;
                }

            }
            
            virtual void setupFBOs(Area i_FboArea){
                
                Fbo::Format blitterFormat;
                blitterFormat.setSamples(0);
                blitterFormat.enableDepthBuffer(false);
                
                _downsampledFbos.push_back(std::vector< boost::shared_ptr<Fbo> >()); _downsampledFbos.push_back(std::vector< boost::shared_ptr<Fbo> >());
                
                _aliasedFbo = boost::shared_ptr<Fbo>(new Fbo(i_FboArea.getWidth(), i_FboArea.getHeight(), blitterFormat));
                                
                for(int i = 0; i < _layers; i++){
                    
                    boost::shared_ptr<Fbo> nFbo( new Fbo(i_FboArea.getWidth()/(i+(_scale-1)), i_FboArea.getHeight()/(i+(_scale-1)), blitterFormat) );
                    _downsampledFbos[0].push_back(nFbo);
                    boost::shared_ptr<Fbo> nFbo2( new Fbo(i_FboArea.getWidth()/(i+(_scale-1)), i_FboArea.getHeight()/(i+(_scale-1)), blitterFormat) );
                    _downsampledFbos[1].push_back(nFbo2);
                    
                }
                
                _cam = CameraPersp( i_FboArea.getWidth(), i_FboArea.getHeight(), 60.0f );
                _cam.setPerspective( 60, i_FboArea.getWidth()/(i_FboArea.getHeight()+0.0), 1, 1000 );

            }
            
            virtual void apply(boost::shared_ptr<Fbo> i_Fbo){
                
                gl::color(1,1,1,1);
                
                blit(i_Fbo, _aliasedFbo, true);
                
                
                for(int i = 0; i < _downsampledFbos[0].size(); i++){
                    
                    gl::color(1,1,1,1);
                   
                    blit(_aliasedFbo, _downsampledFbos[0][i]);
                    
                    gl::pushMatrices();
                    gl::setMatrices(_cam);
                    
                    for(int j = 0; j < _blurPasses; j++){
                        
                        _downsampledFbos[0][i]->bindTexture();
                        
                        _blurShader.bind();
                        _blurShader.uniform("source", 0);
                        _blurShader.uniform("coefficients", _blurKernel, 3);
                        _blurShader.uniform("offsetx", 1.2f/(_downsampledFbos[0][i]->getWidth()+0.0f));
                        _blurShader.uniform("offsety", 0.0f);
                        
                        //HORISONTAL BLUR
                        _downsampledFbos[1][i]->bindFramebuffer();
                        gl::clear(ColorA(0,0,0,0));
                        gl::drawSolidRect(Rectf(0,0,_downsampledFbos[0][i]->getWidth(), _downsampledFbos[0][i]->getHeight()));
                        _downsampledFbos[1][i]->unbindFramebuffer();
                        _downsampledFbos[0][i]->unbindTexture();
                        
                        _blurShader.uniform("offsetx", 0.0f);
                        _blurShader.uniform("offsety", 1.2f/(_downsampledFbos[0][i]->getWidth()+0.0f));
                        
                        //VERTICAL BLUR
                        _downsampledFbos[1][i]->bindTexture();
                        _downsampledFbos[0][i]->bindFramebuffer();
                        gl::clear(ColorA(0,0,0,0));
                        gl::drawSolidRect(Rectf(0,0,_downsampledFbos[0][i]->getWidth(), _downsampledFbos[0][i]->getHeight()));
                        _downsampledFbos[0][i]->unbindFramebuffer();
                        _downsampledFbos[1][i]->unbindTexture();
                        
                        _blurShader.unbind();
                    }
                    
                    gl::popMatrices();

                    
                }
                
                
                //DRAW TO INPUT/OUTPUT FBO
                _combiner.bind();
                
                _aliasedFbo->bindTexture(0);
                
                for(int i = 0; i < _downsampledFbos[0].size(); i++){
                    _downsampledFbos[0][i]->bindTexture(i+1);
                }
                
                _combiner.uniform("Pass0", 0);
                _combiner.uniform("Pass1", 1);
                _combiner.uniform("Pass2", 2);
                _combiner.uniform("Pass3", 3);
                _combiner.uniform("Pass4", 4);
                _combiner.uniform("Pass5", 5);
                _combiner.uniform("Pass6", 6);
                _combiner.uniform("Pass7", 7);
                
                _combiner.uniform("bkgd", Vec4f(0,0,0,0));
                
                
                  i_Fbo->bindFramebuffer();
                  gl::clear(ColorA(0,0,0,0));
                  gl::drawSolidRect(Rectf(0,0,i_Fbo->getWidth(), i_Fbo->getHeight()));
             
                  i_Fbo->unbindFramebuffer();
                
                _combiner.unbind();
                
                
            }
            
        private:
            
            int _blurPasses, _scale, _layers;

            
            ci::gl::GlslProg _blurShader;
            ci::gl::GlslProg _combiner;
            
            boost::shared_ptr<Fbo> _aliasedFbo;
            
            std::vector< std::vector< boost::shared_ptr<Fbo> > > _downsampledFbos;
            CameraPersp _cam;
            float _blurKernel[3];
        };

    }
}

#endif