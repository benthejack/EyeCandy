//
//  EyeCandyLib library, Ben Jack's personal creative coding library.
//
//  ObjLoader.h
//
//  Ben Jack 22/02/13
//
//--------------------------------------------------------------------
//
//    Classes for parsing .obj Files
//
//--------------------------------------------------------------------



#ifndef __EYECANDY_OBJLOADER_H_
#define __EYECANDY_OBJLOADER_H_


#include "GeometryGenerator.h"
#include <vector>
#include <boost/shared_array.hpp>
#include "boost/regex.hpp"

namespace EyeCandy{
    namespace GeomUtils{
        
        /************************************************/
        /*          base class for obj loading          */
        /************************************************/
        class ObjGenerator: public GeometryGenerator{
          
        public:
            //-------------CONSTRUCTOR/DESTRUCTOR-------------
            ObjGenerator(std::string i_assetPath);
            ~ObjGenerator(){};
                        
            //--------------------METHODS--------------------
            virtual void generate(bool i_yFlipped = true);

            
        private:
            
            int searchFloatsByRegex(std::string & i_file, boost::regex & i_expression, boost::shared_array<float> i_array, int i_stride);
            
            void pullIndicesFromStrArray(boost::shared_array<u_int32_t>, std::vector<std::string>&, int i_offset);
            void pullFloatsFromStrArray(boost::shared_array<float>, std::vector<std::string>&);
            void splitFaces(boost::shared_array<float> i_output, boost::shared_array<u_int32_t> i_inds, boost::shared_array<float> i_rawData, int i_size, int i_stride);
            void calculateFaceNormals(boost::shared_array<u_int32_t> i_indices, int i_indexCount, boost::shared_array<float> i_verts, boost::shared_array<float> i_normals);
            
            /* this is to rearrange the normal and uv arrays to point to the correct values. The OBJ file has 3 indices so these need to be crunched to be the same as the vertex
             position indices*/
            void correctIndicesArray(boost::shared_array<float>& i_input, boost::shared_array<u_int32_t>& i_wrongIndices, boost::shared_array<u_int32_t>& i_rightIndices, int i_indexCount, int i_step);
            
            cinder::IStreamFileRef _file;
            
        };
        
        
    }
}

#endif