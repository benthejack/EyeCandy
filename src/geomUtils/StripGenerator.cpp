#define MIDPOINT -5
#define TOPLEFT -4
#define TOPRIGHT -3
#define BOTTOMLEFT -2
#define BOTTOMRIGHT -1


#include "GeometryGenerator.h"
#include "StripGenerator.h"


namespace EyeCandy{ namespace GeomUtils{

    StripGenerator::StripGenerator(){
        
    }
    
    void StripGenerator::generate(bool i_yFlipped){
        
    }

    TriStripGenerator::TriStripGenerator(Vec2i i_size, Vec2i i_segs)
    {
        _size = i_size;
        _segs = i_segs;
        
        _vertexCount = (_segs.x)*(_segs.y) * 4;
        _vertices = boost::shared_array<float>(new float[_vertexCount*3]);
        _normals  = boost::shared_array<float>(new float[_vertexCount*3]);
        _uvs      = boost::shared_array<float>(new float[_vertexCount*2]);
        _indexCount = (_segs.x)*(_segs.y)*3*2;
        _indices  = boost::shared_array<u_int32_t>(new u_int32_t[_indexCount]);
    }


    void TriStripGenerator::generate(bool i_yFlipped){
        
        register float xSpace = _size.x/_segs.x;
        register float ySpace = _size.y/_segs.y;
        register float xP, yP;
        
        register int ind = 0, indVert = 0, indUV = 0, vertCount = 0;
        
        for(int i = 0; i < _segs.x; i++){
            for(int j = 0; j < _segs.y; j++){
                
                xP = i*xSpace;
                yP = j*ySpace;
                
                _normals[indVert] = 0; _vertices[indVert++] = (xP); _normals[indVert] = 0; _vertices[indVert++] = (yP); _normals[indVert] = 1; _vertices[indVert++] = (0); //topLeft
                _uvs[indUV++] = xP/_size.x; _uvs[indUV++] = i_yFlipped ? 1.0 - (yP/_size.y) : yP/_size.y;
                
                _normals[indVert] = 0; _vertices[indVert++] = (xP+xSpace); _normals[indVert] = 0; _vertices[indVert++] = (yP); _normals[indVert] = 1; _vertices[indVert++] = (0); //topRight
                _uvs[indUV++] = (xP+xSpace)/_size.x; _uvs[indUV++] = i_yFlipped ? 1.0 - (yP/_size.y) : yP/_size.y;
                
                _normals[indVert] = 0; _vertices[indVert++] = (xP); _normals[indVert] = 0; _vertices[indVert++] = (yP+ySpace); _normals[indVert] = 1; _vertices[indVert++] = (0); //bottomLeft
                _uvs[indUV++] = (xP)/_size.x; _uvs[indUV++] = i_yFlipped ? 1.0 - ((yP+ySpace)/_size.y) : (yP+ySpace)/_size.y;
                
                _normals[indVert] = 0; _vertices[indVert++] =  (xP+xSpace); _normals[indVert] = 0; _vertices[indVert++] =  (yP+ySpace); _normals[indVert] = 1; _vertices[indVert++] =  (0); //bottomRight
                _uvs[indUV++] = (xP+xSpace)/_size.x; _uvs[indUV++] = i_yFlipped ? 1.0 - ((yP+ySpace)/_size.y) : (yP+ySpace)/_size.y;
                
                vertCount += 4;

                //TOP TRI
                _indices[ind++] = vertCount+BOTTOMLEFT;
                _indices[ind++] = vertCount+TOPRIGHT;
                _indices[ind++] = vertCount+TOPLEFT;
                
                //LEFT TRI
                _indices[ind++] = vertCount+BOTTOMLEFT;
                _indices[ind++] = vertCount+BOTTOMRIGHT;
                _indices[ind++] = vertCount+TOPRIGHT;
                
            }
        }
        
        _generated = true;
        
    }


    DiamondTriStripGenerator::DiamondTriStripGenerator(Vec2i i_size, Vec2i i_segs)
    {
        _size = i_size;
        _segs = i_segs;
        
        _vertexCount = (_segs.x)*(_segs.y) * 5;
        _vertices = boost::shared_array<float>(new float[_vertexCount*3]);
        _normals  = boost::shared_array<float>(new float[_vertexCount*3]);
        _uvs      = boost::shared_array<float>(new float[_vertexCount*2]);
        _indexCount = (_segs.x)*(_segs.y)*3*4;
        _indices  = boost::shared_array<u_int32_t>(new u_int32_t[_indexCount]);
        
    }


    void DiamondTriStripGenerator::generate(bool i_yFlipped){
    
        register float xSpace = _size.x/_segs.x;
        register float ySpace = _size.y/_segs.y;
        register float hX = xSpace/2.0;
        register float hY = ySpace/2.0;
        register float xP, yP, mX, mY;

        
        register int ind = 0, indVert = 0, indUV = 0, vertCount = 0;
        
        for(int i = 0; i < _segs.x; i++){
            for(int j = 0; j < _segs.y; j++){
                
                xP = i*xSpace;
                yP = j*ySpace;
                mX = i*xSpace + hX;
                mY = j*ySpace + hY;
                
                //create the vertices
                _normals[indVert] = 0; _vertices[indVert++] = (mX); _normals[indVert] = 0; _vertices[indVert++] = (mY); _normals[indVert] = 1; _vertices[indVert++] = (0); //midpoint
                _uvs[indUV++] = mX/_size.x; _uvs[indUV++] = i_yFlipped ? 1.0 - (mY/_size.y) : mY/_size.y;
                
                _normals[indVert] = 0; _vertices[indVert++] = (xP); _normals[indVert] = 0; _vertices[indVert++] = (yP); _normals[indVert] = 1; _vertices[indVert++] = (0); //topLeft
                _uvs[indUV++] = xP/_size.x; _uvs[indUV++] = i_yFlipped ? 1.0 - (yP/_size.y) : yP/_size.y;
                
                _normals[indVert] = 0; _vertices[indVert++] = (xP+xSpace); _normals[indVert] = 0; _vertices[indVert++] = (yP); _normals[indVert] = 1; _vertices[indVert++] = (0); //topRight
                _uvs[indUV++] = (xP+xSpace)/_size.x; _uvs[indUV++] = i_yFlipped ? 1.0 - (yP/_size.y) : yP/_size.y;

                _normals[indVert] = 0; _vertices[indVert++] = (xP); _normals[indVert] = 0; _vertices[indVert++] = (yP+ySpace); _normals[indVert] = 1; _vertices[indVert++] = (0); //bottomLeft
                _uvs[indUV++] = (xP)/_size.x; _uvs[indUV++] = i_yFlipped ? 1.0 - ((yP+ySpace)/_size.y) : (yP+ySpace)/_size.y;

                _normals[indVert] = 0; _vertices[indVert++] =  (xP+xSpace); _normals[indVert] = 0; _vertices[indVert++] =  (yP+ySpace); _normals[indVert] = 1; _vertices[indVert++] =  (0); //bottomRight
                _uvs[indUV++] = (xP+xSpace)/_size.x; _uvs[indUV++] = i_yFlipped ? 1.0 - ((yP+ySpace)/_size.y) : (yP+ySpace)/_size.y;

                vertCount += 5;
                
                
                //------------add tris to the indices array----------
                //TOP TRI
                _indices[ind++] = vertCount+MIDPOINT;
                _indices[ind++] = vertCount+TOPRIGHT;
                _indices[ind++] = vertCount+TOPLEFT;

                //LEFT TRI
                _indices[ind++] = vertCount+MIDPOINT;
                _indices[ind++] = vertCount+TOPLEFT;
                _indices[ind++] = vertCount+BOTTOMLEFT;
                
                //BOTTOM TRI
                _indices[ind++] = vertCount+MIDPOINT;
                _indices[ind++] = vertCount+BOTTOMLEFT;
                _indices[ind++] = vertCount+BOTTOMRIGHT;
                
                //RIGHT TRI
                _indices[ind++] = vertCount+MIDPOINT;
                _indices[ind++] = vertCount+BOTTOMRIGHT;
                _indices[ind++] = vertCount+TOPRIGHT;
                
            }
        }
        
        std::cout<<"_indCount:"<<_indexCount<<"  foundIndCount:"<< ind <<"  indices:"<<_indices<< "_vertCount:"<<_vertexCount<< "  foundVertCount:"<<indVert<< std::endl;
        
        _generated = true;
    }

}}