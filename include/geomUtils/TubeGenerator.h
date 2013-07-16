//
//  EyeCandyLib library, Ben Jack's personal creative coding library.
//
//  TubeGenerator
//
//  Ben Jack 17/02/13
//
//--------------------------------------------------------------------
//
//    Classes for creating rectangular ribbons of vertices in various
//    arrangements.
//
//--------------------------------------------------------------------



#ifndef __EYECANDY_STRIPGENERATOR_H_
#define __EYECANDY_STRIPGENERATOR_H_

#include "GeometryGenerator.h"
#include "cinder/Vector.h"

using namespace ci;

namespace EyeCandy{
    namespace GeomUtils{
        
        /************************************************/
        /*         base class for a simple Tube         */
        /************************************************/
        
        class TubeGenerator: public GeometryGenerator{
            
        public:
            //-------------------------------CONSTRUCTORS/DESTRUCTOR-----------------------------
            TubeGenerator(float i_height, float i_radius, float i_thickness, Vec3i i_segments = Vec3i(10,10,1));
            ~TubeGenerator(){}
            
            
            //-----------------------------------GETTERS/SETTERS---------------------------------
            float radius() const    { return _radius; }
            float height() const    { return _height; }
            float thickness() const { return _radius; }
            
            Vec3i segments() const { return _segments; }
            
            void setSize(float i_height, float i_radius, float i_thickness){
                _radius = i_radius;
                _height = i_height;
                _thickness = i_thickness < i_radius ? i_thickness : i_radius;
            }
            
            //this takes a vec3i that is x=angular segments, y = height segments, z = thickness segments
            void setSegents(cinder::Vec3i i_segments){
                _segments = i_segments;
            }
            
            //---------------------------------------METHODS-------------------------------------
            
            //vertex allocation and initial positioning
            virtual void generate(bool i_yFlipped = true);
            
            
        protected:
            float _radius;
            float _height;
            float _thickness;
            
            ci::Vec3i _segments;
        };
        
    }
}

#endif