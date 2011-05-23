#ifndef PP_WIDGET_H
#define PP_WIDGET_H

#include <Qt\qwidget.h>
#include <Qt\qpainter.h>
#include <Qt\qtimer.h>
#include <Qt\qevent.h>

#include "PPEnvironment.h"

class PPWidget : public QWidget {
	Q_OBJECT;

public:
	static const int DEFAULT_WIDTH;
	static const int DEFAULT_HEIGHT;
	static const int DEFAULT_TIMEOUT;

private:
	QTimer* _timer;
	PPEnvironment* _environment;

	PPWidget(void) {}
	PPWidget(const PPWidget&) {}

protected:
     void paintEvent(QPaintEvent* event);
	 void mouseMoveEvent(QMouseEvent* event);

public:
	PPWidget(PPEnvironment* environment);
	virtual ~PPWidget(void){
		delete(_environment);
	}

	QTimer* getTimer(void) const { return _timer; }

public slots:
	void update(void);
	void launchAnimation(void);
};

#endif //PP_WIDGET_H