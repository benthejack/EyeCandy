#include "LayerManager.h"
#include "cinder/app/App.h"

namespace EyeCandy{
    namespace GfxUtils{

        //------------------------------------LAYER-------------------------------------
        
        Layer::Layer(int i_width, int i_height, int i_msaaLevel){
            _width = i_width;
            _height = i_height;
            
            
            gl::Fbo::Format msaaFormat;
            
            if(i_msaaLevel > 0)
                msaaFormat.setSamples( i_msaaLevel );
            
            _fbo =  boost::shared_ptr<Fbo>(new Fbo(i_width, i_height, msaaFormat));
            _postPing = boost::shared_ptr<Fbo>(new Fbo(i_width, i_height, msaaFormat));
            _postPong = boost::shared_ptr<Fbo>(new Fbo(i_width, i_height, msaaFormat));
            _currentFbo = _fbo;
            
            _unPremult = false;

            
            try{
                _unPremultShader = Shader_ptr(new GlslProg( ci::app::loadAsset("shaders/passThru_vert.vs"), ci::app::loadAsset("shaders/UnPremult.fs")));
            } catch( const std::exception &e ) {
                cinder::app::console() << e.what() << std::endl;
            }
            
        }
        
        void Layer::addRenderable(Renderable_ptr i_renderable){
            _renderList.push_back(i_renderable);
        }
        
        void Layer::removeRenderable(Renderable_ptr i_renderable){
            
            for (unsigned i=0; i<_renderList.size(); ++i){
                if(_renderList[i] == i_renderable){
                    _renderList.erase(_renderList.begin()+i);
                    break;
                }
            }
            
        }
        
        
        void Layer::addPostPass(Shader_ptr i_shader, boost::function<void(Shader_ptr)> i_setterFunc){
            _postEffects.push_back(i_shader);
            _postShaderSetterFunctions.push_back(i_setterFunc);
        }
        
        void Layer::render(){
        
            _fbo->bindFramebuffer();
            
            gl::enableDepthWrite();
            gl::enableDepthRead();
            
            ci::gl::clear( ColorA( 0, 0, 0, 0) );

            if(_mainShader)
                _mainShader->bind();
            
            if(_glSetFunc)
                _glSetFunc();
           
            
            std::vector<Renderable_ptr>::iterator it = _renderList.begin();
            
            while(it != _renderList.end()){
                (*it)->render();
                ++it;
            }
            
            gl::disableDepthWrite();
            gl::disableDepthRead();
            
            if(_glPullDownFunc)
                _glPullDownFunc();
            
            if(_mainShader)
                _mainShader->unbind();
            
            _fbo->unbindFramebuffer();
        

            
            //----adding post effects----
            gl::color(1,1,1);
            if(_postEffects.size() > 0 || _unPremult){
                
                _currentFbo = _fbo;
                
                for( int i = 0; i <= _postEffects.size(); i++){
                    
                    if (_currentFbo == _fbo || _currentFbo == _postPong) {
                        _currentFbo == _fbo ? _fbo->bindTexture() : _postPong->bindTexture();
                        _currentFbo = _postPing;
                    }else{
                        _currentFbo = _postPong;
                        _postPing->bindTexture();
                    }
                    
                    _currentFbo->bindFramebuffer();
                    gl::clear(ColorA(0,0,0,0));
                    
                    //regular post effects
                    if(i != _postEffects.size()){
                        
                        _postEffects[i]->bind();
                        _postShaderSetterFunctions[i](_postEffects[i]);
                    
                        gl::drawSolidRect(Rectf(0,_height, _width, 0));
                    
                        _postEffects[i]->unbind();
                        
                    }else if(_unPremult){
                    //unpremult pass
                     //   cinder::app::console() << "blah"<<std::endl;

                        _unPremultShader->bind();
                        _unPremultShader->uniform("tex0", 0);
                            gl::drawSolidRect(Rectf(0,_height, _width, 0));
                        _unPremultShader->unbind();
        
                    }
                    
                    _currentFbo->unbindFramebuffer();
                    
                }
            }
            
            
                
        }
        
        
        
        
        //------------------------------------LAYER MANAGER-------------------------------------
        
        LayerManager::LayerManager(int i_width, int i_height, int i_msaaLevel){
            _width = i_width;
            _height = i_height;
            
            _msaaLevel = i_msaaLevel;
        }
        
        Layer_ptr LayerManager::pushLayer(){
            Layer_ptr newLayer = Layer_ptr(new Layer(_width, _height, _msaaLevel));
            _layers.push_back(newLayer);
            return newLayer;
        }
        
        Layer_ptr LayerManager::addLayerAt(int i_index){
            Layer_ptr newLayer = Layer_ptr(new Layer(_width, _height, _msaaLevel));
            _layers.insert(_layers.begin()+i_index, newLayer);
            return newLayer;
        }
        
        Layer_ptr LayerManager::getLayer(int i_layer){
            return _layers[i_layer];
        }
        
        void LayerManager::removeLayer(Layer_ptr i_del){

            for (unsigned i=0; i< _layers.size(); ++i){
                if(_layers[i] == i_del){
                    _layers.erase(_layers.begin()+i);
                    break;
                }
            }
            
        }
        
        void LayerManager::render(){
            
            gl::enableAlphaBlending(true);

            ci::gl::Texture currTex = _layers[0]->getFbo()->getTexture();
            Area bounds = currTex.getBounds();
            std::swap<int32_t>( bounds.y1, bounds.y2 );

            
            gl::color(1,1,1);
            
            for(int i = 0; i < _layers.size(); ++i){

                _layers[i]->render();

                //TODO implement layer blend modes;
                currTex =  _layers[i]->getFbo()->getTexture();
                

                currTex.enableAndBind();
                
                gl::pushMatrices();
                gl::draw(currTex, bounds);
                gl::popMatrices();
                
                currTex.disable();

                
            }
        
        
        
            gl::disableAlphaBlending();

            
        }

    }
}