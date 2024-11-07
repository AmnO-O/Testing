#pragma once
#include "Constant.h"

std::mt19937 rd(time(0));

int random(int l, int r) {
	return l + rd() % (r - l + 1);
}

Figure::Figure(int t, Texture& path) {
	shape.setTexture(path);
	float scaleX = SIZE / shape.getGlobalBounds().width;
	float scaleY = SIZE / shape.getLocalBounds().height;
	float scale = min(scaleX, scaleY);
	shape.setScale(scale, scale);
	type = t; played = 0;
}

Figure::Figure() {

}

void Figure::SetPosition(int x, int y) {
	shape.setPosition(x, y);
}

void Figure::Setposition(Vector2i pos) {
	shape.setPosition(pos.x, pos.y);
}