#ifndef PP_BOT_H
#define PP_BOT_H

#include <cmath>
#include <Qt\qpainter.h>
#include <Qt\qvector2d.h>

class PPEnvironment;

class PPBot{

protected:
	const double PI;

	double _radius;
	double _slowing_distance;
	double _mass;
	double _max_speed;
	double _max_force;
	double _prediction_coeff;

	double _speed;

	PPEnvironment* _environment;

	QPointF* _position;
	QPointF* _futurePosition;
	QPointF* _target;

	QVector2D* _forward;
	QVector2D* _side;

	QVector2D* _steering;
	QVector2D* _correction;
	QVector2D* _velocity;

	QPolygonF* _sight;

	virtual void updateFuturePosition();
	virtual void updateForces(void);
	virtual void computeCorrections(void);

	QVector2D truncate(QVector2D v, double max);
	void drawVector(QPainter* painter, QVector2D* v, const QColor& c, double scale);
	void drawPoint(QPainter* painter, QPointF* p, const QColor& c, double radius);


private:
	PPBot(const PPBot&) : PI(atan(1.0) * 4.0), _radius(15.0), _slowing_distance(100), _mass(1.0), _max_speed(1.0), _max_force(0.05), _prediction_coeff(60.0) { _environment = 0; }

public:
	PPBot(void) : PI(atan(1.0) * 4.0), _radius(15.0), _mass(1.0), _slowing_distance(100), _max_speed(1.0), _max_force(0.05), _prediction_coeff(60.0){
		
		_environment = 0;
		
		_speed = 0;

		_position = new QPointF(0, 0);
		_futurePosition = new QPointF(0, 0);
		_target = new QPointF(0, 0);

		_forward = new QVector2D(0, 0);
		_side = new QVector2D(0, 0);

		_steering = new QVector2D(0, 0);
		_correction = new QVector2D(0, 0);
		_velocity = new QVector2D(0, 0);

		_sight = new QPolygonF();
	}

	virtual ~PPBot(void){
		delete(_position);
		delete(_futurePosition);
		delete(_target);
		delete(_forward);
		delete(_steering);
		delete(_correction);
		delete(_velocity);
	}

	virtual void setMousePosition(const QPoint&){
		//Nothing
	}

	void setEnvironment(PPEnvironment* environment){
		_environment = environment;
	}

	void setTarget(const QPointF& target){
		(*_target) = target;
	}

	const QPointF& getTarget(void) const{
		return (*_target);
	}

	void setPosition(const QPointF& position){
		(*_position) = position;
	}

	const QPointF getPosition(void) const{
		return (*_position);
	}

	const QVector2D getForward(void) const{
		return (*_forward);
	}

	virtual void draw(QPainter* painter);
	virtual void update(void);
};

#endif //PP_BOT_H