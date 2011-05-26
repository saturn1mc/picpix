#ifndef PP_ELEMENT_H
#define PP_ELEMENT_H

#include <Qt\qpainter.h>
#include <Qt\qlist.h>

class PPElement{
private:
	QPolygon* _shape;
	QList<QRect*>* _bounds;

	bool _drawBounds;

	PPElement(const PPElement&) {}

public:
	PPElement(void) { 
		_drawBounds = true;
		_shape = new QPolygon();
		_bounds = new QList<QRect*>();
	}

	virtual ~PPElement(void){

		delete(_shape);

		for(QList<QRect*>::iterator boundIter = _bounds->begin(); boundIter!=_bounds->end(); boundIter++){
			delete(*boundIter);
		}

		_bounds->clear();
		delete(_bounds);
	}

	void setShape(QPolygon* shape){
		_shape = shape;
	}

	const QPolygon getShape(void) const{
		return (*_shape);
	}

	void enableBoundsDrawing(bool enable){
		_drawBounds = enable;
	}

	void addBound(QRect* bound){
		_bounds->append(bound);
	}

	QList<QRect*>* getBounds(void) const{ return _bounds; }
	
	virtual bool shouldAvoid(void) const { return false; }
	virtual bool shouldSeek(void) const { return false; }
	virtual bool shouldFollow(void) const { return false; }

	virtual void draw(QPainter* painter){ 
		if(_shape != 0){

			QColor shapeColor(100, 100, 100);

			painter->setBrush(shapeColor);
			painter->setPen(shapeColor);

			painter->drawPolygon(*_shape);
		}

		if(_bounds != 0 && _drawBounds){

			QColor boundColor(255, 0, 0, 100);

			painter->setBrush(boundColor);
			painter->setPen(boundColor);

			for(QList<QRect*>::iterator boundIter = _bounds->begin(); boundIter!=_bounds->end(); boundIter++){
				painter->drawEllipse(**boundIter);
			}
		}
	}

	virtual void update(void){
		//Nothing
	}
};

#endif //PP_ELEMENT_H