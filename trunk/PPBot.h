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

	QRectF* _boundingBox;
	QPolygonF* _sight;
	QPolygonF* _personalSpace;

	bool isAhead (const QPointF& target, double cosThreshold = 0.707) const
	{
		QVector2D targetDirection(target.x() - _position->x(), target.y() - _position->y());
		targetDirection.normalize();

		return QVector2D::dotProduct((*_forward), targetDirection) > cosThreshold;
	}

	bool isAside (const QVector2D& target, double cosThreshold = 0.707) const
	{
		QVector2D targetDirection(target.x() - _position->x(), target.y() - _position->y());
		targetDirection.normalize();

		double dp = QVector2D::dotProduct((*_forward), targetDirection);
		return (dp < cosThreshold) && (dp > -cosThreshold);
	}

	bool isBehind (const QVector2D& target, double cosThreshold = -0.707) const
	{
		QVector2D targetDirection(target.x() - _position->x(), target.y() - _position->y());
		targetDirection.normalize();

		return QVector2D::dotProduct((*_forward), targetDirection) < cosThreshold;
	}

	virtual void updateFuturePosition();
	virtual void updateForces(void);
	virtual void computeCorrections(void);

	virtual QVector2D seekSteering(void);
	virtual QVector2D fleeSteering(void);
	virtual QVector2D alignSteering(void);
	virtual QVector2D separativeSteering(void);
	virtual QVector2D cohesiveSteering(void);
	virtual QVector2D avoidSteering(void);

	virtual void updateBoundingBox(void);
	virtual void updateSight(void);
	virtual void updatePersonalSpace(void);

	QVector2D truncate(QVector2D v, double max);
	void drawVector(QPainter* painter, QVector2D* v, const QColor& c, double scale) const;
	void drawPoint(QPainter* painter, QPointF* p, const QColor& c, double radius) const;

private:
	PPBot(const PPBot&) : PI(atan(1.0) * 4.0) {}

public:
	PPBot(void) : PI(atan(1.0) * 4.0), _radius(8.0), _mass(1.0), _slowing_distance(100), _max_speed(1.0), _max_force(0.1), _prediction_coeff(60.0){

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

		_boundingBox = new QRectF();
		_sight = new QPolygonF();
		_personalSpace = new QPolygonF();
	}

	virtual ~PPBot(void){
		delete(_position);
		delete(_futurePosition);
		delete(_target);
		delete(_forward);
		delete(_steering);
		delete(_correction);
		delete(_velocity);
		delete(_sight);
		delete(_personalSpace);
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

	const QRectF getBoundingBox(void) const{
		return (*_boundingBox);
	}

	const QPolygonF getSight(void) const{
		return (*_sight);
	}

	const QPolygonF getPersonalSpace(void) const{
		return (*_personalSpace);
	}

	virtual void draw(QPainter* painter) const;
	virtual void update(void);
};

#endif //PP_BOT_H