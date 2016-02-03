#include "SurfaceManager.h"

namespace ofx {
namespace piMapper {

SurfaceManager::SurfaceManager(){
	mediaServer = 0;
	selectedSurface = 0;
	ofAddListener(_surfaces.vertexChangedEvent, this, &SurfaceManager::onVertexChanged);
	ofAddListener(_surfaces.verticesChangedEvent, this, &SurfaceManager::onVerticesChanged);
}

void SurfaceManager::draw(){
	_surfaces.draw();
}

void SurfaceManager::addSurface(BaseSurface * surface){
	_surfaces.push_back(surface);
}

void SurfaceManager::removeSelectedSurface(){
	if(selectedSurface == 0){
		return;
	}
	
	for(int i = 0; i < _surfaces.size(); i++){
		if(_surfaces[i] == selectedSurface){
			_surfaces.erase(i);
			selectedSurface = 0;
			break;
		}
	}
}

void SurfaceManager::removeSurface(){
	if(_surfaces.size() <= 0){
		return;
	}
	delete _surfaces.back();
	_surfaces.pop_back();
}

void SurfaceManager::clear(){
	while(_surfaces.size()){
		delete _surfaces.back();
		_surfaces.pop_back();
	}
}

void SurfaceManager::saveXmlSettings(string fileName){
	if(mediaServer == 0){
		ofLogFatalError("SurfaceManager") << "Media server not set";
		exit(EXIT_FAILURE);
	}
	
	SettingsLoader::instance()->save(_surfaces, fileName);
}

bool SurfaceManager::loadXmlSettings(string fileName){
	if(mediaServer == 0){
		ofLogFatalError("SurfaceManager") << "Media server not set";
		exit(EXIT_FAILURE);
	}
	
	return SettingsLoader::instance()->load(_surfaces, *mediaServer, fileName);
}

void SurfaceManager::setMediaServer(MediaServer * newMediaServer){
	mediaServer = newMediaServer;
}

BaseSurface * SurfaceManager::selectSurface(int index){
	if(index >= _surfaces.size()){
		throw runtime_error("Surface index out of bounds.");
	}

	selectedSurface = _surfaces[index];
	ofSendMessage("surfaceSelected");
	return selectedSurface;
}

BaseSurface * SurfaceManager::selectSurface(BaseSurface * surface){
	for(int i = 0; i < _surfaces.size(); i++){
		if(_surfaces[i] == surface){
			selectedSurface = surface;
			ofSendMessage("surfaceSelected");
			ofNotifyEvent(surfaceSelectedEvent, i, this);
			return selectedSurface;
		}
	}
	
	deselectSurface();
	return 0;
}

BaseSurface * SurfaceManager::selectNextSurface(){
	int next;
	
	if(selectedSurface == 0){
		next = 0;
		selectedSurface = selectSurface(next);
		ofNotifyEvent(surfaceSelectedEvent, next, this);
		return selectedSurface;
	}
	
	for(int i = 0; i < _surfaces.size(); ++i){
		if(_surfaces[i] == selectedSurface){
			if(i < _surfaces.size() - 1){
				next = i + 1;
			}else{
				next = 0;
			}
			
			selectedSurface = _surfaces[next];
			ofNotifyEvent(surfaceSelectedEvent, next, this);
			return selectedSurface;
		}
	}
	
	return 0;
}

BaseSurface * SurfaceManager::selectPrevSurface(){
	int prev;
	
	if(selectedSurface == 0){
		prev = _surfaces.size() - 1;
		selectedSurface = selectSurface(prev);
		ofNotifyEvent(surfaceSelectedEvent, prev, this);
		return selectedSurface;
	}
	
	for(int i = 0; i < _surfaces.size(); ++i){
		if(_surfaces[i] == selectedSurface){
			if(i > 0){
				prev = i - 1;
			}else{
				prev = _surfaces.size() - 1;
			}
			
			selectedSurface = _surfaces[prev];
			ofNotifyEvent(surfaceSelectedEvent, prev, this);
			return selectedSurface;
		}
	}
	
	return 0;
}

BaseSurface * SurfaceManager::getSelectedSurface(){
	return selectedSurface;
}

void SurfaceManager::deselectSurface(){
	selectedSurface = 0;
}

BaseSurface * SurfaceManager::getSurface(int index){
	if(index >= _surfaces.size()){
		throw runtime_error("Surface index out of bounds.");
		return 0;
	}
	
	return _surfaces[index];
}

int SurfaceManager::size(){
	return _surfaces.size();
}

void SurfaceManager::onVertexChanged(ofVec3f & vertex){
	ofNotifyEvent(vertexChangedEvent, vertex, this);
}

void SurfaceManager::onVerticesChanged(vector<ofVec3f> & vertices){
	ofNotifyEvent(verticesChangedEvent, vertices, this);
}

} // namespace piMapper
} // namespace ofx
