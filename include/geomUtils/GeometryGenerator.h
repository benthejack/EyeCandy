//
//  EyeCandyLib library, Ben Jack's personal creative coding library.
//
//  GeometryGenerator.h
//
//  Ben Jack 17/02/13
//
//--------------------------------------------------------------------
//
//    Classes for creating basic geometry
//
//--------------------------------------------------------------------

#ifndef __EYECANDY_GEOMETRYGENERATOR_H_
#define __EYECANDY_GEOMETRYGENERATOR_H_

#include <vector>
#include <boost/shared_array.hpp>
#include "cinder/gl/Vbo.h"


namespace EyeCandy{
    namespace GeomUtils{
        
        /************************************************/
        /*       base class for geometry generators     */
        /************************************************/
        class GeometryGenerator{
            
        public:
            
            //--------------------------constructor/destructor-----------------------------//
            GeometryGenerator(){
                _generated = false;
                _splitFaces = false;
                _faceNormals = false;
            };
            
            
            ~GeometryGenerator(){
            }

            virtual void generate(bool i_yFlipped = true){};
            virtual boost::shared_ptr<cinder::gl::VboMesh> getVBO(cinder::gl::VboMesh::Layout& i_layout);
            virtual boost::shared_ptr<cinder::gl::VboMesh> getNormalsVBO();
            
            std::vector<ci::Vec3f> getPositionsVector();
            std::vector<ci::Vec3f> getNormalsVector();
            std::vector<ci::uint32_t> getIndicesVector();
            std::vector<ci::Vec2f> getUvsVector();
            std::vector<ci::Vec3f> getTriCentroids(bool i_perVertex = true);
            std::vector<ci::Vec3f> getNormalLines(float i_length = 0.2);
            std::vector<ci::Vec3f> getBounds();
            
            //------------------------------getters/setters-------------------------------//
            void useSplitFaces(bool i_split = true){
                _splitFaces = i_split;
            }
            
            void useFaceNormals(bool i_faceNormals = true){
                _faceNormals = i_faceNormals;
                _splitFaces = i_faceNormals ? true : _splitFaces;
            }
            
            bool isGenerated(){
                return _generated;
            }
            
            boost::shared_array<float> vertices(){
                return _vertices;
            }
            
            boost::shared_array<u_int32_t> indices(){
                return _indices;
            }
            
            int numVertices(){
                return _vertexCount;
            }
            
            int numIndices(){
                return  _indexCount;
            }
            
        protected:
            
            bool _generated;
            boost::shared_array<float> _vertices;
            boost::shared_array<float> _normals;
            boost::shared_array<float> _uvs;
            boost::shared_array<u_int32_t> _indices;
            
            std::vector<ci::Vec3f> _positionsVector;
            std::vector<ci::Vec3f> _normalsVector;
            std::vector<ci::uint32_t> _indicesVector;
            std::vector<ci::Vec2f> _uvsVector;

            int _vertexCount;
            int _indexCount;
            
            bool _splitFaces, _faceNormals;
            
        };
        
        typedef boost::shared_ptr<GeometryGenerator> GeometryGenerator_ptr;

        
            
        }
    
}

#endif