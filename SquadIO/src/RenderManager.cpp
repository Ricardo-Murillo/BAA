#pragma once
#include "include\RenderManager.h"


RenderManager RenderManager::renderManager;

RenderManager::RenderManager(){
	ID = 1;
}

RenderManager* RenderManager::getRenderManager(){
	return &renderManager;
}

SDL_Renderer* RenderManager::getRenderManagerRenderer(){
	RenderManager* manager = &renderManager;
	return manager->renderer;
}		

bool RenderManager::init(unsigned int width, unsigned int height, bool fullScreen, char* WindowTitle){
	SceneManager* sceneManager = SceneManager::GetSceneManager();
	if (SDL_Init(SDL_INIT_VIDEO) < 0){
		std::cout << "Error: Could not initialize SDL Render" << std::endl;
		return false;
	}
	else if (!fullScreen){
		std::cout << "not fullscreen" << std::endl;
	}
	//renderWindow = SDL_CreateWindow(WindowTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, fullScreen);
	SDL_CreateWindowAndRenderer(width, height, fullScreen, &renderWindow, &renderer);
	if (!renderWindow){
		//there was an error creating the window
		return false;
	}
	zoom = 1;
	minZoom = .05f;
	cameraPoint = { 0, 0 , -10};
	flippedScreen = false;
	//Get window surface
	SDL_Surface* screenSurface = SDL_GetWindowSurface(renderWindow);
	//Fill the surface white 
	SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0, 0, 0 ) ); 
	return true;
}

void RenderManager::update(){
	//clear screen
	windowSurface = SDL_GetWindowSurface(renderWindow);

	//Fill the surface white
	SDL_FillRect(windowSurface, NULL, SDL_MapRGB(windowSurface->format, 0, 0, 0));
	SDL_RenderClear(renderer);
	renderBackground();
	//interate through renderables, and generate the current frame
	renderAllObjects(); //SHOULD BE UPDATED TO BE RENDERSCENE

	SDL_UpdateWindowSurface(renderWindow);

	SDL_RenderPresent(renderer);

	//SDL_Delay(20); //needs to be taken out?
}

//TODO: this function is necessary, but we need a resource manager first
gameResource* RenderManager::loadResourceFromXML(tinyxml2::XMLElement *elem){
	if (elem){
		
		gameResource* resource = new RenderResource(); // This is uber important

		for (const tinyxml2::XMLAttribute* elementAttrib = elem->FirstAttribute(); elementAttrib; elementAttrib = elementAttrib->Next()){
			std::string AttribName = elementAttrib->Name();
			std::string AttribValue = elementAttrib->Value();
			if (AttribName == "UID"){                                                                                                                          
				resource->m_ResourceID = atoi(AttribValue.c_str());
			}
			if (AttribName == "filename"){
				resource->m_Filename = AttribValue;
			}
			if (AttribName == "scenescope"){
				resource->m_Scope = atoi(AttribValue.c_str());
			}
			if (AttribName == "width"){
				resource->width = atoi(AttribValue.c_str());
			}
			if (AttribName == "height"){
				resource->height = atoi(AttribValue.c_str());
			}
			if (AttribName == "max"){
				resource->max = atoi(AttribValue.c_str());
			}
		}
		return resource;
	}
	return NULL;
}


void RenderManager::setBackground(std::string filename){
	std::string path = "resources/" + filename; //append the folder name
	SDL_Surface *tempSurface = IMG_Load(path.c_str()); //load image as surface
	if (tempSurface){
		//if surface is loaded correctly, then make texture
		SDL_Texture*tempTexture = SDL_CreateTextureFromSurface(RenderManager::getRenderManagerRenderer(), tempSurface);
		//free old buffer
		SDL_FreeSurface(tempSurface);
		if (tempTexture){
			//if texture is made correctly, free old background data, and replace with new one
			if (background){
				SDL_DestroyTexture(background);
			}
			background = tempTexture;
		}
	}
	else{
		printf("Unable to load the image %s! SDL_image Error: %s\n", filename, IMG_GetError());
	}
}
void RenderManager::setBackground(SDL_Texture* texture){
	//std::string path = "resources/" + filename; //append the folder name
	//SDL_Surface *tempSurface = IMG_Load(path.c_str()); //load image as surface
	if (texture){
		background = texture;
	}
	else{
		printf("Unable to load the image %s! SDL_image Error: %s\n", IMG_GetError());
	}
}
float RenderManager::zoomRatio(float x1, float y1, float minSize, float scaling){
	float dist1 = sqrt(pow(x1 - cameraPoint.x, 2) + pow(y1 - cameraPoint.y, 2));//distance between center and (x1,y1)
	int wWidth = 0;
	int wHeight = 0;
	SDL_GetWindowSize(renderWindow, &wWidth, &wHeight);//width and height of the window
	float m = (y1 - cameraPoint.y) / (x1 - cameraPoint.x); //slope of the line that connects the center and (x1,y1)
	float mi = (x1 - cameraPoint.x) / (y1 - cameraPoint.y); //inverse of the slope, if one is #INF than the other is 0
	//what happens next depends on what quadrant the point is in, aka, which edge of the window is closest to (x1,y1)
	if (x1 < cameraPoint.x && abs(atan(m)) < abs(atan(wHeight / float(wWidth)))){
		//if we draw a line from the center to (x1,y1), then borderPoint is where that line crosses the edge of the window
		float borderPoint = m*(cameraPoint.x - (wWidth / 2)) + y1 - m*x1;
		float dist2 = sqrt(pow((cameraPoint.x - (wWidth / 2)) - cameraPoint.x, 2)
			+ pow(borderPoint - cameraPoint.y, 2));//distance between borderPoint and center
		return ((dist1 / dist2)*scaling)>minSize ? (dist1 / dist2)*scaling : minSize;
		//return the ratio of the two distances; a ratio <1 zooms out, >1 zooms in, ==1 requires no zooming in to see it
		//the minimum size determines how small it's allowed to go, as the closer a point is to the center,
		//the closer the ratio gets to #INF, so a minimum size is manditory
	}
	else if (y1 < cameraPoint.y && abs(atan(mi)) <= abs(atan(wWidth / float(wHeight)))){
		float borderPoint = mi*(cameraPoint.y - (wHeight / 2)) - mi*y1 + x1;
		float dist2 = sqrt(pow((cameraPoint.y - (wHeight / 2)) - cameraPoint.y, 2)
			+ pow(borderPoint - cameraPoint.x, 2));
		return ((dist1 / dist2)*scaling)>minSize ? (dist1 / dist2)*scaling : minSize;
	}
	else if (x1 > cameraPoint.x && abs(atan(m)) < abs(atan(wHeight / float(wWidth)))){
		float borderPoint = m*(cameraPoint.x + (wWidth / 2)) + y1 - m*x1;
		float dist2 = sqrt(pow((cameraPoint.x + (wWidth / 2)) - cameraPoint.x, 2)
			+ pow(borderPoint - cameraPoint.y, 2));
		return ((dist1 / dist2)*scaling)>minSize ? (dist1 / dist2)*scaling : minSize;
	}
	else if (y1 > cameraPoint.y && abs(atan(mi)) <= abs(atan(wWidth / float(wHeight)))){
		float borderPoint = mi*(cameraPoint.y + (wHeight / 2)) - mi*y1 + x1;
		float dist2 = sqrt(pow((cameraPoint.y + (wHeight / 2)) - cameraPoint.y, 2)
			+ pow(borderPoint - cameraPoint.x, 2));
		return ((dist1 / dist2)*scaling)>minSize ? (dist1 / dist2)*scaling : minSize;
	}
	return minSize;//if the point is at the center, than just return the minimum size
}
void RenderManager::renderBackground(){
	//to avoid using a null background
	if (zoom < minZoom){ zoom = minZoom; }
	if (background){
		SDL_Rect wholeWindow = { 0, 0, windowSurface->w, windowSurface->h};
		SDL_RenderCopy(renderer, background, NULL, &wholeWindow);
		/*//maybe invert z = 1/zoom
		float z = 1 / zoom;
		int textWidth;
		int textHeight;
		SDL_QueryTexture(background, NULL, NULL, &textWidth, &textHeight);
		//offset is for how the background tiles tile. it tells you the offset of the centermost tile 
		//it should give the illusion that the tiling begins at (0,0)
		float centerOffsetX = windowSurface->w / 2 - (int(cameraPoint.x) % textWidth)*z;
		float centerOffsetY = windowSurface->h / 2 - (int(cameraPoint.y) % textHeight)*z;
		float WorldTextWidth = textWidth* z;//the size of the texture after being multiplied by zoom
		float WorldTextHeight = textHeight* z;//>1 means zoom in, <1 means zoom out
		//tiling the image
		SDL_Rect srcrect = { 0, 0, textWidth, textHeight };
		SDL_Rect dstrect = { 0, 0, WorldTextWidth, WorldTextHeight };
		//float start = 0;
		//for (float x = centerOffsetX - ceil(centerOffsetX / (WorldTextWidth))*WorldTextWidth; x < windowSurface->w; x += dstrect.w){
		for (float x = 0; x < windowSurface->w; x += dstrect.w){
			//x = the offset - the number of times the background needs to be repeated from the offset point and (0,0) on the window and keep the background static
			dstrect.x = round(x);//rounding to make it less jagged
			if (x==0){
				dstrect.w = ceil(centerOffsetX - floor(centerOffsetX / (WorldTextWidth))*WorldTextWidth);
				if (dstrect.w == 0) dstrect.w = WorldTextWidth;
				srcrect.w = ceil(dstrect.w / z);
				srcrect.x = textWidth - srcrect.w;
			}
			else if (x + WorldTextWidth > windowSurface->w){
				dstrect.w = ceil(windowSurface->w - x);
				srcrect.x = 0;
				srcrect.w = ceil(dstrect.w/z);
			}
			else {
				dstrect.w = WorldTextWidth;
				srcrect.x = 0;
				srcrect.w = textWidth;
			}
			for (float y = centerOffsetY - ceil(centerOffsetY / (WorldTextHeight))*WorldTextHeight; y < windowSurface->h; y += dstrect.h){
				dstrect.y = round(y);
				if (y + WorldTextHeight > windowSurface->h){
					dstrect.h = ceil(windowSurface->h - y);
					srcrect.h = ceil(dstrect.h / z);
				}
				else {
					dstrect.h = WorldTextHeight;
					srcrect.h = textHeight;
				}
				SDL_RenderCopy(renderer, background, &srcrect, &dstrect);
			}
		}*/
	}
}

void RenderManager::worldCoordToWindowCoord(int &winx, int &winy, float worx, float wory, float worz){
	// make sure that worz does not equal cameraPoint.z
	float proj = -cameraPoint.z / (worz - cameraPoint.z);
	float flip = (flippedScreen) ? -1.0f : 1.0f;
	winx = int((worx - cameraPoint.x)*flip*proj/zoom + windowSurface->w / 2);
	winy = int((wory - cameraPoint.y)*flip*proj/zoom + windowSurface->h / 2);
}
void RenderManager::windowCoordToWorldCoord(float &worx, float &wory, int winx, int winy, float worz){
	//make sure that cameraPoint.z is not at 0
	float proj = (worz - cameraPoint.z) / (-cameraPoint.z);
	float flip = (flippedScreen) ? -1.0f : 1.0f;
	worx = (float(winx) - windowSurface->w / 2)*zoom*proj*flip + cameraPoint.x;
	wory = (float(winy) - windowSurface->h / 2)*zoom*proj*flip + cameraPoint.y;
}

void RenderManager::renderObjectAsRect(SDLRenderObject * obj){
	if (obj->getPosZ() > cameraPoint.z){
		//SDL_Rect pos;
		int posx = 0;
		int posy = 0;
		worldCoordToWindowCoord(posx, posy, obj->getPosX(),obj->getPosY());
		float anchorx = 0;
		float anchory = 0;
		float proj = -cameraPoint.z / (obj->posZ - cameraPoint.z); 
		if (flippedScreen){
			anchorx = 1- obj->getAnchorX();
			anchory = 1- obj->getAnchorY();
		}
		else{
			anchorx = obj->getAnchorX();
			anchory = obj->getAnchorY();
		}
		float w = obj->getWidth()*proj / zoom;
		float h = obj->getHeight()*proj / zoom;
		float r = obj->getRotation() * (float)( M_PI/180);
		//r *= (!(obj->flipH && obj->flipV)&& (obj->flipH || obj->flipV))? - 1: 1;
		//SDL_RenderDrawRect(renderer, &pos);

		SDL_RenderDrawLine(renderer,(int) (posx + (-w*anchorx)*cos(r) - (-h*anchory)*sin(r)),
									(int) (posy + (-w*anchorx)*sin(r) + (-h*anchory)*cos(r)),
									(int) (posx + (w*(1-anchorx))*cos(r) - (h*(1-anchory))*sin(r)),
									(int) (posy + (w*(1-anchorx))*sin(r) + (h*(1-anchory))*cos(r))	 );
		SDL_RenderDrawLine(renderer,(int) (posx + (w*(1 - anchorx))*cos(r) - (-h*anchory)*sin(r)),
								    (int) (posy + (w*(1 - anchorx))*sin(r) + (-h*anchory)*cos(r)),
									(int) (posx + (-w*anchorx)*cos(r) - (h*(1 - anchory))*sin(r)),
									(int) (posy + (-w*anchorx)*sin(r) + (h*(1 - anchory))*cos(r)));
		SDL_RenderDrawLine(renderer,(int) (posx + (-w*anchorx)*cos(r) - (-h*anchory)*sin(r)),
									(int) (posy + (-w*anchorx)*sin(r) + (-h*anchory)*cos(r)),
									(int) (posx + (-w*anchorx)*cos(r) - (h*(1 - anchory))*sin(r)),
									(int) (posy + (-w*anchorx)*sin(r) + (h*(1 - anchory))*cos(r)));
		SDL_RenderDrawLine(renderer,(int) (posx + (-w*anchorx)*cos(r) - (h*(1 - anchory))*sin(r)),
									(int) (posy + (-w*anchorx)*sin(r) + (h*(1 - anchory))*cos(r)),
									(int) (posx + (w*(1 - anchorx))*cos(r) - (h*(1 - anchory))*sin(r)),
									(int) (posy + (w*(1 - anchorx))*sin(r) + (h*(1 - anchory))*cos(r)));
		SDL_RenderDrawLine(renderer,(int) (posx + (w*(1 - anchorx))*cos(r) - (h*(1 - anchory))*sin(r)),
									(int) (posy + (w*(1 - anchorx))*sin(r) + (h*(1 - anchory))*cos(r)),
									(int) (posx + (w*(1 - anchorx))*cos(r) - (-h*anchory)*sin(r)),
									(int) (posy + (w*(1 - anchorx))*sin(r) + (-h*anchory)*cos(r)));
		SDL_RenderDrawLine(renderer,(int) (posx + (w*(1 - anchorx))*cos(r) - (-h*anchory)*sin(r)),
									(int) (posy + (w*(1 - anchorx))*sin(r) + (-h*anchory)*cos(r)),
									(int) (posx + (-w*anchorx)*cos(r) - (-h*anchory)*sin(r)),
									(int) (posy + (-w*anchorx)*sin(r) + (-h*anchory)*cos(r)));
	}
}
void RenderManager::renderObjectAsImage(SDLRenderObject * obj){
	if (obj->getPosZ() > cameraPoint.z){
		SDL_Rect pos;
		SDL_Point anchor;
		//transforms the world positions of the object to window position
		//if the screen is flipped, the math is a bit diffirent to accomadate it
		float proj = -cameraPoint.z / (obj->posZ - cameraPoint.z);
		if (flippedScreen){
			worldCoordToWindowCoord(pos.x, pos.y, obj->getPosX() + obj->getWidth()*(1 - obj->getAnchorX()), obj->getPosY() + obj->getHeight()*(1 - obj->getAnchorY()),obj->getPosZ());
			anchor = { int(obj->getWidth()*proj*(1 - obj->getAnchorX())/zoom), int(obj->getHeight()*proj*(1 - obj->getAnchorY())/zoom) };
		}
		else{
			worldCoordToWindowCoord(pos.x, pos.y, obj->getPosX() - obj->getWidth()*obj->getAnchorX(), obj->getPosY() - obj->getHeight()*obj->getAnchorY(), obj->getPosZ());
			anchor = { int(obj->getWidth()*proj*obj->getAnchorX()/zoom), int(obj->getHeight()*proj*obj->getAnchorY()/zoom) };
		}
		pos.w = (int) (obj->getWidth()*proj/zoom);
		pos.h = (int) (obj->getHeight()*proj/zoom);
		//uses the object's anchor value as a general position, and multiplies it with the proper w and h
		//flip the sprite based on some bool values
		SDL_RendererFlip flip = (flippedScreen) ? SDL_RendererFlip(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL) : SDL_FLIP_NONE;
		if (obj->isFlippedH()){ flip = (flippedScreen) ? SDL_FLIP_VERTICAL : SDL_FLIP_HORIZONTAL; }
		if (obj->isFlippedV()){ flip = (flippedScreen) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_VERTICAL; }
		if (obj->isFlippedH() && obj->isFlippedV()){
			flip = (flippedScreen) ? SDL_FLIP_NONE : SDL_RendererFlip(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
		}
		//SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL;
		//add the object to the render
		SDL_Rect rect = obj->getRenderRect();
		SDL_RenderCopyEx(renderer, obj->renderResource->mTexture, &rect, &pos, obj->getRotation(), &anchor, flip);
	}

}

bool sortRendObj(SDLRenderObject * lhs, SDLRenderObject * rhs){
	return lhs->posZ > rhs->posZ;
}

void RenderManager::renderAllObjects(){
	//NOTE: this list might need to be changed to be pointers
	//NOTE: May have to be based on a subset of renderobjects, not all of them
	if (zoom < minZoom){ zoom = minZoom; }
	float z = 1 / zoom; //maybe invert
	renderObjects.sort(sortRendObj);
	std::list<SDLRenderObject*>::iterator iter;
	for (iter = renderObjects.begin(); iter != renderObjects.end(); iter++){
		if ((*iter)->isVisible()){
			if ((*iter)->ifRenderImage) renderObjectAsImage((*iter));
			if ((*iter)->ifRenderRect) renderObjectAsRect((*iter));
		}
	}
}

void RenderManager::renderScene() { //will need modification to support more flags other than visible
	SceneManager* sceneManager = SceneManager::GetSceneManager();
	if (sceneManager) { //scenemanager needs to be filled somewhere else before running this function
		std::list<Layer*>::iterator i;
		for (i = sceneManager->m_Layers.begin(); i != sceneManager->m_Layers.end(); i++) {
			Layer* layer = *i;
			if (layer->m_Visible) {
				std::list<SDLRenderObject*>::iterator obj_it;
				for (obj_it = layer->m_SceneObjects.begin(); obj_it != layer->m_SceneObjects.end(); obj_it++){
					SDLRenderObject* obj = *obj_it;
					if (obj->visible) {
						obj->update();

						/*SDL_Rect Pos;
						Pos.x = int(layer->m_PosX) + int(obj->posX);
						Pos.y = int(layer->m_PosY) + int(obj->posY);
						SDL_Surface* renderSurface = SDL_GetWindowSurface(renderWindow);
						SDL_BlitSurface(obj->renderResource->mSurface, &obj->renderRect, renderSurface, &Pos); */

						//this update is a SpriteObject specific method for spritesheets
						//(*iter)->update();
						SDL_Rect pos;
						pos.x = int((*obj_it)->posX);
						pos.y = int((*obj_it)->posY);
						pos.w = (*obj_it)->renderRect.w;
						pos.h = (*obj_it)->renderRect.h;

						//TODO: replace NULL parameters with meaningful SDL_Rects
						//uses the object's anchor value as a general position, and multiplies it with the proper w and h
						SDL_Point anchor = { (int) ((*obj_it)->renderRect.w*(*obj_it)->anchor.x), (int) ((*obj_it)->renderRect.h*(*obj_it)->anchor.y) };
						SDL_RendererFlip flip = (flippedScreen) ? SDL_RendererFlip(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL): SDL_FLIP_NONE;
						if ((*obj_it)->flipH){ flip = (flippedScreen) ? SDL_FLIP_VERTICAL : SDL_FLIP_HORIZONTAL; }
						if ((*obj_it)->flipV){ flip = (flippedScreen) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_VERTICAL; }
						if ((*obj_it)->flipH && (*obj_it)->flipV){
							flip = (flippedScreen) ? SDL_FLIP_NONE: SDL_RendererFlip(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
						}
						//SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL;
						SDL_RenderCopyEx(renderer, (*obj_it)->renderResource->mTexture, NULL, &pos, (*obj_it)->rotation, &anchor, flip);
					}
				}
			}
		}
	}
}

void RenderManager::free(){
	SDL_DestroyWindow(renderWindow);
	SDL_DestroyRenderer(renderer);
	SDL_FreeSurface(windowSurface);
	//TODO: this obviously isn't a complete implementation of the free function
	std::list<SDLRenderObject*>::iterator iter;
	for (iter = renderObjects.begin(); iter != renderObjects.end(); iter++){
		(*iter)->renderResource->unload();
	}
}

bool RenderManager::isReadyToQuit(){
	//returns whether or not an SDL_QUIT event has been made
	//When we want to quit, we can push a SDL_QUIT from anywhere,
	//and the rendermanager will see it here and initiate program shutdown
	std::list<SDL_Event*>events;
	std::list<SDL_Event*>::iterator iter;
	SDL_Event event;
	while (SDL_PollEvent(&event)){
		events.push_back(&event);
		for (iter = events.begin(); iter != events.end(); iter++){
			switch ((*iter)->type){
			case SDL_QUIT: return true;
				//This case is just for debugging purposes for the moment
			case SDL_KEYDOWN:{
								 if (event.key.keysym.sym == SDLK_ESCAPE)return true;
			}
			}
		}
	}

	//No quit event was found, so put everything back into the event queue
	while (events.size() > 0){
		SDL_PushEvent(events.front());
		events.pop_front();
	}
	return false;
}


void RenderManager::setCameraZ(float z){
	cameraPoint.z = z;
}
void RenderManager::setCameraPoint(float x, float y){
	cameraPoint.x = x;
	cameraPoint.y = y;
}
void RenderManager::setCameraPoint(float x, float y, float z){
	setCameraPoint(x, y);
	setCameraZ(z);
}
