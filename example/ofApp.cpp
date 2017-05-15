#include "ofApp.h"


void ofApp::setup(){

	ofBackground(22);
	TIME_SAMPLE_ENABLE();
	TIME_SAMPLE_SET_AVERAGE_RATE(0.1);
	TIME_SAMPLE_SET_DRAW_LOCATION(TIME_SAMPLE_DRAW_LOC_BOTTOM_RIGHT);
	TIME_SAMPLE_SET_PRECISION(4);

	RUI_SETUP();
	RUI_SHARE_PARAM(debug);

	bool debugNanoVG = true;
	fonts.setup(debugNanoVG);

	fonts.addFont("Arial", "fonts/Arial Unicode.ttf");
	fonts.addFont("veraMono", "fonts/VeraMono.ttf");
	fonts.addFont("veraMonoBold", "fonts/VeraMono-Bold.ttf");
	fonts.addFont("Helvetica", "fonts/HelveticaNeue.ttf");

	ofxFontStashStyle style;
	style.fontID = "Arial";
	style.fontSize = 33;
	style.color = ofColor::yellow;
	fonts.addStyle("style1", style);

	style.color = ofColor::turquoise;
	fonts.addStyle("style2", style);

	style.color = ofColor::lightCyan;
	style.fontID = "veraMono";
	style.fontSize = 55;
	style.blur = 4;
	fonts.addStyle("style3", style);

	ofDisableAntiAliasing(); //to get precise lines
	fonts.pixelDensity = 2.0;

	#ifdef TEST_OFX_NANOVG_COMPAT
	ofxNanoVG::one().setup(true, true);
	ofxNanoVG::one().addFont("VeraMono", "fonts/VeraMono.ttf");
	#endif

	//test fallback font with emoji
	string emojiPath = "fonts/NotoEmoji-Regular.ttf";
	fonts.addFont("emoji-fallback", emojiPath);
	fonts.setGlobalFallbackFont("emoji-fallback");
}


void ofApp::draw(){

	float xx = 40;
	float yy = 40;
	int testID = 0;
	ofScale(0.9, 0.9);
	TSGL_START("d");

	string tMsg = ofToString(testID) + " draw";
	TS_START(tMsg);
	drawID(testID, xx, yy); testID++;
	testDraw(xx,yy);
	TS_STOP(tMsg);

	yy += 100;
	tMsg = ofToString(testID) + " testDrawColumn";
	TS_START(tMsg);
	drawID(testID, xx, yy); testID++;
	yy += testDrawColumn(xx,yy);
	TS_STOP(tMsg);

	yy += 20;
	tMsg = ofToString(testID) + " testDrawColumnNVG";
	TS_START(tMsg);
	drawID(testID, xx, yy); testID++;
	yy += testDrawColumnNVG(xx,yy);
	TS_STOP(tMsg);

	yy += 20;
	tMsg = ofToString(testID) + " testDrawTabs";
	TS_START(tMsg);
	drawID(testID, xx, yy); testID++;
	testDrawTabs(xx, yy);
	TS_STOP(tMsg);

	yy = 40;
	xx = 600;
	tMsg = ofToString(testID) + " testDrawFormatted";
	TS_START(tMsg);
	drawID(testID, xx, yy); testID++;
	testDrawFormatted(xx, yy);
	TS_STOP(tMsg);

	yy += 100;
	tMsg = ofToString(testID) + " testDrawFormattedColumn";
	TS_START(tMsg);
	drawID(testID, xx, yy); testID++;
	yy += testDrawFormattedColumn(xx, yy);
	TS_STOP(tMsg);

	yy += 100;
	tMsg = ofToString(testID) + " testDiyPlainLayout";
	TS_START(tMsg);
	drawID(testID, xx, yy); testID++;
	yy += testDiyPlainLayout(xx, yy);
	TS_STOP(tMsg);

	yy += 100;
	tMsg = ofToString(testID) + " testDiyFormattedLayout";
	TS_START(tMsg);
	drawID(testID, xx, yy); testID++;
	yy += testDiyFormattedLayout(xx, yy);
	TS_STOP(tMsg);

	TSGL_STOP("d");

	#ifdef TEST_OFX_NANOVG_COMPAT
	ofxNanoVG::one().beginFrame(ofGetWidth(), ofGetHeight(), 1.0);

	ofSetColor(255,0,0);
	ofxNanoVG::one().applyOFStyle();
	ofxNanoVG::one().rect(ofRectangle(0,0,200,200));
	ofxNanoVG::one().fillPath();

	ofSetColor(0,255,0);
	ofxNanoVG::one().applyOFStyle();
	ofxNanoVG::one().drawText("VeraMono", 20, 20, "test", 22);
	ofSetColor(0,0,255,128);
	ofDrawRectangle(100,100,200,200);
	ofSetColor(255,128);
	ofxNanoVG::one().applyOFStyle();
	ofxNanoVG::one().drawText("VeraMono", 20, 60, "test", 22);

	ofSetColor(255,255,0,128);
	ofDrawRectangle(200,200,200,200);

	ofxNanoVG::one().endFrame();
	#endif
}


void ofApp::testDraw(float x, float y){

	ofxFontStashStyle style;
	style.fontID = "Arial";
	style.fontSize = 22;
	{
		string text = "testDraw(): method with one-line string. バナナのようなサル";
		ofRectangle bounds = fonts.getTextBounds(text, style, x, y);
		ofSetColor(0,255,0,33);
		ofDrawRectangle(bounds);
		drawInsertionPoint(x,y,100);
		fonts.draw(text, style, x, y);
	}

	y += 30;

	{
		string text = "testDraw(): methods with\na multiline string (not supported).";
		ofRectangle bounds = fonts.getTextBounds(text, style, x, y);
		ofSetColor(0,255,0,33);
		ofDrawRectangle(bounds);
		drawInsertionPoint(x,y,100);
		fonts.draw(text, style, x, y);
	}
}


float ofApp::testDrawColumn(float x, float y){

	ofxFontStashStyle style;
	style.fontID = "Arial";
	style.fontSize = 45;
	ofAlignHorz align = getCurrentAlignment();

	string text = "testDrawColumn(): methods with a long string and no line breaks whatsoever. Also, adding some fancy unicode バナナのようなサル. And back to normal...";
	drawInsertionPoint(x,y,100);
	float colW = 300 + (0.5 + 0.5 * sin(ofGetElapsedTimef() * 0.33)) * 200;
	ofSetColor(255,33);
	float boxH = 300;
	ofDrawLine(x , y - 15, x, y + boxH);
	ofDrawLine(x + colW, y - 15, x + colW, y + boxH);
	ofRectangle bbox = fonts.drawColumn(text, style, x, y, colW, align, debug);
	ofSetColor(255,13);
	ofDrawRectangle(bbox);
	return bbox.height;
}

float ofApp::testDrawColumnNVG(float x, float y){

	ofxFontStashStyle style;
	style.fontID = "Arial";
	style.fontSize = 22;
	ofAlignHorz align = getCurrentAlignment();

	string text = "testDrawColumnNVG(): El català és una llengua de transició entre les llengües iberoromàniques i les llengües gal·loromàniques, encara que antigament fos molt pròxima a l'occità, amb qui comparteix origen i grup: l'occitanoromànic.";
	drawInsertionPoint(x,y,100);
	float colW = 300 + (0.5 + 0.5 * sin(ofGetElapsedTimef() * 0.33)) * 200;
	ofSetColor(255,33);
	float boxH = 300;
	ofDrawLine(x , y - 15, x, y + boxH);
	ofDrawLine(x + colW, y - 15, x + colW, y + boxH);
	fonts.drawColumnNVG(text, style, x, y, colW, align);
	ofRectangle bounds = fonts.getTextBoundsNVG(text, style, x, y, colW, align);
	ofSetColor(255,22);
	ofDrawRectangle(bounds);
	return bounds.height;
}


void ofApp::testDrawFormatted(float x, float y){

	string styledText = "<style id='style1'>testDrawFormatted(): this is style1.</style><style id='style2'>And this is Style 2</style>";
	float w = fonts.drawFormatted(styledText, x, y);
	drawInsertionPoint(x,y,w);
}


float ofApp::testDrawFormattedColumn(float x, float y){
	string styledText = "<style id='style1'>testDrawFormattedColumn(): This is style1 style.</style> <style id='style2'>And this is Style2, adjusted to the column width.</style>";
	styledText += " <style id='style3'>And this is stlye3, which has a bigger font size.</style>";
	drawInsertionPoint(x,y,100);
	float colW = 300 + (0.5 + 0.5 * sin(ofGetElapsedTimef() * 0.33)) * 200;
	ofSetColor(255,33);
	float boxH = 300;
	ofDrawLine(x , y - 15, x, y + boxH);
	ofDrawLine(x + colW, y - 15, x + colW, y + boxH);
	auto align = getCurrentAlignment();
	ofRectangle bbox = fonts.drawFormattedColumn(styledText, x, y, colW, align, debug);
	ofSetColor(255,16);
	ofDrawRectangle(bbox);
	return bbox.height;
}


void ofApp::testDrawTabs(float x, float y){

	string code =
	"//testDrawTabs():\n"
	"class ofApp : public ofBaseApp{\n"
	"	public:\n"
	"		void setup(); //some emoji too 😀💩🐙🐵\n"
	"		void update();\n"
	"}";

	ofxFontStashStyle style;
	style.fontID = "veraMono";
	style.fontSize = 16;
	style.color = ofColor::white;

	fonts.drawColumn(code, style, x, y, ofGetWidth(), OF_ALIGN_HORZ_LEFT, debug);
}

float ofApp::testDiyFormattedLayout(float x, float y){

	string myStyledText = "<style id='style1'>testDiyFormattedLayout(): hello</style><style id='style2'>banana</style><style id='style3'>monkey</style>";
	float colW = 300 + (0.5 + 0.5 * sin(ofGetElapsedTimef() * 0.33)) * 200;
	auto align = getCurrentAlignment();
	vector<StyledText> parsed = fonts.parseStyledText(myStyledText);
	vector<StyledLine> laidOutLines = fonts.layoutLines(parsed, colW, align);

	//we only want to draw whatever fits in 2 lines
	int numLines = 2;
	vector<StyledLine> drawnLines(laidOutLines.begin(), laidOutLines.begin() + MIN(numLines, laidOutLines.size()));

	ofRectangle bbox = fonts.drawLines(drawnLines, x, y);
	ofSetColor(255,16);
	ofDrawRectangle(bbox);
	return bbox.height;
}


float ofApp::testDiyPlainLayout(float x, float y){

	ofxFontStashStyle style = fonts.getStyles()["style1"];
	string text = "testDiyPlainLayout(): L'italiano è una tra le ventiquattro lingue ufficiali dell'Unione europea ed è lingua ufficiale dell'Italia, di San Marino, della Svizzera, della Città del Vaticano e del Sovrano Militare Ordine di Malta.";
	float colW = 300 + (0.5 + 0.5 * sin(ofGetElapsedTimef() * 0.33)) * 200;
	auto align = getCurrentAlignment();
	vector<StyledLine> laidOutLines = fonts.layoutLines({{text, style}}, colW, align);

	//we only want to draw whatever fits in 2 lines
	int numLines = 2;
	vector<StyledLine> drawnLines(laidOutLines.begin(), laidOutLines.begin() + MIN(numLines, laidOutLines.size()));

	ofRectangle bbox = fonts.drawLines(drawnLines, x, y);
	ofSetColor(255,32);
	ofDrawRectangle(bbox);
	return bbox.height;
}


void ofApp::keyPressed(int key){

	if(key == 'd'){
		debug ^= true;
		RUI_PUSH_TO_CLIENT();
	}
}



void ofApp::drawInsertionPoint(float x, float y, float w){
	ofSetColor((ofGetFrameNum() * 20)%255,200);
	ofDrawCircle(x,y, 1.5);
	ofSetColor(255,64);
	ofDrawLine(x - 10, y, x + w, y);
	ofSetColor(255);
}

void ofApp::drawID(int ID, int x, int y){

	ofSetColor(255,0,0);
	ofDrawBitmapStringHighlight(ofToString(ID), x - 15, y, ofColor::red * 0.5, ofColor::white);
	ofSetColor(255);
}


ofAlignHorz ofApp::getCurrentAlignment(){

	vector<ofAlignHorz> hor = {OF_ALIGN_HORZ_LEFT, OF_ALIGN_HORZ_CENTER, OF_ALIGN_HORZ_RIGHT};
	float pct = (ofGetFrameNum()%300 / 300.0f) ;
	int hIndex = (ofMap(pct, 0, 1, 0, hor.size()));

	hIndex = ofMap(ofGetMouseX(), 0, ofGetWidth(), 0, 3, true);
	return hor[MIN(hIndex, 2)];
}
