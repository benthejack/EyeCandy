//
//  EyeCandyLib library, Ben Jack's personal creative coding library.
//
//  LayerManager.h
//
//  Ben Jack 19/03/13
//
//--------------------------------------------------------------------
//
//    LayerManager and Layer classes
//
//--------------------------------------------------------------------





#ifndef __EYECANDY_LAYERMANAGER_H_
#define __EYECANDY_LAYERMANAGER_H_

#include <vector>
#include "boost/Function.hpp"
#include "cinder/gl/Fbo.h"
#include "Renderable.h"
#include "PostEffect.h"

namespace EyeCandy{
    namespace GfxUtils{
        
        using namespace ci;
        using namespace ci::gl;
        
        /**********************************************************/
        /*    Layer class for creating Fbos with multiple items   */
        /**********************************************************/
        class Layer{
            
        public:
            
            //---------------------CONSTRUCTORS/DESTRUCTOR--------------------
            Layer(int i_width, int i_height, int i_msaaLevel = 8);
            ~Layer(){}

            
            //-------------------------getters/setters------------------------

            void setGLFlagFunction(boost::function<void()> i_glSetFunc, boost::function<void()> i_glpullDownFunc = NULL){ _glSetFunc = i_glSetFunc;  _glPullDownFunc = i_glpullDownFunc;};
            void setGLSLFlagFunction(boost::function<void(Shader_ptr)> i_glslSetFunc){ _glslSetFunc = i_glslSetFunc; };
            
            void setMask(bool i_isMask = true){ _isMask = i_isMask; };
            bool isMask() const{ return _isMask; }
            void setOpacity(float i_opacity) { _opacity = i_opacity; };
            float opacity() const{ return _opacity; }
            void unPreult(bool i_premult = true){ _unPremult = i_premult; }
            
            void setShader(Shader_ptr i_mainShader){ _mainShader = i_mainShader; }
            
            boost::shared_ptr<Fbo> getFbo() { return _currentFbo; }
            
            //----------------------------methods-----------------------------
            void addPostPass(boost::shared_ptr<PostEffect> i_postEffect);
            void addRenderable(Renderable_ptr i_r);
            void removeRenderable(Renderable_ptr i_r);
            
            virtual void render();
            void doUnpremult();
            void applyPostEffects();

            
        private:
            //--------------------------class members-------------------------
            int _width, _height;
            
            std::vector<Renderable_ptr> _renderList;
            std::vector<boost::shared_ptr<PostEffect> > _postEffects;
            
            boost::function<void()> _glSetFunc, _glPullDownFunc;
            boost::function<void(Shader_ptr)> _glslSetFunc;
            
            boost::shared_ptr<Fbo> _currentFbo;
            boost::shared_ptr<Fbo> _fbo;
            boost::shared_ptr<Fbo> _postPing, _postPong;
            
            Shader_ptr _mainShader;
            Shader_ptr _unPremultShader;
            
            float _opacity;
            bool _isMask;
            bool _unPremult;
                        
        };

        typedef boost::shared_ptr<Layer> Layer_ptr;
        
        class LayerManager: public Renderable{
          
        public:
            //---------------------CONSTRUCTORS/DESTRUCTOR--------------------
            
            LayerManager(int i_width, int i_height, int i_msaaLevel = 8);
            ~LayerManager(){};
            
            //-------------------------getters/setters------------------------
            
            
            
            //----------------------------methods-----------------------------
            
            Layer_ptr pushLayer();
            Layer_ptr addLayerAt(int i_index);
            Layer_ptr getLayer(int i_layer);
            
            void removeLayer(Layer_ptr);
            
            virtual void render();

            
        private:
            //--------------------------class members-------------------------
            
            int _width, _height;
            std::vector<Layer_ptr> _layers;
            int _msaaLevel;
        };
        
    }
}


#endif