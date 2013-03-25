//
//  ObjLoader.cpp
//  EyeCandyLib
//
//  Created by Ben Jack on 16/03/13.
//  Copyright (c) 2013 Ben Jack. All rights reserved.
//

#include <cstdio>
#include "boost/algorithm/string/split.hpp"
#include "boost/algorithm/string/classification.hpp"

#include "ObjLoader.h"
#include "cinder/app/App.h"


using namespace ci;
using namespace std;

namespace EyeCandy { namespace GeomUtils{
    

    
    
    ObjGenerator::ObjGenerator(std::string i_assetPath){
        
        DataSourceRef ref = ci::app::loadAsset(i_assetPath);
        
        try{
            FILE * f = std::fopen(ref->getFilePath().c_str(), "r");
            _file = IStreamFile::create(f, true, 2048*10);
        }catch( const std::exception &e ) {
            cinder::app::console() << e.what() << std::endl;
            return;
        }
    
    }
    
    void ObjGenerator::generate(bool i_yFlipped){
        
        vector<string> sVertices;
        vector<string> sUvs;
        vector<string> sNormals;
        vector<string> sIndices;
        
        string str;
        vector<string> strs;
        
        while (!_file->isEof()) {
            str = _file->readLine();
            
            boost::split(strs,str,boost::is_any_of(" "));
            
            if(strs[0] == "v"){
                sVertices.insert(sVertices.end(), strs.begin()+1, strs.end());
            }else if(strs[0] == "vt"){
                sUvs.insert(sUvs.end(), strs.begin()+1, strs.end());
            }else if(strs[0] == "vn"){
                sNormals.insert(sNormals.end(), strs.begin()+1, strs.end());
            }else if(strs[0] == "f"){
                sIndices.insert(sIndices.end(), strs.begin()+1, strs.end());
            }
        }
        
        _indexCount = sIndices.size();
        _indices  = boost::shared_array<u_int32_t>(new u_int32_t[_indexCount]);
        boost::shared_array<u_int32_t> normalIndices = boost::shared_array<u_int32_t>(new u_int32_t[_indexCount]);
        boost::shared_array<u_int32_t> textureIndices = boost::shared_array<u_int32_t>(new u_int32_t[_indexCount]);
        
        pullIndicesFromStrArray(_indices, sIndices, 1);
        pullIndicesFromStrArray(textureIndices, sIndices, 2);
        pullIndicesFromStrArray(normalIndices, sIndices, 3);
        
        _vertexCount = sVertices.size()/3;
        _vertices = boost::shared_array<float>(new float[sVertices.size()]);
        _normals  = boost::shared_array<float>(new float[sNormals.size()]);
        _uvs      = boost::shared_array<float>(new float[sUvs.size()]);
            
        pullFloatsFromStrArray(_vertices, sVertices);
        pullFloatsFromStrArray(_normals,  sNormals);
        pullFloatsFromStrArray(_uvs, sUvs);
            
        correctIndicesArray(_normals, normalIndices, _indices, _indexCount);
        correctIndicesArray(_uvs, textureIndices, _indices,  _indexCount);
        
         //ci::app::console() << "verts:" << sVertices.size() << "  norms:"<< sNormals.size()<< "  tex:"<<sUvs.size()<< std::endl;
        
        _generated = true;

    }
    
    void ObjGenerator::pullIndicesFromStrArray(boost::shared_array<u_int32_t> i_output, vector<string>& i_strs, int i_offset){
        
        vector<string> runningTotal;
        
        
        for(vector<string>::iterator it = i_strs.begin(); it != i_strs.end(); ++it){
            
            vector<string> strs;
            boost::split(strs, *it , boost::is_any_of("/"));
            runningTotal.insert(runningTotal.end(), strs.begin()+(i_offset-1), strs.begin()+i_offset);
            
        }
        
        int i = 0;
        for(vector<string>::iterator it = runningTotal.begin(); it != runningTotal.end(); ++it, ++i){
            i_output[i] = atoi(it->c_str())-1;
        }
        
    }
    
    void ObjGenerator::pullFloatsFromStrArray(boost::shared_array<float> i_output, std::vector<std::string>& i_strs){
        
        int i = 0;
        for(vector<string>::iterator it = i_strs.begin(); it != i_strs.end(); ++it, ++i){
            i_output[i] = atof(it->c_str());
        }
    }

    void ObjGenerator::splitFaces(boost::shared_array<float> i_output, boost::shared_array<u_int32_t> i_inds, boost::shared_array<float> i_rawData, int i_size, int i_stride){
        
        register int i, j;
        
        for(i = 0; i < i_size; ++i){
            for(j = 0; j < i_stride; ++j){
                i_output[(i*i_stride)+j] = i_rawData[(i_inds[i]*i_stride)+j];
            }
        }
        
    }
    
    void ObjGenerator::correctIndicesArray(boost::shared_array<float>& i_input, boost::shared_array<u_int32_t>& i_wrongIndices, boost::shared_array<u_int32_t>& i_rightIndices, int i_indexCount){
        
        boost::shared_array<float> tempArr = boost::shared_array<float>(new float[i_indexCount*3]);
        
        register int rInd, wInd;
        
        for(int i = 0; i < i_indexCount; i++){
            rInd = (i_rightIndices[i]*3);
            wInd = (i_wrongIndices[i]*3);
            tempArr[rInd] = i_input[wInd];
            tempArr[rInd+1] = i_input[wInd+1];
            tempArr[rInd+2] = i_input[wInd+2];
        }
        
        i_input = tempArr;
        
        
    }


    
    
    
    

    }
}