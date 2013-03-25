//
//  EyeCandyLib library, Ben Jack's personal creative coding library.
//
//  StripGenerator.h
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
        /*    base class for a simple segmented plane   */
        /************************************************/
        class StripGenerator: public GeometryGenerator{
            
        public:
            //-------------------------------CONSTRUCTORS/DESTRUCTOR-----------------------------
            StripGenerator();
            ~StripGenerator(){}


            //-----------------------------------GETTERS/SETTERS---------------------------------
            Vec2f size() const   { return _size; }
                        
            Vec2i segments() const { return _segs; }
            
            
            
            void setSize(cinder::Vec2f i_size){
                _size = i_size;
            }
            
            void setSegents(cinder::Vec2i i_seg){
                _segs = i_seg;
            }
            
            
            //---------------------------------------METHODS-------------------------------------

            //vertex allocation and initial positioning
            virtual void generate(bool i_yFlipped = true);
            
            
        protected:
            Vec2f _size;
            Vec2i _segs;
        };
        
        
        class TriStripGenerator: public StripGenerator{
        
        public:
            
            //-------------------------------CONSTRUCTORS/DESTRUCTOR-----------------------------
            TriStripGenerator(Vec2i i_size = Vec2i(1,1), Vec2i i_segs = Vec2i(1,1));
            ~TriStripGenerator(){};
            
            //---------------------------------------METHODS-------------------------------------
            virtual void generate(bool i_yFlipped = true);
            
        };
        
        
        
        class DiamondTriStripGenerator: public StripGenerator{
        
        public:
            
            //-------------------------------CONSTRUCTORS/DESTRUCTOR-----------------------------
            DiamondTriStripGenerator(Vec2i i_size = Vec2i(1,1), Vec2i i_segs = Vec2i(1,1));
            ~DiamondTriStripGenerator(){};
            
            DiamondTriStripGenerator(const DiamondTriStripGenerator &cSource)
            {

            }
            
            //---------------------------------------METHODS-------------------------------------
            virtual void generate(bool i_yFlipped = true);
            
        };
        
        
        class StripPlaneCutter{
            
        };
    }
}

#endif