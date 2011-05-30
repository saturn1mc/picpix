#ifndef PP_IMG
#define PP_IMG

#include <math.h>
#include <Qt\qimage.h>

using namespace std;

class PPImage : public QImage{
private:

	QRgb _avgRGB;

	PPImage() : QImage() { _avgRGB = 0; }
	PPImage(const PPImage& ppImage) : QImage(ppImage) { _avgRGB = ppImage._avgRGB; }

public:
	PPImage(const QString& filename, const QString& = 0) : QImage(filename) {
		_avgRGB = areaAverageRGB(this);
	}

	virtual ~PPImage(void) {
		QImage::~QImage();
	}

	static const QRgb& areaAverageRGB(const QImage* image){
		areaAverageRGB(image, 0, 0, image->width(), image->height());
	}

	static const QRgb& areaAverageRGB(const QImage* image, int x, int y, int w, int h){
		QRgb avgRGB = 0;

		for(int w = 0; w < image->width(); w++){
			for(int h = 0; h < image->height(); h++){
				avgRGB += image->pixel(w, h);
			}
		}

		avgRGB /= (image->width() * image->height());

		return avgRGB;
	}

	const QRgb& averageRGB(void) const {
		return _avgRGB;
	}

	double distanceTo(const PPImage& ppImage) const{
		return distanceTo(ppImage._avgRGB);
	}

	double distanceTo(const QRgb& qRGB) const{
		return pow(pow(qRed(qRGB) - qRed(_avgRGB), 2.0) + pow(qGreen(qRGB) - qGreen(_avgRGB), 2.0) + pow(qBlue(qRGB) - qBlue(_avgRGB), 2.0) + pow(qAlpha(qRGB) - qAlpha(_avgRGB), 2.0), 1.0/2.0);
	}
};

#endif //PP_IMG