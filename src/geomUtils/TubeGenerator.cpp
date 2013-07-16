#include "GeometryGenerator.h"
#include "TubeGenerator.h"


namespace EyeCandy{ namespace GeomUtils{
    
    TubeGenerator::TubeGenerator(float i_height, float i_radius, float i_thickness, Vec3i i_segments){
        _height = i_height;
        _radius = i_radius;
        _thickness = i_thickness;
        _segments = i_segments;
        
        _vertexCount = (_segments.x*(_segments.y+1) * 2) + (_segments.x*(_segments.z-1)) * 2;
        _vertices = boost::shared_array<float>(new float[_vertexCount*3]);
        _normals  = boost::shared_array<float>(new float[_vertexCount*3]);
        _uvs      = boost::shared_array<float>(new float[_vertexCount*2]);
        _indexCount = (_segments.x)*(_segments.y+1)*2*3*2;
        _indices  = boost::shared_array<u_int32_t>(new u_int32_t[_indexCount]);
    }
    
    void TubeGenerator::generate(bool i_yFlipped){
        
        register float theta = (3.14159*2)/(_segments.x+0.0);
        register float segHeight = _height/(_segments.y+0.0);
        register float uvXSegment = 1/(_segments.x+0.0);
        register float uvYSegment = 1/(_segments.y+0.0);
        
        register float xP, yP, zP, tXp, tYp;
        
        register int ind = 0, indVert = 0, indUV = 0, vertCount = 0;
    
        register int heightSegs = _segments.y;
        register int angularSegs = _segments.x;
        
        for(int inner = 0; inner < 2; inner++){
            
            for(int i = 0; i <= heightSegs; i++){
                for(int j = 0; j < angularSegs; j++){
                    
                    
                        xP = inner == 0 ? cos(theta*j)*_radius : cos(theta*j)*(_radius-_thickness);
                    yP = i*segHeight-_height/2.0;
                    zP = inner == 0 ? sin(theta*j)*_radius : sin(theta*j)*(_radius-_thickness);
                    
                    Vec3f norm = Vec3f(xP, 0, zP);
                    norm.normalize();
                    
                    if(inner == 1)
                        norm.invert();

                    tXp = uvXSegment*j;
                    tYp = uvYSegment*i;
                    
                    _normals[indVert] = norm.x; _vertices[indVert++] = (xP); _normals[indVert] = norm.y; _vertices[indVert++] = (yP); _normals[indVert] = norm.z; _vertices[indVert++] = zP; //topLeft
                    _uvs[indUV++] = tXp; _uvs[indUV++] = tYp;
                    
                    vertCount++;
                    
                    
                    if(i > 0){

                        _indices[ind++] = j > 0 ? vertCount-2 : vertCount + angularSegs - 2;
                        _indices[ind++] = vertCount-1;
                        _indices[ind++] = vertCount-angularSegs-1;
                        
                        _indices[ind++] = vertCount-1;
                        _indices[ind++] = vertCount-angularSegs-1;
                        _indices[ind++] = j < (angularSegs-1) ? vertCount - (angularSegs-1) - 1 : vertCount - ((angularSegs*2)-1) - 1;
                    
                    }
                    
                }
            }
        }
        
        _generated = true;
        
    }
    
}}