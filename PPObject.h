#ifndef PP_OBJECT_H
#define PP_OBJECT_H

#include <cmath>
#include <Qt\qpainter.h>
#include <Qt\qvector2d.h>

class PPObject{

protected:
	const double PI;

	const double _radius;
	const double _mass;
	const double _max_speed;
	const double _max_force;
	const double _prediction_coeff;

	double _speed;

	QPointF* _position;
	QPointF* _futurePosition;
	QPointF* _target;

	QVector2D* _forward;
	QVector2D* _side;

	QVector2D* _steering;
	QVector2D* _correction;
	QVector2D* _velocity;

	QPolygonF* _sight;

	void updateFuturePosition() {                 
		(*_futurePosition) = QPoint(_position->x() + (_velocity->x() * _prediction_coeff), _position->y() + (_velocity->y() * _prediction_coeff));        
	} 

	void updateForces(void){
		if(_speed < _max_speed){
			_speed += 0.01 * _max_speed;
		}


		(*_steering) += truncate((*_correction), _max_force);
		_steering->normalize();

		/* Computing acceleration */
		QVector2D steeringForce(truncate((*_steering), _max_force));
		QVector2D acceleration(steeringForce);
		acceleration*=(1.0 / _mass);

		/* Computing velocity */
		QVector2D vForce(_velocity->toPointF());
		vForce += acceleration;

		(*_velocity) = truncate(vForce, _speed);

	}

	void computeTrajectoryCorrection(void) {
		(*_correction) = QVector2D(_target->x() - _futurePosition->x(), _target->y() - _futurePosition->y());
	}

	QVector2D truncate(QVector2D v, double max) {
		QVector2D vT(v);
		double l = vT.length();   
		if (l > max) {     
			vT *= (max/l);    
		}
		return vT;   
	} 

	void drawVector(QPainter* painter, QVector2D* v, const QColor& c, double scale) {
		painter->setBrush(c);
		painter->setPen(c);
		painter->drawLine((int) _position->x(), (int) _position->y(), (int) (_position->x() + (v->x() * scale)), (int) (_position->y() + (v->y() * scale)));
	}


	void drawPoint(QPainter* painter, QPointF* p, const QColor& c, double radius) {
		painter->setBrush(c);
		painter->setPen(c);
		painter->drawEllipse((int) (p->x() - (radius / 2.0)), (int) (p->y() - (radius / 2.0)), (int) radius, (int) radius);
	}


private:
	PPObject(const PPObject&) : PI(atan(1.0) * 4.0), _radius(15.0), _mass(1.0), _max_speed(1.0), _max_force(0.05), _prediction_coeff(60.0) {}

public:
	PPObject(void) : PI(atan(1.0) * 4.0), _radius(15.0), _mass(1.0), _max_speed(1.0), _max_force(0.05), _prediction_coeff(60.0){
		_speed = 0;

		_position = new QPointF(0, 0);
		_futurePosition = new QPointF(0, 0);
		_target = new QPointF(800, 600);

		_forward = new QVector2D(0, 0);
		_side = new QVector2D(0, 0);

		_steering = new QVector2D(0, 0);
		_correction = new QVector2D(0, 0);
		_velocity = new QVector2D(0, 0);

		_sight = new QPolygonF();
	}

	virtual ~PPObject(void){
		delete(_position);
		delete(_futurePosition);
		delete(_target);
		delete(_forward);
		delete(_steering);
		delete(_correction);
		delete(_velocity);
	}

	void setMousePosition(const QPoint& pos){
		setTarget(pos);
	}

	void setTarget(const QPoint& target){
		(*_target) = target;
	}

	void draw(QPainter* painter){
		//TEST
		painter->setBrush(Qt::blue);
		painter->drawEllipse((int) (_position->x() - (_radius / 2.0)), (int) (_position->y() - (_radius / 2.0)), (int) _radius, (int) _radius);
		
		/* Its sight rectangle*/
		painter->setBrush(Qt::red);
		painter->drawPolygon(*_sight);

		/* Its velocity */
		drawVector(painter, _velocity, QColor(255,0,255), 20.0);

		/* Its trajectory correction */
		drawVector(painter, _correction, QColor(0,255,0), 1.0);

		/* Its local space*/
		drawVector(painter, _forward, QColor(0,0,255), 20.0);
		drawVector(painter, _side, QColor(0,0,255), 20.0);

		/* Its future position and the corresponding point on the road */
		drawPoint(painter, _futurePosition, QColor(255,0,0), 8.0);
		drawPoint(painter, _target, QColor(0,0,0), 8.0);
		//END TEST
	}

	void update(void){
		(*_forward) = (*_velocity);
		_forward->normalize();

		(*_side) = QVector2D((_forward->x() * cos(PI / 2.0)) - (_forward->y() * sin(PI / 2.0)), (_forward->y()  * cos(PI / 2.0)) + (_forward->x()  * sin(PI / 2.0)));
		_side->normalize();

		updateForces();

		updateFuturePosition();
		computeTrajectoryCorrection();


		/* Updating position */
		(*_position) += (_velocity->toPointF());

		/* Updating sight */
		_sight->clear();
		(*_sight) << (QPointF(_position->x() + (_side->x() * (_radius / 2.0)), _position->y() + (_side->y() * (_radius / 2.0))));
		(*_sight) << (QPointF(_position->x() - (_side->x()* (_radius / 2.0)), _position->y() - (_side->y() * (_radius / 2.0))));
		(*_sight) << (QPointF(_futurePosition->x() - (_side->x() * (_radius / 2.0)), _futurePosition->y() - (_side->y() * (_radius / 2.0))));
		(*_sight) << (QPointF(_futurePosition->x() + (_side->x() * (_radius / 2.0)), _futurePosition->y() + (_side->y() * (_radius / 2.0))));
	}
};

#endif //PP_OBJECT_H