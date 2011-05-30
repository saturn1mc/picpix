#ifndef PP_MOSAIC_PAINTER
#define PP_MOSAIC_PAINTER

#include <limits>

#include <Qt\qdir.h>
#include <Qt\qpainter.h>
#include <Qt\qimagereader.h>

#include "PPImage.h"

class PPMosaicPainter{
private:

	int _tileWidth;
	int _tileHeight;

	int _precision;

	const QImage* _original;
	QList<PPImage*>* _images;

	PPMosaicPainter(void) {}
	PPMosaicPainter(const PPMosaicPainter&) {}

	void handleFile(const QFileInfo& fileInfo){
		if(!QImageReader::imageFormat(fileInfo.filePath()).isEmpty()){
			_images->push_back(new PPImage(fileInfo.filePath()));
		}
	}

	void handleDirectory(const QFileInfo& dirInfo){
		QDir subDir(dirInfo.path());
		subDir.setFilter(QDir::Files | QDir::Dirs | QDir::Hidden | QDir::NoSymLinks | QDir::NoDotAndDotDot);

		QFileInfoList list = subDir.entryInfoList();

		for (int i = 0; i < list.size(); ++i) {
			QFileInfo fileInfo = list.at(i);

			if(fileInfo.isDir()){
				handleDirectory(fileInfo);
			}
			else{
				handleFile(fileInfo);
			}
		}
	}

public:
	PPMosaicPainter(const QImage* original, int tileWidth = 30, int tileHeight = 30, int precision = 10) {
		_tileWidth = tileWidth;
		_tileHeight = tileHeight;
		_precision = precision;
		
		_original = original;
		_images = new QList<PPImage*>();
	}

	virtual ~PPMosaicPainter(void){
		for(QList<PPImage*>::iterator imgIter = _images->begin(); imgIter != _images->end(); imgIter++){
			delete(*imgIter);
		}

		_images->clear();
		delete(_images);
	}

	bool loadImages(const std::string& imageDirectory){
		QDir imgDir(imageDirectory.c_str());
		imgDir.setFilter(QDir::Files | QDir::Dirs | QDir::Hidden | QDir::NoSymLinks | QDir::NoDotAndDotDot);

		if(imgDir.exists()){

			QFileInfoList list = imgDir.entryInfoList();

			for (int i = 0; i < list.size(); ++i) {

				QFileInfo fileInfo = list.at(i);

				if(fileInfo.isDir()){
					handleDirectory(fileInfo);
				}
				else{
					handleFile(fileInfo);
				}
			}

			return true;
		}
		else{
			return false;
		}
	}

	const PPImage* getClosestImageFor(const PPImage* ppImage) const{
		return getClosestImageFor(ppImage->averageRGB());
	}

	const PPImage* getClosestImageFor(const QRgb& qRGB) const{
		
		double minDist = numeric_limits<double>::max();
		PPImage* closest = 0;
		
		for(QList<PPImage*>::const_iterator imgIter = _images->begin(); imgIter != _images->end(); imgIter++){
			double dist = (*imgIter)->distanceTo(qRGB);
			
			if(dist < minDist){
				minDist = dist;
				closest = (*imgIter);
			}
		}

		return closest;
	}

	const QImage& paintMosaic(void);
};

#endif //PP_MOSAIC_PAINTER