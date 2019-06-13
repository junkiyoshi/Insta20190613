#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(2);
	
	this->fbo.allocate(ofGetWidth() + 25, ofGetHeight() + 25);
	this->fbo.readToPixels(this->pixels);
	this->pixels_mat = cv::Mat(this->pixels.getHeight(), this->pixels.getWidth(), CV_8UC4, this->pixels.getData());
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->fbo.begin();
	ofClear(0);

	for (int x = 200; x < ofGetWidth(); x += 360) {

		ofPushMatrix();
		ofTranslate(x, ofGetHeight() * 0.5 - 5);

		int len = 300;
		for (int i = 0; i < 8; i++) {

			vector<glm::vec2> vertices;
			int width = ofRandom(1, 3);

			float noise_seed_1 = ofRandom(1000);
			float noise_seed_2 = ofRandom(1000);

			int param_start = ofMap(ofNoise(noise_seed_1, ofGetFrameNum() * 0.003), 0, 1, 0, 100);
			for (int param = param_start; param <= param_start + width; param++) {

				vertices.push_back(this->make_point(len, param));
			}

			int tmp_param_start = param_start + ofMap(ofNoise(noise_seed_2, ofGetFrameNum() * 0.003), 0, 1, 25, 50);
			for (int param = tmp_param_start; param <= tmp_param_start + width; param++) {

				vertices.push_back(this->make_point(len, param));
			}

			ofFill();
			ofSetColor(39);
			ofBeginShape();
			ofVertices(vertices);
			ofEndShape(true);

			ofNoFill();
			ofSetColor(239);
			ofBeginShape();
			ofVertices(vertices);
			ofEndShape(true);
		}

		ofPopMatrix();
	}

	this->fbo.end();
	this->fbo.readToPixels(this->pixels);

	cv::GaussianBlur(this->pixels_mat, this->pixels_mat, cv::Size(19, 19), 10, 10);
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofSetColor(255, 192);
	ofImage draw_image;
	draw_image.setFromPixels(this->pixels);
	draw_image.draw(-5, -5);

	ofSetColor(255);
	this->fbo.draw(-25, -25);
}

//--------------------------------------------------------------
glm::vec2 ofApp::make_point(int len, int param) {

	param = param % 100;
	if (param < 25) {

		return glm::vec2(ofMap(param, 0, 25, -len * 0.5, len * 0.5), -len * 0.5);
	}
	else if (param < 50) {

		return glm::vec2(len * 0.5, ofMap(param, 25, 50, -len * 0.5, len * 0.5));
	}
	else if (param < 75) {

		return glm::vec2(ofMap(param, 50, 75, len * 0.5, -len * 0.5), len * 0.5);
	}
	else {

		return glm::vec2(-len * 0.5, ofMap(param, 75, 100, len * 0.5, -len * 0.5));
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}