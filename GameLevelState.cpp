#include "GameLevelState.h"
#include <WIZ/xml.h>

#include <iostream>

GameLevelState::GameLevelState(SpriteBatch *batch, int level) {
	this->batch = batch;
	
	// load level
	XmlDocument doc;
	doc.load("resources/levels/level_" + std::to_string(level) + ".xml");
	std::cout <<"here after loaded!"<< std::endl;
	if (doc.getRoot() == nullptr)
		std::cout <<"here null root! "<< std::endl;
	for (auto it = doc.getRoot()->getChildren()->begin(); it != doc.getRoot()->getChildren()->end(); it++) {
		std::cout <<"here2"<< std::endl;
		XmlElement *elementx = *it;
		if (elementx->getTag() == "shoots") {
			std::cout <<"here"<< std::endl;
		} else if (elementx->getTag() == "layers") {
			for (int i = 0; i < elementx->getChildrenCount(); i++) {
				XmlElement *layer = *(elementx->getChildren()->begin() + i);
				for (int j = 0; j < layer->getChildrenCount(); j++) {
					XmlElement *artefact = *(layer->getChildren()->begin() + j);
					if (artefact->getTag() == "artefact") {
						std::string type = artefact->getString("type");
						if (type == "solid_block") {
							Sprite *block = new Sprite(WIZ::getTextureManager().loadTexture("resources/block_1.png"));
							this->rootNode.addChild(block);
							block->position.x = artefact->getFloat("x");
							block->position.y = artefact->getFloat("y");
							block->rotation = artefact->getFloat("rotation");
						}
					}
				}
			}
		}
	}
}

GameLevelState::~GameLevelState() {

}

void GameLevelState::update(float delta) {

}

void GameLevelState::draw() {
	batch->begin();
	rootNode.drawNode(*batch);
	batch->end();
}