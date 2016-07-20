/*
 * ScreenDrawing.cpp
 *
 *  Created on: 15 juil. 2016
 *      Author: ludovic
 */

#include "ScreenDrawing.h"

using namespace std;
using namespace libconfig;


ScreenDrawing::ScreenDrawing(){
	myScreenReference=NULL;
	myGlobalSettings=NULL;
	m_screenNeedRefresh=false;

	m_lastFontSizeUsed=0;
	maxSelectedMenuItem=0;

	refreshFunction=&ScreenDrawing::DrawMainScreen;
	myCurrentMenuPath.push_back("MainScreen");
	myCurrentSelectedMenuItem.push_back(0);
	m_tmpBoolValue=false;
	m_tmpFloatValue=0.0;
	m_tmpIntValue=0;
	m_tmpStringValue="";

}


ScreenDrawing::~ScreenDrawing(){

}





void ScreenDrawing::setScreenReference(SDL_Surface *screenReference){
	myScreenReference=screenReference;
}

void ScreenDrawing::setSettingsReference(Settings *globalSettingsReference){
	myGlobalSettings=globalSettingsReference;

	m_triggerLastHitValues.clear();

	// Init the vector with enough elements for each trigger input :
	for (unsigned int i=0; i<myGlobalSettings->getNumTriggerInputs();i++){
		m_triggerLastHitValues.push_back(0);
	}

}

void ScreenDrawing::setLastTriggerVelocity(unsigned int TriggerInput, unsigned int TriggerVelocity){
	m_triggerLastHitValues[TriggerInput]=TriggerVelocity;
}


void ScreenDrawing::DrawSplashScreen(){

	SDL_Color orange={255,162,0};

	DrawLabel("ur", 30,  100, 100 );
	DrawLabel("Drummer", 30, 128, 100, orange );
	DrawLabel("v0.1a", 10,  290, 225);

	// Draw the logo
	DrawIcon(myGlobalSettings->getUserDirectory() + "/.urDrummer/res/urdrummer_logo.gif", 140, 30, false);


    SDL_Flip(myScreenReference);

}

void ScreenDrawing::DrawIcon(const string iconPath, int posX, int posY,  bool selected){
	SDL_Surface *icon = NULL;
	SDL_Rect  positionIcon;

	positionIcon.x=posX;
	positionIcon.y=posY;

	icon = IMG_Load(iconPath.c_str());
	SDL_SetColorKey(icon, SDL_SRCCOLORKEY, SDL_MapRGB(icon->format, 0, 0, 0));
	SDL_BlitSurface(icon, NULL, myScreenReference, &positionIcon);

	delete icon;
}

void ScreenDrawing::DrawLabel( std::string labelText, int fontSize, int posX, int posY){
	SDL_Color white={255,255,255};
	DrawLabel(labelText, fontSize, posX, posY,  white, false);
}

void ScreenDrawing::DrawLabel( std::string labelText, int fontSize, int posX, int posY, bool selected=false){
	SDL_Color white={255,255,255};

	DrawLabel(labelText, fontSize, posX, posY, white, selected);
}


void ScreenDrawing::DrawLabel( std::string labelText, int fontSize, int posX, int posY, SDL_Color color){

	DrawLabel(labelText, fontSize, posX, posY, color, false);
}


void ScreenDrawing::DrawLabel( std::string labelText, int fontSize, int posX, int posY, SDL_Color color, bool selected=false){
	SDL_Rect position;
	SDL_Color invertColor;

	string fontPath = myGlobalSettings->getUserDirectory() + "/.urDrummer/res/arial.ttf";

	if (m_lastFontSizeUsed!=fontSize){
		if (m_lastFontSizeUsed!=0) TTF_CloseFont(myFont);
		// Open the font with the new size :
		m_lastFontSizeUsed=fontSize,
		myFont = TTF_OpenFont(fontPath.c_str(), fontSize);
	}

	SDL_Surface *texte ;
	position.x=posX;
	position.y=posY;



	if (selected){

		invertColor.b=255-color.b;
		invertColor.r=255-color.r;
		invertColor.g=255-color.g;

		texte = TTF_RenderText_Blended(myFont, labelText.c_str(), invertColor);


		SDL_Surface *BGSurf;
		BGSurf=SDL_CreateRGBSurface(SDL_HWSURFACE,texte->w, texte->h, 16,0,0,0,0);
		SDL_FillRect(BGSurf, NULL, SDL_MapRGB(BGSurf->format, color.r, color.g, color.b));
		SDL_BlitSurface(BGSurf, NULL, myScreenReference, &position); /* Blit background*/

		delete BGSurf;

	}else{
		texte = TTF_RenderText_Blended(myFont, labelText.c_str(), color);
	}

	SDL_BlitSurface(texte, NULL, myScreenReference, &position); /* Blit text */

	delete texte;

}


void ScreenDrawing::DrawLabelToList(SDL_Surface *dest, std::string labelText, int fontSize, int posX, int posY, SDL_Color color, bool selected=false){
	SDL_Rect position;
	SDL_Color invertColor;

	string fontPath = myGlobalSettings->getUserDirectory() + "/.urDrummer/res/arial.ttf";

	if (m_lastFontSizeUsed!=fontSize){
		if (m_lastFontSizeUsed!=0) TTF_CloseFont(myFont);
		// Open the font with the new size :
		m_lastFontSizeUsed=fontSize,
		myFont = TTF_OpenFont(fontPath.c_str(), fontSize);
	}

	SDL_Surface *texte ;
	position.x=posX;
	position.y=posY;



	if (selected){

		invertColor.b=255-color.b;
		invertColor.r=255-color.r;
		invertColor.g=255-color.g;

		texte = TTF_RenderText_Blended(myFont, labelText.c_str(), invertColor);


		SDL_Surface *BGSurf;
		BGSurf=SDL_CreateRGBSurface(SDL_HWSURFACE,dest->w, texte->h, 16,0,0,0,0);
		SDL_FillRect(BGSurf, NULL, SDL_MapRGB(BGSurf->format, color.r, color.g, color.b));
		SDL_BlitSurface(BGSurf, NULL, dest, &position); /* Blit background*/

		delete BGSurf;

	}else{
		texte = TTF_RenderText_Blended(myFont, labelText.c_str(), color);
	}

	SDL_BlitSurface(texte, NULL, dest, &position); /* Blit text */

	delete texte;

}



void ScreenDrawing::handleKeyPress(unsigned int keyEvent){

	string currentScreen;
	unsigned int currentSelectedItem;
	if (myCurrentMenuPath.size()>0){
		currentScreen=myCurrentMenuPath[myCurrentMenuPath.size()-1];
	}else{
		currentScreen="";
	}

	if (myCurrentSelectedMenuItem.size()>0){
		currentSelectedItem=myCurrentSelectedMenuItem[myCurrentSelectedMenuItem.size()-1];
	}else{
		currentSelectedItem=0;
	}

	cerr << "Received key event num " << keyEvent << endl;
	switch (keyEvent){
	case 112:
		// Handle play/pause action whenever we are..
		break;
	case 13:	// ENTER key
		if (currentScreen=="MainScreen"){
			// If we are on main screen, then we go to main menu
			myCurrentMenuPath.push_back("MainMenu");
			myCurrentSelectedMenuItem.push_back(0);
			maxSelectedMenuItem=5;

			// Save the function name to call it later if needed !
			refreshFunction=&ScreenDrawing::DrawMainMenu;
		}else if (currentScreen=="MainMenu"){
				myCurrentSelectedMenuItem.push_back(0);
				switch (currentSelectedItem){
				case 0:	// Kit Select
					myCurrentMenuPath.push_back("KitSelect");
					refreshFunction=&ScreenDrawing::DrawKitKitSelect;
					break;
				case 1: // Metronome
					myCurrentMenuPath.push_back("Metronome");
					refreshFunction=&ScreenDrawing::DrawMetronomeSetup;
					break;
				case 2:	// Audio Player
					myCurrentMenuPath.push_back("AudioPlayer");
					refreshFunction=&ScreenDrawing::DrawAudioPlayer;
					break;
				case 3:	// Training
					myCurrentMenuPath.push_back("Training");
					refreshFunction=&ScreenDrawing::DrawTrainingMenu;
					break;
				case 4: // Kit Setup
					myCurrentMenuPath.push_back("KitSetup");
					refreshFunction=&ScreenDrawing::DrawKitSetupMenu;
					break;
				case 5:	// Global Setup
					myCurrentMenuPath.push_back("GlobalSetup");
					refreshFunction=&ScreenDrawing::DrawGlobalSettingsMenu;
					break;
				}
		}else if (currentScreen=="KitSelect"){
			myGlobalSettings->loadDrumKit(myGlobalSettings->getDrumKitList()[currentSelectedItem]);
			// and exit to mainscreen (pop 2 times : 1 to go back to main menu, and 1 more to go back to main screen!)
			myCurrentMenuPath.pop_back();
			myCurrentSelectedMenuItem.pop_back();
			myCurrentMenuPath.pop_back();
			myCurrentSelectedMenuItem.pop_back();
			refreshFunction=&ScreenDrawing::DrawMainScreen;

		}else if(currentScreen=="Metronome"){
			// Manage value change !
			myCurrentMenuPath.push_back("MetronomeVal");
			myCurrentSelectedMenuItem.push_back(0);
		}else if (currentScreen=="MetronomeVal"){
			// Confirm value change
			myCurrentMenuPath.pop_back();
			myCurrentSelectedMenuItem.pop_back();
		}

		break;
	case 27:	// ESCAPE key
		if (currentScreen=="MainScreen"){
			// TODO : what to do with "escape key" when on main screen ?
			refreshFunction=&ScreenDrawing::DrawMainScreen;
		}else{

			cerr << "previousMenu is " << currentScreen << endl;
			cerr << "previousSel is " << currentScreen << endl;

			myCurrentMenuPath.pop_back();
			myCurrentSelectedMenuItem.pop_back();

			currentScreen=myCurrentMenuPath[myCurrentMenuPath.size()-1];

			cerr << "New CurrentScreen is " << currentScreen << endl;


			if (currentScreen=="MainScreen"){
				// Go back to previous menu :
				// Save the function name to call it later if needed !
				refreshFunction=&ScreenDrawing::DrawMainScreen;
			}else if (currentScreen=="MainMenu"){
				// Save the function name to call it later if needed !
				refreshFunction=&ScreenDrawing::DrawMainMenu;
			}

		}
		break;
	case 276:	// LEFT key
		if (currentScreen=="MainScreen"){
			// If we are on main screen, then we handle special action (
			// TODO : handle special action from main screen
			myGlobalSettings->DecMetronomeBpm(5);

		}else if (currentScreen=="MetronomeVal"){
				switch(myCurrentSelectedMenuItem[myCurrentSelectedMenuItem.size()-2]){
					case 0:
						myGlobalSettings->DecMetronomeBpm(1);
						break;
					case 1:
						m_tmpIntValue=myGlobalSettings->getMetronomeVolume();
						if (m_tmpIntValue>1){
							myGlobalSettings->setMetronomeVolume(--m_tmpIntValue);
						}
						break;
					case 2:
						m_tmpIntValue=myGlobalSettings->getMetronomeBCount();
						if (m_tmpIntValue>0){
							myGlobalSettings->setMetronomeBCount(--m_tmpIntValue);
						}
						break;
					case 3:
						myGlobalSettings->setMetronomeOn(!myGlobalSettings->isMetronomeOn());
						break;
				}
			}else{
				if (currentSelectedItem > 0){
					myCurrentSelectedMenuItem[myCurrentSelectedMenuItem.size()-1]--;

				}
		}
		break;
	case 275:	// RIGHT key
		if (currentScreen=="MainScreen"){
			// If we are on main screen, then we handle special action (
			// TODO : handle special action from main screen
			myGlobalSettings->IncMetronomeBpm(5);
		}else if (currentScreen=="MetronomeVal"){
			switch(myCurrentSelectedMenuItem[myCurrentSelectedMenuItem.size()-2]){
				case 0:
					myGlobalSettings->IncMetronomeBpm(1);
					break;
				case 1:
					m_tmpIntValue=myGlobalSettings->getMetronomeVolume();
					if (m_tmpIntValue<32){
						myGlobalSettings->setMetronomeVolume(m_tmpIntValue+1);
					}
					break;
				case 2:
					m_tmpIntValue=myGlobalSettings->getMetronomeBCount();
					if (m_tmpIntValue<32){
						myGlobalSettings->setMetronomeBCount(m_tmpIntValue+1);
					}
					break;
				case 3:
					myGlobalSettings->setMetronomeOn(!myGlobalSettings->isMetronomeOn());
					break;
			}
		}
		else {
			if (currentSelectedItem < maxSelectedMenuItem){
				myCurrentSelectedMenuItem[myCurrentSelectedMenuItem.size()-1]++;
			}
		}
		break;
	}


	// Call the function to draw the screen :
	(*this.*refreshFunction)();

	cerr << "Current menu (" << myCurrentMenuPath.size() << ") is " << myCurrentMenuPath[myCurrentMenuPath.size()-1] << endl;


}

void ScreenDrawing::DrawMainScreen(){
	/*
	 * Main screen shows :
	 * 	- current Kit Name
	 * 	- current BPM
	 * 	- Loaded audio file name
	 * 	- Position in playing track
	 * 	- Last Hit pad and velocity
	 */
	string kitName;
	string Vol;

	// Clean the screen
	SDL_FillRect(myScreenReference, NULL, SDL_MapRGB(myScreenReference->format, 0, 0, 0));

	// An horizontal line on top and bottom:
	line(myScreenReference, 0, 20, 319, 20, SDL_MapRGB(myScreenReference->format, 255, 255, 255));
	line(myScreenReference, 0, 220, 319, 220, SDL_MapRGB(myScreenReference->format, 255, 255, 255));
	// An vertical line on top and bottom, to split the zone:
	line(myScreenReference, 100, 0, 100, 20, SDL_MapRGB(myScreenReference->format, 255, 255, 255));
	line(myScreenReference, 100, 220, 100, 240, SDL_MapRGB(myScreenReference->format, 255, 255, 255));

	// Kit Name :
	if (myGlobalSettings!=NULL && myGlobalSettings->getCurrentDrumKit() != NULL){
		// Kit name in middle of the screen:
		kitName=myGlobalSettings->getCurrentDrumKit()->getKitName();
	}else{
		kitName="(no Kit loaded)";
	}

	DrawLabel(kitName, 30, 0, 22, false );

	// Metronome BPM : bottom left
	std::stringstream sbpm;
	sbpm << "BPM : " << myGlobalSettings->getMetronomeBpm();
	DrawLabel(sbpm.str(), 18, 2, 220, false );

	// Volume : top left
	std::stringstream svol;
	svol<< "Vol : " << myGlobalSettings->getVolume();
	DrawLabel(svol.str() , 18, 2, 0, false );

	// Last trigger Hits for each input :


    // finally, update the screen :)
    SDL_Flip(myScreenReference);

}

void ScreenDrawing::DrawMainMenu(){

	unsigned int currentSel=myCurrentSelectedMenuItem[myCurrentSelectedMenuItem.size()-1];

	// Clean the screen
	SDL_FillRect(myScreenReference, NULL, SDL_MapRGB(myScreenReference->format, 0, 0, 0));

	// First line of icons
	DrawIcon(myGlobalSettings->getUserDirectory() + "/.urDrummer/res/kitsel.gif",44, 20, false);
	DrawIcon(myGlobalSettings->getUserDirectory() + "/.urDrummer/res/metronome.gif",136, 20, false);
	DrawIcon(myGlobalSettings->getUserDirectory() + "/.urDrummer/res/audioplayer.gif",228, 20, false);

	// Labels
	DrawLabel("Kit Sel." , 18, 34, 70, (currentSel==0?true:false));
	DrawLabel("Metronome" , 18, 106, 70, (currentSel==1?true:false));
	DrawLabel("Audio Play" , 18, 208, 70, (currentSel==2?true:false));


	// Second line of icons
	DrawIcon(myGlobalSettings->getUserDirectory() + "/.urDrummer/res/training.gif",44, 130, false);
	DrawIcon(myGlobalSettings->getUserDirectory() + "/.urDrummer/res/kitsetup.gif",136, 130, false);
	DrawIcon(myGlobalSettings->getUserDirectory() + "/.urDrummer/res/settings.gif",228, 130, false);
	DrawLabel("Trainer" , 18, 34, 180, (currentSel==3?true:false));
	DrawLabel("Kit Setup" , 18, 126, 180, (currentSel==4?true:false));
	DrawLabel("Settings" , 18, 223, 180, (currentSel==5?true:false));

    // finally, update the screen :)
    SDL_Flip(myScreenReference);

}


void ScreenDrawing::DrawKitKitSelect(){

	std::vector<std::string> ListOfKits;
	unsigned int selItem;

	// Clean the screen
	SDL_FillRect(myScreenReference, NULL, SDL_MapRGB(myScreenReference->format, 0, 0, 0));

	DrawLabel("Kit Select" , 18, 110, 0, false);
	line(myScreenReference, 0, 20, 319, 20, SDL_MapRGB(myScreenReference->format, 255, 255, 255));

	maxSelectedMenuItem= myGlobalSettings->getDrumKitList().size()-1;

	// Draw the list of available drumkits :
	for(unsigned int i=0;i <= maxSelectedMenuItem;i++){
		ListOfKits.push_back(myGlobalSettings->getDrumKitList()[i]->getKitName());
	}

	if (myCurrentSelectedMenuItem.size()>0){
		selItem=myCurrentSelectedMenuItem[myCurrentSelectedMenuItem.size()-1];
	}else{
		selItem=0;
	}

	DrawList(ListOfKits, 0, 21, 320, 219, selItem);


    // finally, update the screen :)
    SDL_Flip(myScreenReference);

}

void ScreenDrawing::DrawMetronomeSetup(){

	vector<string> settingsList;
	vector<string> valueList;
	unsigned int selItem;
	bool valChange;

	settingsList.push_back("BPM");
	settingsList.push_back("Volume");
	settingsList.push_back("B. Count");
	settingsList.push_back("On/Off");


	std::stringstream myItoA;
	myItoA << myGlobalSettings->getMetronomeBpm();
	valueList.push_back(myItoA.str());

	myItoA.str("");
	myItoA << myGlobalSettings->getMetronomeVolume();
	valueList.push_back(myItoA.str());

	myItoA.str("");
	myItoA << myGlobalSettings->getMetronomeBCount();
	valueList.push_back(myItoA.str());

	if (myGlobalSettings->isMetronomeOn()){
		valueList.push_back("On");
	}else{
		valueList.push_back("Off");
	}

	// Clean the screen
	SDL_FillRect(myScreenReference, NULL, SDL_MapRGB(myScreenReference->format, 0, 0, 0));

	DrawLabel("Metronome" , 18, 110, 0, false);
	line(myScreenReference, 0, 20, 319, 20, SDL_MapRGB(myScreenReference->format, 255, 255, 255));


	// Draw the list of available settings :
	maxSelectedMenuItem= settingsList.size()-1;

	if (myCurrentMenuPath[myCurrentMenuPath.size()-1]=="MetronomeVal"){
		selItem=myCurrentSelectedMenuItem[myCurrentSelectedMenuItem.size()-2];
	}else{
		selItem=myCurrentSelectedMenuItem[myCurrentSelectedMenuItem.size()-1];
	}

	DrawList(settingsList, 0, 21, 100, 219, selItem);
	// Draw a line at right of the list :
	line(myScreenReference, 100, 20, 100, 239, SDL_MapRGB(myScreenReference->format, 255, 255, 255));

	// Draw the current setting on right :
	DrawLabel(settingsList[selItem] , 30, 160, 85, false);

	if (myCurrentMenuPath[myCurrentMenuPath.size()-1]=="MetronomeVal"){
		valChange=true;
	}else{

		valChange=false;
	}
	// Write the corresponding value under the label :
	DrawLabel(valueList[selItem] , 30, 160, 120, valChange);


    // finally, update the screen :)
    SDL_Flip(myScreenReference);

}

void ScreenDrawing::DrawAudioPlayer(){

	// Clean the screen
	SDL_FillRect(myScreenReference, NULL, SDL_MapRGB(myScreenReference->format, 0, 0, 0));

	DrawLabel("Audio Player" , 18, 110, 0, false);
	line(myScreenReference, 0, 20, 319, 20, SDL_MapRGB(myScreenReference->format, 255, 255, 255));


    // finally, update the screen :)
    SDL_Flip(myScreenReference);

}

void ScreenDrawing::DrawTrainingMenu(){

	// Clean the screen
	SDL_FillRect(myScreenReference, NULL, SDL_MapRGB(myScreenReference->format, 0, 0, 0));

	DrawLabel("Training" , 18, 120, 0, false);
	line(myScreenReference, 0, 20, 319, 20, SDL_MapRGB(myScreenReference->format, 255, 255, 255));


    // finally, update the screen :)
    SDL_Flip(myScreenReference);

}

void ScreenDrawing::DrawKitSetupMenu(){

	// Clean the screen
	SDL_FillRect(myScreenReference, NULL, SDL_MapRGB(myScreenReference->format, 0, 0, 0));

	DrawLabel("Kit Setup" , 18, 120, 0, false);
	line(myScreenReference, 0, 20, 319, 20, SDL_MapRGB(myScreenReference->format, 255, 255, 255));


    // finally, update the screen :)
    SDL_Flip(myScreenReference);

}

void ScreenDrawing::DrawGlobalSettingsMenu(){

	// Clean the screen
	SDL_FillRect(myScreenReference, NULL, SDL_MapRGB(myScreenReference->format, 0, 0, 0));

	DrawLabel("Global Settings" , 18, 100, 0, false);
	line(myScreenReference, 0, 20, 319, 20, SDL_MapRGB(myScreenReference->format, 255, 255, 255));


    // finally, update the screen :)
    SDL_Flip(myScreenReference);

}

void ScreenDrawing::DrawList(std::vector<std::string> listText, int x, int y, int w, int h, unsigned int selectedItem){
	SDL_Surface *ListSurface;
	SDL_Surface *Container;
	SDL_Color white={255,255,255};
	SDL_Rect position;
	SDL_Rect positionList;

	position.x=x;
	position.y=y;

	// We center the list on the middle : the selected item is always on center .
	positionList.x=0;
	positionList.y=(h/2)-(selectedItem*20);

	unsigned int i; // List index
	unsigned int listHeight;
	// With a 18 font size, each line should be about 20 pixel high.
	listHeight=listText.size()*20;

	//Create the container surface :
	Container=SDL_CreateRGBSurface(SDL_HWSURFACE,w, h, 16,0,0,0,0);


	// Create the receiving surface :
	ListSurface=SDL_CreateRGBSurface(SDL_HWSURFACE,w, listHeight, 16,0,0,0,0);

	// Fill the receiving surface :
	for (i=0; i<listText.size(); i++){
		DrawLabelToList(ListSurface, listText[i],18,0,i*20,white,(selectedItem==i?true:false));

	}

	SDL_BlitSurface(ListSurface, NULL, Container, &positionList); /* Blit List to the container*/
	SDL_BlitSurface(Container, NULL, myScreenReference, &position); /* Blit container to screen*/



	delete ListSurface;
	delete Container;

}


void ScreenDrawing::RefreshScreen(){
	// Call the function to draw the screen :
	(*this.*refreshFunction)();

	// Fall off the peaks of histogram :

}