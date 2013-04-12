#include "GeometryRenderer.h"


using namespace ci;
using namespace ci::gl;

namespace EyeCandy{
    namespace GeomUtils{

        
        GeometryRenderer::GeometryRenderer(GeometryGenerator_ptr i_geom, Shader_ptr i_shader, bool i_loadNormals){
            
            VboMesh::Layout tLayout = gl::VboMesh::Layout();
            init(i_geom, tLayout, i_shader, i_loadNormals);
            
        }
        
        
        GeometryRenderer::GeometryRenderer(GeometryGenerator_ptr i_geom, cinder::gl::VboMesh::Layout& i_layout, Shader_ptr i_shader, bool i_loadNormals){
            
            init(i_geom, i_layout, i_shader, i_loadNormals);
            
        }
        
        void GeometryRenderer::init(GeometryGenerator_ptr i_geom, cinder::gl::VboMesh::Layout& i_layout, Shader_ptr i_shader, bool i_loadNormals){
            _useShader = true;
            _wireframe = false;
            _showNormals = false;
            
            _position = Vec3f(0,0,0);
            _scale    = Vec3f(1,1,1);
            _rotation = Quatf::identity();
            
            _generator = i_geom;
            
            if(i_shader)
                _shader = i_shader;
            
            if(i_layout.isDefaults()){
                i_layout.setStaticPositions();
                i_layout.setStaticIndices();
                i_layout.setStaticTexCoords2d();
                i_layout.setStaticNormals();
            }
            
            if(!_generator->isGenerated())
                _generator->generate();
            
            _mesh = _generator->getVBO(i_layout);
            
            if (i_loadNormals) {
                _normalMesh = _generator->getNormalsVBO();
            }
        }

        
        void GeometryRenderer::addCustomVec3f(std::vector<Vec3f>& i_custom, std::string i_name){
            
            GLuint loc = _shader->getAttribLocation(i_name);
            
            _customLocations.push_back(i_name);
            int pos = _customLocations.size()-1;
            _mesh->setCustomDynamicLocation(pos, loc);

            gl::VboMesh::VertexIter iter = _mesh->mapVertexBuffer();
            int vertCount = _mesh->getNumVertices();
            
            for( int i = 0; i < vertCount; i++) {
                
                iter.setCustomVec3f(pos, i_custom[i]);
                ++iter;
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