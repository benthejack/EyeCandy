//
//  ObjLoader.cpp
//  EyeCandyLib
//
//  Created by Ben Jack on 16/03/13.
//  Copyright (c) 2013 Ben Jack. All rights reserved.
//

#include <cstdio>

#include "cinder/stream.h"

#include "boost/algorithm/string/split.hpp"
#include "boost/algorithm/string/classification.hpp"

#include "ObjLoader.h"
#include "cinder/app/App.h"

#include "boost/date_time/posix_time/posix_time.hpp"



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
        
        boost::posix_time::ptime time = boost::posix_time::microsec_clock::local_time();
        long filesize = _file->size();
        char * charPtr = new char[filesize];
        

        boost::shared_array<u_int32_t> sIndicesT;
        boost::shared_array<u_int32_t> sIndicesN;
        
        _vertices = boost::shared_array<float>(new float[filesize/20]);
        _uvs      = boost::shared_array<float>(new float[filesize/20]);
        _normals  = boost::shared_array<float>(new float[filesize/20]);
        _indices  = boost::shared_array<u_int32_t>(new u_int32_t[filesize/20]);
        sIndicesT  = boost::shared_array<u_int32_t>(new u_int32_t[filesize/20]);
        sIndicesN  = boost::shared_array<u_int32_t>(new u_int32_t[filesize/20]);
        
        size_t sze = _file->readDataAvailable(charPtr, filesize);
        string fileString(charPtr, filesize);

        
        
        std::cout<<"strlen:"<<sze<<std::endl;
        
        boost::regex vSearch;
        boost::regex tSearch;
        boost::regex nSearch;
        boost::regex fSearch;
        
        std::string vertexSearch = "^v ([-+]?[0-9]+\.?[0-9]*) ([-+]?[0-9]+\.?[0-9]*) ([-+]?[0-9]+\.?[0-9]*)";

        std::string textureSearch = "^vt ([-+]?[0-9]+\.?[0-9]*) ([-+]?[0-9]+\.?[0-9]*)";
        
        std::string normalSearch = "^vn ([-+]?[0-9]+\.?[0-9]*) ([-+]?[0-9]+\.?[0-9]*) ([-+]?[0-9]+\.?[0-9]*)";
        
        std::string faceSearch = "^f ([0-9]+)/([0-9]*)/([0-9]*) ([0-9]+)/([0-9]*)/([0-9]*) ([0-9]+)/([0-9]*)/([0-9]*)";// try .* instead of (0-9)
        
        try
        {
            vSearch.assign(vertexSearch, boost::regex_constants::icase);
            tSearch.assign(textureSearch, boost::regex_constants::icase);
            nSearch.assign(normalSearch, boost::regex_constants::icase);
            fSearch.assign(faceSearch, boost::regex_constants::icase);
        }
        catch (boost::regex_error& e)
        {
            cout << " is not a valid regular expression: \""
            << e.what() << "\"" << endl;
        }
        
       
        _vertexCount = searchFloatsByRegex(fileString, vSearch, _vertices, 3);
        searchFloatsByRegex(fileString, tSearch, _uvs, 2);
        searchFloatsByRegex(fileString, nSearch, _normals, 3);
        
        string match, match2, match3;
        
        std::string::const_iterator start, end;
        boost::match_results<std::string::const_iterator> matches;
        boost::match_flag_type flags = boost::match_default;
        
        start = fileString.begin();
        end   = fileString.end();
        
        register int count = 0;
                
        while (boost::regex_search(start, end, matches, fSearch, flags))
        {

            for (int i = 1, j = 0; i < matches.size(); i+=3, ++j)
            {
                match = string(matches[i].first, matches[i].second);
                match2 = string(matches[i+1].first, matches[i+1].second);
                match3 = string(matches[i+2].first, matches[i+2].second);

                _indices[count*3 + j]  = atoi(match.c_str())-1;
                sIndicesT[count*3 + j] = atoi(match2.c_str())-1;
                sIndicesN[count*3 + j] = atoi(match3.c_str())-1;
            }
            
            start = matches[9].second;
            end = start + 80;
            ++count;
        }
    
        _indexCount = count*3;
    

        correctIndicesArray(_normals, sIndicesN, _indices, _indexCount, 3);
        correctIndicesArray(_uvs, sIndicesT, _indices,  _indexCount, 2);
        
        
        boost::posix_time::ptime time2 = boost::posix_time::microsec_clock::local_time();
        std::cout<<"get tex coord indices from strings time: "<<time2-time<<std::endl;
        time = time2;
        
                
        _generated = true;

    }
    
    int ObjGenerator::searchFloatsByRegex(string & i_file, boost::regex & i_expression, boost::shared_array<float> i_array, int i_stride){
        
        string match;
        
        std::string::const_iterator start, end;
        boost::match_results<std::string::const_iterator> matches;
        boost::match_flag_type flags = boost::match_default;
        
        start = i_file.begin();
        end   = i_file.end();
        
        register int count = 0;
        
        while (boost::regex_search(start, end, matches, i_expression, flags))
        {
            for (int i = 1; i < matches.size(); i++)
            {
                match = string(matches[i].first, matches[i].second);
                i_array[count*i_stride+(i-1)] = atof(match.c_str());
            }
                        
            start = matches[matches.size()-1].second;
            end = start + 120;
            ++count;
        }
        
        
        cout<<"vertCount:"<<count<<endl;
        return count;
        
    }
    
    
    
    
    void ObjGenerator::pullIndicesFromStrArray(boost::shared_array<u_int32_t> i_output, vector<string>& i_strs, int i_offset){
        
        
        vector<string> strsTest;
        boost::split(strsTest, i_strs[0], boost::is_any_of("/"));
        
        if(strsTest.size() >= i_offset){

            vector<string> runningTotal;
            
            for(vector<string>::iterator it = i_strs.begin(); it != i_strs.end(); ++it){
                
                vector<string> strs;
                boost::split(strs, *it , boost::is_any_of("/"));
                //std::cout<<"len:"<<strs.size()<<" 1:"<<strs[0]<<" 2:"<<strs[1]<<" 3:"<<strs[2]<<endl;
                runningTotal.insert(runningTotal.end(), strs.begin()+(i_offset-1), strs.begin()+i_offset);
                
            }
            
            int i = 0;
            for(vector<string>::iterator it = runningTotal.begin(); it != runningTotal.end(); ++it, ++i){
                i_output[i] = atoi(it->c_str())-1;
            }
            
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
    
    void ObjGenerator::correctIndicesArray(boost::shared_array<float>& i_input, boost::shared_array<u_int32_t>& i_wrongIndices, boost::shared_array<u_int32_t>& i_rightIndices, int i_indexCount, int i_step){
        
        boost::shared_array<float> tempArr = boost::shared_array<float>(new float[i_indexCount*3]);
        
        register int rInd, wInd, j;
        
        for(int i = 0; i < i_indexCount; ++i){
            rInd = (i_rightIndices[i]*i_step);
            wInd = (i_wrongIndices[i]*i_step);
            
           // cout<<"right:"<<rInd<<" wrong:"<<wInd<<endl;
            for(j = 0; j < i_step; ++j)
                tempArr[rInd+j] = i_input[wInd+j];

        }
        
        i_input = tempArr;
        
        
    }


    
    
    
    

    }
}