#include "GeometryRenderer.h"

using namespace ci;
using namespace ci::gl;

namespace EyeCandy{
    namespace GeomUtils{

        
        GeometryRenderer::GeometryRenderer(GeometryGenerator_ptr i_geom, Shader_ptr i_shader, bool i_loadNormals){
            
            _useShader = true;
            _wireframe = false;
            _showNormals = false;
            
            _position = Vec3f(0,0,0);
            _scale    = Vec3f(1,1,1);
            _rotation = Quatf::identity();
            
            _generator = i_geom;
            
            if(i_shader)
                _shader = i_shader;
            
            VboMesh::Layout layout;
            layout.setStaticPositions();
            layout.setStaticIndices();
            layout.setStaticTexCoords2d();
            layout.setStaticNormals();
            
            _generator->generate();
            _mesh = _generator->getVBO(layout);
            
            if (i_loadNormals) {
                _normalMesh = _generator->getNormalsVBO();
            }
            
        }
        
        void GeometryRenderer::toggleShader(){
            _useShader = !_useShader;
        }
        
        void GeometryRenderer::render(){
            
            if(_useShader && _shader){
                _shader->bind();
                if(_shaderFunc)
                    _shaderFunc(_shader);
            }
            
            if(_glSetupFunc)
            	_glSetupFunc();
            
            if(_wireframe){
                glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
                glLineWidth(2);
            }
            
            gl::pushMatrices();
            
                gl::translate(_position);
                gl::rotate(_rotation);
                gl::scale(_scale);
            
                if(_mesh)
                    gl::draw(*_mesh);
            
                if(_useShader && _shader)
                    ci::gl::GlslProg::unbind();
            
                if(_normalMesh && _showNormals){
                        gl::color(0, 1, 0);
                        glPointSize(4);
                        gl::draw(*_normalMesh);
                }
                
            gl::popMatrices();
            
            if(_glPulldownFunc)
            	_glPulldownFunc();
            
            if(_wireframe){
                glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
                glLineWidth(1.0);

            }
            
          
            
        }


    }
}