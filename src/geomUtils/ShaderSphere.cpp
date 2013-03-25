//
//  ShaderSphere.cpp
//  EyeCandyLib
//
//  Created by Ben Jack on 16/03/13.
//  Copyright (c) 2013 Ben Jack. All rights reserved.
//

#include "ShaderSphere.h"

#include "cinder/gl/GLSLProg.h"
#include "cinder/app/App.h"

using namespace ci;
using namespace ci::gl;

namespace EyeCandy {

    namespace GeomUtils {

        ShaderSphere::ShaderSphere(float i_radius){
            
            _time = 0;
            _drawSphere = true;
            
            _shader =  ci::gl::GlslProg( ci::app::loadAsset("shaders/passThru_vert.vs"), ci::app::loadAsset("shaders/white.fs"));
           // _shader =  ci::gl::GlslProg( ci::app::loadAsset("shaders/passThru_vert.vs"), ci::app::loadAsset("shaders/white.fs"));
            
            VboMesh::Layout layout;
            layout.setStaticPositions();
            layout.setStaticIndices();
            layout.setStaticTexCoords2d();
            
            DiamondTriStripGenerator * billboard = new DiamondTriStripGenerator(Vec2f(i_radius*2, i_radius*2), Vec2i(1, 1));
            billboard->generate();
            _mesh = billboard->getVBO(layout);
                        
        }

        void ShaderSphere::draw(){
            
            _time += 0.1;
            
            gl::color(1,1,1);
            
            if(_drawSphere){
                _shader.bind();
                _shader.uniform("time", _time);
                _shader.uniform("lightPos", _lightDir);
            }
            
            gl::draw(*_mesh);
            
            if(_drawSphere)
                _shader.unbind();
            
        }

    }
    
}