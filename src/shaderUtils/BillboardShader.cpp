//
//  GeometryGenerator.cpp
//  EyeCandyLib
//
//  Created by Ben Jack on 16/03/13.
//  Copyright (c) 2013 Ben Jack. All rights reserved.
//

#include "BillboardShader.h"
#include "cinder/Vector.h"
#include "StripGenerator.h"


using namespace ci;

namespace EyeCandy { namespace ShaderUtils{
    
    BillboardShader::BillboardShader(Vec2i i_size, Shader_ptr i_shader){
        
        _size = i_size;
        
        boost::shared_ptr<GeomUtils::TriStripGenerator> billboard = boost::shared_ptr<GeomUtils::TriStripGenerator>(new GeomUtils::TriStripGenerator(_size));
        _geomRend = GeomUtils::GeomRenderer_ptr(new GeomUtils::GeometryRenderer(billboard, i_shader));
        
    }
    
    
    }
}