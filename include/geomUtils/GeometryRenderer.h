//
//  EyeCandyLib library, Ben Jack's personal creative coding library.
//
//  GeometryRenderer.h
//
//  Ben Jack 17/03/13
//
//--------------------------------------------------------------------
//
//    Classes for rendering GeometryGenerator objects
//
//--------------------------------------------------------------------

#ifndef __EYECANDY_GEOMETRYRENDERER_H_
#define __EYECANDY_GEOMETRYRENDERER_H_

#include <vector>
#include "boost/Function.hpp"
#include "Renderable.h"
#include "GeometryGenerator.h"
#include "cinder/gl/GLSLProg.h"
#include "cinder/gl/Vbo.h"

using namespace EyeCandy::GfxUtils;

namespace EyeCandy{
    namespace GeomUtils{
        
        
        /************************************************/
        /*       base class for geometry renderers      */
        /************************************************/
        class GeometryRenderer: public GfxUtils::Renderable{
            
            public:
            
            GeometryRenderer(){};
            GeometryRenderer(GeometryGenerator_ptr i_geom, Shader_ptr i_shader = Shader_ptr(), bool i_loadNormals = false);
            
            ~GeometryRenderer(){
                _shader.reset();
                _generator.reset();
            };
            
            void showNormals(bool i_showNormals=true){ _showNormals = i_showNormals; }
            
            
            void setPosition(ci::Vec3f& i_pos){ _position = i_pos; }
            void setScale(ci::Vec3f& i_scale) { _scale = i_scale; }
            void setRotation(ci::Quatf& i_rotation) { _rotation = i_rotation; }

            
            void toggleShader();
            void renderWireFrame(bool i_wireframe = true){_wireframe = i_wireframe;}
            
            void setShaderFunc( boost::function<void (Shader_ptr)> i_shaderFunc = NULL ){
                _shaderFunc = i_shaderFunc;
            }

            void setGLSetupFunc( boost::function<void ()> i_GLSetupFunc = NULL ){
                _glSetupFunc = i_GLSetupFunc;
            }
            
            void setGLPulldownFunc( boost::function<void ()> i_GLPulldownFunc = NULL ){
                _glPulldownFunc = i_GLPulldownFunc;
            }
            
            virtual void render();
            
            protected:
            
            boost::shared_ptr<cinder::gl::VboMesh> _mesh;
            boost::shared_ptr<cinder::gl::VboMesh> _normalMesh;
            
            boost::function<void (Shader_ptr)> _shaderFunc;
            boost::function<void ()> _glSetupFunc, _glPulldownFunc;
            
            ci::Vec3f _position, _scale;
            ci::Quatf _rotation;
            
            bool _wireframe;
            bool _useShader;
            bool _showNormals;
            
            Shader_ptr _shader;
            GeometryGenerator_ptr _generator;

        };

        typedef boost::shared_ptr<GeomUtils::GeometryRenderer> GeomRenderer_ptr;

    }
    
}

#endif