#include <Qt\qapplication.h>
#include "PPImageAnalyser.h"

int main(int argc, char* argv[]){
	QApplication app(argc, argv);
	
	PPImageAnalyser imgAnalyser;
	imgAnalyser.loadImages("D:\\Pictures\\Monochromes");

	PPImage image("D:\\Pictures\\Monochromes\\yellow.png");

	const PPImage* result = imgAnalyser.getClosestImageFor(&image);

	return app.exec();
}