//
//  GeometryGenerator.cpp
//  EyeCandyLib
//
//  Created by Ben Jack on 16/03/13.
//  Copyright (c) 2013 Ben Jack. All rights reserved.
//


#include <float.h>
#include "GeometryGenerator.h"
#include "cinder/Vector.h"
#include "boost/scoped_array.hpp"

using namespace ci;

namespace EyeCandy { namespace GeomUtils{
    
    boost::shared_ptr<cinder::gl::VboMesh> GeometryGenerator::getVBO(cinder::gl::VboMesh::Layout& i_layout){
        
        std::vector<ci::Vec3f> positions = getPositionsVector();
        std::vector<uint32_t> inds = getIndicesVector();
        std::vector<ci::Vec2f> texCoords = getUvsVector();
        std::vector<ci::Vec3f> normals = getNormalsVector();
        
        boost::shared_ptr<cinder::gl::VboMesh> mesh(new cinder::gl::VboMesh(positions.size(), inds.size(), i_layout, GL_TRIANGLES));
        
        
        if(i_layout.hasStaticPositions()){
            mesh->bufferPositions(positions);
        }else{
            
            gl::VboMesh::VertexIter iter = mesh->mapVertexBuffer();
            for( int x = 0; x < positions.size(); ++x ) {
                //positions.at(x) *= 1.001;
                iter.setPosition(positions[x]);
                ++iter;
            }
        }
        
        if(i_layout.hasStaticIndices()){
            mesh->bufferIndices(inds);
        }
        
        if(i_layout.hasStaticTexCoords()){
            mesh->bufferTexCoords2d(0, texCoords);
        }else{
            gl::VboMesh::VertexIter iter = mesh->mapVertexBuffer();
            for( int x = 0; x < texCoords.size(); ++x ) {
                //positions.at(x) *= 1.001;
                iter.setTexCoord2d0(texCoords[x]);
                ++iter;
            }
        }
        
        if(i_layout.hasStaticNormals()){
            mesh->bufferNormals(normals);
        }else{
            gl::VboMesh::VertexIter iter = mesh->mapVertexBuffer();
            for( int x = 0; x < normals.size(); ++x ) {
                //positions.at(x) *= 1.001;
                iter.setPosition(normals[x]);
                ++iter;
            }
        }
        
        return mesh;
        
    }
    
    boost::shared_ptr<cinder::gl::VboMesh> GeometryGenerator::getNormalsVBO(){
        
        cinder::gl::VboMesh::Layout layout;
        layout.setStaticPositions();
        layout.setStaticIndices();
        
        
        std::vector<Vec3f> verts = getNormalLines();
        std::vector<uint32_t> inds;
        
        for(int i = 0; i < verts.size(); i++){
            inds.push_back(i);
        }
        
        boost::shared_ptr<cinder::gl::VboMesh> mesh(new cinder::gl::VboMesh(verts.size(), inds.size(), layout, GL_LINES));
        
        mesh->bufferPositions(verts);
        mesh->bufferIndices(inds);
        
        return mesh;
        
    }
    
    std::vector<ci::Vec3f> GeometryGenerator::getPositionsVector(){
        
        if(_positionsVector.size() == 0){
            if(_splitFaces || _faceNormals){
                
                register int ind;
                
                for(int i = 0; i < _indexCount; ++i ){
                    ind = _indices[i];
                    _positionsVector.push_back(Vec3f(_vertices[ind*3], _vertices[ind*3+1], _vertices[ind*3+2]));
                }
                
            }else{
                for(int i = 0; i < _vertexCount; ++i ){
                    _positionsVector.push_back(Vec3f(_vertices[i*3], _vertices[i*3+1], _vertices[i*3+2]));
                }
            }
        }
        
        return _positionsVector;        
        
    }

    std::vector<ci::Vec3f> GeometryGenerator::getNormalsVector(){
        
        std::vector<Vec3f> normals;
        
        if(_faceNormals){
                        
            ci::Vec3f one, two, three, norm;
            
            getPositionsVector();
            
            for(int i = 0; i < _positionsVector.size(); i+=3){
                one = _positionsVector[i];
                two = _positionsVector[i+1];
                three = _positionsVector[i+2];
                
                norm = (two-one).cross(three-one);
                normals.push_back(norm); normals.push_back(norm); normals.push_back(norm);
            }
            
            return normals;
            
        }
        
        if(_splitFaces){
            
            register int ind;
            
            for(int i = 0; i < _indexCount; ++i ){
                ind = _indices[i];
                normals.push_back(Vec3f(_normals[ind*3], _normals[ind*3+1], _normals[ind*3+2]));
            }
            
        }else{
            
            for(int i = 0; i < _vertexCount; ++i ){
                normals.push_back(Vec3f(_normals[i*3], _normals[i*3+1], _normals[i*3+2]));
            }
            
        }
        
        return normals;
        
    }
    
    std::vector<u_int32_t> GeometryGenerator::getIndicesVector(){
        
        getPositionsVector();
    
        if(_indicesVector.size() == 0){
            if(_splitFaces || _faceNormals){
                            
                for(int i = 0; i < _positionsVector.size(); ++i ){
                    _indicesVector.push_back(i);
                }
                
            }else{
                for(int i = 0; i < _indexCount; ++i){
                    _indicesVector.push_back(_indices[i]);
                }
            }
            
            std::cout<<"INDICES:"<<_indicesVector.size()<<std::endl;
        }
        
        return _indicesVector;
    }
    
    std::vector<ci::Vec2f> GeometryGenerator::getUvsVector(){

        std::vector<Vec2f> uvs;
        
        if(_splitFaces || _faceNormals){
            
            register int ind;
            
            for(int i = 0; i < _indexCount; ++i ){
                ind = _indices[i];
                uvs.push_back(Vec2f(_uvs[ind*2], _uvs[ind*2+1]));
            }
            
        }else{
            
            for(int i = 0; i < _vertexCount; ++i ){
                uvs.push_back(Vec2f(_uvs[i*2], _uvs[i*2+1]));
            }
            
        }
        
        return uvs;

    }
    
    std::vector<ci::Vec3f> GeometryGenerator::getTriCentroids(bool i_perVertex){
        
        std::vector<Vec3f> centroids;
        
        if(i_perVertex)
            centroids.reserve(_indexCount);
        
        register int ind1, ind2, ind3;
        register float x, y, z;
        
        Vec3f cent;
        
        for(int i = 0; i < _indexCount; i+=3){
            ind1 = _indices[i]*3; ind2 = _indices[i+1]*3; ind3 = _indices[i+2]*3;
            x = (_vertices[ind1] + _vertices[ind2] + _vertices[ind3])/3.0;
            y = (_vertices[ind1+1] + _vertices[ind2+1] + _vertices[ind3+1])/3.0;
            z = (_vertices[ind1+2] + _vertices[ind2+2] + _vertices[ind3+2])/3.0;
            
            cent = Vec3f(x, y, z);
            
            if(i_perVertex){
                centroids.push_back(cent);
                centroids.push_back(cent);
                centroids.push_back(cent);

            }else{
                centroids.push_back(cent);
            }
            
        }
            
        return centroids;
        
    }
    
    std::vector<ci::Vec3f> GeometryGenerator::getNormalLines(float i_length){
        
        getPositionsVector();
        std::vector<Vec3f> norms = getNormalsVector();
        std::vector<Vec3f> output;
        
        for(int i = 0; i < _positionsVector.size(); ++i){
                        
            output.push_back(_positionsVector[i]);
            norms[i].normalize();
            norms[i] *= i_length;
            output.push_back(_positionsVector[i]+norms[i]);
        }
        
        return output;
        
    }
    
    std::vector<ci::Vec3f> GeometryGenerator::getBounds(){
        
        float minX = 100000000000, minY = 100000000000, minZ = 100000000000;
        float maxX = -100000000000, maxY = -100000000000, maxZ = -100000000000;
        
        for(int i = 0; i < _vertexCount; i+=3){
            minX = _vertices[i]   < minX ? _vertices[i] : minX;
            minY = _vertices[i+1] < minY ? _vertices[i+1] : minY;
            minZ = _vertices[i+2] < minZ ? _vertices[i+2] : minZ;

            maxX = _vertices[i]   > maxX ? _vertices[i] : maxX;
            maxY = _vertices[i+1] > maxY ? _vertices[i+1] : maxY;
            maxZ = _vertices[i+2] > maxZ ? _vertices[i+2] : maxZ;
        }
        
        Vec3f min(minX, minY, minZ);
        Vec3f max(maxX, maxY, maxZ);
        
        std::vector<Vec3f> vecOut;
        vecOut.push_back(min);
        vecOut.push_back(max);
        
        return vecOut;
    }
    
    
}}
